// Astrotracker H1 - a DIY star tracker for astrophotography with a 360° azimuthal axis, adjustable tilt, and automatic North Pole finding.
// Made by Jayden Leung (Hack Club Slack: @loliipoppi)
// https://github.com/jaydenaleung/astrotrackerH1

// here include libraries
#include <TMC2209.h>

// here initialize stepper driver and set up library
TMC2209 raSD; # ra = rotational axis, SD for the stepper driver, this is for the RA stepper motor 
const TMC2209::SerialAddress SERIAL_ADDRESS_RA = TMC2209::SERIAL_ADDRESS_RA;
TMC2209 aoeSD; # aoe = angle of elevation, SD for the stepper driver, this is for the AOE stepper motor
const TMC2209::SerialAddress SERIAL_ADDRESS_AOE = TMC2209::SERIAL_ADDRESS_AOE;
TMC2209 swvSD; # swv = swivel plate, SD for the stepper driver, this is for the swivel plate stepper motor
const TMC2209::SerialAddress SERIAL_ADDRESS_SWV = TMC2209::SERIAL_ADDRESS_SWV;
const long SERIAL_BAUD_RATE = 115200;

HardwareSerial & serial_stream = Serial1;

// here define pins
#define UART_PIN 17 # UART connection pin, with PDN connected as well

// Rotational axis (RA) motor
#define RA_EN_PIN 19
#define RA_STEP_PIN 4
#define RA_DIR_PIN 15
#define RA_M1A 
#define RA_M1B 
#define RA_M2A 
#define RA_M2B 

// Angle of elevation (AOE) motor
#define AOE_EN_PIN 18
#define AOE_STEP_PIN 0
#define AOE_DIR_PIN 8 // needs to be changed to another pin
#define AOE_M1A 
#define AOE_M1B 
#define AOE_M2A 
#define AOE_M2B 

// Swivel plate (SWV) motor
#define SWV_EN_PIN 5
#define SWV_STEP_PIN 2
#define SWV_DIR_PIN 7
#define SWV_M1A 
#define SWV_M1B 
#define SWV_M2A 
#define SWV_M2B 

void setup() {
  // stepper motor setup
  raSD.setup(Serial1, SERIAL_BAUD_RATE, SERIAL_ADDRESS_RA);
  aoeSD.setup(Serial1, SERIAL_BAUD_RATE, SERIAL_ADDRESS_AOE);
  swvSD.setup(Serial1, SERIAL_BAUD_RATE, SERIAL_ADDRESS_SWV);

  // set pinmodes
  pinMode(UART_PIN, OUTPUT);

  raSD.setHardwareEnablePin(RA_EN_PIN);
  pinMode(RA_STEP_PIN, OUTPUT);
  pinMode(RA_DIR_PIN, OUTPUT);

  aoeSD.setHardwareEnablePin(AOE_EN_PIN);
  pinMode(AOE_STEP_PIN, OUTPUT);
  pinMode(AOE_DIR_PIN, OUTPUT);

  swvSD.setHardwareEnablePin(SWV_EN_PIN);
  pinMode(SWV_STEP_PIN, OUTPUT);
  pinMode(SWV_DIR_PIN, OUTPUT);
}

void loop() {
  // put your main code here, to run repeatedly:

}
