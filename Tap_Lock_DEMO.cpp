#include "pch.h"
#include <iostream>
#include <chrono>
#include <thread>
#include <Windows.h>
#include <chrono>


#define SOFT_PRESS_KEY 'Q'
#define HARD_PRESS_KEY 'W'
#define COMMIT_KEY 'E'
#define MAX_PASS_LENGTH 10
//how many times user must repeat the password when setting a new one
#define PASSWORD_REPEAT 3
//defines error margin in rythym accuracy (0-1) where 1 is 100% accurate
#define RYTHMIC_ACCURACY 0.92

//imitate button press
bool checkForButtonPress(char key, bool & buttonDown) {
	if (!buttonDown) {
		buttonDown = GetAsyncKeyState(key) < 0;
		return false;
	}
	bool released = !(GetAsyncKeyState(key) < 0);
	buttonDown = !released;
	return released;
}


bool checkForSoftPress(bool & softDown) {
	return checkForButtonPress(SOFT_PRESS_KEY, softDown);
}

bool checkForHardPress(bool & hardDown) {
	return checkForButtonPress(HARD_PRESS_KEY, hardDown);
}

bool checkForEnterPress(bool & enterDown) {
	return checkForButtonPress(COMMIT_KEY, enterDown);
}

//returns 0-soft press, 1-hard press, -1 for none
int checkForButtonPress(bool & softDown, bool & hardDown) {
	if (checkForSoftPress(softDown)) return 0;
	if (checkForHardPress(hardDown)) return 1;
	return -1;
}

//struct for holding rythmic patterns
struct RythmPattern {
	int length = 0;
	//0- soft press,1-hard press
	bool pressSequence[MAX_PASS_LENGTH] = { 0 };
	float timeBetween[MAX_PASS_LENGTH - 1] = { 0.0 };
};

//RYTHM PATTERN FUNCTIONS

//for adding first press (no time)
void addPressToSequence(RythmPattern &pattern, bool pressType) {
	pattern.pressSequence[pattern.length] = pressType;
	pattern.length++;
}

//for adding subsequent presses (includes time between)
void addPressToSequence(RythmPattern &pattern, bool pressType, float time) {
	int len = pattern.length;
	if (len == 0) return;

	pattern.pressSequence[len] = pressType;
	pattern.timeBetween[len - 1] = time;
	pattern.length++;
}

void printPattern(RythmPattern pattern) {
	std::cout << "-----\n";
	std::cout << "n.  strength gapAfter\n";
	for (int i = 0; i < pattern.length; i++) {
		std::cout << "[" << i << "] " << pattern.pressSequence[i] << "        " << pattern.timeBetween[i] << "\n";
	}
	std::cout << "-----\n";
}



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

void normalizePattern(RythmPattern & pattern) {
	//pick longest distance between 
	float max=maxTimeBetween(pattern.timeBetween);

	//TODO floor all the rest to some accuracy?
	for (int i = 0; i < MAX_PASS_LENGTH - 1; i++) {
		pattern.timeBetween[i] = pattern.timeBetween[i] / max;
	}
}

bool patternsMatch(RythmPattern pat1, RythmPattern pat2) {
	std::cout << "MATCHING PATTERNS...\n";


	if (pat1.length != pat2.length) {
		std::cout << "WRONG LENGTH "<< "\n";
		return false;
	}

	for (int i = 0; i < pat1.length; i++){
		if (pat1.pressSequence[i] != pat2.pressSequence[i]) {
			std::cout << "WRONG STRENGTH AT POS "<<i<<"\n";
			return false;
		}

		float accuracy =1- abs(pat1.timeBetween[i] - pat2.timeBetween[i]);
		std::cout <<"["<< i << "] accuracy: "<< accuracy <<"\n";
		if (accuracy< RYTHMIC_ACCURACY) {
			std::cout << "WRONG ACCURACY!!\n";
			return false;
		}
	}

	return true;
}



