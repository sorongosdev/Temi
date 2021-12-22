#include "temi_header.h"
#include "temi_name.h"

//main
void setup() {
  pinMode(SS_PIN, OUTPUT);
  digitalWrite(SS_PIN, HIGH);
     
  Serial.begin(9600);        // Initialize serial communications with the PC
  SPI.begin();
  mfrc522.PCD_Init();        // Init MFRC522
  dht.begin();
  servo_init(DOOR_LOCK_PIN); // attaches the servo on pin to the servo object     

  strip.begin(); //Init Neopixel
  strip.show();
  pm2008.begin(); //Init fine sensor
  pm2008.command();
#ifdef PM2008N
  // wait for PM2008N to be changed to I2C mode
  delay(10000);
#endif
} 
 
//loop
void loop() {
  
  if(Serial.available()){
    pixel_color = Serial.read(); //read led color
    pixel_led();
    
    locker = Serial.read(); //read motor lock
    locker_control();
  }

  //rfid  
  if( mfrc522.PICC_IsNewCardPresent() && mfrc522.PICC_ReadCardSerial()) // 태그가 있으면서, 테미가 태그 가능한 상태일 때만 rfid 함수 동작
    readRFID(mfrc522.uid.uidByte, mfrc522.uid.size);
  else
    rfid_id = id_flag; //untapped  

  //humidity dust
  Time = millis();
  if((Time - time_flag) >= dht_frequency){ // 습도, 미세먼지를 읽어 30초마다 출력
    send_humidity();
    send_dust();
    time_flag = Time;
   }  
   

//  print in main
  Serial.print(rfid_id);
  Serial.print(humidifier);
  Serial.print(airpurifier);
  Serial.println();
}

//Read to RFID uid
void readRFID(byte *buffer, byte bufferSize){
  for (byte i = 0; i < bufferSize; i++) {
    strID += (mfrc522.uid.uidByte[i] < 0x10 ? "0" : "") + String(mfrc522.uid.uidByte[i], HEX) + (i != 3 ? ":" : "");
  }
  strID.toUpperCase(); 
  send_name();
  strID="";
}

void send_name(){ // send name to firebase(it can be either a sender or a receiver)
  mfrc522.PICC_HaltA();       // Halt PICC
  mfrc522.PCD_StopCrypto1();  // Stop encryption on PCD

  for(int i=0; i<known_keys; i++){      
    if(strID == knownKeys[i]){ // tag success
      rfid_id = i;  //Name[i] -> i
      id_flag = i;
      break;        
    }
    else{
      rfid_id = 8; // unregistered
      id_flag = 8;
    }
  }
}

void send_humidity(){ // reading DHT
  h = dht.readHumidity();
  if(h<40)
    humidifier = '1';
  else
    humidifier = '0';
  
//  Serial.print(humidifier);
//  Serial.println(h);
}

void send_dust(){
  uint8_t ret = pm2008.read();
  if (ret == 0){
    if(pm2008.pm10_grimm > 80)
      airpurifier = '0';
    else
      airpurifier = '1';
  }
//  Serial.println(airpurifier);
//    Serial.println(pm2008.pm10_tsi);
}

// Servo initiaize
void servo_init(int pin)
{
  servo.attach(pin);

  locker = '7';  // Set doorlock state UNLOCK
  locker_flag = '6';
  locker_control();

}
 
// Function for doorlock unlock
void locker_control()
{
  if(locker_flag == '6' && locker == '7'){
    control_servo(UNLOCK, DOOR_LOCK_OPEN_START, DOOR_LOCK_OPEN_STOP);
    locker_flag = locker;
  }
  else if(locker_flag == '7' && locker == '6'){
    control_servo(LOCK, DOOR_LOCK_CLOSE_START, DOOR_LOCK_CLOSE_STOP);
    locker_flag = locker;
  }  
}

// Function for control servo angle
void control_servo(int dir, int st, int ed){
  if (dir == 1){
    for (pos = st; pos >= ed; pos--){     // goes from 180 degrees to 0 degrees 
        servo.write(pos);              // tell servo to go to position in variable 'pos'
        delay(20);                       // wait for the servo to reach the position (20ms period)
    }
  }
  else {
    for (pos = st; pos <= ed; pos++){     // goes from 0 degrees to 180 degrees 
       servo.write(pos);              // tell servo to go to position in variable 'pos' 
        delay(20);                       // wait for the servo to reach the position (20ms period)
    }
  }
}

// led color
void pixel_led(){
  if(prev_color != pixel_color) {
    if(pixel_color=='3'){ //red
      r=255,g=0,b=0;
    }
    else if(pixel_color=='4'){ //yellow
      r=255,g=255,b=0;
    }
    else if(pixel_color=='5'){ //green
      r=0,g=255,b=0;
    }
    
    for(int i=0; i<N_LED; i++){
      strip.setPixelColor(i,r,g,b);
    }
    strip.show();
    prev_color = pixel_color;
  }
}
