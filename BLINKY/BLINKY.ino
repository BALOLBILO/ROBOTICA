#define MOTOR_IZQ_A 8
#define MOTOR_IZQ_B 9
#define MOTOR_DER_A 7
#define MOTOR_DER_B 4

#define ENABLE_IZQ 5
#define ENABLE_DER 6
void setup() {
 pinMode(MOTOR_IZQ_A, OUTPUT);
  pinMode(MOTOR_IZQ_B, OUTPUT);
  pinMode(MOTOR_DER_A, OUTPUT);
  pinMode(MOTOR_DER_B, OUTPUT);
  pinMode(ENABLE_DER, OUTPUT);
  pinMode(ENABLE_IZQ, OUTPUT);
}

void loop() {
digitalWrite(8,LOW);
}
