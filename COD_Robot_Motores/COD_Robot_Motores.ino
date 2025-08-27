
#include <TimerOne.h>

#define LED1 12
#define LED2 11
#define LED3 10
#define LED4 9

#define MOTOR_IZQ_A 4
#define MOTOR_IZQ_B 7
#define MOTOR_DER_A 8
#define MOTOR_DER_B 16

#define ENABLE_IZQ 5
#define ENABLE_DER 6
#define PULSADO LOW

#define N_PULSADO HIGH

#define T_REBOTE 10

#define ESPERA 10
#define CONFIRMACION 11
#define LIBERACION 12

int mSeg = 0;

#define BOTON1 2
#define BOTON2 3

int estadoBoton1 = ESPERA;
int msBoton1 = 0;

int estadoBoton2 = ESPERA;
int msBoton2 = 0;


int  msBotonEspera = 0;
bool esperandoEjecucion = false;
int tiempoDesdePresionado = 0;

void setup() {
  
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
  Timer1.attachInterrupt(ISR_Timer);
  Timer1.initialize(1000);  // 1 ms
  ///

  Serial.begin(9600);
}
void loop() {
avanzar(255,255);
delay(2000);
detener();
delay(2000);
retroceder(255,255);
delay(2000);
girarIzq(255,255);
delay(2000);
girarDer(255,255);
delay(2000);
}

void avanzar(int velocidadIzq, int velocidadDer) {
  digitalWrite(MOTOR_IZQ_A, HIGH);
  digitalWrite(MOTOR_IZQ_B, LOW);
  analogWrite(ENABLE_IZQ, velocidadIzq);

  digitalWrite(MOTOR_DER_A, HIGH);
  digitalWrite(MOTOR_DER_B, LOW);
  analogWrite(ENABLE_DER, velocidadDer);

}
void retroceder(int velocidadIzq, int velocidadDer) {
  digitalWrite(MOTOR_IZQ_A, LOW);
  digitalWrite(MOTOR_IZQ_B, HIGH);
  analogWrite(ENABLE_IZQ, velocidadIzq);

  digitalWrite(MOTOR_DER_A, LOW);
  digitalWrite(MOTOR_DER_B, HIGH);
  analogWrite(ENABLE_DER, velocidadDer);

}
void girarIzq(int velocidadIzq, int velocidadDer) {
  digitalWrite(MOTOR_IZQ_A, LOW);
  digitalWrite(MOTOR_IZQ_B, HIGH);
  analogWrite(ENABLE_IZQ, velocidadIzq);

  digitalWrite(MOTOR_DER_A, HIGH);
  digitalWrite(MOTOR_DER_B, LOW);
  analogWrite(ENABLE_DER, velocidadDer);

}
void girarDer(int velocidadIzq, int velocidadDer) {
  digitalWrite(MOTOR_IZQ_A, HIGH);
  digitalWrite(MOTOR_IZQ_B, LOW);
  analogWrite(ENABLE_IZQ, velocidadIzq);

  digitalWrite(MOTOR_DER_A, LOW);
  digitalWrite(MOTOR_DER_B, HIGH);
  analogWrite(ENABLE_DER, velocidadDer);

}
void detener() {
  digitalWrite(MOTOR_IZQ_A, LOW);
  digitalWrite(MOTOR_IZQ_B, LOW);
  digitalWrite(MOTOR_DER_A, LOW);
  digitalWrite(MOTOR_DER_B, LOW);

}
bool antiRebote(int pin, int &estadoMaquina, int &msBoton) {
  bool lectura = digitalRead(pin);

  switch (estadoMaquina) {
    case ESPERA:
      if (lectura == PULSADO) {
        msBoton = 0;
        estadoMaquina = CONFIRMACION;
      }
      break;

    case CONFIRMACION:
      if (msBoton >= T_REBOTE) {
        if (lectura == PULSADO) {
          estadoMaquina = LIBERACION;
        } else {
          estadoMaquina = ESPERA;
        }
      }
      break;

    case LIBERACION:
      if (lectura == N_PULSADO) {
        estadoMaquina = ESPERA;
        return true;  // flanco detectado
      }
      break;
  }

  return false;  // no flanco
}
void ISR_Timer() {
  msBoton1++;
  msBoton2++;
  msBotonEspera++;

  if (esperandoEjecucion) {
    tiempoDesdePresionado++;
  }

}
