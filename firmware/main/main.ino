// Astrotracker H1 - a DIY star tracker for astrophotography with a 360° azimuthal axis, adjustable tilt, and automatic North Pole finding.
// Made by Jayden Leung (Hack Club Slack: @loliipoppi)
// https://github.com/jaydenaleung/astrotrackerH1

// Disclaimer: I used some of the code from various tutorial websites and library examples around the internet. This was mainly just general code such as
// starting the serial, defining library class objects, etc. that is generally required to use the library anyway. The only significant portion of code that
// was not mine is under the calibrateCompass() function which comes installed with the QMC5883LCompass Arduino library.

// NOTE: MANY QMC5883L MODULES DO NOT HAVE THE GENUINE QMC5883L CHIP, EVEN IF THEY CLAIM THEY DO. THERE ARE TWO OPTIONS FROM HERE: BUY A MAGNOMETER CHIP FROM A MORE REPUTABLE
// SELLER OR CONSIDER THAT YOUR CHIP MAY ACTUALLY BE A QMC5883P CHIP. THIS CODE IS WRITTEN FOR THE QMC5883P CHIP, SINCE THAT'S WHAT I HAVE. THE DATASHEET FOR IT IS IN THE FOLDER PATH:
// astrotrackerH1\pcb\libraries\QMC5883L\QMC5883P Datasheet Rev.C (considering that my QMC5883L is actually a QMC5883P. The footprint and schematic are the same nevertheless.)

// NOTE: THE NEMA 17 MODEL LISTED IN THE BOM'S WIRING IS OFTEN NOT AS IS SAID ON AMAZON. MY MOTOR WIRING IS: 1A/A+ = BLACK; 2A/A- = RED; 1B/B+ = GREEN; 2B/B- = BLUE.

//////////////////////////////////////////////////////////////////

// here include libraries
#include <Wire.h> // comes preinstalled on ESP32's Arduino core
#include <HardwareSerial.h> // comes preinstalled on ESP32's Arduino core
#include <Preferences.h> // comes preinstalled on ESP32's Arduino core
#include <LiquidCrystal_I2C.h> // LiquidCrystal I2C library by Frank de Brabander
#include <TinyGPSPlus.h> // TinyGPSPlus library by Mikal Hart
#include <QMC5883LCompass.h> // QMC5883LCompass library by MPrograms
#include <AccelStepper.h> // AccelStepper library

// initialize preferences
Preferences prefs;

// set serial baud rate
const long SERIAL_BAUD_RATE = 115200;
const long MOTOR_BAUD_RATE = 115200;
const long GPS_BAUD_RATE = 9600;

// here initialize objects and set up libraries
LiquidCrystal_I2C lcd(0x27, 16, 2); // confirm I2C address later
TinyGPSPlus gps;
HardwareSerial gpsSerial(2);
QMC5883LCompass compass;

// here define pins
#define GPS_RX_PIN 17 // GPS's RX pin goes to the ESP32 TX pin (17)
#define GPS_TX_PIN 16 // GPS's TX pin goes to the ESP32 RX pin (16)

// Rotational axis (RA) motor
#define RA_EN_PIN 19
#define RA_STEP_PIN 4
#define RA_DIR_PIN 25

// Angle of elevation (AOE) motor
#define AOE_EN_PIN 18
#define AOE_STEP_PIN 13
#define AOE_DIR_PIN 26

// Swivel plate (SWV) motor
#define SWV_EN_PIN 5
#define SWV_STEP_PIN 14
#define SWV_DIR_PIN 27

// some pins not used for now
#define START_PIN 35
// #define STOP_PIN 26
#define ALIGN_PIN 34
// #define UP_PIN 34
// #define DOWN_PIN 33
// #define LEFT_PIN 35
// #define RIGHT_PIN 32

AccelStepper raSD(1,RA_STEP_PIN,RA_DIR_PIN); // ra = rotational axis, SD for the stepper driver, this is for the RA stepper motor 
AccelStepper aoeSD(1,AOE_STEP_PIN,AOE_DIR_PIN); // aoe = angle of elevation, SD for the stepper driver, this is for the AOE stepper motor
AccelStepper swvSD(1,SWV_STEP_PIN,SWV_DIR_PIN); // swv = swivel plate, SD for the stepper driver, this is for the swivel plate stepper motor

