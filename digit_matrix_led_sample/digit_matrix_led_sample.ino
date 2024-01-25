#define sin_1 2
#define sin_2 3
#define sin_3 4
#define clock 5
#define latch 6
#define enable 7

uint16_t common = 0b0101010101010101;
uint16_t col_a = 0b0101010101010101;
uint16_t col_b = 0b0101010101010101;

void setup() {
  pinMode(sin_1, OUTPUT);
  pinMode(sin_2, OUTPUT);
  pinMode(sin_3, OUTPUT);
  pinMode(clock, OUTPUT);
  pinMode(latch, OUTPUT);
  pinMode(enable, OUTPUT);
}

void loop() {
  digitalWrite(enable, LOW);
  digitalWrite(latch, HIGH);
  for (int i = 0; i < 16; i++) {
    digitalWrite(clock, HIGH);
    digitalWrite(sin_1, (common >> i) & 0b0000000000000001 ? HIGH : LOW);
    digitalWrite(sin_2, (col_a >> i) & 0b0000000000000001 ? HIGH : LOW);
    digitalWrite(sin_3, (col_b >> i) & 0b0000000000000001 ? HIGH : LOW);
    digitalWrite(clock, LOW);
  }
  digitalWrite(latch, LOW);

}
