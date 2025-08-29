//#include <TimerOne.h>



/////Ultrasonico
#define TRIG_IZQ A3
#define ECHO_IZQ A2
#define TRIG_CEN A4
#define ECHO_CEN A5
#define TRIG_DER A1
#define ECHO_DER A0
////

/////Sensor piso
#define PISO_IZQ A6
#define PISO_DER A7
////

//////Leds
#define LED1 12
#define LED2 11
#define LED3 10
#define LED4 13
////



////Motores
#define MOTOR_IZQ_A 8
#define MOTOR_IZQ_B 9
#define MOTOR_DER_A 7
#define MOTOR_DER_B 4

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
int borde(int pin) ;
void sensorPiso(int pin);
void avanzar(int velocidadIzq, int velocidadDer);
void retroceder(int velocidadIzq, int velocidadDer);
void girarIzq(int velocidadIzq, int velocidadDer);
void girarDer(int velocidadIzq, int velocidadDer);
void detener();
bool antiRebote(int pin, int &estadoMaquina, int &msBoton);
//void ISR_Timer();



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
  pinMode(13, OUTPUT);
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
  //Timer1.attachInterrupt(ISR_Timer);
  //Timer1.initialize(1000);  // 1 ms
  ///

  Serial.begin(9600);
  Serial.println("48 Horas para Trelew");
}

long millisVD = 0;
long millisVC = 0;
long millisVI = 0;
int VisorDer = 0;
int VisorCen = 0;
int VisorIzq = 0;
int PisoDer = 0; 
int PisoIzq = 0;

void loop() {
  digitalWrite(13, HIGH);

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
int borde(int pin) {
  int lectura = analogRead(pin);
  return lectura ; // devuelve true si detecta blanco (borde)
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
//void ISR_Timer() {
  //msBoton1++;
  //msBoton2++;
  //msBotonEspera++;

  //if (esperandoEjecucion) {
    //tiempoDesdePresionado++;
  //}

//}
////


////
void codigo (byte sensores) {
  switch (sensores) {

    case 0b00000: // BUSCAR
      avanzar(255,255);
      Serial.println("Avanzar 1");
      break;

    case 0b00001: // PISO DERECHA
      girarIzq(255,255);
      Serial.println("Girar Izquierda 1");
      break;

    case 0b00010: // PISO IZQUIERDA
      girarDer(255,255);
      Serial.println("Girar Derecha 1");
      break;

    case 0b00011: // DOBLE PISO
      retroceder(255,255);
      Serial.println("Retroceder 1");
      break;

    case 0b00100: // VISION DERECHA
      girarDer(255,255);
      Serial.println("Girar Derecha 2");
      break;

    case 0b00101: // VISION DERECHA Y SENSOR DERECHA
      girarIzq(255,255);
      Serial.println("Girar Izquierda 2");
      break;

    case 0b00110: // VISION DERECHA Y SENSOR IZQUIERDA
      girarDer(255,255);
      Serial.println("Girar Derecha 3");
      break;

    case 0b00111: // VISION DERECHA Y AMBOS SENSORES
      retroceder(255,255);
      Serial.println("Retroceder 2");
      break;

    case 0b01000: // SENSOR CENTRO
      avanzar(255,255);
      Serial.println("Avanzar 2");
      break;

    case 0b01001: // SENSOR CENTRO Y PISO DERECHA
      girarIzq(255,255);
      Serial.println("Girar Izquierda 3");
      break;

    case 0b01010: // SENSOR CENTRO Y PISO IZQUIERDA
      girarDer(255,255);
      Serial.println("Girar Derecha 4");
      break;

    case 0b01011: // SENSOR CENTRO Y AMBOS PISOS
      retroceder(255,255);
      Serial.println("Retroceder 3");
      break;

    case 0b01100: // SENSOR CENTRO Y SENSOR DERECHA
      avanzar(255,255);
      Serial.println("Avanzar 3");
      break;

    case 0b01101: // SENSOR CENTRO Y SENSOR DERECHA PISO DER
      girarIzq(255,255);
      Serial.println("Girar Izquierda 4");
      break;

    case 0b01110: // SENSOR CENTRAL SENSOR DERECHA Y PISO IZQ
      girarDer(255,255);
      Serial.println("Girar Derecha 5");
      break;

    case 0b01111: // SENSOR CENTRAL SENSOR DERECHA AMBOS PISOS
      retroceder(255,255);
      Serial.println("Retroceder 4");
      break;

    case 0b10000: // SENSOR IZQUIERDA
      girarIzq(255,255);
      Serial.println("Girar Izquierda 5");
      break;

    case 0b10001: // SENSOR IZQUIERDA PISO DERECHA
      girarIzq(255,255);
      Serial.println("Girar Izquierda 6");
      break;

    case 0b10010: // SENSOR IZQUIERDA PISO IZQUIERDA
      girarDer(255,255);
      Serial.println("Girar Derecha 6");
      break;

    case 0b10011: // SENSOR IZQUIERDA AMBOS PISOS
      retroceder(255,255);
      Serial.println("Retroceder 5");
      break;

    case 0b10100: // SENSOR IZQUIERDA SENSOR DERECHA
      girarIzq(255,255);
      Serial.println("Girar Izquierda 7");
      break;

    case 0b10101: // SENSOR IZQUIERDA SENSOR DERECHA PISO DER
      girarIzq(255,255);
      Serial.println("Girar Izquierda 8");
      break;

    case 0b10110: // 22
      girarDer(255,255);
      Serial.println("Girar Derecha 7");
      break;

    case 0b10111: // 23
      retroceder(255,255);
      Serial.println("Retroceder 6");
      break;

    case 0b11000: // 24
      avanzar(255,255);
      Serial.println("Avanzar 4");
      break;

    case 0b11001: // 25
      girarIzq(255,255);
      Serial.println("Girar Izquierda 9");
      break;

    case 0b11010: // 26
      girarDer(255,255);
      Serial.println("Girar Derecha 8");
      break;

    case 0b11011: // 27
      retroceder(255,255);
      Serial.println("Retroceder 7");
      break;

    case 0b11100: // 28
      avanzar(255,255);
      Serial.println("Avanzar 5");
      break;

    case 0b11101: // 29
      girarIzq(255,255);
      Serial.println("Girar Izquierda 10");
      break;

    case 0b11110: // 30
      girarDer(255,255);
      Serial.println("Girar Derecha 9");
      break;

    case 0b11111: // 31
      retroceder(255,255);
      Serial.println("Retroceder 8");
      break;
  }
}
