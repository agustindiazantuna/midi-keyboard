#include <Arduino.h>
#include <MIDIUSB.h>
#include <DueTimer.h>

#include <keyboard.h>

void noteOn(byte channel, byte pitch, byte velocity) {
    midiEventPacket_t noteOn = {0x09, 0x90 | channel, pitch, velocity};
    MidiUSB.sendMIDI(noteOn);
}

void noteOff(byte channel, byte pitch, byte velocity) {
    midiEventPacket_t noteOff = {0x08, 0x80 | channel, pitch, velocity};
    MidiUSB.sendMIDI(noteOff);
}


boolean led = false;
void timer3Handler(void) {
    led = !led;
    digitalWrite(LED_BUILTIN, led);
}

void setup() {
    keyboard_init();
    Serial.begin(115200);
    pinMode(LED_BUILTIN, OUTPUT);
	Timer3.attachInterrupt(timer3Handler);
	Timer3.start(500 * 1000); // Calls every 50ms
}

void loop() {
    uint32_t midi_keyboard;
    debounce_t midi_status;
    keyboard_debouncer(&midi_keyboard, &midi_status);
    if (midi_keyboard != 0) {
        if (DETECTED == midi_status) {
            // Channel 0, midi detected, normal velocity 100
            noteOn(0, midi_keyboard, 100);
        } else if (RELEASED == midi_status) {
            // Channel 0, midi detected, normal velocity 100
            noteOff(0, midi_keyboard, 100);
    }
}