bool trackingState = false;
bool aligning = false;

double latReading; // reading from GPS
double azReading; // reading from magnometer
char hemisphere = 'N'; // default northern hemisphere

int secsLeft = 0; // default 0 seconds left for aligning
double upButtonTime; // counts for how many seconds the up button was held down through watching millis()
double downButtonTime; // same but with down button
double startTime; // counted in milliseconds thru millis() at the start of tracking, to counter the time elapsed since tracking started
String formattedSecsCounter = "0h 0m 0.0s"; // formatted into hh:mm:ss.ms

const int32_t STOP_VELOCITY = 0; // defined once

/* The following is commented out for TMC2209 use */

// const int32_t RA_RUN_VELOCITY = 2; // sidereal alignment (star tracking) at 0.00417807462 deg/s (~15 arcseconds/s) with 1/64 microstepping. NEMA 17 moves at 200 steps/rev = 12800 microsteps/rev. 12.5:1 belt drive gear reduction, 12800 microsteps = 1 rev of the smaller gear = 1/12.5 rev of the bigger gear (we want this to move) = 28.8 deg of the bigger gear; 0.00014507 microsteps = 28.8 deg; 1.8569201964 microsteps/s = 0.00417807462 deg/s. Round to 2 microsteps/s with ~7.7% error.
// const int RA_MSTP = 64; // microstepping 1/64
// const uint8_t RA_RUN_CURRENT_PERCENT = 50;

// const int32_t AOE_RUN_VELOCITY = 9600; // latitudinal alignment at 18 deg/s (deg/s should be a multiple of 9) with 1/16 microstepping. NEMA 17 moves at 200 steps/rev = 3200 microsteps/rev. 60:1 worm gear reduction, 3200 microsteps = 1 rev of the worm gear = 1/60 rev of plate gear = 6 deg of latitude change; 18 deg latitude change = 9600 microsteps. 18 deg/s = 9600 microsteps/s
// const int AOE_MSTP = 16;
// const uint8_t AOE_RUN_CURRENT_PERCENT = 50;
double latAngle; // latitudinal positioning of mechanism (saved in computer, physically set at start)

// const int32_t SWV_RUN_VELOCITY = 480; // azimuthal alignment at 54 deg/s with 1/16 microstepping. NEMA 17 moves at 200 steps/rev = 3200 microsteps/rev. Only one single gear attached to motor. 3200 microsteps = 360 deg; 480 microsteps = 54 deg; 480 microsteps/s = 54 deg/s
// const int SWV_MSTP = 16;
// const uint8_t SWV_RUN_CURRENT_PERCENT = 50;
double azAngle; // azithumal positioning (heading) of mechanism

/* The following is for A4988 use */

const int RA_RUN_VELOCITY = 0.464230513; // 1/16 microstepping; sidereal alignment (star tracking) at 0.00417807462 deg/s with 1/16 microstepping. NEMA 17 moves at 200 steps/rev = 3200 microsteps/rev. 12.5:1 belt drive gear reduction, 3200 microsteps = 1 rev of the smaller gear = 1/12.5 rev of the bigger gear = 28.8 deg of the bigger gear per 3200 microsteps. Sidereal rate 0.00417807462 deg/s; 6893.12725 seconds per 3200 microsteps. 3200 microsteps/6893.12725s = 0.464230513 microsteps/s
const int AOE_RUN_VELOCITY = 2400;
const int SWV_RUN_VELOCITY = 120;

