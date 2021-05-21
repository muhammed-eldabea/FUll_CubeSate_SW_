

#include <Wire.h>
#include <Servo.h>


/*****************************************************/
#define MOTOR_SERVO_ATTACH_PIN 9
#define PAYLOAD_PIN 5
#define CHARGE_PIN_INDICATOR 4 

/******************************************************/
Servo myservo ;


/******************************************************/

int Command = 0 ; 
int Data[3] = {0,} ; 
int counter = 0 ;
int Test_CHARGE_MODE  = 0  ; 
int PAYLOAD_DELAY_TO_RUN = 0;
int PAYLOAD_RUN_Duration   = 0 ;
int Data_in[4] = {0,} ; 



void setup()
{
  Wire.begin(9);                // join i2c bus with address #8
  Wire.onRequest(requestEvent); // register event
  Wire.onReceive(receiveEvent);
  myservo.attach(MOTOR_SERVO_ATTACH_PIN);
  pinMode(PAYLOAD_PIN, OUTPUT);
  pinMode(CHARGE_PIN_INDICATOR, OUTPUT);
}

void loop() {
  Read_Sensor_Vaule() ;
  Update_Telemetry_data() ;
  delay(2000) ;  
  RUN_PAY_LOAD();
  CHECK_FOR_CHARGE() ; 
}

// function that executes whenever data is requested by master
// this function is registered as an event, see setup()
void requestEvent() {
  for (int i =  0 ; i < 3 ; i++) 
  { 
  Wire.write(Data[i]); // respond with message of 6 bytes
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

  Data_in[0] = map(analogRead(A0), 0, 1023, 0, 255); /*Temp*/
  Data_in[1] = map(analogRead(A1), 0, 1023, 0, 255); /*Volat*/

  Data_in[2] = map(analogRead(A2), 0, 1023, 0, 255);   /*LDR1*/
  Data_in[3] = map(analogRead(A0), 0, 1023, 0, 155);   /*LDR2*/

} 





int Get_MAx_LDR_Value () 
{
  /*--------------------------------------------------------------------------------------
[FUNCTION NAME] :Get_MAx_LDR_Value
[DESRIPTION]    : Get the max value of LDR sensor that 
                  used to control the coordinate of the cube  
[ARGUMENT(S)]   : 
   [IN]         : void 
[Return]        : int data represent the max value 
---------------------------------------------------------------------------------------*/

  int max = Data_in[2];
  for (int i = 3; i < 4; i++)
  {
    if (Data_in[i] >= max)
    {
      max = Data_in[i];
    }
}
return max ; 
} 


int Get_MAx_LDR_index()
{
  /*--------------------------------------------------------------------------------------
[FUNCTION NAME] :Get_MAx_LDR_index
[DESRIPTION]    : get index of max value to select the coordinte angle for charge battery   
[ARGUMENT(S)]   : 
   [IN]         : void 
[Return]        : int data represent the max value 
---------------------------------------------------------------------------------------*/

  int max = Get_MAx_LDR_Value() ; 
  int index = 0 ;
  for (int i = 2; i < 4; i++)
  {
    if (Data_in[i] >= max)
    {
      index = i ; 
    }
  }
  return i ; 
}

void Update_Telemetry_data() 
{
   
  Data[0] = Data_in[0];
  Data[1] = Data_in[1];
  Data[2] = Get_MAx_LDR_Value();
} 

void receiveEvent(int howMany) {

  PAYLOAD_DELAY_TO_RUN = wire.read() ;
  PAYLOAD_RUN_Duration = wire.read() ;
  Test_CHARGE_MODE = wire.read();
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



void CHECK_FOR_CHARGE() 
{
  if ((Data_in[1]) < 3 || (Test_CHARGE_MODE ==1)) 
  {
    SERVO_CONTROL_ANGLE(Get_MAx_LDR_index()) ; 
    
    for (int i = 0 ; i <4 ; i++) {
    digitalWrite(CHARGE_PIN_INDICATOR,HIGH) ; 
    delay(1500) ;
    digitalWrite(CHARGE_PIN_INDICATOR, LOW);
   } 

  }
} 


void SERVO_CONTROL_ANGLE(iny index )  
{ 
  switch (index+1)
  {
  case 1 :
    myservo.write(0);
    break;
  case 2 :
    myservo.write(90);
    break;

  default:
    myservo.write(0);
    break;
  }

}