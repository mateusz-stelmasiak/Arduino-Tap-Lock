//----
//VARIABLES
//----
int successLED= LED_BUILTIN; 
int failureLED= 8;

//TODO 

//repeats pattern back to user on the led
void repeatPattern(){

}

void notifySuccess(){
  pinMode(successLED, OUTPUT);
  digitalWrite(successLED,1);
}

void notifyFailure(){
  pinMode(failureLED, OUTPUT);
  digitalWrite(failureLED,1);
}
