#include "keyboard.h"

typedef struct button_tag {
    debounce_t  status;
    uint32_t    debounce_counter;
    uint32_t    midi_note;
} button_t;

button_t key[KEY_OUT_LEN][KEY_IN_LEN] = {
    { {RELEASED, 0, 99}, {RELEASED, 0, 71}, {RELEASED, 0, 63}, {RELEASED, 0, 55}, {RELEASED, 0, 47}, {RELEASED, 0, 39}, {RELEASED, 0, 31}, }, // OUT 1
    { {RELEASED, 0, 99}, {RELEASED, 0, 70}, {RELEASED, 0, 62}, {RELEASED, 0, 54}, {RELEASED, 0, 46}, {RELEASED, 0, 38}, {RELEASED, 0, 30}, }, // OUT 2
    { {RELEASED, 0, 99}, {RELEASED, 0, 69}, {RELEASED, 0, 61}, {RELEASED, 0, 53}, {RELEASED, 0, 45}, {RELEASED, 0, 37}, {RELEASED, 0, 29}, }, // OUT 3
    { {RELEASED, 0, 99}, {RELEASED, 0, 68}, {RELEASED, 0, 60}, {RELEASED, 0, 52}, {RELEASED, 0, 44}, {RELEASED, 0, 36}, {RELEASED, 0, 28}, }, // OUT 4
    { {RELEASED, 0, 99}, {RELEASED, 0, 67}, {RELEASED, 0, 59}, {RELEASED, 0, 51}, {RELEASED, 0, 43}, {RELEASED, 0, 35}, {RELEASED, 0, 27}, }, // OUT 5
    { {RELEASED, 0, 99}, {RELEASED, 0, 66}, {RELEASED, 0, 58}, {RELEASED, 0, 50}, {RELEASED, 0, 42}, {RELEASED, 0, 34}, {RELEASED, 0, 26}, }, // OUT 6
    { {RELEASED, 0, 99}, {RELEASED, 0, 65}, {RELEASED, 0, 57}, {RELEASED, 0, 49}, {RELEASED, 0, 41}, {RELEASED, 0, 33}, {RELEASED, 0, 25}, }, // OUT 7
    { {RELEASED, 0, 72}, {RELEASED, 0, 64}, {RELEASED, 0, 56}, {RELEASED, 0, 48}, {RELEASED, 0, 40}, {RELEASED, 0, 32}, {RELEASED, 0, 24}, }, // OUT 8
//  IN          9               10                  11                  12                  13                 14                  15
};

uint32_t key_in[KEY_IN_LEN] = {
    PIN_9,
    PIN_10,
    PIN_11,
    PIN_12,
    PIN_13,
    PIN_14,
    PIN_15,
};

uint32_t key_out[KEY_OUT_LEN] = {
    PIN_1,
    PIN_2,
    PIN_3,
    PIN_4,
    PIN_5,
    PIN_6,
    PIN_7,
    PIN_8,
};


uint32_t _aweful_translate(uint32_t arduino_pin) {
    switch (arduino_pin) {
        case 22: return 1;
        case 24: return 2;
        case 26: return 3;
        case 28: return 4;
        case 30: return 5;
        case 32: return 6;
        case 34: return 7;
        case 36: return 8;
        case 38: return 9;
        case 40: return 10;
        case 42: return 11;
        case 44: return 12;
        case 46: return 13;
        case 48: return 14;
        case 50: return 15;
        default: return 0;
    }
}

char buff[50] = {0};

void keyboard_debouncer(uint32_t *midi_keyboard, debounce_t *midi_status) {
    uint32_t i, j, pin_status;
    *midi_keyboard = 0;

    for (i = 0; i < KEY_OUT_LEN; i++) {
        digitalWrite(key_out[i], LOW);

        for (j = 0; j < KEY_IN_LEN; j++) {
            pin_status = digitalRead(key_in[j]);
            if (LOW == pin_status) {
                if (RELEASED == key[i][j].status) {
                    key[i][j].status = REL_TO_DET;
                    key[i][j].debounce_counter = 0;
                } else if (REL_TO_DET == key[i][j].status) {
                    key[i][j].debounce_counter++;
                    if (DEBOUNCE_CYCLE < key[i][j].debounce_counter) {
                        key[i][j].status = DETECTED;
                        *midi_keyboard = key[i][j].midi_note;
                        *midi_status = key[i][j].status;
                    }
                } else {
                    key[i][j].status = DETECTED;
                }
            } else if (HIGH == pin_status) {
                if (DETECTED == key[i][j].status) {
                    key[i][j].status = DET_TO_REL;
                    key[i][j].debounce_counter = 0;
                } else if (DET_TO_REL == key[i][j].status) {
                    key[i][j].debounce_counter++;
                    if (DEBOUNCE_CYCLE < key[i][j].debounce_counter) {
                        key[i][j].status = RELEASED;
                        *midi_keyboard = key[i][j].midi_note;
                        *midi_status = key[i][j].status;
                    }
                } else {
                    key[i][j].status = RELEASED;
                }
            }
        }

        digitalWrite(key_out[i], HIGH);
    }
}

void keyboard_reader(void) {
    uint32_t i, j, pin_status;

    for (i = 0; i < KEY_OUT_LEN; i++) {
        digitalWrite(key_out[i], LOW);

        for (j = 0; j < KEY_IN_LEN; j++) {
            pin_status = digitalRead(key_in[j]);
            if (LOW == pin_status) {
                sprintf(buff, "out: PIN_%d in: PIN_%d\n", (int)_aweful_translate(key_out[i]),
                                                          (int)_aweful_translate(key_in[j]));
                Serial.println(buff);
            }
        }

        digitalWrite(key_out[i], HIGH);
    }
}

void keyboard_init(void) {
    int i;
    for (i = 0; i < KEY_IN_LEN; i++) {
        pinMode(key_in[i], INPUT_PULLUP);
    }
    for (i = 0; i < KEY_OUT_LEN; i++) {
        pinMode(key_out[i], OUTPUT);
        digitalWrite(key_out[i], HIGH);
    }
}
