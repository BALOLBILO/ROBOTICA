/////Ultrasonico
#define TRIG_IZQ A1
#define ECHO_IZQ A0
#define TRIG_CEN A3
#define ECHO_CEN A2
#define TRIG_DER A4
#define ECHO_DER A5

long leerUltrasonico(int triggerPin, int echoPin);

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


  Serial.begin(9600);
  Serial.println("48 Horas para Trelew");
}

int VisorDer = 0;
int VisorCen = 0;
int VisorIzq = 0;

long millisVD = 0;
long millisVC = 0;
long millisVI = 0;


void loop() {
if (millis() - millisVD >= 60){
  VisorDer = leerUltrasonico(TRIG_DER,ECHO_DER);
  millisVD = millis();
}
if (millis() - millisVC >= 60){
  VisorCen = leerUltrasonico(TRIG_CEN,ECHO_CEN);
  millisVC = millis();
}
if (millis() - millisVI >= 60){
  VisorIzq = leerUltrasonico(TRIG_IZQ,ECHO_IZQ);
  millisVI = millis();
}

Serial.print("Sensores: Izq: ");
Serial.print(VisorIzq);
Serial.print("  Cen ");
Serial.print(VisorCen);
Serial.print("  Der  ");
Serial.println(VisorDer);

}


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
