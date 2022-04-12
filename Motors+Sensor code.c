// Include the AccelStepper Library
#include <AccelStepper.h>

// Define pin connections
// LOW 
const int dirPin1 = 2;
const int stepPin1 = 3;

const int dirPin2 = 4;
const int stepPin2 = 5;
const int stepsPerRevolution = 60;
const int trigPin = 9;
const int echoPin = 10;

const int metal_distance = 0;
const int glass_distance = 1350;
const int paper_distance = 2850;
const int plastic_distance = metal_distance;
// defines variables
long duration;
int distance;

int DIST_OBJECT;
int IndVal;
int CapVal1;
int CapVal2;
// Define motor interface type  
#define motorInterfaceType 1

// Creates an instance
AccelStepper riley(motorInterfaceType, stepPin1, dirPin1);
AccelStepper sam(motorInterfaceType, stepPin2, dirPin2);

void setup() {
  // set the maximum speed, acceleration factor,
  // initial speed and the target position
  riley.setMaxSpeed(1000);
  riley.setAcceleration(50);
  riley.setSpeed(200);

  sam.setMaxSpeed(1000);
  sam.setAcceleration(700);
  sam.setSpeed(400);
  sam.moveTo(500);

  pinMode(stepPin2, OUTPUT);
  pinMode(dirPin2, OUTPUT);
  
  pinMode(6, INPUT);
  pinMode(7, INPUT);
  pinMode(8, INPUT);

  pinMode(11, OUTPUT);
  pinMode(12, OUTPUT);
  pinMode(13, OUTPUT);

  pinMode(trigPin, OUTPUT); // Sets the trigPin as an Output
  pinMode(echoPin, INPUT); // Sets the echoPin as an Input
  Serial.begin(9600); // Starts the serial communication
}

void loop() { 
  // if the distance sensor detect garbage - identify its type 
  if (read_distance() > 850){
    delay(3000);
    IndVal = digitalRead(6);
    CapVal1 = digitalRead(7);                                                                                                                                                                                                          
    CapVal2 = digitalRead(8);

    // metal
    if (IndVal == LOW) {
      digitalWrite(11, HIGH);
      Serial.println("metal");
      move_motor1(metal_distance);
    }
    
    //glass
    else if (IndVal == HIGH && CapVal1 == LOW && CapVal2 == LOW){
      digitalWrite(12, HIGH);  
      Serial.println("glass1");
     move_motor1(glass_distance);
    }
    
    //paper
    else if(IndVal == HIGH && CapVal1 == HIGH && CapVal2 == LOW){
      digitalWrite(13, HIGH);
      Serial.println("paper");
     move_motor1(paper_distance);
    }
    
    //plastic
    else if(IndVal == HIGH && CapVal1 == HIGH && CapVal2 == HIGH){
      digitalWrite(11, HIGH);
      Serial.println("plastic1");
      move_motor1(metal_distance);
    }
    
    //plastic 
    else{
    Serial.println("plastic2");
    led_on();
    move_motor1(metal_distance);
    }
    
    led_off();
  }
}


// detect if there are any trash
int read_distance() {
  // distance sensor code
  // Clears the trigPin
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  // Sets the trigPin on HIGH state for 10 micro seconds
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  // Reads the echoPin, returns the sound wave travel time in microseconds
  duration = pulseIn(echoPin, HIGH);
  // Calculating the distance
  distance = duration * 0.034 / 2;
  // Prints the distance on the Serial Monitor
  Serial.print("Distance: ");
  Serial.println(distance);
  return distance;
  delay(200);
}

// move motor to the correct bin
void move_motor1(int dist) {
  riley.move(dist);
  riley.runToPosition();
  move_motor2();
  delay(2000);
  riley.move(-dist);
  riley.runToPosition();
}

// move motor to tilt the lid 
void move_motor2(){
  sam.move(-55);
  sam.runToPosition();
  delay(2000);
  sam.move(55);
  sam.runToPosition();
  
  /*digitalWrite(dirPin2, LOW);

  // Spin motor 
  for(int x = 0; x < stepsPerRevolution; x++)
  {
    digitalWrite(stepPin2, HIGH);
    delayMicroseconds(5000);        //lower number = faster spin a
    digitalWrite(stepPin2, LOW);
    delayMicroseconds(5000);
  }
  delay(2000); // Wait a second
  
  //go back

  digitalWrite(dirPin2, HIGH);

  for(int x = 0; x < stepsPerRevolution; x++)
  {
    digitalWrite(stepPin2, HIGH);
    delayMicroseconds(5000);        //lower number = faster spin a
    digitalWrite(stepPin2, LOW);
    delayMicroseconds(5000);
  }*/
  
}

void led_off(){
    digitalWrite(11, LOW);
    digitalWrite(12, LOW);
    digitalWrite(13, LOW);
}

void led_on(){
  digitalWrite(11, HIGH);
    digitalWrite(12, HIGH);
    digitalWrite(13, HIGH);
}