//----
//VARIABLES
//----
int successLED= LED_BUILTIN; 
int failureLED= 8;

//TODO 


//repeats pattern back to user on the led diode
void repeatPattern(){

}

void notifySuccess(){
  myServo.write(180);
  pinMode(successLED, OUTPUT);
  digitalWrite(successLED,0);
  delay(100);
  digitalWrite(successLED,1);
  delay(100);
  digitalWrite(successLED,0);
  delay(100);
  digitalWrite(successLED,1);
  delay(100);
  digitalWrite(successLED,0);
  delay(100);
  digitalWrite(successLED,1);
  delay(100);
}

void blinkOnce() {
  digitalWrite(successLED,0);
  delay(100);
  digitalWrite(successLED,1);
}

void openServo() {
  digitalWrite(successLED,0);
  delay(5000);
  digitalWrite(successLED,1);
  myServo.write(0);
}

void lockServo(){
    myServo.write(180);
}

void notifyFailure(){
  pinMode(successLED, OUTPUT);
  digitalWrite(successLED,0);
  digitalWrite(BUZZER_PIN,1);
  delay(1000);
  digitalWrite(successLED,1);
  digitalWrite(BUZZER_PIN,0);
}
