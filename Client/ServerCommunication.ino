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
const int serverPort = 8070; //server port

WiFiClient client;


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
  if (!client.connected()){
    client.connect(serverIP, serverPort);
  }
  
  if (client.connected()){
     client.print(msg);
  }
}


void sendResponseToServer(float matchScore)
{
    sendToServer("%"+String(matchScore));
}

String readFromServer(){
  String msg = String(client.read());
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
