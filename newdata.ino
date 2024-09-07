#include <Wire.h>
#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>

#define NUM_SAMPLES     20
#define NUM_GESTURES    30
#define G               9.80665f
#define ACC_THRESHOLD   (1.1f*G)
#define VIBRATION_PERIOD 20 // Vibrate every 20 samples
const int motionThreshold = 5000; 
#define GESTURE_0       0
#define GESTURE_1       1
#define GESTURE_TARGET  GESTURE_0

#define FLEX_SENSOR_1_PIN A0  // Connect flex sensor 1 to analog pin A0
#define FLEX_SENSOR_2_PIN A1  // Connect flex sensor 2 to analog pin A1
#define VIBRATOR_PIN 2        // Connect vibrator motor to digital pin 2

Adafruit_MPU6050 mpu;

void setup() {
  Serial.begin(115200);
  while (!Serial) {
    delay(100);
  }

  pinMode(VIBRATOR_PIN, OUTPUT); // Set vibrator pin as output

  if (!mpu.begin()) {
    Serial.println("Failed to initialize IMU!");
    while (1) {
      delay(100);
    }
  }
  
  mpu.setAccelerometerRange(MPU6050_RANGE_16_G);
  mpu.setGyroRange(MPU6050_RANGE_250_DEG);
  mpu.setFilterBandwidth(MPU6050_BAND_21_HZ);

  // Print CSV column names
  Serial.print("aX");
  for (int i = 0; i < NUM_SAMPLES; i++) {
    Serial.print(",");
    Serial.print("aY");
    Serial.print(i);
    Serial.print(",");
    Serial.print("aZ");
    Serial.print(i);
    Serial.print(",");
    Serial.print("gX");
    Serial.print(i);
    Serial.print(",");
    Serial.print("gY");
    Serial.print(i);
    Serial.print(",");
    Serial.print("gZ");
    Serial.print(i);
    Serial.print(",");
    Serial.print("flex1");
    Serial.print(i);
    Serial.print(",");
    Serial.print("flex2");
    Serial.print(i);
  }
  Serial.println(",target");
}

void loop() {
  sensors_event_t a, g, temp;

  bool movementDetected = false;
  int gesturesRead = 0;

  while (gesturesRead < NUM_GESTURES) {
    // Wait until movement is detected
    while (!movementDetected) {
      mpu.getEvent(&a, &g, &temp);
      int totalAcceleration = abs(a.acceleration.x) + abs(a.acceleration.y) + abs(a.acceleration.z);
      // Check for movement
      if (totalAcceleration > motionThreshold) 
        {
        movementDetected = true;
      }
      delay(100);
    

    // Start sampling
    int samplesRead = 0;
    while (samplesRead < NUM_SAMPLES) {
      mpu.getEvent(&a, &g, &temp);

      // Read flex sensor values
      int flex1Value = analogRead(FLEX_SENSOR_1_PIN);
      int flex2Value = analogRead(FLEX_SENSOR_2_PIN);
  
      samplesRead++;

      Serial.print(a.acceleration.x, 3);
      Serial.print(',');
      Serial.print(a.acceleration.y, 3);
      Serial.print(',');
      Serial.print(a.acceleration.z, 3);
      Serial.print(',');
      Serial.print(g.gyro.x, 3);
      Serial.print(',');
      Serial.print(g.gyro.y, 3);
      Serial.print(',');
      Serial.print(g.gyro.z, 3);
      Serial.print(',');
      Serial.print(flex1Value);
      Serial.print(',');
      Serial.print(flex2Value);

      if (samplesRead == NUM_SAMPLES) {
        Serial.print(',');
        Serial.println(GESTURE_TARGET);
        // Vibrate after collecting 20 samples
        digitalWrite(VIBRATOR_PIN, HIGH); // Activate vibrator motor
        delay(100); // Vibration duration
        digitalWrite(VIBRATOR_PIN, LOW); // Turn off vibrator motor
      } else {
        Serial.print(',');
      }

      delay(100);
    }
    gesturesRead++;
    movementDetected = false; // Reset movement detection for the next gesture
  }

  // Stop the loop once all gestures are read
  while (true) {}
}
}
