#include "temi_header.h"

//Object
Servo servo;  // create servo object to control a servo 
MFRC522 mfrc522(SS_PIN, RST_PIN);  // Create MFRC522 instance
DHT dht(DHT_PIN, DHTTYPE);
Adafruit_NeoPixel strip = Adafruit_NeoPixel(N_LED, NEO_PIN, NEO_GRB + NEO_KHZ800);
PM2008_I2C pm2008;

//Const
const String knownKeys[] = {"8B:A8:0C:22","E7:FB:D4:10","C9:98:E3:4F"}; //White, Pink, ID
const String Name[] = {"DSR","YYR","YJY","CYS","LSH","BJC"};

String Name="",strID = "", prev_id = "",id_flag = ""; //RFID
bool tag_state = ABLE;

char Char_h, Char_f; //DHT
double h,f;
bool humidifier = OFF; //humidifier
bool airpurifier = OFF; //airpurifier

int pos;    // Servo
bool motor_lock = LOCK;

int pixel_delay = 100; //pixel
int r,g,b;
String pixel_color = "blue", prev_color;

long unsigned Time,time_flag=0; //Time
