

#include <Wire.h>



#define SYNCH_DONE         99 
#define SEND_PING_TO_GROUND 12
#define SEND_TELM_TO_GROUND 11 
#define GET_COMMAND_FROM_GROUND 10 

int Data[5] = {0,} ; 

String Data_str ; 

/*  << PING >> << COM_FLAG >> <<T1>> <<T2>> <<T3>>                           */
int Data_FROM_OBC[6] = {0,0,0,0,0,0} ;  

void setup() {
  Wire.begin(8);                // join i2c bus with address #8
  Wire.onRequest(requestEvent); // register event
  Wire.onReceive(receiveEvent);
  Serial.begin(38400) ; 
}


void loop() {
 // delay(4010); 
  for (int i = 0 ; i<6 ; i++) 
  {
    Serial.println(Data_FROM_OBC[i]) ; 
  }
  if (Data_FROM_OBC[0] == 1 ) 
  {
    Serial.write(SEND_PING_TO_GROUND) ;  
  /* while(Serial.read() != SYNCH_DONE) {
    Serial.write(SEND_PING_TO_GROUND) ;}*/
    Data_FROM_OBC[0] = 0 ;  /*clear the PING_FLAG */
  }
delay(1000) ; 
  
  if (Data_FROM_OBC[1] == 1 ) 
  {
   Serial.write(GET_COMMAND_FROM_GROUND) ;
  /* 
   while(Serial.read() != SYNCH_DONE) 
   {
       Serial.write(GET_COMMAND_FROM_GROUND) ;
   }*/ 
    COMM_GET_COMMAND_FRON_GS() ; 

    delay(15) ; 
    Serial.write(SEND_TELM_TO_GROUND) ; 
  /*  while(Serial.read() != SYNCH_DONE) {
    Serial.write(SEND_TELM_TO_GROUND) ;}
    */
    COMM_SEND_TELM_DATA() ; 
    
    Data_FROM_OBC[1] = 0 ;  /*clear the COMM_FLAG */
  }
  
   
  for (int i = 0 ; i <5 ; i++) 
  {
    Serial.println(Data[i]) ; 
  }
}



void requestEvent() {
   for (int i =  0 ; i < 6 ; i++) 
  { 
  Wire.write(Data[i]); 
  
}
}

void receiveEvent(int howMany) {
   
  for (int counter = 0 ; counter<5 ; counter++) { // loop through all but the last
    Data_FROM_OBC[counter] = Wire.read(); 
            
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
 
  for (int counter = 0 ; counter<5 ; counter++) 
    {
    Data_str = Serial.read() ; 
    Data[counter]= Data_str.toInt() ; 
     
  }
  
}