// create custom characters for LCD
byte upChar[] = { B00100, B01110, B10101, B00100, B00100, B00100, B00100, B00100 };
byte downChar[] = { B00100, B00100, B00100, B00100, B00100, B10101, B01110, B00100 };
byte leftChar[] = { B00000, B00000, B00100, B01000, B11111, B01000, B00100, B00000 };
byte rightChar[] = { B00000, B00000, B00100, B00010, B11111, B00010, B00100, B00000 };
byte hemL[] = { B00111, B01000, B10000, B11000, B10111, B10000, B01000, B00111 };
byte hemR[] = { B10000, B01000, B00100, B01100, B10100, B00100, B01000, B10000 };
byte lat[] = { B00000, B00100, B01110, B10101, B10101, B10101, B01110, B00100 };
byte az[] = { B10000, B01000, B00100, B00100, B01110, B01010, B10001, B10001 };

void setup() {
  // begin serial
  Wire.begin(); // just in case Arduino code to ESP32 needs this
  Serial.begin(SERIAL_BAUD_RATE);
  gpsSerial.begin(GPS_BAUD_RATE, SERIAL_8N1, GPS_RX_PIN, GPS_TX_PIN);
  
  // set pinmodes
  pinMode(RA_EN_PIN, OUTPUT);
  pinMode(RA_STEP_PIN, OUTPUT);
  pinMode(RA_DIR_PIN, OUTPUT);

  pinMode(AOE_EN_PIN, OUTPUT);
  pinMode(AOE_STEP_PIN, OUTPUT);
  pinMode(AOE_DIR_PIN, OUTPUT);

  pinMode(SWV_EN_PIN, OUTPUT);
  pinMode(SWV_STEP_PIN, OUTPUT);
  pinMode(SWV_DIR_PIN, OUTPUT);

  pinMode(START_PIN, INPUT);
  // pinMode(STOP_PIN, INPUT);
  pinMode(ALIGN_PIN, INPUT);
  // pinMode(UP_PIN, INPUT);
  // pinMode(DOWN_PIN, INPUT);
  // pinMode(LEFT_PIN, INPUT);
  // pinMode(RIGHT_PIN, INPUT);

  // set initial digitalWrites
  digitalWrite(RA_EN_PIN,HIGH);
  digitalWrite(AOE_EN_PIN,HIGH);
  digitalWrite(SWV_EN_PIN,HIGH);

  // stepper motor setup for TMC2209
  // raSD.setup(Serial2, MOTOR_BAUD_RATE, SERIAL_ADDRESS_RA);
  // aoeSD.setup(Serial2, MOTOR_BAUD_RATE, SERIAL_ADDRESS_AOE);
  // swvSD.setup(Serial2, MOTOR_BAUD_RATE, SERIAL_ADDRESS_SWV);

  // raSD.setRunCurrent(RA_RUN_CURRENT_PERCENT);
  // raSD.enableCoolStep();
  // raSD.setMicrostepsPerStep(RA_MSTP);
  // raSD.disable();

  // aoeSD.setHardwareEnablePin(AOE_EN_PIN);
  // aoeSD.setRunCurrent(AOE_RUN_CURRENT_PERCENT);
  // aoeSD.enableCoolStep();
  // aoeSD.setMicrostepsPerStep(AOE_MSTP);
  // aoeSD.disable();

  // swvSD.setHardwareEnablePin(SWV_EN_PIN);
  // swvSD.setRunCurrent(SWV_RUN_CURRENT_PERCENT);
  // swvSD.enableCoolStep();
  // swvSD.setMicrostepsPerStep(SWV_MSTP);
  // swvSD.disable();


  // stepper motor setup for A4988
  raSD.setMaxSpeed(10000);
  aoeSD.setMaxSpeed(10000);
  swvSD.setMaxSpeed(10000);
  
  raSD.setSpeed(0);
  aoeSD.setSpeed(0);
  swvSD.setSpeed(0);

  // get position on startup
  getPosition();
  hemisphere = determineHemisphere();

  // reset values when starting up
  prefs.begin("angles", true); // open the "angles" namespace
  latAngle = prefs.getDouble("latitude", 25.0); // retrieve the physical mechanism latitude tilt that was saved the previous time using it, default to 25.0 degrees (else the polar scope will bump into the base plate)
  prefs.end();
  azAngle = azReading;

  // initialize modules
  lcd.init();
  lcd.backlight();

  lcd.createChar(0, upChar);
  lcd.createChar(1, downChar);
  lcd.createChar(2, leftChar);
  lcd.createChar(3, rightChar);
  lcd.createChar(4, hemL);
  lcd.createChar(5, hemR);
  lcd.createChar(6, lat);
  lcd.createChar(7, az);

  // default message
  home();
  
  compass.init();
  calibrateCompass();
  /* compass.setCalibration(); SET THIS FUNCTION AFTER CALIBRATION */
}

