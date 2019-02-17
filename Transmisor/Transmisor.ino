
#include <SoftwareSerial.h>
#include <TimerOne.h>

#define codigoModulo "MT 3-1 (1T08686)"
#define lRx 2
#define lTx 3

bool depuracionSerial = true;
int timerCount = 0;

#define puerta1 4
#define puerta2 5
#define puerta3 6
#define puerta4 7

#define ledArduino 13
#define timeDelay 0.20*60*1000

SoftwareSerial loraSerial(lRx, lTx);   // Pines de comunicacion con el LoRa

void setup() {
  // Inicializacion de las entradas
  pinMode(puerta1,INPUT_PULLUP);
  pinMode(puerta2,INPUT_PULLUP);
  pinMode(puerta3,INPUT_PULLUP);
  pinMode(puerta4,INPUT_PULLUP);
  //Salidas
  pinMode(ledArduino,OUTPUT);
  //inicializacion puerto serie del LORA
  loraSerial.begin(9600);
  //Puerto serie debug
  Serial.begin(9600);
  //Preparacion del timer que envia un mensaje avisando que esta en funcioamiento
  Timer1.initialize(10000000);             // Activa la interrupcion cada 10 segundos
  Timer1.attachInterrupt(countertimer);    // Cuenta el tiempo

  imprimirPuertoSerie("Programa iniciado - Monitoreando");
  mensajeStatus("Programa iniciado - Monitoreando");
  digitalWrite(ledArduino,HIGH);
}

void countertimer(void)                 // Funcion activada por timer interno
{
  timerCount = timerCount + 1;
  if (timerCount == 6) {
    mensajeStatus("");
    timerCount = 0;
  }
}

void loop() {
  // put your main code here, to run repeatedly:
  //Leer interruptores de puerta
  int puerta = leerPuertas();
  if(puerta!=0)
  {
    enviarAlerta(puerta);
    smartDelay(timeDelay, puerta);
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
int leerPuertas()
{
  if(digitalRead(puerta1) == LOW)
  {
    return 1;
  }
  else if(digitalRead(puerta2) == LOW)
  {
    return 2;
  }else if(digitalRead(puerta3) == LOW)
  {
    return 3;
  }else if(digitalRead(puerta4) == LOW)
  {
    return 4;
  }
  else
  {
    return 0;
  }
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
