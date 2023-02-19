#include <ESP8266WiFi.h>
#include <WiFiUdp.h>

const char* ssid = "mohamed";
const char* password = "123456789";

//#include <SoftwareSerial.h>
//SoftwareSerial mySerial(D7,D8); // RX, TX
#include <string.h>
#include <ctype.h>

WiFiUDP Udp;
unsigned int localUdpPort = 8888 ;  // local port to listen on
char incomingPacket[255];  // buffer for incoming packets
//String incomingPacket="";  // buffer for incoming packets
char  replyPacekt[] = "Acknowledged";  // a reply string to send back
int reply;
String msg = "";
char msg_buffer[4096];
int i=0;
String stringmsg = "";
int packetSize;
int len;

IPAddress ip(192, 168, 173, 45) ;
IPAddress gateway(192, 168, 173, 1);
IPAddress subnet(255, 255, 255, 0);
IPAddress DNS(10, 0, 2, 1);

void setup()
{
  delay(500);
  Serial.begin(19200);

  Serial.printf("Connecting to %s ", ssid);
  WiFi.config(ip, gateway, subnet, DNS);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
  //  Serial.print(".");
  }
  //Serial.println(" connected");

  Udp.begin(localUdpPort);
  Serial.printf("Now listening at IP %s, UDP port %d\n", WiFi.localIP().toString().c_str(), localUdpPort);
}


void loop()
{
      packetSize = Udp.parsePacket(); 
      
        if (Serial.available())
      {
        msg_buffer[i]=Serial.read();
        stringmsg += msg_buffer[i];
               if (msg_buffer[i] && msg_buffer[i]== '\n')   
               {
               // Serial.print(stringmsg);
                Udp.beginPacket(Udp.remoteIP(), 8888);
                Udp.print(stringmsg);
//                Udp.print("***");
//                Udp.print(WiFi.RSSI()); 
                 Udp.endPacket();
                stringmsg="";
                //memset (msg_buffer , '\0', strlen(msg_buffer));
                i = 0;
               }  
      }
           
        if (packetSize)
      {
      // receive incoming UDP packets
      //Serial.printf("Received %d bytes from %s ;", packetSize, Udp.remoteIP().toString().c_str());
      len = Udp.read(incomingPacket, 255);
       if (len > 0)
       {
        incomingPacket[len] = 0;
       }
      Serial.printf("%s\n", incomingPacket);
      //Serial.printf("UDP packet contents: %s\n", incomingPacket);
            if(incomingPacket[0]=='1' && incomingPacket[1]=='2' && incomingPacket[2]=='3' && incomingPacket[3]=='*' && incomingPacket[4]=='@')
          Serial.print("************");  // for Arduino mega

          if(incomingPacket[0]=='x' && incomingPacket[1]=='x' && incomingPacket[2]=='x' && incomingPacket[3]=='x' && incomingPacket[4]=='x')
          Serial.print("xxxxxxxxxxxx");          
      }

if (i < 4096) i++;
      else i = 0;
}
