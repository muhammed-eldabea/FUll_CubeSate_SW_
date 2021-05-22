

#include <Wire.h>




#define SEND_PING_TO_GROUND 12
#define SEND_TELM_TO_GROUND 11 
#define GET_COMMAND_FROM_GROUND 10 

int Data[6] = {6,7,8,9,10,11} ; 

/*  << PING >> << COM_FLAG >> <<T1>> <<T2>> <<T3>>                           */
int Data_FROM_OBC[6] = {0,} ;  

void setup() {
  Wire.begin(8);                // join i2c bus with address #8
  Wire.onRequest(requestEvent); // register event
  Wire.onReceive(receiveEvent);
  Serial.begin(9600) ; 
}


void loop() {
  delay(100); 
  if (Data_FROM_OBC[0] == 1 ) 
  {
	  Serial.write(SEND_PING_TO_GROUND) ; 
	  Data_FROM_OBC[0] = 0 ;  /*clear the PING_FLAG */
  }
  if (Data_FROM_OBC[1] == 1 ) 
  {
	  Serial.write(SEND_TELM_TO_GROUND) ; 
	  COMM_SEND_TELM_DATA() ; 
	  Serial.write(GET_COMMAND_FROM_GROUND) ; 
	  COMM_GET_COMMAND_FRON_GS() ; 
	  Data_FROM_OBC[1] = 0 ;  /*clear the COMM_FLAG */
  }
	
  
  
}


void requestEvent() {
   for (int i =  0 ; i < 6 ; i++) 
  { 
  Wire.write(Data[i]); 
  
}
}

void receiveEvent(int howMany) {
  
  volatile int counter = 0 ; 
  while ((1 < Wire.available()) && (counter != 6)) { // loop through all but the last
    Data_FROM_OBC[counter] = Wire.read(); 
    counter++ ;         
  }
         
}

void COMM_SEND_TELM_DATA() 
{
	for (int i = 2 ; i<6 ; i++)
	{
		Serial.write(Data_FROM_OBC[i]) ;  
	}
} 


void COMM_GET_COMMAND_FRON_GS() 
{ 
volatile int counter = 0 ; 
	while ( (Serial.available() > 1) && (counter != 6 ) )
	{
		Data[counter]=Serial.read() ; 
		counter++ ; 
	}
	
}