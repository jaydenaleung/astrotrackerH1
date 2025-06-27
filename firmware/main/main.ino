// Astrotracker H1 - a DIY star tracker for astrophotography with a 360° azimuthal axis, adjustable tilt, and automatic North Pole finding.
// Made by Jayden Leung (Hack Club Slack: @loliipoppi)
// https://github.com/jaydenaleung/astrotrackerH1


// here include libraries
#include <TMC2209.h>

// here initialize stepper driver and set up library
TMC2209 raSD; // ra = rotational axis, SD for the stepper driver, this is for the RA stepper motor 
const TMC2209::SerialAddress SERIAL_ADDRESS_RA = TMC2209::SERIAL_ADDRESS_RA;
TMC2209 aoeSD; // aoe = angle of elevation, SD for the stepper driver, this is for the AOE stepper motor
const TMC2209::SerialAddress SERIAL_ADDRESS_AOE = TMC2209::SERIAL_ADDRESS_AOE;
TMC2209 swvSD; // swv = swivel plate, SD for the stepper driver, this is for the swivel plate stepper motor
const TMC2209::SerialAddress SERIAL_ADDRESS_SWV = TMC2209::SERIAL_ADDRESS_SWV;
const long SERIAL_BAUD_RATE = 115200;

const stepper_motor motors[] = [raSD, aoeSD, swvSD];

HardwareSerial & serial_stream = Serial1;

// here define pins
#define UART_PIN 17 // UART connection pin, with PDN connected as well

// Rotational axis (RA) motor
#define RA_EN_PIN 19
#define RA_STEP_PIN 4
#define RA_DIR_PIN 15

// Angle of elevation (AOE) motor
#define AOE_EN_PIN 18
#define AOE_STEP_PIN 0
#define AOE_DIR_PIN 8 // needs to be changed to another pin

// Swivel plate (SWV) motor
#define SWV_EN_PIN 5
#define SWV_STEP_PIN 2
#define SWV_DIR_PIN 7

#define START_PIN 25
#define STOP_PIN 26
#define ALIGN_PIN 27
#define UP_PIN 34
#define DOWN_PIN 33
#define LEFT_PIN 35
#define RIGHT_PIN 32

bool trackingState = false;
bool aligning = false;

double latReading; // reading from GPS
double azReading; // reading from magnometer

const int32_t STOP_VELOCITY = 0; // defined once

const int32_t RA_RUN_VELOCITY = 2; // sidereal alignment (star tracking) at 0.00417807462 deg/s (~15 arcseconds/s) with 1/64 microstepping. NEMA 17 moves at 200 steps/rev = 12800 microsteps/rev. 12.5:1 belt drive gear reduction, 12800 microsteps = 1 rev of the smaller gear = 1/12.5 rev of the bigger gear (we want this to move) = 28.8 deg of the bigger gear; 0.00014507 microsteps = 28.8 deg; 1.8569201964 microsteps/s = 0.00417807462 deg/s. Round to 2 microsteps/s with ~7.7% error.
const int RA_MSTP = 64; // microstepping 1/64
const uint8_t RA_RUN_CURRENT_PERCENT = 50;
double raAngle; // RA angle of the mechanism
bool NORTH_HEM = true;

const int32_t AOE_RUN_VELOCITY = 9600; // latitudinal alignment at 18 deg/s (deg/s should be a multiple of 9) with 1/16 microstepping. NEMA 17 moves at 200 steps/rev = 3200 microsteps/rev. 60:1 worm gear reduction, 3200 microsteps = 1 rev of the worm gear = 1/60 rev of plate gear = 6 deg of latitude change; 18 deg latitude change = 9600 microsteps. 18 deg/s = 9600 microsteps/s
const int AOE_MSTP = 16;
const uint8_t AOE_RUN_CURRENT_PERCENT = 50;
double latAngle; // latitudinal positioning of mechanism (saved in computer, physically set at start)

const int32_t SWV_RUN_VELOCITY = 480; // azimuthal alignment at 54 deg/s with 1/16 microstepping. NEMA 17 moves at 200 steps/rev = 3200 microsteps/rev. Only one single gear attached to motor. 3200 microsteps = 360 deg; 480 microsteps = 54 deg; 480 microsteps/s = 54 deg/s
const int SWV_MSTP = 16;
const uint8_t SWV_RUN_CURRENT_PERCENT = 50;
double azAngle; // azithumal positioning (heading) of mechanism

