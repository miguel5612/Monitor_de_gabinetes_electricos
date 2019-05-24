
boolean estadoLed = false;  //guarda el estado del led (encendido o apagado)
unsigned long periodo = 30*1000;  // tiempo que esta el LED en alto y bajo
unsigned long tiempoAnterior = 0;  //guarda tiempo de referencia para comparar

void setup() {
    pinMode(13,OUTPUT);  //configura la salida
}

void loop() {
  ControlEspera(periodo, 1);
}

void ControlEspera(unsigned long Pausa, int numPuerta)
{
  if(millis()-tiempoAnterior>=Pausa){  //si ha transcurrido el periodo programado
    estadoLed=!estadoLed;  //cambia el estado del led cada 100ms
    digitalWrite(13,estadoLed);  //actualiza el estado del led
    tiempoAnterior=millis();  //guarda el tiempo actual como referencia
 }
}