char determineHemisphere() {
  if (latReading > 0) {
    return 'N';
  } else if (latReading < 0) {
    return 'S';
  } else {
    return 'E'; // equator
  }
}

// LCD screens/messages
void home() {
  lcd.clear();
  lcd.setCursor(0,0); // return cursor to top left corner on startup
  lcd.print("READY FOR");
  lcd.setCursor(13,0);
  lcd.write((byte)4);
  lcd.setCursor(14,0);
  lcd.write((byte)5);
  lcd.setCursor(15,0);
  lcd.print("N");
  lcd.setCursor(0,1);
  lcd.print("ALIGNMENT");
}

void align() {
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("ALIGNING");
  lcd.setCursor(9,0);
  lcd.write((byte)6);
  lcd.setCursor(10,0);
  lcd.print(latReading >= 10.0 ? round(latReading*10.0)/10.0 : round(latReading*100.0)/100.0); // round to 1 decimal place if the latitude is >= to 10, 2 if not. e.g. 42.3, 9.87. i.e. 3 sig figs.
  lcd.setCursor(14,0);
  lcd.write((byte)7);
  lcd.setCursor(15,0);
  lcd.write(hemisphere);
  lcd.setCursor(0,1);
  lcd.print(secsLeft);
  lcd.setCursor(0,3);
  lcd.print("secs left...");
}

void alignComplete() {
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("ALIGN COMPLETE");
  delay(1000);
}

void up() {
  if (aligning == false && trackingState == false) {
    lcd.setCursor(15,1);
    lcd.write((byte)0);
  }
}

void down() {
  if (aligning == false && trackingState == false) {
    lcd.setCursor(15,1);
    lcd.write((byte)1);
  }
}

void left() {
  if (aligning == false && trackingState == false) {
    lcd.setCursor(15,1);
    lcd.write((byte)2);
  }
}

void right() {
  if (aligning == false && trackingState == false) {
    lcd.setCursor(15,1);
    lcd.write((byte)3);
  }
}

void track() {
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("TRACKING...");
  lcd.setCursor(0,1);
  lcd.print(formattedSecsCounter);
}

void count() {
  lcd.setCursor(0,1);
  lcd.print(formattedSecsCounter);
}

void trackStopped() {
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("STOPPED");
  delay(1000);
}