void setup() {
  // stepper motor setup
  raSD.setup(Serial1, SERIAL_BAUD_RATE, SERIAL_ADDRESS_RA);
  aoeSD.setup(Serial1, SERIAL_BAUD_RATE, SERIAL_ADDRESS_AOE);
  swvSD.setup(Serial1, SERIAL_BAUD_RATE, SERIAL_ADDRESS_SWV);

  raSD.setHardwareEnablePin(RA_EN_PIN);
  raSD.setRunCurrent(RUN_CURRENT_PERCENT);
  raSD.enableCoolStep();
  raSD.setMicrostepsPerStep(RA_MSTP);
  raSD.disable();

  aoeSD.setHardwareEnablePin(AOE_EN_PIN);
  aoeSD.setRunCurrent(RUN_CURRENT_PERCENT);
  aoeSD.enableCoolStep();
  aoeSD.setMicrostepsPerStep(AOE_MSTP);
  aoeSD.disable();

  swvSD.setHardwareEnablePin(SWV_EN_PIN);
  swvSD.setRunCurrent(RUN_CURRENT_PERCENT);
  swvSD.enableCoolStep();
  swvSD.setMicrostepsPerStep(SWV_MSTP);
  swvSD.disable();

  // set pinmodes
  raSD.setHardwareEnablePin(RA_EN_PIN);
  pinMode(RA_STEP_PIN, OUTPUT);
  pinMode(RA_DIR_PIN, OUTPUT);

  aoeSD.setHardwareEnablePin(AOE_EN_PIN);
  pinMode(AOE_STEP_PIN, OUTPUT);
  pinMode(AOE_DIR_PIN, OUTPUT);

  swvSD.setHardwareEnablePin(SWV_EN_PIN);
  pinMode(SWV_STEP_PIN, OUTPUT);
  pinMode(SWV_DIR_PIN, OUTPUT);

  pinMode(START_PIN, INPUT_PULLUP);
  pinMode(STOP_PIN, INPUT_PULLUP);
  pinMode(ALIGN_PIN, INPUT_PULLUP);
  pinMode(UP_PIN, INPUT_PULLUP);
  pinMode(DOWN_PIN, INPUT_PULLUP);
  pinMode(LEFT_PIN, INPUT_PULLUP);
  pinMode(RIGHT_PIN, INPUT_PULLUP);

  // reset values when starting up
  raAngle = 0;
  latAngle = 25; // else the polar scope will bump into the base plate
  azAngle = 0;

  Serial.begin(SERIAL_BAUD_RATE);
}

void loop() {
  // setup for custom functions
  digitalWrite(RA_DIR_PIN, LOW);
  digitalWrite(AOE_DIR_PIN, LOW);
  digitalWrite(SWV_DIR_PIN, LOW);
  
  if (NORTH_HEM) {
    raSD.enableInverseMotorDirection(); // counterclockwise - rotate this way for tracking in Northern Hemisphere
  } else {
    raSD.disableInverseMotorDirection(); // clockwise - Southern Hemisphere
  }

  // button logic
  if (digitalRead(START_PIN) == LOW && trackingState == false) { // button pressed to start
    trackingState = true;
    trackingStart();
  }

  if (digitalRead(STOP_PIN) == LOW && trackingState == true) { // button pressed to start
    trackingState = false;
    trackingStop();
  }

  if (digitalRead(ALIGN_PIN) == LOW && aligning == false) {
    aligning = true;
    latitudeAlign(latReading); // get latReading from the GPS = your goal latitude
    azithumalAlign();
    aligning = false;
  }

  if (digitalRead(UP_PIN) == LOW) {
    aoeSD.enable();
    if (trackingState == true) {trackingState = false;} // stop tracking // no conditions for azimuthal and latitudinal auto-aligning because they have the program-pausing function .delay()
    aoeSD.enableInverseMotorDirection(); // counterclockwise = latitude up
    aoeSD.moveAtVelocity(AOE_RUN_VELOCITY);
  } else {
    aoeSD.disable();
  }

  if (digitalRead(DOWN_PIN) == LOW) {
    aoeSD.enable();
    if (trackingState == true) {trackingState = false;}
    aoeSD.disableInverseMotorDirection(); // clockwise = latitude down
    aoeSD.moveAtVelocity(AOE_RUN_VELOCITY);
  } else {
    aoeSD.disable();
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

void trackingStart() { // RA motor
  raSD.enable();
  raSD.moveAtVelocity(RA_RUN_VELOCITY);
}

void trackingStop() { // RA motor
  raSD.moveAtVelocity(STOP_VELOCITY); // stop
  raSD.disable();
}

void latitudeAlign(double angle) { // AOE motor - 9600 microsteps per second = 18 degrees per second (60:1 worm gear reduction)
  aoeSD.enable();
  
  if (angle > latAngle) { // 'angle' is the goal latitude
    aoeSD.enableInverseMotorDirection(); // counterclockwise - turning the worm gear this way increases the latitudinal angle
  } else {
    aoeSD.disableInverseMotorDirection() // clockwise - turning the worm gear this way decreases the latitudinal angle
  }
  
  const int duration = (angle / 18) * 1000; // in milliseconds
  aoeSD.moveAtVelocity(AOE_RUN_VELOCITY);
  delay(duration);
  aoeSD.moveAtVelocity(STOP_VELOCITY); // stop

  aoeSD.disable()
}

void azimuthalAlign() { // swivel motor - 480 microsteps per second = 54 degrees per second
  swvSD.enable()

  if (azAngle < 180 || azAngle = 180) { // 0 deg is the goal heading, direction depends on mechanism heading given by magnometer
    swvSD.enableInverseMotorDirection(); // counterclockwise - turning the mechanism towards North
    const int duration = (azAngle / 54) * 1000; // in milliseconds
  } else {
    swvSD.disableInverseMotorDirection() // clockwise - turning the mechanism towards North
    const int tempAngle = azAngle - 180;
    const int duration = (tempAngle / 54) * 1000;
  }
  
  swvSD.moveAtVelocity(SWV_RUN_VELOCITY);
  delay(duration);
  swvSD.moveAtVelocity(STOP_VELOCITY); // stop

  swvSD.disable()
}