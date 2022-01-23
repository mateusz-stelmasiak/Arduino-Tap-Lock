//-------------
//HANDLE COMMUNICATION WITH THE SERVER
//-------------

//TODO 

#include <ESP8266WiFi.h>

int port = 8070;  //Port number
WiFiServer server(port);

//Server connect to WiFi Network
const char *ssid = "D:::::::";  //your wifi SSID
const char *password = "policja997";  //your wifi Password
const char *serverIP = "172.20.10.6"; //server ip

//const char *ssid = "INEA-5971_2.4G";  //your wifi SSID
//const char *password = "QEfXeXc5";  //your wifi Password
//const char *serverIP = "192.168.137.17"; //server ip

const int serverPort = 8070; //server port


void setupTCP() 
{

  Serial.println();

  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password); //Connect to wifi
 
  // Wait for connection  
  Serial.println("Connecting to Wifi");
  while (WiFi.status() != WL_CONNECTED) {   
    delay(500);
    Serial.print(".");
    delay(500);
  }

  Serial.println("");
  Serial.print("Connected to ");
  Serial.println(ssid);

  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());  
  server.begin();
}

void sendToServer(String msg){
  WiFiClient client;

client.connect(serverIP, serverPort);


      Serial.println("CONNECTING to server");
           client.print(msg);
       Serial.println("Sending to server" + msg);
  
  
//  if (client.connected()){

//  }
}


void sendResponseToServer(float matchScore)
{
    sendToServer("%"+String(matchScore));
}

String readFromServer(){
WiFiClient client;

client.connect(serverIP, serverPort);
  
  String msg = String(client.read());

Serial.print("CZYTAM: ");
Serial.println(msg);
  
  return msg;
}


//after a new password has been set
void sendNewPasswordToServer(RythmPattern pattern){
  
}

//when user tries to unlock 
void tryPasswordOnServer(RythmPattern pattern){
  //send to server
  //await response
  //handle too long wait
  //handle response - use notifySuccess() or notifyFailure()
}
