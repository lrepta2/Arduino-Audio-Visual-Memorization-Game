// Names:
// Luke Repta
// Timothy Jang
// Aleksandar Kartelyan
// 
// Title - Audio-Visual Memorization Game
// Description: 
// An audio-visual memorization game implemented on the Arduino Uno
// References: 
//   - https://www.arduino.cc/en/Tutorial/BuiltInExamples/Debounce

#include <SoftwareSerial.h>
// Need pins 0 and 1 to be reserved for communication from hardware serial to the softwareSerial object we create

// Change these if we want
const int winLEDPin = 2;
const int loseLEDPin = 13;

const int shortLEDPin = 3;
const int longLEDPin = 4;

const int shortLEDBtn = 5;
const int longLEDBtn = 6;

const int shortSoundBtn = 7;
const int longSoundBtn = 8;

const int soundPin = 9;

const int softwareRXPin = 11;
const int softwareTXPin = 12;

// Could secretely decrement these values as the user advances levels, making each
// stimulus shorter and as such harder to remember
// Possible adjuster if we find even the highest level is too easy
const int shortLength = 1000; // Length of the short flashes/buzzes in milliseconds
const int longLength = 2000; // Length of the long flashes/buzzes in milliseconds

const int soundFrequency = 1000; // Frequency of the buzzer in Hz

SoftwareSerial softSerial(softwareRXPin, softwareTXPin); // RX, TX

int level = 1;

void setup() {
    // Generate a random seed based on an unused analog pin

    pinMode(shortLEDPin, OUTPUT);
    pinMode(longLEDPin, OUTPUT);

    pinMode(winLEDPin, OUTPUT);
    pinMode(loseLEDPin, OUTPUT);

    pinMode(soundPin, OUTPUT);

    pinMode(shortLEDBtn, INPUT);
    pinMode(longLEDBtn, INPUT);
    pinMode(shortSoundBtn, INPUT);
    pinMode(longSoundBtn, INPUT);
  
  Serial.begin(9600);
  while (!Serial) {
    ;
  }

  softSerial.begin(9600);
  generateSequence();
}

// someDataStructure correctSequence;
// someDataStructure userSequence;

// Sequence array is of max length 15, initialized values to 0 as 1-4 is used for LED/Sound
int sequenceArray[] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
int sequenceLength = 0;

// user sequence is of max length 15, initialized values to 0 as 1-4 is used for LED/Sound
int userSequence[] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
int userSequenceLength = 0;

