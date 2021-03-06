#pragma once

#include "components.h"
#include "types.h"

// Behaviors

class RobotLEDBehavior : public Behavior {

private:
  RGBColor transition_color;
  bool flash_on_flag;
  byte selected_light_preset;
  byte current_fade_preset;
  float brightness;

  void triggerLightPreset(int preset_number, State * state);
  void flashBehavior(State * state, Output * output);
  void fadeBehavior(State * state, Output * output);
  void pulseBehavior(unsigned short dt, State * state, Output * output);

public:
  RobotLEDBehavior();
  void updateBehavior(unsigned short dt, State * state, Output * output);
  void updateBehaviorKey(byte control_number, byte value);

  void updateState(byte control_number, byte value, State * state);

};


class RobotSoundBehavior : public Behavior {

private:
  Melody * melody_one;
  Melody * melody_two;
public:
  RobotSoundBehavior();
  void updateBehavior(unsigned short dt, State * state, Output * output);
  void updateBehaviorKey(byte control_number, byte value);
  void updateState(byte control_number, byte value, State * state);

};


// Main interface
class Robot {

public:
  Robot(HardwareInterface * hardware);

  HardwareInterface * hardware;
  
  State * state;

  Output * output;
  RobotLEDBehavior * led_behavior;
  RobotSoundBehavior * sound_behavior;
  
  void handleInput(byte control_number, byte value);
  void updateBehavior(unsigned short dt);
  void updateHardware();

  void noteOnControl(byte channel, byte note, byte velocity);
  void noteOffControl(byte channel, byte note, byte velocity);

};
