

/*==================================================================*/

                /* Include the main Library needded */

#include <Wire.h>  


/*===================================================================*/
                    /* System Address Defination   */ 
#define OBC_ADD  1
#define EPS_ADD  2
#define COM_ADD  3
#define PAYLOAD_ADD  4 

/*===================================================================*/
                    /* COMMAND IDs  */

#define PayLoad_Start_Action  1 
#define Send_Telemetry_Data   2 
#define Delay_for_Time        3 




/*===================================================================*/
                    /* MODES IDs  */ 
#define Initial_Mode    1 
#define Imaging_Mode    2 
#define DownLoad_Mode   3 
#define Comm_Mode




/*===================================================================*/
                    /* Tasks Decleration  */




/*===================================================================*/
            /* Defination for Data structre used */

unsigned char Command_Buffer[20] = {0,} ;
unsigned char Telemetry_Buffer[20] = {0,} ;  


/*===================================================================*/

void setup() {
  // put your setup code here, to run once:

}

void loop() {
  // put your main code here, to run repeatedly:

} 



/*===================================================================*/
                  
                  /* Main Tasks Defiantion */
void OBC_TeleComand_Task() 
{
  
}  



/*===================================================================*/

                        /* Sub-Task */ 

void OBC_GetDataFrom_COM() 
{
    Get_telemetry_Data_FROM_COM() ; 
} 






/*===================================================================*/
              
                 /*Hardware Communication Task */ 

void  Get_telemetry_Data_FROM_System(unsigned char system_ID , unsigned char telemetry_size) 
{ 
 unsigned char counter = 0 ;  
   Wire.requestFrom(system_ID, telemetry_size);    // request 6 bytes from slave device #8
  while (Wire.available()) { // slave may send less than requested
    Telemetry_Buffer[counter] = Wire.read();
    counter++ ;  
  }
  
} 

void OBC_Send_Command (unsigned char system_ID ,unsigned char command  ) 
{ 
    Wire.beginTransmission(system_ID); // transmit to device ID
    Wire.write(command);              // sends one command
    Wire.endTransmission();    // stop transmitting

  
}
