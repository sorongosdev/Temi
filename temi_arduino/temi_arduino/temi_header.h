#include <SPI.h>
#include <MFRC522.h>
#include <Ethernet2.h>
#include <Servo.h>
#include <DHT.h>
#include <Adafruit_NeoPixel.h>
#include <SoftwareSerial.h>
#include <Wire.h>
#include <pm2008_i2c.h>
 
//RFID
#define RST_PIN         8
#define SS_PIN          9
#define known_keys 6

//rfid
#define UNABLE 0
#define ABLE 1

//dht
#define dht_frequency 100
#define DHT_PIN 2
#define DHTTYPE DHT22


//for DOOR LOCK
#define DOOR_LOCK 0
#define UNLOCK 1
#define LOCK 0

#define DOOR_LOCK_PIN  5
#define DOOR_LOCK_OPEN_START 85   // DoorLock unlock start angle (0~180) (case by user)
#define DOOR_LOCK_OPEN_STOP 20 // DoorLock unlock stop angle (0~180) (case by user)
#define DOOR_LOCK_CLOSE_START  20  // DoorLock lock start angle (0~180) (case by user)
#define DOOR_LOCK_CLOSE_STOP  85  // DoorLock lock stop angle (0~180) (case by user)

//NeoPixel define
#define NEO_PIN 12                                                                                                                                                                
#define N_LED 24    // Num of Pixel 24

void readRFID(byte *buffer, byte bufferSize); //Read to RFID uid
void send_name();

void send_humidity(); // reading DHT
void send_dust(); // dust

void servo_init(int door_sel, int pin);
void locker_control(); // Function for doorlock lock
void control_servo(int door_sel, int dir, int st, int ed);//servo angle

void pixel_led();
