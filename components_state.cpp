#include "components.h"
#include "types.h"

#include "constants_actions.h"
#include "helper_smoothing.h"


State::State() {

    this->led_storage_ = new LEDStorage();
    this->led_storage_->readFromEEPROM();
    
    // TODO must update this if we get a low value. Saves some meory.
    this->input_values = new InputValues(RED_CC);

    // Default to "1000"
    this->updateInput(RATE1_CC, (byte) 60);

    // input_values->storeInput(RATE1_CC, (byte) 84); // Default to 1000
    this->sound_state_ = new SoundState(112);

}

void State::updateInput(byte control_number, byte value) {
    this->input_values->storeInput(control_number, value);
}

LEDStorage * State::led_storage() {
    return this->led_storage_;
}

SoundState * State::sound_state() {
    return this->sound_state_;
}

void State::setCurrentLEDValues(byte r, byte g, byte b) {
    this->input_values->storeInput(RED_CC, r);
    this->input_values->storeInput(GREEN_CC, g);
    this->input_values->storeInput(BLUE_CC, b);
}

// Wrapper to input values
bool State::bypassRandomColor() {
    return Smoothing::booleanButton(this->input_values->getValue(COLORJITTERBYPASS_CC));
}

bool State::bypassRandomNote() {
    return Smoothing::booleanButton(this->input_values->getValue(NOTEJITTERBYPASS_CC));
}

float State::randomness() {
    return Smoothing::mapByteToPercentage(this->input_values->getValue(JITTER_CC));
}

unsigned int State::decay() {
    return (unsigned int) this->input_values->getValue(FADESPD_CC);
}

int State::rate() {
    return Smoothing::smoothRateFader(this->input_values->getValue(RATE1_CC));
}
    
byte State::ledRedValue() {
    return Smoothing::smoothRBGFader(this->input_values->getValue(RED_CC));
}

byte State::ledGreenValue() {
    return Smoothing::smoothRBGFader(this->input_values->getValue(GREEN_CC));
}

byte State::ledBlueValue() {
    return Smoothing::smoothRBGFader(this->input_values->getValue(BLUE_CC));
}

byte State::pulseValue() {
    return this->input_values->getValue(DYNAMIC_CC);
}

bool State::colorOn() {
    return Smoothing::booleanButton(this->input_values->getValue(SETCOLQ_CC));
}

bool State::recordEEPROMArmed() {
    return Smoothing::booleanButton(this->input_values->getValue(ARMEEPROM_CC));
}

void State::disarmRecordEEPROM() {
    this->input_values->storeInput(ARMEEPROM_CC, (byte) 0);
}

bool State::saveColorOn() {
    return Smoothing::booleanButton(this->input_values->getValue(WRITECOLOR_CC));
}

bool State::soundActionOn() {
    return this->melodyOneOn() || this->melodyTwoOn() || this->keyModeOn();
}

bool State::melodyOneOn() {
    return Smoothing::booleanButton(this->input_values->getValue(MEL1TRIG_CC));
}

bool State::melodyTwoOn() {
    return Smoothing::booleanButton(this->input_values->getValue(MEL2TRIG_CC));

}

bool State::keyModeOn() {
    return Smoothing::booleanButton(this->input_values->getValue(KEYACT_CC));

}

bool State::midiPanicOn() {
    return Smoothing::booleanButton(this->input_values->getValue(MIDIPANIC_CC));
}
