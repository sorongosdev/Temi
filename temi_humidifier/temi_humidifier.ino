
int humidifier = '0';
int humidified = 5;
int led = 6;
int trig = 12;
int echo = 13;
int time_flag=0;

void setup() {
  pinMode(humidified, OUTPUT);
  pinMode(led, OUTPUT);
  Serial.begin(9600);
}

void loop() {
  if(Serial.available())
    humidifier = Serial.read();
      
  if (humidifier == '1') {
    digitalWrite(humidified, HIGH);
    digitalWrite(led, HIGH);
  }
  else {
    digitalWrite(humidified, LOW);
    digitalWrite(led, LOW);
  }
}
