#include <SPI.h>
#include <Ethernet.h>

byte mac[] = {
  0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED
};
IPAddress ip(192, 168, 137, 177);
EthernetServer server(80);
String readString;
int ledOn = 0;
int alarmActiva = 0;
const int led = 13;

const int trigPin = 8;
const int echoPin = 9;
const int buzzerPin = 6;
int redPin= A1;
int greenPin = A2;
int bluePin = A3;

long duration;
int distance;
int contador = 0;

void setup() {
  pinMode(redPin, OUTPUT);
  pinMode(greenPin, OUTPUT);
  pinMode(bluePin, OUTPUT);
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  pinMode(buzzerPin, OUTPUT);
  Serial.begin(9600);
  Serial.println("Ethernet WebServer Example");
  pinMode(led, OUTPUT);
  digitalWrite(led, LOW);
  Ethernet.begin(mac, ip);
  if (Ethernet.hardwareStatus() == EthernetNoHardware) {
    Serial.println("Ethernet shield was not found.  Sorry, can't run without hardware. :(");
    while (true) {
      delay(1);
    }
  }
  if (Ethernet.linkStatus() == LinkOFF) {
    Serial.println("Ethernet cable is not connected.");
  }
  

  // start the server
  server.begin();
  Serial.print("server is at ");
  Serial.println(Ethernet.localIP());
}

void loop() {
  // listen for incoming clients
  if(alarmActiva == 1){
    digitalWrite(trigPin, LOW);
    delayMicroseconds(2);
    
    digitalWrite(trigPin, HIGH);
    delayMicroseconds(10);
    digitalWrite(trigPin, LOW);
    
    duration = pulseIn(echoPin, HIGH);
    
    distance= duration*0.034/2;
    
    Serial.print("Distance: ");
    Serial.println(distance);
    //TODO: Determinar umbral de activacion para RGB
    if(distance < 50){
        tone(buzzerPin, 1000);
        switch(contador){
           case 0:
             setColor(255, 0,0);
             break;
           case 1:
             setColor(0,255,0);
             break;
           case 2:
             setColor(0,0,255);
             break;
        }
        contador++;
        if(contador == 3){
          contador = 0;
        } 
    }
  } else {
    noTone(buzzerPin);
  }
   EthernetClient client = server.available();
   if (client) {
        while (client.connected()) {
        if (client.available()) {
          char c = client.read();
  
          //read char by char HTTP request
          if (readString.length() < 100) {
            readString += c;
            
           }
  
          if (c == '\n') {
             Serial.println(readString);
             client.println("HTTP/1.1 200 OK"); //send new page
             client.println("Content-Type: text/html");
             client.println();     
             client.println("<HTML>");
             client.println("<HEAD>");
             client.println("<meta name='apple-mobile-web-app-capable' content='yes' />");
             client.println("<meta name='apple-mobile-web-app-status-bar-style' content='black-translucent' />");
             client.println("<link rel='stylesheet' type='text/css' href='https://randomnerdtutorials.com/ethernetcss.css' />");
             client.println("<TITLE>Examen</TITLE>");
             client.println("</HEAD>");
             client.println("<BODY>");
             client.println("<H1>Examen</H1>");
             client.println("<hr />");
             client.println("<br />");  
             client.println("<H2>Arduino with Ethernet Shield</H2>");
             client.println("<br />");  
             client.println("<a href=\"/?button1on\"\">Activar</a>");
             client.println("<a href=\"/?button1off\"\">Apagar</a><br />");   
             client.println("<br />");     
             client.println("<br />"); 
             client.println("</BODY>");
             client.println("</HTML>");
             client.println();
               delay(2000);
            // close the connection:
            client.stop();
            //controls the Arduino if you press the buttons
             if (readString.indexOf("?button1on") > 0){
                 alarmActiva = 1;
  //               digitalWrite(led, HIGH);
             }
             if (readString.indexOf("?button1off") > 0){
                 alarmActiva = 0;
  //               digitalWrite(led, LOW);
             }
             //clearing string for next read
              readString="";  
          }
          
        }
      }  
  }

  
}

void setColor(int redValue, int greenValue, int blueValue) {
    analogWrite(redPin, redValue);
    analogWrite(greenPin, greenValue);
    analogWrite(bluePin, blueValue);
  }
  
