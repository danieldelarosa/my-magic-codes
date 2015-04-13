// Orlando Almario - Emerge Tecnologias

/* Este programa es un ejemplo de como crear un gateway panstamp bastante sencillo
   Recibe paquetes e imprime por consola cuando ha recibido uno, tambien envia 
   distintos paquetes cuando se oprimen las teclas 1, 2 o 3 del teclado.
   
   este sketch debe usarse en conjunto con el archivo semilla_panstamp_test
*/


// la libreria de panstamp no incluye comunicacion por serial por defecto, aqui la agregamos
#include "HardwareSerial.h"
#include "swap.h"

// definiendo las variables de comunicacion para la panstamp
#define RFCHANNEL  0  //canal de comunicacion
#define SYNCWORD1  0xB5 // palabra de seguridad uno
#define SYNCWORD0 0x47  //palabra de seguridad dos
#define SOURCE_ADDR 4   //Direccion del dispositivo


// aqui declaramos los paquetes que enviaremos a la semilla para encender cada uno de los leds
CCPACKET packetA;
CCPACKET packetB;
CCPACKET packetC;


// esta funcion sera ejecutada cada vez que la panstamp reciba un mensaje
void rfPacketReceived(CCPACKET *packet){
  //revisamos que el paquete no este vacio, es decir que su length sea mayor que 0
  if(packet->length > 1){
    //imprimimos por serial que se obtuvo algo
    Serial.println("i got something");
  
  // los siguientes if statements revisan si el primer byte del paquete recibido es un 0 o un 1
  /*
   if(packet->data[0]==1){
     // si era un 1 imprime "it was a 1"
    Serial.println("it was a 1");
   }
   else if(packet->data[0]==0){
     // si era un 0 imprime "it was a 0"
    Serial.println("it was a 0");
   }
   else{
     // en el caso de no ser ni 0 ni 1 imprime "i dont know what it was"
    Serial.println("i dont know what it was");
   }*/
   
   // este bloque de codigo imprime en consola el contenido completo del array recibido
   
   byte i;
    for(i=0 ; i<packet->length ; i++)
      {
        if (packet->data[i] < 0x10)
          Serial.print('-');    // imprime un cero antes del byte
          Serial.print(packet->data[i], HEX); //imprime el byte actual
      }
      
      Serial.println(""); // deja un espacio al final para el resto de numeros.
  }
}


int incomingByte = 0; // una variable para capturar la entrada del serial

void setup() {
  
  // inicializamos una variable para el i de los loop for que tenemos mas adelante
  byte i;   
  
  // preparamos el led integrado de la panstamp para prenderlo cuando recibamos algo.
  pinMode(LED,OUTPUT);
  digitalWrite(LED,LOW);
  
  
  // configuramos las opciones de la panstamp
  panstamp.radio.setChannel(RFCHANNEL);  // asignamos el canal de transmision
  panstamp.radio.setSyncWord(SYNCWORD1,SYNCWORD0); // configuramos las palabras clave
  panstamp.radio.setDevAddress(SOURCE_ADDR); // le damos una direccion local al dispositivo
  panstamp.radio.setCCregs();  // registramos las credenciales
  panstamp.setHighTxPower(); // aumentamos la potencia de transmision
  
  swap.init();
  
  // incicializamos el serial 
  Serial.begin(38400);
  Serial.println("");
  
  // avisamos que el moden se encuentra listo
  Serial.println("Receiver is Ready!");
  // desabilitamos el filtro de addres, para que reciba todos los mensajes.
  //panstamp.radio.disableAddressCheck();
  
  // con esta linea agregamos la funcion que se ejectua cada vez que recibimos un mensaje
  panstamp.attachInterrupt(rfPacketReceived);
  
  //definimos el tamano de los paquetes que se enviaran
  packetA.length = 2; packetB.length = 2; packetC.length = 2;
  
  
  // estos loop for llenan los paquetes de datos.
  for(i=0; i<packetA.length ; i++){
    packetA.data[i] = 0;
  }
  for(i=0; i<packetB.length ; i++){
    packetB.data[i] = 1;
  }
  for(i=0; i<packetC.length ; i++){
    packetC.data[i] = 2;
  }
  
}

void loop() {
  //en nuestra funcion loop primero revisamos si hay algo en el serial
   if (Serial.available() > 0) {
                // asignamos la entrada del serial a nuestra variable
                incomingByte = Serial.read();

                // imprimimos por consola lo que se recibio
                Serial.print("I received: ");
                Serial.println(incomingByte);
                
                // si el codigo recibido es 49 (la tecla 1) enviamos el paquete A
                if(incomingByte == 49){
                   digitalWrite(LED, HIGH);
                   // esta funcion es la que envia el paquete, en este bloque tambien 
                   // prendemos y apagamos un led
                   panstamp.radio.sendData(packetA);
                   Serial.print("activate led a");
                   delay(1000);
                   digitalWrite(LED, LOW);
                }
                 // si el codigo recibido es 50 (la tecla 2) enviamos el paquete B
                if(incomingByte == 50){
                   digitalWrite(LED, HIGH);
                   panstamp.radio.sendData(packetB);
                   Serial.print("activate led b");
                   delay(1000);
                   digitalWrite(LED, LOW);
                }
                // si el codigo recibido es 51 (la tecla 3) enviamos el paquete C
                if(incomingByte == 51){
                   digitalWrite(LED, HIGH);
                   panstamp.radio.sendData(packetC);
                   Serial.print("activate led c");
                   delay(1000);
                   digitalWrite(LED, LOW);
                }
        }
}