void loop() {
  
  // if (hemisphere == 'N' || hemisphere == 'E') {
  //   raSD.enableInverseMotorDirection(); // counterclockwise - rotate this way for tracking in Northern Hemisphere
  // } else {
  //   raSD.disableInverseMotorDirection(); // clockwise - Southern Hemisphere
  // }

  getPosition();

  // button logic
  if (digitalRead(START_PIN) == LOW && trackingState == false) { // button pressed to start
    trackingState = true;
    startTime = (double)(millis()); // must convert millis() to double because it returns as an unsigned long (integer)
    track(); // update LCD
    trackingStart();
  }

  if (trackingState == true && digitalRead(START_PIN) == HIGH && digitalRead(STOP_PIN) == HIGH) { // during tracking
    formattedSecsCounter = updateCount();
    count();
  }

  if (digitalRead(STOP_PIN) == LOW && trackingState == true) { // button pressed to start
    trackingStop();
    trackStopped(); // update LCD
    trackingState = false;
  }

  if (digitalRead(ALIGN_PIN) == LOW && aligning == false) {
    aligning = true;
    secsLeft = round(((latReading / 18) * 1000) + ((azAngle / 54) * 1000)); // calculate remaining seconds
    align(); // update LCD
    latitudeAlign(); // align latitudinally
    secsLeft = round((azAngle / 54) * 1000); // calculate remaining seconds - latiudinal alignment seconds
    align(); // update LCD
    azithumalAlign(); // align azithumally
    alignComplete(); // update LCD
    aligning = false;
  }

  if (digitalRead(UP_PIN) == LOW) {
    aoeSD.enable();
    if (trackingState == true) {trackingState = false;} // stop tracking // no conditions for azimuthal and latitudinal auto-aligning because they have the program-pausing function .delay()
    aoeSD.enableInverseMotorDirection(); // counterclockwise = latitude up
    aoeSD.moveAtVelocity(AOE_RUN_VELOCITY);
  } else {
    aoeSD.disable();
    latAngle += ((double)(millis()) - upButtonTime) * 18; // get the time that the button was pressed for and convert it to degrees
    saveMechLat();
  }
  if (digitalRead(UP_PIN) == HIGH) { // crucial that this code is after and seperate from the else block, so that upButtonTime doesn't reset before it updates latAngle
    upButtonTime = (double)(millis());
  }

  if (digitalRead(DOWN_PIN) == LOW) {
    aoeSD.enable();
    if (trackingState == true) {trackingState = false;}
    aoeSD.disableInverseMotorDirection(); // clockwise = latitude down
    aoeSD.moveAtVelocity(AOE_RUN_VELOCITY);
  } else {
    aoeSD.disable();
    latAngle -= ((double)(millis()) - downButtonTime) * 18;
    saveMechLat();
  }
  if (digitalRead(DOWN_PIN) == HIGH) {
    downButtonTime = (double)(millis());
  }

  if (digitalRead(LEFT_PIN) == LOW) {
    swvSD.enable();
    if (trackingState == true) {trackingState = false;}
    swvSD.enableInverseMotorDirection(); // counterclockwise = left
    swvSD.moveAtVelocity(AOE_RUN_VELOCITY);
  } else {
    swvSD.disable();
  }

  if (digitalRead(RIGHT_PIN) == LOW) {
    swvSD.enable();
    if (trackingState == true) {trackingState = false;}
    swvSD.disableInverseMotorDirection(); // clockwise = right
    swvSD.moveAtVelocity(AOE_RUN_VELOCITY);
  } else {
    swvSD.disable();
  }
}

void saveMechLat() {
  prefs.begin("angles", false); // open a namespace in the ESP32 non-volatile memory
  prefs.putDouble("latitude", latAngle); // save the latAngle in long-term memory
  prefs.end();
}

void calibrateCompass() { // DISCLAIMER: THIS CODE SECTION IS NOT MINE. I TOOK IT FROM THE GIVEN "EXAMPLES" SECTION OF THE ARDUINO LIBRARY I USED (QMC5883LCompass.h library by MPrograms). I TAKE NO CREDIT FOR THE CODE WITHIN THIS FUNCTION.
  Serial.println("This will provide calibration settings for your QMC5883L chip. When prompted, move the magnetometer in all directions until the calibration is complete.");
  Serial.println("Calibration will begin in 5 seconds.");
  delay(5000);

  Serial.println("CALIBRATING. Keep moving your sensor...");
  compass.calibrate();

  Serial.println("DONE. Copy the lines below and paste it into your projects sketch.);");
  Serial.println();
  Serial.print("compass.setCalibrationOffsets(");
  Serial.print(compass.getCalibrationOffset(0));
  Serial.print(", ");
  Serial.print(compass.getCalibrationOffset(1));
  Serial.print(", ");
  Serial.print(compass.getCalibrationOffset(2));
  Serial.println(");");
  Serial.print("compass.setCalibrationScales(");
  Serial.print(compass.getCalibrationScale(0));
  Serial.print(", ");
  Serial.print(compass.getCalibrationScale(1));
  Serial.print(", ");
  Serial.print(compass.getCalibrationScale(2));
  Serial.println(");");
}

