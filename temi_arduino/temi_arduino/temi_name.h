#include "temi_header.h"

//Object
Servo servo;  // create servo object to control a servo 
MFRC522 mfrc522(SS_PIN, RST_PIN);  // Create MFRC522 instance
DHT dht(DHT_PIN, DHTTYPE);
Adafruit_NeoPixel strip = Adafruit_NeoPixel(N_LED, NEO_PIN, NEO_GRB + NEO_KHZ800);
PM2008_I2C pm2008;

//Const
const String knownKeys[] = {"C9:98:E3:4F","79:3E:AB:E1","21:F4:22:50","AF:1C:96:60","59:54:23:D2","CF:9F:95:60"}; //White, Pink, ID

String strID = ""; //RFID
int rfid_id = 9, id_flag=9;

double h,f;
char humidifier = '0'; //humidifier
char airpurifier = '0'; //airpurifier

int pos;    // Servo
int locker,locker_flag;

int pixel_delay = 100; //pixel
int r,g,b;
int pixel_color, prev_color='g';

long unsigned Time,time_flag=0; //Time
