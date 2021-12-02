//-------------
//FORCE SENSOR HANDLING
//-------------
#define MAX_SENSOR_VALUE 1024
#define IDLE_SENSOR_THRESHOLD 50 //

//---------
//VARIABLES
//--------
int sensorPin = 0;    
int sensorValue = 0; //value detected from sensor
int maxtapValue=0; //max sensor value recorded in a tap
bool currentlyPressing=false;
unsigned long lastTapTimestamp= micros();

//---------
//FUNCTIONS
//--------
bool recordMaxValueInPress() {
  //getsensor value
  sensorValue = analogRead(sensorPin);
  currentlyPressing = (sensorValue>IDLE_SENSOR_THRESHOLD);
  
  //print for debugging
//  Serial.print(sensorValue);
//  Serial.print("\n");
  
  //record highest value in single press
  if(currentlyPressing){
    if(sensorValue>maxtapValue) maxtapValue=sensorValue;
  }
  
}

//returns -1 for none, [0-1] for touch strength
double detectTap(){
  recordMaxValueInPress();
  
  if(maxtapValue>0 && !currentlyPressing){
    //save timestamp of last tap
    lastTapTimestamp=micros();
    
    //save tapvalue to temp and reset it
    double tapValDuplicate=maxtapValue;
    maxtapValue=0;
 
    //divide by max value to normalize between 0-1
    return (tapValDuplicate/(double)MAX_SENSOR_VALUE);
  }
  
  return -1;
}
