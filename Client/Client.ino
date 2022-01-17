//-------------
//ADDITIONAL LIBRARIES 
//-------------
#include <Servo.h>

//-------------
//PROGRAM CONSTANTS 
//-------------
#define MAX_PASS_LENGTH 15 //max taps in a password
#define PASSWORD_REPEAT 3 //how many times must the user repeat his password when setting a new one
#define REPEAT_ACCURACY 0.8 //minimum match score for repetitions of password when user is setting a new one
#define UNLOCK_ACCURACY 0.92 //minimum match score for pattern to succesfully unlock
#define COMMIT_TIME 2000000 //time that needs to pass after a sequence input to count is as finished (microseconds)
#define SERVO_PIN 4
#define BUZZER_PIN 5

//-------------
//PROTOTYPES & STRUCTURES
//-------------
struct RythmPattern {
  int length = 0;
  double pressSequence[MAX_PASS_LENGTH] = { 0 };  //0- soft press,1-hard press
  float timeBetween[MAX_PASS_LENGTH - 1] = { 0.0 };
};

bool checkForCommit(RythmPattern pattern);
double detectTap();
void recordMaxValueInPress();
//OUTPUT FUNCTIONS
void printDebug(String msg);
void notifySuccess();
void notifyFailure();
void openServo();
//RYTHM PATTERN FUNCTIONS
void addPressToSequence(RythmPattern &pattern, bool pressType);
void addPressToSequence(RythmPattern &pattern, bool pressType, float time);
RythmPattern calculatePattern(RythmPattern patterns[]);
void normalizePattern(RythmPattern & pattern);
float matchPatterns(RythmPattern pat1, RythmPattern pat2);
//SERVER COMMUNICATION FUNCTIONS
void sendNewPasswordToServer(RythmPattern pattern);
void tryPasswordOnServer(RythmPattern pattern);


//-------------
//SETUP
//-------------

//VARIABLES
//modes
//0-ready for password input
//1-record new password
int currentMode;
struct RythmPattern lockPattern; //holds current password rythm
double tapValue; //helper variable in detecting button presses (-1-none, between [0-1] for hard to soft)
unsigned long  lastTimestamp; //helper variable used for messuring distances between taps

Servo myServo;

void setup() {
  tapValue = -1;
  currentMode = 1;
  lastTimestamp== micros();
  Serial.begin(9600);   // for sending debugging information via the Serial monitor
  myServo.attach(SERVO_PIN);
  pinMode(BUZZER_PIN, OUTPUT);
  digitalWrite(BUZZER_PIN,0);
}

//-------------
//MAIN LOOP
//-------------

//HELPER FUNCTIONS
bool checkForCommit(RythmPattern pattern){
  //can't commit empty pattern
  if(pattern.length==0) return false;
  
  unsigned long timeSinceLastTap = micros() - lastTimestamp;
  if(timeSinceLastTap>COMMIT_TIME) return true;
  return false;
}


void loop() 
{    
  //Serial.print("1\n");//dev
    //INPUT PASSWORD MODE
    if (currentMode == 0){
       inputPassword: //label for goto
       Serial.print("INPUT PASSWORD!\n");
       struct RythmPattern currentPattern;
       //Serial.print("2\n");//dev
       while (!checkForCommit(currentPattern))
       {
        tapValue = detectTap();
            
        if (tapValue != -1)
        {
          Serial.print("Tap strength: ");
          Serial.print(tapValue);
          Serial.print("\n");
          
          //for first tap in a sequence
          if (currentPattern.length == 0)
          {
            addPressToSequence(currentPattern, tapValue);
            lastTimestamp = micros();
          }
          
          //subsequent taps 
          else
          {
            //check for max pass length
            if (currentPattern.length > MAX_PASS_LENGTH) 
            {
              Serial.print("PASSWORD TOO LONG!\n");
              goto inputPassword;
            }

            //restart timer
            unsigned long elapsed = micros() - lastTimestamp;
            lastTimestamp = micros();

            //record time & press type
            addPressToSequence(currentPattern, tapValue, elapsed);
          }
          
        }
       }
       //after commited
       normalizePattern(currentPattern);
       printPattern(currentPattern);
       float matchScore=matchPatterns(currentPattern,lockPattern);
       Serial.print("Match to lock password: ");
       Serial.print(matchScore);
       Serial.print("\n");
       
       Serial.print("SENDING PASSWORD TO SERVER...\n");
       tryPasswordOnServer(currentPattern);
       if (matchScore > 0.5)
       {
        notifySuccess();
        openServo();
       }
       else
       {
        notifyFailure();
       }
    }
  
    //SETTING NEW PASSWORD MODE
    if (currentMode == 1)
    {
      //Serial.print("3\n");//dev
      setNewPassword: //label for goto
      Serial.print("SET NEW PASSWORD!\n");
      struct RythmPattern passwordSource[PASSWORD_REPEAT];
      int currentRepetition=0;

      while(currentRepetition<PASSWORD_REPEAT)
      { 
          while (!checkForCommit(passwordSource[currentRepetition]))
          {
            tapValue = detectTap();
            
            if (tapValue != -1)
            {
              Serial.print("Tap strength: ");
              Serial.print(tapValue);
              Serial.print("\n");
              
              //for first tap in a sequence
              if (passwordSource[currentRepetition].length == 0)
              {
                addPressToSequence(passwordSource[currentRepetition], tapValue);
                lastTimestamp = micros();
              }
              
              //subsequent taps 
              else
              {
                
                //check for max pass length
                if (passwordSource[currentRepetition].length > MAX_PASS_LENGTH) 
                {
                  Serial.print("PASSWORD TOO LONG!\n");
                  notifyFailure();
                  goto setNewPassword;
                }
    
                //restart timer
                unsigned long elapsed = micros() - lastTimestamp;
                lastTimestamp = micros();
    
                //record time & press type
                addPressToSequence(passwordSource[currentRepetition], tapValue, elapsed);
              }
              
            }
          }
          
          notifySuccess(); 
           
          Serial.print("Repetition ");
          Serial.print(currentRepetition+1);
          Serial.print(" completed\n");
          
          normalizePattern(passwordSource[currentRepetition]);
          printPattern(passwordSource[currentRepetition]);
          
          //match to previuos repetitions, to see if it's not too off
          if(currentRepetition>0)
          {
            
            float matchScore=0;
            for(int j=currentRepetition-1;j>=0;j--)
            {
               matchScore=matchPatterns(passwordSource[currentRepetition],passwordSource[j]);
               Serial.print("Match to prev rep: ");
               Serial.print(matchScore);
               Serial.print("\n");
               if(matchScore<REPEAT_ACCURACY){
                notifyFailure();
                goto setNewPassword;
               }
            }
          }
          
          currentRepetition++;
      }
      
      Serial.print("SUCCESS! SETTING NEW PASSWORD...\n");
      notifySuccess();
      notifySuccess();
      notifySuccess();
      lockPattern=calculatePattern(passwordSource);
      lockServo();
      sendNewPasswordToServer(lockPattern);
      currentMode=0;  
    }  
}
