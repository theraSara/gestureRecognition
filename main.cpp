#include <Arduino_LSM9DS1.h>
#include <TensorFlowLite.h>
#include <tensorflow/lite/micro/all_ops_resolver.h>
#include <tensorflow/lite/micro/micro_interpreter.h>
#include <tensorflow/lite/schema/schema_generated.h>
#include "model.h"

#define BLUE 22     
#define RED 23
#define LED_PWR 25

enum{Idle, CollectX, Classify, Control};
int current = Idle;

using namespace tflite; 
AllOpsResolver tflOpsResolver; 
const Model* tflModel = nullptr;
MicroInterpreter* tflInterpreter = nullptr;
TfLiteTensor* tflInputTensor = nullptr;
TfLiteTensor* tflOutputTensor = nullptr;

constexpr int tensorArenaSize = 8 * 1024;
byte tensorArena[tensorArenaSize] __attribute__((aligned(16)));

const char* GESTURES[]  = {"punch", "flex"};

float aX, aY, aZ, gX, gY, gZ;
const int numSamples = 119; 
const float accelerationThreshold = 2.5; 
int samplesRead = numSamples; 

void setup() {
  Serial.begin(9600); 
  pinMode(BLUE, OUTPUT);
  pinMode(RED, OUTPUT);
  pinMode(LED_PWR, OUTPUT);

  tflModel = GetModel(model); 
  if(tflModel->version() != TFLITE_SCHEMA_VERSION){
    Serial.println("Wrong");
    while(1); 
  }

  tflInterpreter = new MicroInterpreter(tflModel, tflOpsResolver, 
                                            tensorArena, tensorArenaSize);
  tflInterpreter -> AllocateTensors(); 
  tflInputTensor = tflInterpreter-> input(0); 
  tflOutputTensor = tflInterpreter -> output(0); 
  
  if(!IMU.begin())
  {
    Serial.println("Failed to Load Sensors");
    while(1); 
  }
}

void loop() {
  //  Testing LED's Connection
  //digitalWrite(RED, HIGH); 
  //digitalWrite(BLUE, LOW);
  //delay(100);
  //digitalWrite(BLUE, HIGH); 
  //digitalWrite(RED, LOW);
  switch(current){
    case Idle:
      current = CollectX;
      Serial.println("Start!");
    break;

    case CollectX:
      if(IMU.accelerationAvailable()) 
      {
        IMU.readAcceleration(aX,aY,aZ);
        float aSum = fabs(aX) + fabs(aY) + fabs(aZ); 
        if(aSum >= accelerationThreshold) 
        {
          samplesRead = 0; 
          current = Classify;
        }
      }
    break;

    case Classify:
      while (samplesRead < numSamples)
      {
        if(IMU.accelerationAvailable() && IMU.gyroscopeAvailable())
        {
          IMU.readAcceleration(aX,aY,aZ);
          IMU.readGyroscope(gX,gY,gZ);
          samplesRead++;
          tflInputTensor->data.f[samplesRead*6 +0] = (aX + 4.0)/8.0;   
          tflInputTensor->data.f[samplesRead*6 +1] = (aY + 4.0)/8.0;
          tflInputTensor->data.f[samplesRead*6 +2] = (aZ + 4.0)/8.0;
          tflInputTensor->data.f[samplesRead*6 +3] = (gX + 2000)/4000;
          tflInputTensor->data.f[samplesRead*6 +4] = (gY + 2000)/4000;
          tflInputTensor->data.f[samplesRead*6 +5] = (gZ + 2000)/4000;
        }
        if(samplesRead == numSamples)
        {
          current = Control;
        }
      }
      Serial.println("Recording Done");
      current = Control;
    break;

    case Control:
      TfLiteStatus invokeStatus = tflInterpreter -> Invoke();
      if (invokeStatus != kTfLiteOk)
      {
        Serial.println("Failed");
        while(1);
      }

      double confidenceInPunch = tflOutputTensor->data.f[0];
      double confidenceInFlex = tflOutputTensor->data.f[1];
      Serial.print("Punch: ");
      Serial.println(confidenceInPunch);
      Serial.print("Flex: ");
      Serial.println(confidenceInFlex);

      if (confidenceInPunch > confidenceInFlex) 
      {
        digitalWrite(RED, HIGH); // RED LED on for Punch
        digitalWrite(BLUE, LOW); 
        digitalWrite(LED_PWR, HIGH); 
      } 
      else 
      {
        digitalWrite(RED, LOW); 
        digitalWrite(BLUE, HIGH); // BLUE LED on for Flex
        digitalWrite(LED_PWR, HIGH);
      }
      current = Idle;
    break;
  }
}
