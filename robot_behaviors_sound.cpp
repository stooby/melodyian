#include "robot.h"
#include "constants_actions.h"
#include "constants_pitches.h"

RobotSoundBehavior::RobotSoundBehavior() : Behavior() {

    int m1notes[] = {
        MIDINOTE_C4, 
        MIDINOTE_G3, 
        MIDINOTE_G3, 
        MIDINOTE_A3, 
        MIDINOTE_G3, 
        0, 
        MIDINOTE_B3, 
        MIDINOTE_C4
    };
    int m1durations[] = {4, 8, 8, 4,4,4,4,4};

    this->melody_one = new Melody(8, m1notes, m1durations);

    int m2notes[] = {
        0,
        MIDINOTE_FS3, 
        MIDINOTE_GS3, 
        MIDINOTE_AS3, 
        MIDINOTE_CS4, 
        MIDINOTE_DS4, 
        MIDINOTE_CS4, 
        MIDINOTE_F4, 
        MIDINOTE_DS4
    };

    int m2durations[] = {8,8,8,8,8,16,16,8,8}; 

    this->melody_two= new Melody(9, m2notes, m2durations);

}

void UpdateOutputFromState(State * state, Output * output) {
    if (state->sound_state()->anyActingNotes()) {
        output->setAmpPower(true) ;
        output->tone = state->sound_state()->currentHz();
    } else {
        output->setAmpPower(false);
    }
}

void RobotSoundBehavior::updateBehavior(unsigned short dt, State * state, Output * output) {

    this->incrementTimer(dt);

    switch(behavior_key)
    {
        case(MEL1TRIG_CC):
        {
            this->melody_one->play(dt, state);
            UpdateOutputFromState(state, output);
            break;
        }
        case(MEL2TRIG_CC):
        {
            this->melody_two->play(dt, state);
            UpdateOutputFromState(state, output);
            break;
        }
        case(KEYACT_CC):
        {
            UpdateOutputFromState(state, output);
            break;
        }

        case(MIDIPANIC_CC):
        {
            state->sound_state()->turnOffAllNotes();
            output->setAmpPower(false);
            break;
        }
        default:
        {
            output->setAmpPower(false);
        }

    }


    // Foo
}

void RobotSoundBehavior::updateBehaviorKey(byte control_number, byte value) {

  // Sound behavior keys
  if (control_number == MEL1TRIG_CC || 
      control_number == MEL2TRIG_CC || 
      control_number == KEYACT_CC || 
      control_number == MIDIPANIC_CC) {

    if (value == 127) {
      this->setCurrentBehavior(control_number);
    } else {
      this->clearCurrentBehavior();
    }
  }

}

void RobotSoundBehavior::updateState(byte control_number, byte value, State * state) {

    if (control_number == MEL1TRIG_CC || 
      control_number == MEL2TRIG_CC || 
      control_number == KEYACT_CC || 
      control_number == MIDIPANIC_CC) {
        // Always reset the note state when we change keys
        state->sound_state()->turnOffAllNotes();
    }

    if (control_number == MEL1TRIG_CC) {
        this->melody_one->reset();
        state->sound_state()->turnOffAllNotes();
    } 

    if (control_number == MEL2TRIG_CC) {
        this->melody_two->reset();
        state->sound_state()->turnOffAllNotes();
    }

}
