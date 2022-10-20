#include "Servo.h" 
Servo servoMotor;

const int startButton = 2;

const int countDownLEDPin1 = 3;
const int countDownLEDPin2 = 4;
const int countDownLEDPin3 = 5;

const int player1RedLEDPin = 6;
const int player1YellowLEDPin = 7;
const int player1GreenLEDPin = 8;

const int player2RedLEDPin = 16;
const int player2YellowLEDPin = 17;
const int player2GreenLEDPin = 18;

const int player1RedButtonPin = 9;
const int player1YellowButtonPin = 10;
const int player1GreenButtonPin = 11;

const int player2RedButtonPin = 13;
const int player2YellowButtonPin = 14;
const int player2GreenButtonPin = 15;

const int servoMotorPin = 19;

int player1RandomPin;
int player2RandomPin;

// start button states
int currentStartButtonState;
int lastStartButtonState = LOW;  

//player 1 button states
int currentPlayer1LEDButtonState;
int lastPlayer1LEDButtonState = LOW;  

//player 2 button states
int currentPlayer2LEDButtonState;
int lastPlayer2LEDButtonState = LOW;  

bool player1Reacted = false;
bool player2Reacted = false;

// player 1 reaction time
float player1LightOnTime;
float player1ReactionTime;

// player 2 reaction time
float player2LightOnTime = 0;
float player2ReactionTime = 0;

int gameCounter = 0;
 
void setup() {
  
  Serial.begin(9600);

  servoMotor.attach(servoMotorPin);

  pinMode(startButton, INPUT_PULLDOWN);

// countdown LED pins
  pinMode(countDownLEDPin1, OUTPUT);   
  pinMode(countDownLEDPin2, OUTPUT);   
  pinMode(countDownLEDPin3, OUTPUT); 
  
// player 1 reaction LED
  pinMode(player1RedLEDPin, OUTPUT);   
  pinMode(player1YellowLEDPin, OUTPUT);   
  pinMode(player1GreenLEDPin, OUTPUT);   

// player 2 reaction LED
  pinMode(player2RedLEDPin, OUTPUT);   
  pinMode(player2YellowLEDPin, OUTPUT);   
  pinMode(player2GreenLEDPin, OUTPUT);   

// player 1 buttons
  pinMode(player1RedButtonPin, INPUT_PULLDOWN);
  pinMode(player1YellowButtonPin, INPUT_PULLDOWN);
  pinMode(player1GreenButtonPin, INPUT_PULLDOWN);

// player 2 buttons
  pinMode(player2RedButtonPin, INPUT_PULLDOWN);
  pinMode(player2YellowButtonPin, INPUT_PULLDOWN);
  pinMode(player2GreenButtonPin, INPUT_PULLDOWN);
}

// Reset everything
void reset() {
  player1Reacted = false;
  player2Reacted = false;
  digitalWrite(countDownLEDPin1, LOW);
  digitalWrite(countDownLEDPin2, LOW);
  digitalWrite(countDownLEDPin3, LOW);
  digitalWrite(player1RandomPin, LOW);
  digitalWrite(player2RandomPin, LOW);
  delay(500);
}

// Ligihtup LED for a game play
void lightUpLed() {
    Serial.print("game counter is: ");
    Serial.println(gameCounter);
    delay(500);
    digitalWrite(countDownLEDPin1, HIGH);
    delay(500);
    digitalWrite(countDownLEDPin2, HIGH);
    delay(500);
    digitalWrite(countDownLEDPin3, HIGH);
    delay(500);
    player1RandomPin = random(player1RedLEDPin, player1GreenLEDPin+1);
    player2RandomPin = player1RandomPin + 10;
    digitalWrite(player1RandomPin, HIGH);
    player1LightOnTime = millis();
    digitalWrite(player2RandomPin, HIGH);
    player2LightOnTime = millis();
}

// helper function for determine if a button is pressed
bool isPressed(int current, int prev) {
  return current > prev;
}

// LED light flashing effect for the winner
void winnerLightEffect(int redLedPin, int yellowLedPin, int greenLEDPin) {
  for (int i = 0; i < 5; i++) {
    digitalWrite(redLedPin, HIGH);
    delay(100);
    digitalWrite(yellowLedPin, HIGH);
    delay(100);
    digitalWrite(greenLEDPin, HIGH);  
    delay(200);
    digitalWrite(greenLEDPin, LOW);
    delay(100);
    digitalWrite(yellowLedPin, LOW);
    delay(100);
    digitalWrite(redLedPin, LOW);  
    delay(200);
    }
  for (int i = 0; i < 4; i++) {
    digitalWrite(redLedPin, HIGH);
    digitalWrite(yellowLedPin, HIGH);
    digitalWrite(greenLEDPin, HIGH);  
    delay(400);
    digitalWrite(redLedPin, LOW);
    digitalWrite(yellowLedPin, LOW);
    digitalWrite(greenLEDPin, LOW);  
    delay(400);
    }
  player1ReactionTime = 0;
  player2ReactionTime = 0;
}

void loop() {
  currentStartButtonState = digitalRead(startButton);
  currentPlayer1LEDButtonState = digitalRead(player1RandomPin + 3);
  currentPlayer2LEDButtonState = digitalRead(player2RandomPin - 3);
  if (isPressed(currentStartButtonState, lastStartButtonState)) {
    Serial.println("start button is pressed");
    servoMotor.write(90);
    lightUpLed();
  } else if (isPressed(currentPlayer1LEDButtonState, lastPlayer1LEDButtonState) && gameCounter < 5){
    Serial.println("player 1 reacted");
    player1ReactionTime += millis() - player1LightOnTime;
    player1Reacted = true;
    digitalWrite(player1RandomPin, LOW);
    delay(500);
  } else if (isPressed(currentPlayer2LEDButtonState, lastPlayer2LEDButtonState) && gameCounter < 5){
    Serial.println("player 2 reacted");
    player2ReactionTime += millis() - player2LightOnTime;
    player2Reacted = true;
    digitalWrite(player2RandomPin, LOW);
    delay(500);
  } else if (player1Reacted && player2Reacted && gameCounter < 5){
    gameCounter++;
    if (gameCounter < 5) {
      reset();
      lightUpLed();
    }   
  } else if (gameCounter == 5) {
    if (player1ReactionTime > player2ReactionTime) {
      Serial.print("the winner is: winner 2");
      servoMotor.write(0);
      winnerLightEffect(player2RedLEDPin, player2YellowLEDPin, player2GreenLEDPin);
      } else {
        Serial.print("the winner is: winner 1");
        servoMotor.write(180);
        winnerLightEffect(player1RedLEDPin, player1YellowLEDPin, player1GreenLEDPin);
        }
    reset();
    gameCounter = 0;
  }

  lastStartButtonState = currentStartButtonState; 
  lastPlayer1LEDButtonState = currentPlayer1LEDButtonState;
  lastPlayer2LEDButtonState = currentPlayer2LEDButtonState;
}
