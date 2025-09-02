#include <stdlib.h>

/////Ultrasonico

#define ECHO_CEN A0
#define TRIG_CEN A1
#define ECHO_IZQ A2
#define TRIG_IZQ A3
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
#define LED4 13
////

////Motores
#define MOTOR_IZQ_A 4
#define MOTOR_IZQ_B 7
#define MOTOR_DER_A 9
#define MOTOR_DER_B 8

#define ENABLE_IZQ 5
#define ENABLE_DER 6
///
///Funciones
long leerUltrasonico(int triggerPin, int echoPin);
int borde(int pin);
void avanzar(int velocidadIzq, int velocidadDer);
void retroceder(int velocidadIzq, int velocidadDer);
void girarIzq(int velocidadIzq, int velocidadDer);
void girarDer(int velocidadIzq, int velocidadDer);
void detener();

///NEW
void Lectura(void);
///

///
long distanciaVisorCen = 0;
long distanciaVisorIzq = 0;
long distanciaVisorDer = 0;
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
  pinMode(LED1, OUTPUT);
  pinMode(LED2, OUTPUT);
  pinMode(LED3, OUTPUT);
  pinMode(LED4, OUTPUT);
  //
  pinMode(MOTOR_IZQ_A, OUTPUT);
  pinMode(MOTOR_IZQ_B, OUTPUT);
  pinMode(MOTOR_DER_A, OUTPUT);
  pinMode(MOTOR_DER_B, OUTPUT);
  pinMode(ENABLE_DER, OUTPUT);
  pinMode(ENABLE_IZQ, OUTPUT);
  //
  Serial.begin(9600);
  Serial.println("Empezando codigo jijjiji");
  
  digitalWrite(LED1,HIGH);
  delay(1000);
  digitalWrite(LED2,HIGH);
  delay(1000);
  digitalWrite(LED3,HIGH);
  delay(100);
  digitalWrite(LED1,LOW);
  digitalWrite(LED2,LOW);
  digitalWrite(LED3,LOW);
  
  distanciaVisorCen = 0;
  distanciaVisorIzq = 0;
  distanciaVisorDer = 0;
}
//VARIABLES

long millisVD = 0;
long millisVC = 0;
long millisVI = 0;
int VisorDer = 0;
int VisorCen = 0;
int VisorIzq = 0;
int PisoDer = 0; 
int PisoIzq = 0;
//

void loop() {
  Serial.println("No estoy quemado");
}//Loop End
void Lectura(void){
  PisoDer = (borde(PISO_DER)<960);
  PisoIzq = (borde(PISO_IZQ)<960);
  
  VisorDer = leerUltrasonico(TRIG_DER, ECHO_DER);
  if(VisorDer == 999){
    VisorDer = distanciaVisorDer;
    }else{
     distanciaVisorDer = VisorDer;
    }
   if(VisorDer<= 30){
    VisorDer = 1;
   }
   if(VisorDer>31){
    VisorDer = 0;
   }
    
  VisorCen = leerUltrasonico(TRIG_CEN, ECHO_CEN);
  if(VisorCen == 999){
    VisorCen = distanciaVisorCen;
    }else{
     distanciaVisorCen = VisorCen;
    }
    if(VisorCen<= 30){
    VisorCen = 1;
   }
   if(VisorCen>31){
    VisorCen = 0;
   }
  VisorIzq = leerUltrasonico(TRIG_IZQ, ECHO_IZQ);
  if(VisorIzq == 999){
    VisorIzq = distanciaVisorIzq;
    }else{
     distanciaVisorIzq = VisorIzq;
    }
    if(VisorIzq<= 30){
    VisorIzq = 1;
   }
   if(VisorIzq>31){
    VisorIzq = 0;
   }
 
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

void girarDer(int velocidadIzq, int velocidadDer) {
  digitalWrite(MOTOR_IZQ_A, HIGH);
  digitalWrite(MOTOR_IZQ_B, LOW);
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

void detener() {
  digitalWrite(MOTOR_IZQ_A, LOW);
  digitalWrite(MOTOR_IZQ_B, LOW);
  digitalWrite(MOTOR_DER_A, LOW);
  digitalWrite(MOTOR_DER_B, LOW);
  analogWrite(ENABLE_IZQ, 0);  
  analogWrite(ENABLE_DER, 0);
}
