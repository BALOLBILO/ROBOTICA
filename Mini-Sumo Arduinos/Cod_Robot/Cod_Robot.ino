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

/////Boton
#define BOTON1 2
#define BOTON2 3

int estadoBoton1 = ESPERA;
int msBoton1 = 0;

int estadoBoton2 = ESPERA;
int msBoton2 = 0;


int  msBotonEspera = 0;
bool esperandoEjecucion = false;
int tiempoDesdePresionado = 0;

////



void codigo (byte sensores);
long leerUltrasonico(int triggerPin, int echoPin);
bool borde(int pin) ;
void sensorPiso(int pin);
void avanzar(int velocidadIzq, int velocidadDer);
void retroceder(int velocidadIzq, int velocidadDer);
void girarIzq(int velocidadIzq, int velocidadDer);
void girarDer(int velocidadIzq, int velocidadDer);
void detener();
bool antiRebote(int pin, int &estadoMaquina, int &msBoton);
void ISR_Timer();



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
  delay(5000);
}

void loop() {

  byte lectura_sensores = 0;
  int LecturaIzq = leerUltrasonico(TRIG_IZQ, ECHO_IZQ);
  int LecturaDer = leerUltrasonico(TRIG_DER, ECHO_DER);
  int LecturaCen = leerUltrasonico(TRIG_CEN, ECHO_CEN);
  bool pisoIzq  = borde(PISO_IZQ); // 1 si hay blanco
  bool pisoDer  = borde(PISO_DER);
  bool ultraIzq   = (LecturaIzq < 30&&LecturaIzq!= 999);
  bool ultraCen = (leerUltrasonico(TRIG_CEN, ECHO_CEN) < 30);
  bool ultraDer   = (leerUltrasonico(TRIG_DER, ECHO_DER) < 30);

  lectura_sensores = (ultraIzq << 4) | (ultraCen << 3) |
                     (ultraDer << 2) | (pisoIzq << 1) | (pisoDer);

    codigo(lectura_sensores);
    Serial.println(lectura_sensores);

 
    /*
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
  msBotonEspera++;

  if (esperandoEjecucion) {
    tiempoDesdePresionado++;
  }

}
////


////
void codigo (byte sensores) {
  switch (sensores) {

    case 0b00000: // BUSCAR
      // Acción para 00000
      avanzar(255,255);
      
      break;

    case 0b00001: // PISO DERECHA
      // Acción para 00001
      girarIzq(255,255);
      break;

    case 0b00010: // PISO IZQUIERDA
      // Acción para 00010
      girarDer(255,255);
      break;

    case 0b00011: // DOBLE PISO
      // Acción para 00011
      retroceder(255,255);
      break;

    case 0b00100: // VISION DERECHA
      // Acción para 00100
      girarDer(255,255);
      break;

    case 0b00101: // VISION DERECHA Y SENSOR DERECHA
      // Acción para 00101
      girarIzq(255,255);
      break;

    case 0b00110: // VISION DERECHA Y SENSOR IZQUIERDA
      // Acción para 00110
      girarDer(255,255);
      break;

    case 0b00111: // VISION DERECHA Y AMBOS SENSORES
      // Acción para 00111
      retroceder(255,255);
      break;

    case 0b01000: // SENSOR CENTRO
      // Acción para 01000
      avanzar(255,255);
      break;

    case 0b01001: // SENSOR CENTRO Y PISO DERECHA
      // Acción para 01001
      girarIzq(255,255);
      break;

    case 0b01010: // SENSOR CENTRO Y PISO IZQUIERDA
      // Acción para 01010
      girarDer(255,255);
      break;

    case 0b01011: // SENSOR CENTRO Y AMBOS PISOS
      // Acción para 01011
      retroceder(255,255);
      break;

    case 0b01100: // SENSOR CENTRO Y SENSOR DERECHA
      // Acción para 01100
      avanzar(255,255);
      break;

    case 0b01101: // SENSOR CENTROR Y SENSOR DERECHA PIZO DER
      // Acción para 01101
      girarIzq(255,255);
      break;

    case 0b01110: // SENSOR CENTRAL SENSOR DERECHA Y PISO IZQ
      // Acción para 01110
      girarDer(255,255);
      break;

    case 0b01111: // SENSOR CENTRAL SENSOR DERECHA AMBOS PISOS
      // Acción para 01111
      retroceder(255,255);
      break;

    case 0b10000: // SENSOR IZQUIERDA
      // Acción para 10000
      girarIzq(255,255);
      break;

    case 0b10001: // SENSOR IZQUIERDA PISO DERECHA
      // Acción para 10001
      girarIzq(255,255);
      break;

    case 0b10010: // SENSOR IZQUIERDA PISO IZQUIERDA
      // Acción para 10010
      girarDer(255,255);
      break;

    case 0b10011: // SENSOR IZQUIERDA AMBOS PISOS
      // Acción para 10011
      retroceder(255,255);
      break;

    case 0b10100: // SENSOR IZQUIERDA SENSOR DERECHA
      // Acción para 10100
      girarIzq(255,255);
      break;

    case 0b10101: // SENSOR IZQUIERDA SENSOR DERECHA PISO DER
      // Acción para 10101
      girarIzq(255,255);
      break;

    case 0b10110: // 22
      // Acción para 10110
      girarDer(255,255);
      break;

    case 0b10111: // 23
      // Acción para 10111
      retroceder(255,255);
      break;

    case 0b11000: // 24
      // Acción para 11000
      avanzar(255,255);
      break;

    case 0b11001: // 25
      // Acción para 11001
      girarIzq(255,255);
      break;

    case 0b11010: // 26
      // Acción para 11010
      girarDer(255,255);
      break;

    case 0b11011: // 27
      // Acción para 11011
      retroceder(255,255);
      break;

    case 0b11100: // 28
      // Acción para 11100
      avanzar(255,255);
      break;

    case 0b11101: // 29
      // Acción para 11101
      girarIzq(255,255);
      break;

    case 0b11110: // 30
      // Acción para 11110
      girarDer(255,255);
      break;

    case 0b11111: // 31
      // Acción para 11111
      retroceder(255,255);
      break;



  }
}
