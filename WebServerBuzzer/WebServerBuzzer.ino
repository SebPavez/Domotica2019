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

const int trigPin = 9;
const int echoPin = 10;

long duration;
int distance;

void setup() {
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
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
    //TODO: Encender buzzer si pasa umbral
  } else {
    //TODO: Apagar buzzer
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
  