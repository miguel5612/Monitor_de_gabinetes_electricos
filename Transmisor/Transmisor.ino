
/* Programa para el modulo transmisor
   Cada transmisor debe llevar la identificacion de su gabinete tal y como se muestra la sintaxis en este ejemplo(separado por espacios y guion), 
   de la misma manera se debe escribir en el programa del modulo receptor 
*/

#include <SoftwareSerial.h>
#include <TimerOne.h>

#define IdModulo "MT 3-1 (1T08686)"         // Identificacion del gabinete

#define lRx 3                               // pines de recepcion y transmision del modulo LoRa en el Arduino 
#define lTx 2

bool depuracionSerial = true;               //Funcion para visualizar por el puerto serie

unsigned long  ContadorActivaciones = 0;       //Acumula las veces que se ha activado la interrupcion

#define puerta1 4                           //Definicion de pines para los sensores de las puertas 
#define puerta2 5
#define puerta3 6
#define puerta4 7

#define ledArduino 13                       //Pin para el led indicador del Arduino

#define TiempoEspera 0.2*60*1000              //variable para el control de tiempos de espera  10seg



SoftwareSerial loraSerial(lRx, lTx);        // Pines de comunicacion con el LoRa


void setup() {

  pinMode(puerta1, INPUT_PULLUP);           // Inicializacion de las entradas
  pinMode(puerta2, INPUT_PULLUP);
  pinMode(puerta3, INPUT_PULLUP);
  pinMode(puerta4, INPUT_PULLUP);

 
  pinMode(ledArduino, OUTPUT);                 //Salida para el led 13
  digitalWrite(ledArduino, HIGH);

  loraSerial.begin(9600);                     //inicializacion puerto serie del LORA
  Serial.begin(9600);                         //Puerto serie debug


  //Preparacion del timer que envia un mensaje avisando que esta en funcioamiento
  Timer1.initialize(5000000);                      // Activa la interrupcion cada 5 segundos
  Timer1.attachInterrupt(ConteodeTiempo);          //Activa la funcion Conteo de tiempo

}


void ConteodeTiempo()                                     // Funcion activada por timer interno ---- Se enviara un SMS cada 8 horas
{
  ContadorActivaciones = ContadorActivaciones + 1;        //Cuenta las veces que se ha activado la interrupcion
 // Serial.print("Contador de activaciones: ");            ////////////////////////////////////
 // Serial.println(ContadorActivaciones);                  ///////////////////////////////////

  if (ContadorActivaciones == 8640) {                     //8640*5seg= 43200seg= 12horas
    MensajedeEstado("");
    //Serial.println(ContadorActivaciones);               ////////////////////////////
    ContadorActivaciones = 0;
  }
}


void MensajedeEstado(String mensajeIn)              // Funcion que envia el mensaje del estado de funcionamiento
{
  String mensaje = String(IdModulo) + " - " + "Reporta estado: " + (mensajeIn != "" ? "Reiniciado" : "Encendido") + " y en funcionamiento" + (mensajeIn != "" ? " - " : "") + String(mensajeIn);
  loraSerial.println(mensaje);
  imprimirPuertoSerie("Enviando SMS por que: " + mensaje);        // Imprime por el monitor serial
}


void loop() {

  int puerta = leerPuertas();                  //Leer sensores de puertas

  if (puerta != 0)
  {
    EnviarAlerta(puerta);
    ControlEspera(TiempoEspera, puerta);
  }
}


int leerPuertas()                               //Funcion para leer el estado de los sensores de las puertas
{
  if (digitalRead(puerta1) == HIGH)
  {
    return 1;
  }
  else if (digitalRead(puerta2) == HIGH)
  {
    return 2;
  }
  else if (digitalRead(puerta3) == HIGH)
  {
    return 3;
  }
  else if (digitalRead(puerta4) == HIGH)
  {
    return 4;
  }
  else
  {
    return 0;
  }
}


void EnviarAlerta(int numeroPuerta)                //Funcion para enviar la alerta de activacion
{
  String mensaje = String(IdModulo) + " - " + String(numeroPuerta) + " ha sido activado";             
  
  loraSerial.println(mensaje);                    //Envia al modulo RF
  Serial.println(mensaje);             /////////////////////////////////////////
  
  imprimirPuertoSerie("Enviando SMS por que: " + mensaje);         
}



void ControlEspera(unsigned long Pausa, int numPuerta)
  {
    
  imprimirPuertoSerie("Esperando " + String(Pausa) + " milisegundos.");
  
  unsigned long ProximaVez = millis() + Pausa; 
  
  while (millis() <= ProximaVez)
  {
    //imprimirPuertoSerieNCR(".");             ////////////////////////////////
    ParpadeoLed(numPuerta, Pausa);
  }
  imprimirPuertoSerie("");
  imprimirPuertoSerie("Monitoreando ahora!");
  digitalWrite(ledArduino, HIGH);

  }

void ParpadeoLed(int numPuerta, int Pausa)          // Funcion para activar el led indicador de la tarjeta Arduino (13)
{
  if (numPuerta != 0)
  {
    for (int i = 1; i <= numPuerta; i++)
  
    {
      digitalWrite(ledArduino, HIGH);
      delay(Pausa / 50);                              //tiempo era 12000/50
      digitalWrite(ledArduino, LOW);
      delay(Pausa / 50);
    }
  }
  delay(Pausa / 5);                                 // tiempo era 12000/5
}




//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////7

void imprimirPuertoSerie(String mensaje)          // Funcion para visualizar por el monitor serial
{
  if (depuracionSerial)
  {
    Serial.println(mensaje);
  }
}

void imprimirPuertoSerieNCR(String mensaje)
{
  if (depuracionSerial)
  {
    Serial.print(mensaje);
  }
}
