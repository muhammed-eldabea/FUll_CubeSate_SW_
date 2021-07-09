

#include <Wire.h>
#include <Servo.h>

/*****************************************************/
#define MOTOR_SERVO_ARMS_PIN 9 
#define MOTOR_SERVO_POSITION_PIN 8
#define PAYLOAD_PIN 5
#define INDICATOR_PIN 10 
#define TEMPRETURE_SENSOR_PIN A0
#define VOLTAGE_SENSOR_PIN A1
#define LDR1_PIN A2
#define LDR2_PIN A3 

//#define CHARGE_PIN_INDICATOR 4 

/******************************************************/
Servo myservo ;
Servo initi_servo ; 
/******************************************************/
int Command = 0 ; 
int TELMETRY[3] = {0,0,0} ; 
int counter = 0 ;
int Test_CHARGE_MODE  = 0  ; 
int PAYLOAD_DELAY_TO_RUN = 0;
int PAYLOAD_RUN_Duration   = 0 ;
int Data_in[4] = {0,} ; 



void setup()
{
  Wire.begin(9);                // join i2c bus with address #8
  Serial.begin(9600) ; 
  Wire.onRequest(requestEvent); // register event
  Wire.onReceive(receiveEvent);
    /* Servos Setup*/
  initi_servo.attach(MOTOR_SERVO_ARMS_PIN);
  myservo.attach(MOTOR_SERVO_POSITION_PIN);
 
     /*PIN direction */
  pinMode(PAYLOAD_PIN, OUTPUT);
  pinMode(TEMPRETURE_SENSOR_PIN,INPUT);
  pinMode(VOLTAGE_SENSOR_PIN,INPUT);
  pinMode(LDR1_PIN,INPUT);
  pinMode(LDR2_PIN,INPUT);
  pinMode(INDICATOR_PIN,OUTPUT);
  digitalWrite(INDICATOR_PIN,HIGH);
  
  
   /******************************************************************************************/
  /* servo intitlization */  
     initi_servo.attach(MOTOR_SERVO_ARMS_PIN); 
     for (int pos = 0; pos <= 90; pos += 1) { // goes from 0 degrees to 180 degrees
     initi_servo.write(pos);              // tell servo to go to position in variable 'pos'
     delay(15);                       // waits 15ms for the servo to reach the position
    /*************************************************************************************************/
  } 
}






void loop() {
  //delay(6000) ; 
  Read_Sensor_Vaule() ;
  Update_Telemetry_data() ;
  delay(2000) ;  
  RUN_PAY_LOAD();
 Charge_Mode () ;   
 for (int i =  0 ; i<3 ; i++) 
 {
  switch(i){
    case 0:  Serial.print("tempreture is " ) ; Serial.println (Data_in[i]); break ;
    case 1:  Serial.print("volt is " ) ; Serial.println (Data_in[i]); break ;
    case 2:  Serial.print("ldr is " ) ; Serial.println (Data_in[i]); break ;

    
    
    }
  }
}

// function that executes whenever data is requested by master
// this function is registered as an event, see setup()
void requestEvent() {
  for (int i =  0 ; i < 3 ; i++) 
  { 
  Wire.write(TELMETRY[i]); // respond with message of 2 bytes
  // as expected by master
}
}



void Read_Sensor_Vaule() 
{

/*--------------------------------------------------------------------------------------

[FUNCTION NAME] : this function will get the sensor reading and 
[DESRIPTION]    : read the sensor value 
[ARGUMENT(S)]   : 
   [IN]         : void 
[Return]        : void 

---------------------------------------------------------------------------------------*/

  Data_in[0] = map(analogRead(TEMPRETURE_SENSOR_PIN), 0, 1023, 0, 255); /*Temp*/
  Data_in[1] = map(analogRead(VOLTAGE_SENSOR_PIN), 0, 1023, 0, 5); /*Volat*/

  Data_in[2] = map(analogRead(LDR1_PIN), 0, 1023, 0, 255);   /*LDR1*/
  Data_in[3] = map(analogRead(LDR2_PIN), 0, 1023, 0, 255);   /*LDR2*/

} 




void Update_Telemetry_data() 
{
   
  TELMETRY[0] = Data_in[0];
  TELMETRY[1] = Data_in[1];
  if (Data_in[2] < Data_in[3]) 
{
  
  TELMETRY[2] = Data_in[2];
}
else 
{
 
TELMETRY[2] = Data_in[3];
}
  
} 

void receiveEvent(int howMany) {

  PAYLOAD_DELAY_TO_RUN = Wire.read() ;
  PAYLOAD_RUN_Duration = Wire.read() ;
  Test_CHARGE_MODE = Wire.read(); 
  Serial.println("Command" ) ; 
Serial.println(PAYLOAD_DELAY_TO_RUN) ;
Serial.println(PAYLOAD_RUN_Duration) ;
Serial.println(Test_CHARGE_MODE) ;  
  
}

 
void RUN_PAY_LOAD() 
{

  /*--------------------------------------------------------------------------------------
[FUNCTION NAME] :  RUN_PAY_LOAD 
[DESRIPTION]    : this function is responsiable to take action to the payload 
                  depending on tow paramter the delay time and run time duration 
[ARGUMENT(S)]  : void 
   [IN]  : void 
[Return]      : void 

---------------------------------------------------------------------------------------*/

  if (PAYLOAD_DELAY_TO_RUN > 0 ) 
  {
    delay(PAYLOAD_DELAY_TO_RUN*1000) ; 
    digitalWrite(PAYLOAD_PIN,HIGH) ;
    delay(PAYLOAD_RUN_Duration*1000) ;
    digitalWrite(PAYLOAD_PIN, LOW);
    PAYLOAD_DELAY_TO_RUN  = 0 ;
    PAYLOAD_RUN_Duration = 0 ; 
  }

} 

void Charge_Mode () 
{
  /*--------------------------------------------------------------------------------------
[FUNCTION NAME] :Get_MAx_LDR_Value
[DESRIPTION]    : Get the max value of LDR sensor that 
                  used to control the coordinate of the cube  
[ARGUMENT(S)]   : 
   [IN]         : void 
[Return]        : int data represent the max value 
---------------------------------------------------------------------------------------*/



if (Data_in[2] < Data_in[3]) 
{ 
  

     myservo.write(0);              
    
 
}
else 
{
     myservo.write(180);              

}

 
} 
