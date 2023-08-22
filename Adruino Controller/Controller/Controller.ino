#include <cmath>
#include <deque>
#include <movingAvg.h>

int accelerator = A0; // Accelerator pin output
int brake = A1; // Brake sensor output
int steering = A2;
int accVal = 0;  // variable to store the value read
int brakeVal = 0;
int steerVal = 0;
float new_accVal;
float new_brakeVal;
float new_steerVal;

movingAvg avgTemp(15);

void setup() {
  Serial.begin(115200);
  avgTemp.begin();
}

void loop() {
  accVal = analogRead(accelerator);// read the acceleration range (lookup table needed to find acceleration corresponding to voltage)
  brakeVal = analogRead(brake);   // read the brake value (0 or 1)
  steerVal = analogRead(steering);

  
  float avg = avgTemp.reading(steerVal);

  // new_steerVal = map(avg, 286.0, 713.0, -1.0, 1.0);
  // new_accVal = map(accVal, 277.0, 1023.0, 0.0, 1.0);
  // new_brakeVal = map(steerVal, 0.0, 350.0, 0.0, 1.0);
  new_steerVal = Normalizer(avg, 301, 721, -1, 1);
  new_accVal = Normalizer(accVal, 297, 1023, 0, 1);
  new_brakeVal = Normalizer(brakeVal, 19, 200, 0, 1);

#if 0
  Serial.print(accVal);          // accelerator voltage value
  Serial.print(";");
  Serial.print(brakeVal);
  Serial.print(";");
  Serial.print(steerVal);
  Serial.println(); 
#endif

#if 1
  Serial.print(new_accVal);          // accelerator voltage value
  Serial.print(";");
  Serial.print(1-new_brakeVal);
  Serial.print(";");
  Serial.print(-new_steerVal);
  Serial.println();  
#endif

  delay(15);
}

float Normalizer(float X, float min, float max, float rangeX, float rangeY){
  float copy_X = X;
  if (copy_X > max){
    copy_X = max;
  }
  if (copy_X < min){
    copy_X = min;
  }
  return (copy_X - min) * (rangeY - rangeX) / (max - min) + rangeX;
}