#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_LSM303_U.h>
#include <Adafruit_L3GD20_U.h>
#include <Adafruit_9DOF.h>

/* Assign a unique ID to the sensors */
Adafruit_9DOF                dof   = Adafruit_9DOF();
Adafruit_LSM303_Accel_Unified accel = Adafruit_LSM303_Accel_Unified(30301);
Adafruit_LSM303_Mag_Unified   mag   = Adafruit_LSM303_Mag_Unified(30302);

/* Update this with the correct SLP for accurate altitude measurements */
float seaLevelPressure = SENSORS_PRESSURE_SEALEVELHPA;

/* Assign a unique ID to this sensor at the same time */
Adafruit_LSM303_Accel_Unified accel2 = Adafruit_LSM303_Accel_Unified(54321);

int initialHeading = 0;
int currHeading;
int currPitch;

float ZAccel;

/*Used in calibration step
//array of acceleration values to reference against
//float accelerationProfile1[50];
//float accelerationProfile2[50];
//float accelerationProfile3[50];

float avgAccelProfile[50];
*/
int calibrator = 1;

//legnth of both movingArray and test array
int windowSize = 50;

//constantly updates array
float movingArray[50];
//indexer for moving array
int index = 0;



float refDifference1 = -15.1;
float maxRange1 = refDifference1 - 3;
float minRange1 = refDifference1 + 3;

float refDifference2 = 4.075;
float maxRange2 = refDifference2 + (refDifference2*10);
float minRange2 = refDifference2 - (refDifference2*10);


int counter = 0;



/**************************************************************************/
/*!
    @brief  Initialises all the sensors used by this example
*/
/**************************************************************************/
void initSensors()
{
  if(!accel.begin())
  {
    /* There was a problem detecting the LSM303 ... check your connections */
    //Serial.println(F("Ooops, no LSM303 detected ... Check your wiring!"));
    while(1);
  }
  if(!mag.begin())
  {
    /* There was a problem detecting the LSM303 ... check your connections */
    //Serial.println("Ooops, no LSM303 detected ... Check your wiring!");
    while(1);
  }
}



void displaySensorDetails(void)
{
  sensor_t sensor;
  accel2.getSensor(&sensor);
  /*Serial.println("------------------------------------");
  Serial.print  ("Sensor:       "); Serial.println(sensor.name);
  Serial.print  ("Driver Ver:   "); Serial.println(sensor.version);
  Serial.print  ("Unique ID:    "); Serial.println(sensor.sensor_id);
  Serial.print  ("Max Value:    "); Serial.print(sensor.max_value); Serial.println(" m/s^2");
  Serial.print  ("Min Value:    "); Serial.print(sensor.min_value); Serial.println(" m/s^2");
  Serial.print  ("Resolution:   "); Serial.print(sensor.resolution); Serial.println(" m/s^2");
  Serial.println("------------------------------------");
  Serial.println("");*/
  delay(500);
}




/**************************************************************************/
/*!

*/
/**************************************************************************/
void setup(void)
{
  Serial.begin(115200);
  //Serial.println(F("Adafruit 9 DOF Pitch/Roll/Heading Example")); Serial.println("");

  #ifndef ESP8266
  while (!Serial);     // will pause Zero, Leonardo, etc until serial console opens
  #endif
  Serial.begin(115200);
  //Serial.println("Accelerometer Test"); Serial.println("");

  /* Initialise the sensor */
  if(!accel2.begin())
  {
    /* There was a problem detecting the ADXL345 ... check your connections */
    //Serial.println("Ooops, no LSM303 detected ... Check your wiring!");
    while(1);
  }

  /* Display some basic information on this sensor */
  displaySensorDetails();
  
  /* Initialise the sensors */
  initSensors();
}

