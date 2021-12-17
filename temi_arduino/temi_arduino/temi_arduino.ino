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
  servo_init(DOOR_LOCK, DOOR_LOCK_PIN); // attaches the servo on pin to the servo object     

  strip.begin(); //Init Neopixel
  strip.show();
  pm2008.begin(); //Init fine sensor
  pm2008.command();
}
 
//loop
void loop() {
  if( mfrc522.PICC_IsNewCardPresent() && mfrc522.PICC_ReadCardSerial() && tag_state == ABLE) // 태그가 있으면서, 테미가 태그 가능한 상태일 때만 rfid 함수 동작
    readRFID(mfrc522.uid.uidByte, mfrc522.uid.size);

  Time = millis();
  if((Time - time_flag) >= dht_frequency){ // read Humidity, every 30s
    readDHT();
    readDust();
    time_flag = Time;
  }
  pixel_led();
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
      Name=Name[i];  
      break;        
    }
    else{
      Name="Unregistered";
      pixel_color="red";
    }
  }
  Serial.println(Name);
  Serial.println();
}

void readDHT(){ // reading DHT
  h = dht.readHumidity();
  if(h<40)
    humidifier = ON;
  else
    humidifier = OFF;
  
  Serial.print("H"+String(humidifier));
}

void readDust(){
  if(pm2008.pm10_grimm > 80)
    airpurifier = ON;
  else
    airpurifier = OFF;
  
  Serial.println("A"+String(airpurifier));
}

// Servo initiaize
void servo_init(int door_sel, int pin)
{
    servo.attach(pin);
 
    if (door_sel == DOOR_LOCK)
    {
        locker_unlock();
        motor_lock = UNLOCK;  // Set doorlock state UNLOCK
    }
}
 
// Function for doorlock unlock
void locker_unlock()
{
  if ((motor_lock == LOCK))
  {
      control_servo(DOOR_LOCK, UNLOCK, DOOR_LOCK_OPEN_START, DOOR_LOCK_OPEN_STOP);
      motor_lock = UNLOCK;
  }
}
 
// Function for doorlock lock
void locker_lock()
{
  if ((motor_lock == UNLOCK))
  {
      control_servo(DOOR_LOCK, LOCK, DOOR_LOCK_CLOSE_START, DOOR_LOCK_CLOSE_STOP);
      motor_lock = LOCK;
  }
}

// Function for control servo angle
void control_servo(int door_sel, int dir, int st, int ed){
  if (dir == UNLOCK){
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
  if(prev_color == pixel_color) return;
  
  if(pixel_color=="blue"){
    r=0,g=0,b=255;
  }
  else if(pixel_color=="green"){
    r=0,g=255,b=0;
  }
  else if(pixel_color=="red"){
    r=255,g=0,b=0;
  }
  else if(pixel_color=="yellow"){
    r=255,g=255,b=0;
  }
  for(int i=0; i<N_LED; i++){
    strip.setPixelColor(i,r,g,b);
  }
  strip.show();
  prev_color = pixel_color;
}
