#include <Wire.h>
#include <Stepper.h>

Stepper motor(2038, 12, 11, 10, 9);

//static const float STEPS_PER_MIN = 203,78864;
static const float STEPS_PER_MIN = 204.260375;

float stepsWhole;
float stepsDecimal;

float stepsOverhang;

int stepsTotal = 0;

void motorMove(int time) {
    if (time != 0) {
      int steps = stepsWhole * time;
      stepsOverhang += stepsDecimal * time;

      Serial.println(stepsOverhang);
      
      if (stepsOverhang >= 1) {
        int overhangWhole = floor(stepsOverhang);
        stepsOverhang -= overhangWhole;

        steps += overhangWhole;
        Serial.println(overhangWhole);
      }

      Serial.println(steps);
      motor.step(steps);

      stepsTotal += steps; 
    }
  }

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);

  motor.setSpeed(10);

  stepsWhole = floor(STEPS_PER_MIN);
  stepsDecimal = STEPS_PER_MIN - stepsWhole;

  for (int i = 1; i <= 60; i += 1) {
    Serial.println(i);
    motorMove(10);
    delay(1000);
  }

  Serial.println(stepsTotal);
}

void loop() {
  // put your main code here, to run repeatedly:

}