int main() {
	//modes
	//0-ready for password input
	//1-record new password
	int currentMode = 1;

	//holds current password rythm
	struct RythmPattern lockPattern;

	//helper variable in detecting button presses
	int buttonPressed = -1;
	bool softDown = false;
	bool hardDown = false;
	bool enterDown = false;

	//helper variable for keeping last recorded timestamp
	auto lastTimestamp = std::chrono::high_resolution_clock::now();

	//sampling reduction simulation (TODO?)
	std::chrono::milliseconds samplingFrequency(10);

	std::cout << "WELCOME TO RYTHYM_SECURITY_7000 DEMO" << "\n";
	std::cout << SOFT_PRESS_KEY << "- imitates soft press (marked as 0)\n";
	std::cout << HARD_PRESS_KEY << "- imitates hard press (marked as 1)\n";
	std::cout << COMMIT_KEY << "- enter\n";

	//main program loop
	while (true) {

		buttonPressed = -1;

		//READY FOR PASSWORD INPUT
		if (currentMode == 0) {
			std::cout << "\n-----ENTER CODE-----" << "\n";
			struct RythmPattern currentPattern=  RythmPattern();

			while (!checkForEnterPress(enterDown)) {
				buttonPressed = checkForButtonPress(softDown, hardDown);

				if (buttonPressed != -1) {
					//convert from int to presstype bool
					bool pressType = (bool)buttonPressed;

					std::cout << "READ " << pressType << "\n";

					//for first button pressed in a sequence
					if (currentPattern.length == 0)
					{
						addPressToSequence(currentPattern, pressType);
						//restart timer
						lastTimestamp = std::chrono::high_resolution_clock::now();
					}
					else
					{
						//check for max pass length
						if (currentPattern.length > MAX_PASS_LENGTH) {
							//TODO handle max length
							std::cout << "MAX PASS LENGTH DETECTED\n";
							break;
						}

						//restart timer
						std::chrono::duration<float> elapsed = std::chrono::high_resolution_clock::now() - lastTimestamp;
						lastTimestamp = std::chrono::high_resolution_clock::now();

						//record time & press type
						addPressToSequence(currentPattern, pressType, elapsed.count());
					}
				}

			}
			//check enetered pass against lock pass	
			normalizePattern(currentPattern);
			bool correctPattern= patternsMatch(currentPattern, lockPattern);
			if (correctPattern) 
			{
				std::cout << "CORRECT CODE!!!!\n";
			}else{ std::cout << "WRONG CODE :( TRY AGAIN!\n"; }
		}

		//SET NEW CODE
		if (currentMode == 1) {
			std::cout << "\n-----SET NEW CODE-----" << "\n";
			int passwordRepeated = 0;
			struct RythmPattern passwordSource[PASSWORD_REPEAT];
			std::cout << "---REPEAT " << passwordRepeated+1 << "/" << PASSWORD_REPEAT << "---" << "\n";

			while (passwordRepeated < PASSWORD_REPEAT)
			{
				
				buttonPressed = checkForButtonPress(softDown, hardDown);

				if (buttonPressed != -1)
				{
					bool pressType = (bool)buttonPressed;

					std::cout << "PRESSED " << pressType<< "\n";

					//for first button pressed in a sequence
					if (passwordSource[passwordRepeated].length == 0)
					{
						addPressToSequence(passwordSource[passwordRepeated], pressType);
						//restart timer
						lastTimestamp = std::chrono::high_resolution_clock::now();
					}
					else
					{
						//check for max pass length
						if (passwordSource[passwordRepeated].length > MAX_PASS_LENGTH) {
							//TODO handle max length
							std::cout << "PASS TO LONG\n";
							break;
						}

						//restart timer
						std::chrono::duration<float> elapsed = std::chrono::high_resolution_clock::now() - lastTimestamp;
						lastTimestamp = std::chrono::high_resolution_clock::now();

						//record time & press type
						addPressToSequence(passwordSource[passwordRepeated], pressType, elapsed.count());
					}
				}

				if (checkForEnterPress(enterDown))
				{
					std::cout << "COMMITED\n";
					/*std::cout << "RAW TIMES\n";
					printPattern(passwordSource[passwordRepeated]);*/
					normalizePattern(passwordSource[passwordRepeated]);
					std::cout << "NORMALIZED\n";
					printPattern(passwordSource[passwordRepeated]);
					passwordRepeated++;

					//TODO Check with previous repetitions (at least some accuracy must be kept)

					std::cout << "---REPETITION " << passwordRepeated<< "/" << PASSWORD_REPEAT << "---" << "\n";
				}
			}

			//calculate final pattern and switch mode
			std::cout << "REPEATED ENOUGH TIMES! SETTING PASSWORD\n";
			lockPattern = calculatePattern(passwordSource);
			printPattern(lockPattern);
			currentMode=0;
		}

		//std::this_thread::sleep_for(samplingFrequency);
	}
}


