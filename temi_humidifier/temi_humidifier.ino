#define ON 1
#define OFF 0

bool humidifier = OFF;
int humidified = 5;
int led = 6;
int trig = 12;
int echo = 13;

void setup() {
  pinMode(humidified, OUTPUT);
  pinMode(led, OUTPUT);
}

void loop() {
  if (humidity < 40 || humidifier == ON) {
    digitalWrite(humidified, HIGH);
    digitalWrite(led, HIGH);
  } else {
    digitalWrite(humidified, LOW);
    digitalWrite(led, LOW);
  }
}
