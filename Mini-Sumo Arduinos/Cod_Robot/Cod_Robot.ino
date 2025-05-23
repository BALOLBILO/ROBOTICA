/////Ultrasonico
#define TRIG_CEN A0
#define ECHO_CEN A1
#define TRIG_IZQ A2
#define ECHO_IZQ A3
#define TRIG_DER A4
#define ECHO_DER A5
////

/////Sensor piso
#define PISO_IZQ A6
#define PISO_DER A7
////

//////Leds
#define LED1 12
#define LED2 11
#define LED3 10
#define LED4 9
////

/////Boton
#define BOTON1 2
#define BOTON2 3
////

////Motores
#define MOTOR_IZQ_A 4
#define MOTOR_IZQ_B 7
#define MOTOR_DER_A 8
#define MOTOR_DER_B 16

#define ENABLE_IZQ 5
#define ENABLE_DER 6
///

////




void setup() {
  pinMode(TRIG_IZQ, OUTPUT);
  pinMode(ECHO_IZQ, INPUT);
  ///
  pinMode(TRIG_CEN, OUTPUT);
  pinMode(ECHO_CEN, INPUT);
  ///
  pinMode(TRIG_DER, OUTPUT);
  pinMode(ECHO_DER, INPUT);
  ///
  pinMode(PISO_IZQ, INPUT);
  pinMode(PISO_DER, INPUT);
  ///
  pinMode(LED1, OUTPUT);
  pinMode(LED2, OUTPUT);
  pinMode(LED3, OUTPUT);
  pinMode(LED4, OUTPUT);
  ///
  pinMode(BOTON1, INPUT_PULLUP);
  pinMode(BOTON2, INPUT_PULLUP);
  ///
  pinMode(MOTOR_IZQ_A, OUTPUT);
  pinMode(MOTOR_IZQ_B, OUTPUT);
  pinMode(MOTOR_DER_A, OUTPUT);
  pinMode(MOTOR_DER_B, OUTPUT);
  pinMode(ENABLE_DER, OUTPUT);
  pinMode(ENABLE_IZQ, OUTPUT);
  ///

  Serial.begin(9600);
}

void loop() {
  byte lectura_sensores = 0;

  bool pisoIzq  = leerSensorPiso(PISO_IZQ); // 1 si hay blanco
  bool pisoDer  = leerSensorPiso(PISO_DER);
  bool ultraIzq   = (leerUltrasonico(TRIG_IZQ, ECHO_IZQ) < 30);
  bool ultraCen = (leerUltrasonico(TRIG_CEN, ECHO_CEN) < 30);
  bool ultraDer   = (leerUltrasonico(TRIG_DER, ECHO_DER) < 30);

  lectura_sensores = (ultraIzq << 5) | (ultraCen << 4) |
                     (ultraDer << 3) | (pisoIzq << 2) | (pisoDer << 1);

}


////Sensor ultrasonico
long leerUltrasonico(int triggerPin, int echoPin) {
  digitalWrite(triggerPin, LOW);
  delayMicroseconds(2);

  digitalWrite(triggerPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(triggerPin, LOW);

  long duracion = pulseIn(echoPin, HIGH, 30000); // timeout de 30 ms

  // Si no recibió señal, devuelve un valor alto (sin detección)
  if (duracion == 0) {
    return 999; // Valor fuera de rango
  }

  // Convierte la duración a distancia (en cm)
  long distancia = duracion / 58;

  return distancia;
}
/////

////sensor piso
bool leerSensorPiso(int pin) {
  int lectura = analogRead(pin);
  return (lectura > 600); // devuelve true si detecta blanco (borde)
}
////

////Motores
void avanzar(int velocidad) {
  digitalWrite(MOTOR_IZQ_A, HIGH);
  digitalWrite(MOTOR_IZQ_B, LOW);
  analogWrite(ENEABLE_IZQ, velocidad);

  digitalWrite(MOTOR_DER_A, HIGH);
  digitalWrite(MOTOR_DER_B, LOW);
  analogWrite(ENEABLE_DER, velocidad);

}

void retroceder(int velocidad) {
  digitalWrite(MOTOR_IZQ_A, LOW);
  digitalWrite(MOTOR_IZQ_B, HIGH);
  analogWrite(ENEABLE_IZQ, velocidad);

  digitalWrite(MOTOR_DER_A, LOW);
  digitalWrite(MOTOR_DER_B, HIGH);
  analogWrite(ENEABLE_DER, velocidad);

}

void girarIzq(int velocidad) {
  digitalWrite(MOTOR_IZQ_A, LOW);
  digitalWrite(MOTOR_IZQ_B, HIGH);
  analogWrite(ENEABLE_IZQ, velocidad);

  digitalWrite(MOTOR_DER_A, HIGH);
  digitalWrite(MOTOR_DER_B, LOW);
  analogWrite(ENEABLE_DER, velocidad);

}

void girarDer(int velocidad) {
  digitalWrite(MOTOR_IZQ_A, HIGH);
  digitalWrite(MOTOR_IZQ_B, LOW);
  analogWrite(ENEABLE_IZQ, velocidad);

  digitalWrite(MOTOR_DER_A, LOW);
  digitalWrite(MOTOR_DER_B, HIGH);
  analogWrite(ENEABLE_DER, velocidad);

}

void detener() {
  digitalWrite(MOTOR_IZQ_A, LOW);
  digitalWrite(MOTOR_IZQ_B, LOW);
  digitalWrite(MOTOR_DER_A, LOW);
  digitalWrite(MOTOR_DER_B, LOW);

}
////