int recordInput() {
    Serial.println("Entered the recordInput function");
    // record input on the buttons until the user enters an incorrect input?
    // Otherwise we will need another button or something for the user to indicate when
    // they are done entering what they think the sequence is
    int count = 0;
    long unsigned int prevTime = 0;
    int prevShortLED = LOW;
    int prevLongLED = LOW;
    int prevShortSound = LOW;
    int prevLongSound = LOW;

    int shortLEDState;
    int longLEDState;
    int shortSoundState;
    int longSoundState;

    // Should also check that sequence is correct
    Serial.print("userSequence is: ");
    while(count < 15) {
        delay(200);
        long unsigned int currTime = millis();

        int shortLEDRd = digitalRead(shortLEDBtn);
        int longLEDRd = digitalRead(longLEDBtn);
        int shortSoundRd = digitalRead(shortSoundBtn);
        int longSoundRd = digitalRead(longSoundBtn);


        // Buttons 1 and 2 (short and long LED) are not working correctly
        // Button 1 very rarely detects presses, and when it does it seems
        // to select a number 1-4 at random
        // Button 2 almost seems to cache the last button pressed,
        // And will output 1 if pressed or 2 at other times
        // Button 3 and 4 (long and short sound) are working as expected
        if (shortLEDRd == HIGH) {
            userSequence[count] = 1;
            Serial.print((String)userSequence[count] + ", ");
        } else if (longLEDRd == HIGH) {
            userSequence[count] = 2;
            Serial.print((String)userSequence[count] + ", ");
        } else if (shortSoundRd == HIGH) {
            userSequence[count] = 3;
            Serial.print((String)userSequence[count] + ", ");
        } else if (longSoundRd == HIGH) {
            userSequence[count] = 4;
            Serial.print((String)userSequence[count] + ", ");
        }


        // // Implementation of button debouncing based on arduino built-in
        // // example code:
        // // - https://www.arduino.cc/en/Tutorial/BuiltInExamples/Debounce
        // // If the switch changed, due to noise or pressing:
        // if (
        //     prevShortLED != shortLEDRd ||
        //     prevLongLED != longLEDRd ||
        //     prevShortSound != shortSoundRd ||
        //     prevLongSound != longSoundRd
        //    ) 
        // {
        //     // reset the debouncing timer
        //     prevTime = millis();
        // }

        // if ((millis() - prevTime) > 50) {
        //     // whatever the reading is at, it's been there for longer than the debounce
        //     // delay, so take it as the actual current state:

        //     // Check all of the button states, adding them to the
        //     // sequence as a valid user input only if the input
        //     // has changed since last reading, and the button has been
        //     // pressed. Sets the sequence to the equivalent number
        //     // representing that input, and updates the counter
        //     if (shortLEDRd != shortLEDState) {
        //         shortLEDState = shortLEDRd;
                
        //         if (shortLEDState == HIGH) { userSequence[count] = 1; }
        //         Serial.print((String)userSequence[count] + ", ");
        //     }
        //     else if (longLEDRd != longLEDState) {
        //         longLEDState = longLEDRd;
                
        //         if (longLEDState == HIGH) { userSequence[count] = 2; }
        //         Serial.print((String)userSequence[count] + ", ");
        //     }
        //     else if (shortSoundRd == HIGH) {
        //         shortSoundState = shortSoundRd;
                
        //         if (shortSoundState == HIGH) { userSequence[count] = 3; }
        //         Serial.print((String)userSequence[count] + ", ");
        //     }
        //     else if (longSoundRd != longSoundState) {
        //         longSoundState = longSoundRd;
                
        //         if (longSoundState == HIGH) { userSequence[count] = 4; }
        //         Serial.print((String)userSequence[count] + ", ");
        //     }

        //     // Not certain this is where we should be testing for equality
        //     // Also, should ensure that this code never runs on count = 0
        //     if (userSequence[count] != 0 && userSequence[count] != sequenceArray[count]) {
        //         // User got sequence wrong, so user input stops being read
        //         // and we return 0 to the loop function
        //         Serial.println("count is: " + (String)(count) + " user: " + (String)(userSequence[count]) + " correct: " + (String)(sequenceArray[count]));
        //         Serial.println("Returning 0 from the recordInput function");
        //         return 0;
        //     } else if (userSequence[count] == sequenceArray[count]) {
        //         count++;
        //         userSequenceLength++;
        //     }
        // }

        // // save the reading. Next time through the loop, it'll be the lastButtonState:
        // //lastButtonState = reading;

        // shortLEDState = shortLEDRd;
        // longLEDState = longLEDRd;
        // shortSoundState = shortSoundRd;
        // longSoundState = longSoundRd;

        // record user input, with proper debouncing etc

        // Check if user pressed short LED button
            // userSequence[count++] = 1;
        // Check if user pressed long LED button
            // userSequence[count++] = 2;
        
        // Check if user pressed short sound button
            // userSequence[count++] = 3;
        // Check if user pressed long sound button
            // userSequence[count++] = 4;

        if (userSequence[count] != 0 && userSequence[count] != sequenceArray[count]) {
                // User got sequence wrong, so user input stops being read
                // and we return 0 to the loop function
                Serial.println("\ncount is: " + (String)(count) + " user: " + (String)(userSequence[count]) + " correct: " + (String)(sequenceArray[count]));
                Serial.println("\nReturning 0 from the recordInput function\n");
                digitalWrite(loseLEDPin, HIGH);
                delay(3000);
                digitalWrite(loseLEDPin, LOW);
                return 0;
        } else if (userSequence[count] == sequenceArray[count]) {
                count++;
                userSequenceLength++;
        }

        // If we decide to go that route, check if  
        // user pressed submit sequence button
        if (count == sequenceLength) {
            // User correctly inputted the sequence
            // This is sufficient because we check if each
            // input is correct before it is added
            Serial.println("\nReturning 1 from the recordInput function\n");
            digitalWrite(winLEDPin, HIGH);
            delay(3000);
            digitalWrite(winLEDPin, LOW);
            return 1;
        }

        // if (input sequence is still correct) {
        //     continue;
        // } else {
        //     // Incorrect input

        //     //reset the userSequence
        //     for (int i = 0; i < 15; i++) {
        //         userSequence[i] = 0;
        //     }
        //     return 0;
        // }
    }
}

