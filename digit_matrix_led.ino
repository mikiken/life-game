#define up_sin_1 2
#define up_sin_2 3
#define up_sin_3 4
#define down_sin_1 12
#define down_sin_2 13
#define down_sin_3 14
#define led_clock 5
#define latch 6
#define enable 7

uint32_t pattern[32];

void init_pin() {
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

void convert_matrix_to_pattern(bool matrix[32][32]) {
    for (int row = 0; row < 32; row++) {
        pattern[row] = 0;
        for (int n = 0; n < 32; n++) {
            pattern[row] |= (uint32_t)matrix[row][n] << (31 - n);
        }
    }
}

void display(bool matrix[32][32]) {
    convert_matrix_to_pattern(matrix);
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
