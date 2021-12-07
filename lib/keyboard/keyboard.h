#include "Arduino.h"

// 1  | 2  | 3  | 4  | 5  | 6  | 7  | 8  | 9  | 10 | 11 | 12 | 13 | 14 | 15
// 22 | 24 | 26 | 28 | 30 | 32 | 34 | 36 | 38 | 40 | 42 | 44 | 46 | 48 | 49
// ou | ou | ou | ou | ou | ou | ou | ou | in | in | in | in | in | in | in

#define PIN_1           22
#define PIN_2           24
#define PIN_3           26
#define PIN_4           28
#define PIN_5           30
#define PIN_6           32
#define PIN_7           34
#define PIN_8           36
#define PIN_9           38
#define PIN_10          40
#define PIN_11          42
#define PIN_12          44
#define PIN_13          46
#define PIN_14          48
#define PIN_15          50
#define KEY_IN_LEN      7
#define KEY_OUT_LEN     8

#define DEBOUNCE_CYCLE  3

typedef enum debounce_tag {
    RELEASED = 0,
    REL_TO_DET,
    DETECTED,
    DET_TO_REL,
} debounce_t;

void keyboard_debouncer(uint32_t *midi_keyboard, debounce_t *midi_status);
void keyboard_reader(void);
void keyboard_init(void);
