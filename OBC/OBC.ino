
int counter = 0 ; 
int Data[6] = {0,} ; 
int Telemtry_From_EPS [3] = {0,} ;




#include <Wire.h>

void setup() {
  Wire.begin();        // join i2c bus (address optional for master)
  Serial.begin(9600);  // start serial for output
  pinMode(8,INPUT) ; 
}

void loop() {

delay(5000);

OBC_INITIAL_MODE() ;  /*initial Mode */
delay(4000);
OBC_READ_FROM_EPS()  ; /*HouseKeeping */ 
delay(2000);
OBC_READ_FROM_COM()  ; /*Communication mode*/ 
OBC_SEND_COMMAND_TO_EPS ()  ; /*Payload Run*/
OBC_DOWNLOAD_MODE() ; 

  
}


 void OBC_READ_FROM_COM() 
 {
   int counter= 0 ; 
  Wire.requestFrom(8, 3);    // request 6 bytes from slave device #8

  while ((Wire.available()) && (counter  != 6 )) { // slave may send less than requested
    Telemtry_From_EPS[counter] = Wire.read();      // receive a byte as character
    counter++;                                     // print the character
  }
  delay(500);
 } 
 
 
 void OBC_DOWNLOAD_MODE() 
 { 
 Wire.beginTransmission(8); // transmit to device #8
	wire,write(0) ; 
	wire,write(1) ; /*set the comm flag */
	wire.write(Telemtry_From_EPS[0]) ; /*Temp value*/
	wire.write(Telemtry_From_EPS[1]) ; /* Voltage value */
	wire.write(Telemtry_From_EPS[2]) ; /*Max Value */	 
	if (Telemtry_From_EPS[0] > 25) 
	{
		wire.write(1) ; 
	}
	else 
	{
		wire.write(0) ; 
	}
  Wire.endTransmission();    // stop transmitting

	 
 }
 
    

void OBC_READ_FROM_EPS() 
{
   int counter = 0 ;
  Wire.requestFrom(9, 3);    // request 6 bytes from slave device #8

  while ((Wire.available()) && (counter  != 6 )) { // slave may send less than requested
    Data[counter] = Wire.read(); // receive a byte as character
         counter++;    // print the character
  }
}

void RUN_TEST() 
{
for (int i  = 0 ;  i < 6 ; i++) 
{
  Serial.println(Data[i])  ;  
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