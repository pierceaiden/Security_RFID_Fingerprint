/*
Author : Samdharsi Kumar
Make the following connections on Arduino MEGA

1.RFID EM-18
VCC - 5V
GND - GND
TX - 19(RX)  (Serial1)

2.Fingerprint Sensor Module
VCC (Red) - 5V
GND (Black) - GND
TX (Blue) - 16(RX)  (Serial2) (ignore board TX)
RX (Yellow) - 17(TX)

3.Servo MOTOR
VCC (Red) - 5V
GND (Brown) - GND
DATA (Orange) - 4

MEGA Serial Reference
Serial1 19(RX), 18(TX)
Serial2 16(RX), 17(TX)
Serial3 15(RX), 14(TX)
*/

#include <Servo.h>
//#include<SoftwareSerial.h>
#include <Adafruit_Fingerprint.h>

//SoftwareSerial rfid(19,18);
//SoftwareSerial fingerprint(16, 17);
int read_count=0;
int j=0,k=0,z=0; // Variables to iterate in for loops
char data_temp, RFID_data[12];
Servo motor;
char Saved_Tags[3][12]={
  {'5','A','0','0','1','E','4','2','B','6','B','0'},
  {'1','8','0','0','8','D','C','0','2','E','7','B'},
  {'1','8','0','0','8','F','G','6','7','W','2','A'}};
boolean tag_check,tag_status,entry_control;
int angle = 0,fp_found=0;
Adafruit_Fingerprint finger = Adafruit_Fingerprint(&Serial2);

void setup(){
  Serial.begin(115200);  //To PC SERIAL MONITOR
  Serial1.begin(9600);   //RFID SERIAL
  Serial2.begin(57600);  //FINGERPRINT Serial
  Serial.println("\n\nFP_Sensor Detect Test");
  finger.begin(57600);
  if (finger.verifyPassword()) {
    fp_found = 1;
    Serial.println("Found Fingerprint Sensor!");
    finger.getTemplateCount();
    Serial.print("\n\nDatabase contains ");
    Serial.print(finger.templateCount);
    Serial.println(" finger template(s)");
    Serial.println("Waiting for valid finger...\n\n");
  } 
  else {
    fp_found = 0;
    Serial.println("Did not find Fingerprint Sensor!");
    Serial.println("\n Use RFID!");
  }
}
void loop(){
  if(fp_found == 1){
    getFingerprintIDez();
    delay(50);
  }
  else{
    rfid_check();
    delay(50);
  }
  rfid_check();
}
//=================================================================
void RecieveData(){
  if(Serial1.available()>0)
  {
    data_temp=Serial1.read();
    RFID_data[read_count]=data_temp;
    read_count++;
    j++;
    delay(1);
  }
}
//=================================================================

//=================================================================
void CheckData(){
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
      }
    }
      read_count=0;
  }
}
//=================================================================

//=================================================================
void AccessCheck()
{
  if(entry_control==true)
  {
    if(tag_status==true)
    {
      Serial.println("\n\nRFID Card Accepted");
      Serial.println("Access Granted\n\n");
      //======================= SERVO MOTOR ===========================
      motor.attach(4);
      for(angle = 0; angle < 180; angle += 1)
      {
        motor.write(angle);
        delay(1);
      }
      delay(1000);
      for(angle = 180; angle>=0; angle-=1)
      {
        motor.write(angle);
        delay(5);
      }
      motor.detach();
      //==============================================================
    }
    else
    {
      Serial.println("\n\nRFID Card not Accepted");
      Serial.println("Access Denied\n");
    }
    entry_control=false;
    tag_status=false;
  }}
//=================================================================

//==================================================================
int getFingerprintIDez() {

  uint8_t p = finger.getImage();
  if (p != FINGERPRINT_OK)  return -1;

  p = finger.image2Tz();
  if (p != FINGERPRINT_OK)  return -1;

  p = finger.fingerFastSearch();
  if (p != FINGERPRINT_OK)  return -1;

  // found a match!
  Serial.print("\n\nMatch Found ---> Granting Access\n");
  Serial.print("Found ID #"); Serial.print(finger.fingerID);
  //Serial.print(" with confidence of "); Serial.println(finger.confidence);
  if(finger.confidence >=50){
    //======================= SERVO MOTOR ===========================
    motor.attach(4);
    for(angle = 0; angle < 180; angle += 1)
    {
      motor.write(angle);
      delay(1);
    }
    delay(1000);
    for(angle = 180; angle>=0; angle-=1)
    {
      motor.write(angle);
      delay(5);
    }
    motor.detach();
    //==============================================================
  }
  return finger.fingerID;
}
//==================================================================

void rfid_check(){
  while(z<12){
    RecieveData();
    z++;
  }
  CheckData();
  AccessCheck();
}