/**************************************************************************/
/*!
    @brief  Constantly check the roll/pitch/heading/altitude/temperature
*/
/**************************************************************************/
void loop(void)
{
  sensors_event_t accel_event;
  sensors_event_t mag_event;
  sensors_vec_t   orientation;

 
  
  float percentChange;
  /* Get a new sensor event */
  sensors_event_t event;
  accel2.getEvent(&event);
  mag.getEvent(&mag_event);
  accel.getEvent(&accel_event);

  

//  while(calibrator) {
//    Serial.print("\nGet Ready to Tap on Go!\n");
//    delay(1000);
//    Serial.print("Ready....\n");
//    delay(1000);
//    Serial.print("Set....\n");
//    delay(1000);
//    Serial.print("Go!\n");
//    delay(1000);
//
//    for(int i = 0; i < 50; i++) {
//      accelerationProfile1[i] = event.acceleration.z;
//      accel2.getEvent(&event);
//      delay(30);
//    }
//
//    Serial.print("\nGet Ready to Tap on Go!\n");
//    delay(1000);
//    Serial.print("Ready....\n");
//    delay(1000);
//    Serial.print("Set....\n");
//    delay(1000);
//    Serial.print("Go!\n");
//    delay(1000);
//
//     for(int i = 0; i < 50; i++) {
//      accelerationProfile2[i] = event.acceleration.z;
//      accel2.getEvent(&event);
//      delay(30);
//    }
//
//
//    Serial.print("\nGet Ready to Tap on Go!\n");
//    delay(1000);
//    Serial.print("Ready....\n");
//    delay(1000);
//    Serial.print("Set....\n");
//    delay(1000);
//    Serial.print("Go!\n");
//    delay(1000);
//
//     for(int i = 0; i < 50; i++) {
//      accelerationProfile3[i] = event.acceleration.z;
//      accel2.getEvent(&event);
//      delay(30);
//    }
//
//    for(int i = 0; i < 50; i++) {
//      avgAccelProfile[i] = (accelerationProfile1[i] + accelerationProfile2[i] + accelerationProfile3[i])/3;
//      //Serial.print("Accel 1: "); Serial.print(accelerationProfile1[i]); Serial.print("\n");
//      //Serial.print("Accel 2: "); Serial.print(accelerationProfile2[i]); Serial.print("\n");
//      //Serial.print("Accel 3: "); Serial.print(accelerationProfile3[i]); Serial.print("\n");
//
//      //Serial.print("Accel Avg: "); 
//      Serial.print(avgAccelProfile[i]); Serial.print("\n");
//    }
//    
//    calibrator = 0;
//  }
/*
initialHeading = initialHeading/50;
Serial.print("initialHeading: ");
Serial.print(initialHeading);
Serial.print(F("\n"));
*/ 
ZAccel = event.acceleration.z;
////  
////  
  /* Display the results (acceleration is measured in m/s^2) */
//  Serial.print("X: "); Serial.print(event.acceleration.x); Serial.print("  ");
//  Serial.print("Y: "); Serial.print(event.acceleration.y); Serial.print("  ");
//  Serial.print("Z: "); Serial.print(ZAccel); Serial.print("  ");Serial.println("m/s^2 ");
//


/*Updates the moving array with a new value from the acceleration event*/

  if (index == windowSize) {
    //Serial.print("Replacing Values\n");
    //copy all values one place back effectively deleting first value
    for (int i = 0; i < windowSize - 1; i++) {
      movingArray[i] = movingArray[i+1];
    }
    //copy new value into last place
    movingArray[index-1] = event.acceleration.z;
    if (dof.accelGetOrientation(&accel_event, &orientation))
    {
    currPitch = orientation.pitch;
    /*Serial.print(F("pitch: "));
    Serial.print(orientation.pitch);
    Serial.print(F("\n"));*/
    }
     if (dof.magGetOrientation(SENSOR_AXIS_Z, &mag_event, &orientation))
     {
      currHeading = orientation.heading;
    /* 'orientation' should have valid .heading data now */
    //Serial.print(F("Heading: "));
    //Serial.print(orientation.heading);
    //Serial.print(F("\n"));
    //Serial.print(F("CurrHeading: "));
    //Serial.print(currHeading);
    //Serial.print(F("\n"));
    if(calibrator == 1){
      initialHeading = orientation.heading;
      calibrator++;
      /*Serial.print(F("initialHeading: "));
      Serial.print(initialHeading);
      Serial.print(F("\n"));*/
    }
    /*if(calibrator == 51) {
      initialHeading = initialHeading/50;
      calibrator = 52;
      Serial.print(F("initialHeading: "));
      Serial.print(initialHeading);
      Serial.print(F("\n"));
    }*/
    }
  } else {
    movingArray[index-1] = event.acceleration.z;
    if (dof.accelGetOrientation(&accel_event, &orientation))
    {
    currPitch = orientation.pitch;
    //Serial.print(F("pitch: "));
    //Serial.print(orientation.pitch);
    //Serial.print(F("\n"));
    }
    if (dof.magGetOrientation(SENSOR_AXIS_Z, &mag_event, &orientation))
     {
      currHeading = orientation.heading;
      //Serial.print(F("currHeading: "));
      //Serial.print(currHeading);
      //Serial.print(F("\n"));
    /* 'orientation' should have valid .heading data now */
    //Serial.print(F("Heading: "));
    //Serial.print(orientation.heading);
    //Serial.print(F("\n"));
    //Serial.print(F("CurrHeading: "));
    //Serial.print(currHeading);
    //Serial.print(F("\n"));
    /*if(calibrator < 49){
      initialHeading = initialHeading + orientation.heading;
      calibrator++;
    }
    if(calibrator == 49) {
      initialHeading = initialHeading/49;
      calibrator = 50;
      Serial.print(F("initialHeading: "));
      Serial.print(initialHeading);
      Serial.print(F("\n"));
    }*/
    }
  }
  
/***********************************************************************/

/*Test against difference in heights*/

  float movingDifference = movingArray[41] - movingArray[49];
  //float movingDifference2 = movingArray[49] - movingArray [36];
  
//   Serial.print("Moving Array 2nd Value: "); Serial.print(movingArray[2]); Serial.print("  ");
//   Serial.print("Moving Array 10th Value: "); Serial.print(movingArray[10]); Serial.print("  ");
//   Serial.print("Moving Array 23rd Value: "); Serial.print(movingArray[23]); Serial.print("  ");
//    
  if((movingDifference >= maxRange1 && movingDifference <= minRange1) && !(counter < 15)) {
    if(currHeading >=360){
      currHeading = -(currHeading % 360);
    }
      /*Serial.print(F("pitch: "));
      Serial.print(currPitch);
      Serial.print(F("\n"));
      Serial.print(F("initialHeading: "));
      Serial.print(initialHeading);
      Serial.print(F("\n"));
      Serial.print(F("currHeading: "));
      Serial.print(currHeading);
      Serial.print(F("\n"));
      Serial.print(F("diffHeading: "));
      Serial.print(currHeading-initialHeading);
      Serial.print(F("\n"));*/
    if(currHeading-initialHeading > -100 && currHeading-initialHeading < -35){
      /*Serial.print(F("pitch: "));
      Serial.print(currPitch);
      Serial.print(F("\n"));*/
      if(currPitch < -4){
        /*Serial.print(F("initialHeading: "));
        Serial.print(initialHeading);
        Serial.print(F("\n"));
        Serial.print(F("currHeading: "));
        Serial.print(currHeading);
        Serial.print(F("\n"));
        Serial.print(F("diffHeading: "));
        Serial.print(currHeading-initialHeading);
        Serial.print(F("\n"));
        Serial.print("Hi-hat Hit!--------------------------------------------------------------------------\n");*/
        Serial.write(2); //2 means high-hat
        }else{
      /*Serial.print(F("initialHeading: "));
      Serial.print(initialHeading);
      Serial.print(F("\n"));
      Serial.print(F("currHeading: "));
      Serial.print(currHeading);
      Serial.print(F("\n"));
      Serial.print(F("diffHeading: "));
      Serial.print(currHeading-initialHeading);
      Serial.print(F("\n"));
      Serial.print("Snare Drum Hit!--------------------------------------------------------------------------\n");*/
      Serial.write(1); //1 means snare drum
      }
    }
    if(currHeading-initialHeading > -35 && currHeading-initialHeading < 35){
      /*Serial.print(F("initialHeading: "));
      Serial.print(initialHeading);
      Serial.print(F("\n"));
      Serial.print(F("currHeading: "));
      Serial.print(currHeading);
      Serial.print(F("\n"));
      Serial.print(F("diffHeading: "));
      Serial.print(currHeading-initialHeading);
      Serial.print(F("\n"));
      Serial.print("Toms Hit!--------------------------------------------------------------------------\n");*/
      Serial.write(3); //3 means toms
    }
    if(currHeading-initialHeading > 35){
      if(currPitch < -4){
        /*Serial.print(F("initialHeading: "));
        Serial.print(initialHeading);
        Serial.print(F("\n"));
        Serial.print(F("currHeading: "));
        Serial.print(currHeading);
        Serial.print(F("\n"));
        Serial.print(F("diffHeading: "));
        Serial.print(currHeading-initialHeading);
        Serial.print(F("\n"));
        Serial.print("Crash Cymbal Hit!--------------------------------------------------------------------------\n");*/
        Serial.write(4); //4 means crash cymbal
      }else{
      /*Serial.print(F("initialHeading: "));
      Serial.print(initialHeading);
      Serial.print(F("\n"));
      Serial.print(F("currHeading: "));
      Serial.print(currHeading);
      Serial.print(F("\n"));
      Serial.print(F("diffHeading: "));
      Serial.print(currHeading-initialHeading);
      Serial.print(F("\n"));
      Serial.print("Floor Tom Drum Hit!--------------------------------------------------------------------------\n");*/
      Serial.write(5); //5 means floor toms
      }
    }
    counter = 0;
    //Serial.print("Test Case 1 Passed----------------------\n");
    /*if (movingDifference2 <= maxRange2 && movingDifference >= minRange2) {*/
      //Serial.print("Test Case 2 Passed---------------------------------------------\n");
      //Serial.write(1);
      //Serial.print("Snare Drum Hit!--------------------------------------------------------------------------\n");
      //counter = 0;
    /*} else {
      //Serial.print("Test Case 2 Failed\n");
    }*/
  } else{
    //do nothing
  }
  //Serial.print("\nFirst Difference:  "); Serial.print(movingDifference);
  //Serial.print("\nSecond Difference:  "); Serial.print(movingDifference2);
//
//  
//  /* Calculate pitch and roll from the raw accelerometer data */
//  accel.getEvent(&accel_event);
//  if (dof.accelGetOrientation(&accel_event, &orientation))
//  {
////    /* 'orientation' should have valid .roll and .pitch fields */
////    Serial.print(F("Roll: "));
////    Serial.print(orientation.roll);
////    Serial.print(F("; "));
//    lastPitch = orientation.pitch;
//    Serial.print(F("Pitch: "));
//    Serial.print(lastPitch);
//    Serial.print("\n");
////    Serial.print(F("; "));
//  }
////  
////  /* Calculate the heading using the magnetometer */
////  mag.getEvent(&mag_event);
////  if (dof.magGetOrientation(SENSOR_AXIS_Z, &mag_event, &orientation))
////  {
////    /* 'orientation' should have valid .heading data now */
////    Serial.print(F("Heading: "));
////    Serial.print(orientation.heading);
////    Serial.print(F("; "));
//  }

  //Serial.println(F(""));
  delay(30);

  if(index == windowSize) {
    //Serial.print("Moving Array Filled\n");
  } else {
    index++; //otherwise increment it by one
    //Serial.print("Index Value:  "); Serial.print(index);
    //Serial.print("\nWindow Size:  "); Serial.print(windowSize);
  }
  if (counter == 15) {
    //do nothing;
  } else {
    counter++;
  }
  //Serial.print("\n");
}
