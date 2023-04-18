#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>
#include <Wire.h>
#include <Stepper.h>

Stepper motor(2038, 12, 11, 10, 9);
Adafruit_MPU6050 mpu;

int globalTime;

class ParkManager {
  static const float GRAVITY = 9.71;
  static const float GRAVITY_RANGE = 0.25;

  static const float STEPS_PER_MIN = 203.78864;

  float stepsWhole;
  float stepsDecimal;

  float stepsOverhang;

  int oldTime;
  
  bool moveOnNextUpdate;

  bool checkMovement() {
    sensors_event_t a, g, temp;
    mpu.getEvent(&a, &g, &temp);

    if (a.acceleration.z <= GRAVITY - GRAVITY_RANGE) return true;
    else if(a.acceleration.z >= GRAVITY + GRAVITY_RANGE) return true;
    else return false;
  }

  bool motorMove(int time) {
    if (time != 0) {
      int steps = stepsWhole * time;
      stepsOverhang += stepsDecimal * time;
      
      if (stepsOverhang >= 1) {
        int overhangWhole = floor(stepsOverhang);
        stepsOverhang -= overhangWhole;

        steps += overhangWhole;
      }

      motor.step(steps);
    }
  }

  public:
    ParkManager() {
      stepsOverhang = 0;
      oldTime = 0;

      stepsWhole = floor(STEPS_PER_MIN);
      stepsDecimal = STEPS_PER_MIN - stepsWhole;

      moveOnNextUpdate = false;

      motor.setSpeed(10);
    }

    void Update(int time) {
      bool moving = checkMovement();

      if (moving) {
        if (time % 10 == 0 || moveOnNextUpdate) {
          motorMove(time - oldTime);
          oldTime = time;
          moveOnNextUpdate = false;
        }
      }
      else if (time % 10 == 0) moveOnNextUpdate = true;

      Serial.flush();
      Serial.println("t");
      Serial.println(time);
      Serial.println("n");
      Serial.println(moveOnNextUpdate);
    }
};

ParkManager manager;

// returns how many minutes have elapsed since start-up.
int getMinutes() {
  return millis() / 2500;
}

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);

  if (!mpu.begin())
  {
    while (1) delay(10);
  }
}

void loop() {
  // put your main code here, to run repeatedly:
  globalTime = getMinutes();

  manager.Update(globalTime);

  delay(1000);
}