// Creates the random sequence that the user must repeat
// Follows the following scheme:
// 1 = short LED flash
// 2 = long LED flash
// 3 = short Sound buzz
// 4 = long Sound buzz
// 0 is the uninitialized value, and should never be used
void createSequenceArray() {
    Serial.println("Entered the createSequencArray function");
    randomSeed(analogRead(A2));
    for (int i = 0; i < sequenceLength; i++) {
        sequenceArray[i] = random(1,5);
    }
}

void generateSequence() {
    for (int i = 0; i < 15; i++) {
      userSequence[i] = 0;
      sequenceArray[i] = 0;
    }
    Serial.println("Entered the generateSequence function");
    randomSeed(analogRead(A5));
    int randomLengthAdjuster = random(0, 3);
    sequenceLength = randomLengthAdjuster;
    switch (level) {
        case 1:
            // statements
            sequenceLength += 4;
            break;
        case 2:
            // statements
            sequenceLength += 6;
            break;
        case 3:
            // statements
            sequenceLength += 8;
            break;
        case 4:
            // statements
            sequenceLength += 10;
            break;
        case 5:
            // statements
            sequenceLength += 12;

            break;
        default:
            // This should not be needed, otherwise could use for error checking
            // statements
        break;
    }
    Serial.println("Sequence length is: " + sequenceLength);

    // After the switch statement to get the length of the sequence, generate the sequence
    createSequenceArray();
}

// Interprets the values in the sequenceArray and plays them back to the user
// Follows the following scheme:
// 1 = short LED flash
// 2 = long LED flash
// 3 = short Sound buzz
// 4 = long Sound buzz
void displaySequence() {
    Serial.println("Entered the displaySequence function");
    Serial.print("Sequence is: ");
    for(int i = 0; i < sequenceLength; i++) {
        Serial.print((String)sequenceArray[i] + ", ");
        delay(400); // Brief pause in between displaying each element of the sequence
        switch (sequenceArray[i])
        {
            // short LED flash
            case 1:
                digitalWrite(shortLEDPin, HIGH);
                // Maybe shouldn't use delay if we still want to do things during this
                delay(shortLength);
                digitalWrite(shortLEDPin, LOW);
                break;
            case 2:
                digitalWrite(longLEDPin, HIGH);
                delay(longLength);
                digitalWrite(longLEDPin, LOW);
                break;
            case 3:
                tone(soundPin, soundFrequency, shortLength);
                delay(shortLength);
                // Might need a delay here? I think tone is a blocking call for its length?
                break;
            case 4:
                tone(soundPin, soundFrequency/2, longLength);
                delay(longLength);
                // Might need a delay here? I think tone is a blocking call for its length?
                break;
            default:
                // This case should never happen, only happens if you read the
                // uninitalized 0's -> something is wrong!
                break;
        }
    }
    Serial.println("");
}

void loop() {
    Serial.println("In loop function");
    // Lots of repeated code, could be made prettier
    // generateSequence();
    displaySequence();

  if (level == 1) {
      // This is where the blocking call for recording user input occurs
      // Either returns 1 if the user got the sequence right, or returns
      // 0 if they get it wrong
    if (recordInput() == 1) {
        level++;
    } 
    // Moved generate sequence out of if and else to make code clearer
    // Don't think the else is even necessary unless we want something else
    // to happen when the user fails to guess a level at level 1
    else {
        // reset the level with a new sequence
    }
    generateSequence();
  }
  else if (level == 2) {
      if (recordInput() == 1) {
        level++;
        generateSequence();
    } else {
        level = 1;
        generateSequence();
    }
  }
  else if (level == 3) {
      if (recordInput() == 1) {
        level++;
        generateSequence();
    } else {
        level = 1;
        generateSequence();
    }
  }
  else if (level == 4) {
      if (recordInput() == 1) {
        level++;
        generateSequence();
    } else {
        level = 1;
        generateSequence();
    }
  }
  else if (level == 5) {
      if (recordInput() == 1) {
        level++;
        generateSequence();
    } else {
        level = 1;
        generateSequence();
    }
  }
}
