
#include <SoftwareSerial.h>
#include <TimerOne.h>

#define codigoModulo "MT 3-1 (1T08686)"
#define lRx 2
#define lTx 3


#define s808Rx 4
#define s808Tx 5

bool depuracionSerial = true;

#define ledArduino 13
#define timeDelay 0.20*60*1000

SoftwareSerial loraSerial(lRx, lTx);   // Pines de comunicacion con el LoRa
SoftwareSerial simSerial(s808Rx, s808Tx);   // Pines de comunicacion con el Sim808

void setup() {
  //Salidas
  pinMode(ledArduino,OUTPUT);
  //inicializacion puerto serie del LORA
  loraSerial.begin(9600);
  simSerial.begin(9600);
  //Puerto serie debug
  Serial.begin(9600);

  imprimirPuertoSerie("Programa iniciado - esperando llegada de nuevos mensajes");
  mensajeStatus("Programa iniciado - esperando llegada de nuevos mensajes");
  digitalWrite(ledArduino,HIGH);
}

void loop() {
  // put your main code here, to run repeatedly:
  //Leer interruptores de puerta
  String texto =  "";
  if ( loraSerial.available() )   {
    texto =   loraSerial.readString();
  }
  if(texto!="")
  {
    imprimirPuertoSerie(texto);
  }
}
void ledBlink(int numPuerta,int timeWait)
{
  if(numPuerta!=0)
  {
    for(int i = 1; i<=numPuerta; i++)
    {
       digitalWrite(ledArduino,HIGH);
        delay(timeWait/50);
        digitalWrite(ledArduino,LOW);
        delay(timeWait/50);
    }
  }
  delay(timeWait/5);
}
void mensajeStatus(String mensajeIn)
{
  String mensaje = String(codigoModulo) + " - " + "Reporta estado: " + (mensajeIn!=""?"Reiniciado":"Encendido") + " y en funcionamiento" + (mensajeIn!=""?" - ":"") + String(mensajeIn);
  loraSerial.println(mensaje);
  imprimirPuertoSerie("Enviando SMS por que: " + mensaje);
}
void smartDelay(int timeWait, int numPuerta)
{
  imprimirPuertoSerie("Esperando " + String(timeWait) + " mS.");
  unsigned long nextTime = millis() + timeWait;
  while(millis()<=nextTime)
  {
    imprimirPuertoSerieNCR(".");
    ledBlink(numPuerta,timeWait);
  }
  imprimirPuertoSerie("");
  imprimirPuertoSerie("Monitoreando ahora!");
  digitalWrite(ledArduino,HIGH);
}

void enviarAlerta(int numeroPuerta)
{
  String mensaje = String(codigoModulo) + " - " + String(numeroPuerta);
  loraSerial.println(mensaje);
  imprimirPuertoSerie("Enviando SMS por que: " + mensaje);
}
void imprimirPuertoSerie(String mensaje)
{
  if(depuracionSerial)
  {
    Serial.println(mensaje);
  }
}
void imprimirPuertoSerieNCR(String mensaje)
{
  if(depuracionSerial)
  {
    Serial.print(mensaje);
  }
}
