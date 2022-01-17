#define HARD_SIMILARITY_THRESHOLD 0.7
#define STRENGTH_DISSIMULARITY_PRICE 0
#define RYTHMIC_INACCURACY_THRESHOLD 0.15 

//----------
//FUNCTIONS
//----------

void printPattern(RythmPattern pattern){
  Serial.print("no. strength distance_after\n");
  for(int i=0;i<pattern.length;i++){
    Serial.print("[");
    Serial.print(i);
    Serial.print("] ");
    Serial.print(pattern.pressSequence[i]);
     Serial.print("     ");
    Serial.print(pattern.timeBetween[i]);
    Serial.print("\n");
  }
}

//for adding first press (no time)
void addPressToSequence(RythmPattern &pattern, double tapValue) {
  pattern.pressSequence[pattern.length] = tapValue;
  pattern.length++;
}

//for adding subsequent presses (includes time between)
void addPressToSequence(RythmPattern &pattern, double tapValue, float time) {
  int len = pattern.length;
  if (len == 0) return;

  pattern.pressSequence[len] = tapValue;
  pattern.timeBetween[len - 1] = time;
  pattern.length++;
}

//for calucating a single pattern from many provided when setting up password
RythmPattern calculatePattern(RythmPattern patterns[]) {
  struct RythmPattern resultPattern;
  float avr = 0;

  //average out all recorder patterns
  for (int i = 0; i < patterns[0].length; i++) {
    avr = 0;

    if (i == 0) {
      addPressToSequence(resultPattern, patterns[0].pressSequence[i]);
      continue;
    }

    for (int j = 0; j < PASSWORD_REPEAT; j++) {
      avr += patterns[j].timeBetween[i - 1];
    }

    avr = avr / PASSWORD_REPEAT;
    addPressToSequence(resultPattern, patterns[0].pressSequence[i], avr);
  }

  return resultPattern;
}

float maxTimeBetween(float arr[MAX_PASS_LENGTH - 1]) {
  float max = 0.0;
  for (int i = 0; i < MAX_PASS_LENGTH - 1; i++) {
    if (arr[i] > max) max = arr[i];
  }
  return max;
}

float maxPressStrenght(double arr[MAX_PASS_LENGTH]){
  float max = 0.0;
  for (int i = 0; i < MAX_PASS_LENGTH; i++) {
    if (arr[i] > max) max = arr[i];
  }
  return max;
}


void normalizePattern(RythmPattern & pattern) {
  //pick longest distance between 
  float maxTime=maxTimeBetween(pattern.timeBetween);
  float maxStrength=maxPressStrenght(pattern.pressSequence);
  
  for (int i = 0; i < MAX_PASS_LENGTH; i++) {
    //normalize time gaps
    if(i!=MAX_PASS_LENGTH-1) //skiping last arr index - no time gap there
    {
      pattern.timeBetween[i] = pattern.timeBetween[i] / maxTime;
    }

    //normalize press Strength
    float pressStrength=0.0;
    float pressStrengthFraction=pattern.pressSequence[i]/maxStrength;
    if(pressStrengthFraction>HARD_SIMILARITY_THRESHOLD) pressStrength=1.0;
    pattern.pressSequence[i]= pressStrength; 
  }
}

//returns a score from 0-1 on how closely given patterns match
//0 - don't match at all, 1- exactly the same
float matchPatterns(RythmPattern pat1, RythmPattern pat2) {
  if (pat1.length != pat2.length) return 0.0;

  float matchScore=1;
  int patternLength=pat1.length;
  
  for (int i = 0; i < patternLength ; i++)
  {
    //penilizes for dissimularity in press strength
    if (pat1.pressSequence[i] != pat2.pressSequence[i])
    {
      matchScore -= STRENGTH_DISSIMULARITY_PRICE;
    }
  
    float rythmicInacuracy = abs(pat1.timeBetween[i] - pat2.timeBetween[i]);
    
    //if any single tap is too off beat, return 
    if(rythmicInacuracy>=RYTHMIC_INACCURACY_THRESHOLD) return 0.0;
    
    matchScore -= rythmicInacuracy/patternLength;
  }

  return matchScore;
}


//returns a string uniquely identifying the pattern
String hashPattern(RythmPattern pat){
  
}
