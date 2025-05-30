#include <TimerOne.h>



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

int estadoBoton1 = ESPERA;
int msBoton1 = 0;

int estadoBoton2 = ESPERA;
int msBoton2 = 0;


////

////Motores
#define MOTOR_IZQ_A 4
#define MOTOR_IZQ_B 7
#define MOTOR_DER_A 8
#define MOTOR_DER_B 16

#define ENABLE_IZQ 5
#define ENABLE_DER 6
///

///AntiRebote
#define PULSADO LOW
#define N_PULSADO HIGH

#define T_REBOTE 10

#define ESPERA 10
#define CONFIRMACION 11
#define LIBERACION 12

int mSeg = 0;



///



void codigo (byte sensores);



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
  Timer1.attachInterrupt(ISR_Timer);
  Timer1.initialize(1000);  // 1 ms
  ///

  Serial.begin(9600);
}

void loop() {
  byte lectura_sensores = 0;

  bool pisoIzq  = borde(PISO_IZQ); // 1 si hay blanco
  bool pisoDer  = borde(PISO_DER);
  bool ultraIzq   = (leerUltrasonico(TRIG_IZQ, ECHO_IZQ) < 30);
  bool ultraCen = (leerUltrasonico(TRIG_CEN, ECHO_CEN) < 30);
  bool ultraDer   = (leerUltrasonico(TRIG_DER, ECHO_DER) < 30);

  lectura_sensores = (ultraIzq << 5) | (ultraCen << 4) |
                     (ultraDer << 3) | (pisoIzq << 2) | (pisoDer << 1);

  codigo(lectura_sensores);

/*
  if (antiRebote(BOTON1, estadoBoton1, msBoton1)) {
  Serial.println("Botón 1 presionado");
  // Ejecutar algo
}

if (antiRebote(BOTON2, estadoBoton2, msBoton2)) {
  Serial.println("Botón 2 presionado");
  // Ejecutar algo
}
*/

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
bool borde(int pin) {
  int lectura = analogRead(pin);
  return (lectura > 600); // devuelve true si detecta blanco (borde)
}
////

////Motores
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
////


///antirebote
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
//

///timer
void ISR_Timer() {
  msBoton1++;
  msBoton2++;
}
////


////
void codigo (byte sensores) {
  switch (sensores) {

    case 0b00000:

      break;
  }
}
