// Sensores de distancia
#define SENSOR_FRONTAL_IZQ A4
#define SENSOR_FRONTAL_CENTRO A2
#define SENSOR_FRONTAL_DER A1
#define SENSOR_LATERAL_IZQ A3
#define SENSOR_LATERAL_DER A0
// Sensores de línea
#define CNY_IZQ PORTD5
#define CNY_DER PORTD6
// Motores
#define MOTOR_IZQ_AV PORTB4
#define MOTOR_IZQ_RV PORTB3
#define MOTOR_DER_AV PORTB2
#define MOTOR_DER_RV PORTB1
// Led de inicio del programa
#define LED PORTB0
// Botón de inicio
#define BOTON_INICIO PORTB5

const byte MAXIMA_DISTANCIA = 60;

void setup() {
  DDRB = 0b011111;
  DDRD = 0b10011111;
  DDRC = 0b100000;
  
  while (!(PINB & _BV(BOTON_INICIO)));

  for (byte i = 0; i < 10; i++) {
    PORTB ^= _BV(LED);
    delay(500);
  }

  girar_izquierda();
  buscar_oponente();
}

int distancia(byte sensor_pin) {
  int adc = 0;
  byte num_lecturas = 10;
  
  for (byte i = 0; i < num_lecturas; i++) {
    adc += analogRead(sensor_pin);
  }  
  
  adc /= num_lecturas;
  float distancia_cm = 13469.6309 * pow(adc, -1.1403);
  
  return distancia_cm;
}

void avanzar() {
  PORTB = (PORTB | (_BV(MOTOR_IZQ_AV) | _BV(MOTOR_DER_AV))) & ~(_BV(MOTOR_IZQ_RV) | _BV(MOTOR_DER_RV));
}

void retroceder() {  
  PORTB = (PORTB | (_BV(MOTOR_IZQ_RV) | _BV(MOTOR_DER_RV))) & ~(_BV(MOTOR_IZQ_AV) | _BV(MOTOR_DER_AV));
}

void girar_derecha() {  
  PORTB = (PORTB | (_BV(MOTOR_IZQ_AV) | _BV(MOTOR_DER_RV))) & ~(_BV(MOTOR_IZQ_RV) | _BV(MOTOR_DER_AV));
}

void girar_izquierda() {  
  PORTB = (PORTB | (_BV(MOTOR_IZQ_RV) | _BV(MOTOR_DER_AV))) & ~(_BV(MOTOR_IZQ_AV) | _BV(MOTOR_DER_RV));
}

void buscar_oponente() {
  int sensor_frontal_centro = distancia(SENSOR_FRONTAL_CENTRO);
  const uint32_t TIEMPO_INICIO_GIRO = millis();
  const int MAXIMO_TIEMPO_GIRO_MS = 1000;

  while ((millis() < TIEMPO_INICIO_GIRO + MAXIMO_TIEMPO_GIRO_MS) && !(sensor_frontal_centro <= MAXIMA_DISTANCIA)) {
    sensor_frontal_centro = distancia(SENSOR_FRONTAL_CENTRO);
  }

  avanzar();  
}

void loop() {
  if (PIND & (_BV(CNY_IZQ) | _BV(CNY_DER))) {
    retroceder();
    delay(600);
    girar_derecha();
    delay(450);
    avanzar();
  }

  int sensor_frontal_izq = distancia(SENSOR_FRONTAL_IZQ);
  int sensor_frontal_centro = distancia(SENSOR_FRONTAL_CENTRO);
  int sensor_frontal_der = distancia(SENSOR_FRONTAL_DER);
  int sensor_lateral_izq = distancia(SENSOR_LATERAL_IZQ);
  int sensor_lateral_der = distancia(SENSOR_LATERAL_DER);

  if (sensor_frontal_izq <= MAXIMA_DISTANCIA) {
    girar_izquierda();
    buscar_oponente();
  }
  else if (sensor_frontal_der <= MAXIMA_DISTANCIA) {
    girar_derecha();
    buscar_oponente();
  }
  else if (sensor_lateral_izq <= MAXIMA_DISTANCIA) {
    girar_izquierda();
    buscar_oponente();
  }
  else if (sensor_lateral_der <= MAXIMA_DISTANCIA) {
    girar_derecha();
    buscar_oponente();
  }
}
