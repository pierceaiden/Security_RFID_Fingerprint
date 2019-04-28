#include <Servo.h>
#include<SoftwareSerial.h> 
SoftwareSerial mySerial(10,11);
int read_count=0;
int j=0,k=0; // Variables to iterate in for loops
char data_temp, RFID_data[12];
Servo motor;
char Saved_Tags[3][12]={
                       {'5','A','0','0','1','E','4','2','B','6','B','0'},
                       {'1','8','0','0','8','D','C','0','2','E','7','B'},
                       {'1','8','0','0','8','F','G','6','7','W','2','A'}};
boolean tag_check,tag_status,entry_control;
int angle = 0;
void setup()
{
mySerial.begin(9600);
Serial.begin(9600);

}

void loop()
{
RecieveData();
CheckData();
AccessCheck();
}

void RecieveData()
{
if(mySerial.available()>0)
{
data_temp=mySerial.read();
RFID_data[read_count]=data_temp;
read_count++;
}}

void CheckData()
{
if(read_count==12)
{
entry_control=true;  
for(k=0;k<3;k++)
{
for(j=0;j<12;j++)
{
  if(Saved_Tags[k][j]==RFID_data[j])
 {
  tag_check=true;
  }
  else
  {
  tag_check=false;
  break;
  }
}
if(tag_check==true)
{
tag_status=true;
}}
read_count=0;
}}

void AccessCheck()
{
if(entry_control==true)
{
if(tag_status==true)
{
Serial.println("Access Granted");
motor.attach(4); 
//==============================================================
for(angle = 0; angle < 180; angle += 1)    // command to move from 0 degrees to 180 degrees 
{                                  
  motor.write(angle);                 //command to rotate the servo to the specified angle
  delay(1);                       
} 
delay(1000);

for(angle = 180; angle>=0; angle-=1)     // command to move from 180 degrees to 0 degrees 
{                                
  motor.write(angle);              //command to rotate the servo to the specified angle
  delay(5);                       
}    
//==============================================================
motor.detach();
}
else
{
Serial.println("Access Denied");
}
entry_control=false;
tag_status=false;
}}

