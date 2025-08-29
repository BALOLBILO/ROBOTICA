//#include <TimerOne.h>



/////Ultrasonico
#define ECHO_IZQ A0
#define TRIG_IZQ A1
#define ECHO_CEN A2
#define TRIG_CEN A3
#define TRIG_DER A4
#define ECHO_DER A5

//PROMEDIAR?? podria ser util pero hay que pensar bien fuerte la logica del promdio, capaz mañana al a mañana semeocurre
////

/////Sensor piso
#define PISO_IZQ A6
#define PISO_DER A7
#define UMBRAL 950
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
int sensorPiso (int pin);
//void ISR_Timer();

////
//Añadido el intervalo porque el sensor sino no se recarga:
long intervalo = 60;
long millisUltrasonidoIzq= 0;
long millisUltrasonidoCen= 0;
long millisUltrasonidoDer= 0;


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
  //Timer1.attachInterrupt(ISR_Timer);
  //Timer1.initialize(1000);  // 1 ms
  ///

  Serial.begin(9600);
}

int ValorIzq = 0;
int ValorCen = 0;
int ValorDer = 0;
int PisoDer = 0;
int PisoIzq = 0;
void loop() {

 

PisoIzq = sensorPiso(PISO_DER);
Serial.println(PisoIzq);
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

int sensorPiso (int pin){

return analogRead(pin);

  
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

    case 0b00000: // 0
      // Acción para 00000
      break;

    case 0b00001: // 1
      // Acción para 00001
      break;

    case 0b00010: // 2
      // Acción para 00010
      break;

    case 0b00011: // 3
      // Acción para 00011
      break;

    case 0b00100: // 4
      // Acción para 00100
      break;

    case 0b00101: // 5
      // Acción para 00101
      break;

    case 0b00110: // 6
      // Acción para 00110
      break;

    case 0b00111: // 7
      // Acción para 00111
      break;

    case 0b01000: // 8
      // Acción para 01000
      break;

    case 0b01001: // 9
      // Acción para 01001
      break;

    case 0b01010: // 10
      // Acción para 01010
      break;

    case 0b01011: // 11
      // Acción para 01011
      break;

    case 0b01100: // 12
      // Acción para 01100
      break;

    case 0b01101: // 13
      // Acción para 01101
      break;

    case 0b01110: // 14
      // Acción para 01110
      break;

    case 0b01111: // 15
      // Acción para 01111
      break;

    case 0b10000: // 16
      // Acción para 10000
      break;

    case 0b10001: // 17
      // Acción para 10001
      break;

    case 0b10010: // 18
      // Acción para 10010
      break;

    case 0b10011: // 19
      // Acción para 10011
      break;

    case 0b10100: // 20
      // Acción para 10100
      break;

    case 0b10101: // 21
      // Acción para 10101
      break;

    case 0b10110: // 22
      // Acción para 10110
      break;

    case 0b10111: // 23
      // Acción para 10111
      break;

    case 0b11000: // 24
      // Acción para 11000
      break;

    case 0b11001: // 25
      // Acción para 11001
      break;

    case 0b11010: // 26
      // Acción para 11010
      break;

    case 0b11011: // 27
      // Acción para 11011
      break;

    case 0b11100: // 28
      // Acción para 11100
      break;

    case 0b11101: // 29
      // Acción para 11101
      break;

    case 0b11110: // 30
      // Acción para 11110
      break;

    case 0b11111: // 31
      // Acción para 11111
      break;



  }
}
