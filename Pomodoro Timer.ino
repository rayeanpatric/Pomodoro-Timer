#include <TM1637Display.h>

// Define TM1637 pins
#define CLK 3
#define DIO 4

// Define buzzer pin
#define BUZZER 8

// Initialize the display
TM1637Display display(CLK, DIO);

// Timer variables (in seconds)
const int focusTime = 1500; // 25 minutes
const int shortBreak = 300; // 5 minutes
const int longBreak = 1800; // 30 minutes

int currentTime = focusTime; // Start with focus time
int cycleCount = 0;

enum State { FOCUS, SHORT_BREAK, LONG_BREAK };
State currentState = FOCUS;

void setup() {
  // Set up TM1637 display
  display.setBrightness(7); // Max brightness
  
  // Set buzzer pin as output
  pinMode(BUZZER, OUTPUT);
}

void loop() {
  // Display time in MM:SS format
  displayTime(currentTime);
  delay(1000); // 1-second delay
  currentTime--;

  // Check if the timer has ended
  if (currentTime < 0) {
    // Play the buzzer sound
    playBuzzer();

    // Transition to the next state
    switch (currentState) {
      case FOCUS:
        cycleCount++;
        currentState = (cycleCount % 4 == 0) ? LONG_BREAK : SHORT_BREAK;
        currentTime = (currentState == SHORT_BREAK) ? shortBreak : longBreak;
        break;

      case SHORT_BREAK:
      case LONG_BREAK:
        currentState = FOCUS;
        currentTime = focusTime;
        break;
    }
  }
}

// Function to display time on TM1637 in MM:SS format
void displayTime(int seconds) {
  int mins = seconds / 60;
  int secs = seconds % 60;
  int timeToDisplay = (mins * 100) + secs; // MMSS format
  display.showNumberDecEx(timeToDisplay, 0b01000000, true); // Colon ON
}

// Function to play buzzer sound 5 times (peem-peem-peem-peem-peem)
void playBuzzer() {
  int frequency = 400; // Lower frequency in Hz
  int duration = 200;  // Duration for each beep in milliseconds

  for (int i = 0; i < 5; i++) {
    tone(BUZZER, frequency, duration);
    delay(duration + 100); // Gap between beeps
  }
  noTone(BUZZER); // Ensure the buzzer is off
}

