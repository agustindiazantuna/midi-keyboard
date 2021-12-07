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
    // Serial.println("Sending note on");
    // noteOn(0, 48, 64);   // Channel 0, middle C, normal velocity
    // MidiUSB.flush();
    // delay(500);
    // Serial.println("Sending note off");
    // noteOff(0, 48, 64);  // Channel 0, middle C, normal velocity
    // MidiUSB.flush();
    // delay(1500);

    uint32_t midi_keyboard;
    debounce_t midi_status;
    keyboard_debouncer(&midi_keyboard, &midi_status);
    if (midi_keyboard != 0) {
        if (DETECTED == midi_status) {
            noteOn(0, midi_keyboard, 100);   // Channel 0, midi detected, normal velocity 64
        } else if (RELEASED == midi_status) {
            noteOff(0, midi_keyboard, 100);  // Channel 0, midi detected, normal velocity 64
        }
    }
    
    // digitalWrite(LED_BUILTIN, HIGH);
    // delay(250);
    // digitalWrite(LED_BUILTIN, LOW);
    // delay(500);
}
