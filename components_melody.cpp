#include "components.h"
#include "types.h"
#import "helper_midi.h"


byte MaxRandomNoteValue(float random_jitter, int total_num_notes) {
    return (byte) (random_jitter * total_num_notes);
  }

Melody::Melody(int length, int melody[], int durations[]) {
    this->melody = melody;
    this->durations = durations;
    this->length = length;

    this->note_position = 0;
    this->elapsed = 0;
}

void Melody::reset() {
    this->note_position = 0;
    this->elapsed = 0;    
}

int Melody::current_note_duration(int rate) {
    // To calculate the note duration, take a time value (2ms - ~10 seconds) 
    // divided by the note type. (e.g. quarter note = 1000 / 4, eighth note = 1000/8, etc.)
    
    return 1000 / 8;
    //return rate / this->durations[this->note_position];
}


int Melody::current_note() {
    
    return 60;
    //return this->melody[this->note_position];
}

void Melody::play(unsigned short dt, State * robot_state) {



    float jitter = 0;

    /*
    if (robot_state->bypassRandomNote() == false) {
      jitter = robot_state->randomness();
    }
    */

    this->elapsed += dt;

    int current_note_duration = this->current_note_duration(robot_state->rate());
    int pause_duration = current_note_duration * 1.30; 
    int max_note_event_length = current_note_duration + pause_duration;
   

    MidiOut::WriteMidiCC(this->current_note(), 123);
    MidiOut::WriteMidiCC(66, this->note_position);
    MidiOut::WriteMidiCC(67, this->durations[this->note_position]);

    //MidiOut::WriteMidiCC(68, this->durations[this->note_position]);
   
    // If we are still playing the note
    if (this->elapsed <= current_note_duration) {

        if (this->current_note() == 0) {
           robot_state->sound_state()->turnOffAllNotes();
        } else {
            robot_state->sound_state()->turnNoteOn(this->current_note());
        }

    // If we are during the pause after the note
    } else if (this->elapsed <= max_note_event_length) {
        robot_state->sound_state()->turnOffAllNotes();

    // Progress to next note next time around
    } else {
        this->elapsed = 0;
        this->note_position += 1;
        if (this->note_position >= this->length) {
            this->note_position = 0;
        } 
        // Add jitter
        //byte max_offset = MaxRandomNoteValue(jitter, this->length);
        //byte random_note = 1 + random(0, max_offset);
        
        //this->note_position = this->note_position  + (1 * random_note);
        //if (this->note_position >= this->length) {
        //  this->note_position = this->note_position - this->length; 
        //}

    }
}