#define up_sin_1 2
#define up_sin_2 3
#define up_sin_3 4
#define down_sin_1 12
#define down_sin_2 13
#define down_sin_3 14
#define led_clock 5
#define latch 6
#define enable 7

uint32_t pattern[32] = {
    0b00010000100010000100000000011000,
    0b00010000100010000010001111100000,
    0b00010011111111100001001000000000,
    0b00010000100010000000001000000000,
    0b11111100100010001000001000000000,
    0b00010011111111100100001111111100,
    0b00110000001000000010001000010000,
    0b00111001111111000000001000010000,
    0b01010101001001000000001000010000,
    0b01010001001001000000001000010000,
    0b10010001111111000010111111111110,
    0b00010001001001000010000000000000,
    0b00010001001001000100000100010000,
    0b00010001111111000100001000001000,
    0b00010000100010001000010000000100,
    0b00010011000001101000100000000010,
    0b00000000000000000000000000000000,
    0b00000000000000000000000000000000,
    0b01111111110000000111111111111100,
    0b01000100011111100100000100000100,
    0b01000100010010000100000100000100,
    0b01000100010010000100000100000100,
    0b01111111110010000100000100000100,
    0b01000100010010000100000100000100,
    0b01000100010010000111111111111100,
    0b01000100010010000100000100000100,
    0b01111111110010000100000100000100,
    0b00000000000010000100000100000100,
    0b00000000000010000100000100000100,
    0b00000000001010000100000100000100,
    0b00000000000110000111111111111100,
    0b00000000000000000000000000000000};

void setup() {
    pinMode(up_sin_1, OUTPUT);
    pinMode(up_sin_2, OUTPUT);
    pinMode(up_sin_3, OUTPUT);
    pinMode(down_sin_1, OUTPUT);
    pinMode(down_sin_2, OUTPUT);
    pinMode(down_sin_3, OUTPUT);
    pinMode(led_clock, OUTPUT);
    pinMode(latch, OUTPUT);
    pinMode(enable, OUTPUT);

    digitalWrite(up_sin_1, LOW);
    digitalWrite(up_sin_2, LOW);
    digitalWrite(up_sin_3, LOW);
    digitalWrite(down_sin_1, LOW);
    digitalWrite(down_sin_2, LOW);
    digitalWrite(down_sin_3, LOW);
    for (int i = 0; i < 16; i++) {
        digitalWrite(led_clock, HIGH);
        digitalWrite(led_clock, LOW);
    }
    digitalWrite(latch, HIGH);
    digitalWrite(latch, LOW);
    digitalWrite(enable, LOW);
}

void loop() {
    for (int row = 0; row < 16; row++) {
        for (int n = 0; n < 16; n++) {
            // up
            digitalWrite(up_sin_1, ((0b0000000000000001 << row) >> n) & 0b0000000000000001 ? HIGH : LOW);
            digitalWrite(up_sin_2, (pattern[row] >> (n + 16)) & 0b0000000000000001 ? HIGH : LOW);
            digitalWrite(up_sin_3, (pattern[row] >> n) & 0b0000000000000001 ? HIGH : LOW);
            // down
            digitalWrite(down_sin_1, ((0b0000000000000001 << row) >> n) & 0b0000000000000001 ? HIGH : LOW);
            digitalWrite(down_sin_2, (pattern[row + 16] >> (n + 16)) & 0b0000000000000001 ? HIGH : LOW);
            digitalWrite(down_sin_3, (pattern[row + 16] >> n) & 0b0000000000000001 ? HIGH : LOW);
            // clock
            digitalWrite(led_clock, HIGH);
            digitalWrite(led_clock, LOW);
        }
        digitalWrite(latch, HIGH);
        digitalWrite(latch, LOW);
    }
}
