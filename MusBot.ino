#include <EEPROM.h>
#include <MIDI.h>
#include <midi_Defs.h>
#include <midi_Namespace.h>
#include <midi_Settings.h>
// #include <Wire.h>

#include "hardware_arduino.h"
#include "robot.h"

#include "constants_actions.h"

#include "helper_midi.h"
#include "helper_time.h"

#include "legacy.h"

MIDI_CREATE_DEFAULT_INSTANCE();

Timer * timer;
Robot * robot;
BatteryReader * battery_reader;

void setup()  {

  timer = new Timer();

  HardwareInterface * arduino_hardware = new ArduinoInterface();
  robot = new Robot(arduino_hardware);
  battery_reader = new BatteryReader(arduino_hardware);

  InitializeInputs();
  
} 

void loop()
{
  // Read midi from input. Triggers callbacks.
  bool midi_read = MIDI.read();

  // Move our clock forward
  unsigned short dt = timer->step();

  battery_reader->readBattery(dt, midi_read);

  robot->updateBehavior(dt);
  robot->updateHardware();

  Debug(dt);
}


void InitializeInputs() {

  MIDI.begin();
  Serial.begin(57600);
  MIDI.turnThruOff();
  
  // Think I need to do this so Arduino can work w/ any MIDI message Max will be sending it
  MIDI.setInputChannel(MIDI_CHANNEL_OMNI); 
  MIDI.setHandleControlChange(HandleControlChange);
  MIDI.setHandleNoteOn(NoteOnControl);

  // Only needed if receiving MIDI Note Off messages from controller device to stop notes
  MIDI.setHandleNoteOff(NoteOffControl); 

}

unsigned short debug_every = 3000;
unsigned short debug_timer = 0;

void Debug(unsigned short dt) {

    debug_timer = debug_timer + dt;
    if (debug_timer > debug_every) {
      debug_timer = 0;
      
      // Write out whatever CC values you want to debug here.
      MidiOut::WriteMidiCC(10, robot->state->motorSpeedValue() / 2);
      MidiOut::WriteMidiCC(11, robot->state->steerDirection());

      MidiOut::WriteMidiCC(1, robot->output->motor_a.speed / 2);
      MidiOut::WriteMidiCC(2, robot->output->motor_b.speed / 2);
      MidiOut::WriteMidiCC(3, robot->output->motor_a.direction);
      MidiOut::WriteMidiCC(4, robot->output->motor_b.direction);
    }
}

void HandleControlChange (byte channel, byte number, byte value) {
  // Process our inputs
  robot->handleInput(number, value);
}

// Special-cased event handling
void NoteOnControl (byte channel, byte note, byte velocity) {
  robot->noteOnControl(channel, note, velocity);
}

void NoteOffControl(byte channel, byte note, byte velocity) {
  robot->noteOffControl(channel, note, velocity);
}