void getPosition() { // setup the GPS and magnometer
  // get latitude from GPS
  while (gpsSerial.available() > 0) {
    char gpsData = gpsSerial.read(); // get raw data
    Serial.println(gpsData);
    gps.encode(gpsSerial.read()); // encode into readable format
  }
  if (gps.location.isUpdated()) {
    latReading = gps.location.lat(); // get latitude
  }

  // get heading from magnometer
  compass.read(); // get compass data
  azReading = compass.getAzimuth(); // take the heading data from the compass and assign it to azReading
}

String updateCount() {
  double milsec = (double)(millis()) - startTime;;
  double seconds = milsec / 1000.0; // convert forward all the way to exact number of hours
  milsec = 0;
  double minutes = seconds / 60.0;
  seconds = 0;
  double hours = minutes / 60.0;
  minutes = 0;
  
  minutes = (hours-(double)((int)hours)) * 60.0; // convert exact number of hours into readable hh:mm:ss.ms format
  hours = (int)hours; // correct the hours by only displaying the remainder after 24 hours has been converted into and shown as a day
  seconds = (minutes-(double)((int)minutes)) * 60.0; // now truncate the decimals from days since that remainder time has been transferred to hours 
  minutes = (int)minutes; // does this by truncating the higher order unit (e.g. days is higher than hours) and subtracting it from the original to get the decimal point
  milsec = (seconds-(double)((int)seconds)) * 1000.0; // then it takes that and multiplies it by its respective conversion factor to get to a lower order unit (e.g. hours to minutes)
  seconds = (int)seconds; // keeps it as a double for precision before converting it to a more readable int and String later
  milsec = (int)milsec; // truncate the milliseconds too to make it readable and to fix any accidental decimals that show up in conversion

  return (String)hours + "h " + (String)minutes + "m " + (String)seconds + "." + (String)milsec + "s" // max characters is 16, OK unless hour count goes over 100 (unrealistic)
}

void trackingStart() { // RA motor
  digitalWrite(RA_EN_PIN,LOW);
  raSD.setSpeed(RA_RUN_VELOCITY);
}

void trackingStop() { // RA motor
  raSD.setSpeed(STOP_VELOCITY); // stop
  digitalWrite(RA_EN_PIN,HIGH);
}

void latitudeAlign() { // AOE motor - 9600 microsteps per second = 18 degrees per second (60:1 worm gear reduction)
  digitalWrite(AOE_EN_PIN,LOW);
  
  if (latReading > latAngle) { // 'angle' is the goal latitude
    AOE_RUN_VELOCITY *= -1; // counterclockwise - turning the worm gear this way increases the latitudinal angle
  } else {
    // do nothing = clockwise - turning the worm gear this way decreases the latitudinal angle
  }
  
  const float duration = (latReading / 18)
  aoeSD.setSpeed(AOE_RUN_VELOCITY);
  aoeSD.move((int)(duration*AOE_RUN_VELOCITY));
  aoeSD.setSpeed(STOP_VELOCITY); // stop

  if (latReading > latAngle) { // change it back
    AOE_RUN_VELOCITY *= -1;
  }
  
  latAngle = latReading;
  saveMechLat();

  digitalWrite(AOE_EN_PIN,HIGH);
}

void azimuthalAlign() { // swivel motor - 480 microsteps per second = 54 degrees per second
  digitalWrite(SWV_EN_PIN,LOW);

  if (azAngle < 180 || azAngle = 180) { // 0 deg is the goal heading, direction depends on mechanism heading given by magnometer
    SWV_RUN_VELOCITY *= -1; // counterclockwise - turning the mechanism towards North
    const int duration = (azAngle / 54) * 1000; // in milliseconds
  } else {
    // do nothing - clockwise - turning the mechanism towards North
    const int tempAngle = azAngle - 180;
    const int duration = (tempAngle / 54) * 1000;
  }
  
  swvSD.setSpeed(SWV_RUN_VELOCITY);
  aoeSD.move((int)(duration*SWV_RUN_VELOCITY));
  swvSD.setSpeed(STOP_VELOCITY); // stop

  if (azAngle < 180 || azAngle = 180) {
    SWV_RUN_VELOCITY *= -1; // turn it back
  }

  digitalWrite(SWV_EN_PIN,HIGH);
  secsLeft = 0; // reset after alignment is finished
}