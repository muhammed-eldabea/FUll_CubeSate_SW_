
int counter = 0 ; 
int Data[6] = {0,} ; 
int Telemtry_From_EPS [3] = {4,5,1} ;




#include <Wire.h>

void setup() {
  Wire.begin();        // join i2c bus (address optional for master)
  Serial.begin(9600);  // start serial for output
  pinMode(8,INPUT) ; 
}

void loop() {

//delay(10000);

OBC_INITIAL_MODE() ;  /*initial Mode */
OBC_READ_FROM_EPS()  ; /*HouseKeeping */ 
delay(2000);
OBC_READ_FROM_COM()  ; /*Communication mode*/ 
OBC_DOWNLOAD_MODE() ; 
OBC_SEND_COMMAND_TO_EPS ()  ; /*Payload Run*/

RUN_TEST() ;

}


 void OBC_READ_FROM_COM() 
 {
   int counter= 0 ; 
  Wire.requestFrom(8, 5);    // request 6 bytes from slave device #8

  while ((Wire.available()) && (counter  != 5 )) { // slave may send less than requested
    Data[counter] = Wire.read();      // receive a byte as character
    counter++;                                     // print the character
  }
  delay(500);
 } 
 
 
 void OBC_DOWNLOAD_MODE() 
{ 
  int Flag=0 ; 
  if (Telemtry_From_EPS[0] > 25) 
  {
    Flag=1 ; 
  }
  
 
 Wire.beginTransmission(8); // transmit to device #8
	Wire.write(0) ; 
	Wire.write(1) ; /*set the comm flag */
	Wire.write(Telemtry_From_EPS[0]) ; /*Temp value*/
	Wire.write(Telemtry_From_EPS[1]) ; /* Voltage value */
	Wire.write(Telemtry_From_EPS[2]) ; /*Max Value */	 
  Wire.write(Flag) ; 
  Wire.endTransmission();    // stop transmitting

	 
 }
 
    

void OBC_READ_FROM_EPS() 
{
   int counter = 0 ;
  Wire.requestFrom(9, 3);    // request 6 bytes from slave device #8

  while ((Wire.available()) && (counter  != 3 )) { // slave may send less than requested
    Telemtry_From_EPS[counter] = Wire.read(); // receive a byte as character
         counter++;    // print the character
  }
}

void RUN_TEST() 
{
        Serial.println("Command") ; 
for (int i  = 0 ;  i < 6 ; i++) 
{
  Serial.println(Data[i])  ;  
  }
      Serial.println("TELEMETRY") ; 
  for (int i  = 0 ;  i < 3 ; i++) 
{
  Serial.println(Telemtry_From_EPS[i])  ;  
  }
}

void OBC_INITIAL_MODE() 
{
  OBC_SEND_PING_TO_COMM ()  ; 
}



void OBC_SEND_COMMAND_TO_EPS(int Command) 
{
    Wire.beginTransmission(9); // transmit to device #8
    Wire.write(Command);              // sends one byte
  Wire.endTransmission();    // stop transmitting

}


void OBC_SEND_PING_TO_COMM () 
{
  Wire.beginTransmission(8); // transmit to device #8
  Wire.write(1);
for  (int i = 0 ; i<4 ; i++) 
{
	Wire.write(0);
}
  Wire.endTransmission();    // stop transmitting

	
} 



void OBC_SEND_COMMAND_TO_EPS () 
{
  Wire.beginTransmission(9); // transmit to device #8
  Wire.write(Data[1]);
  Wire.write(Data[2]);
  if (Data[5] == 2)
  { 
/*to test charge mode */
    Wire.write(1);
  }
    else {
      Wire.write(0);
    }

  // sends one byte
  Wire.endTransmission();    // stop transmitting
}
void OBC_RUN_Test_MOde() 
{
  
}
