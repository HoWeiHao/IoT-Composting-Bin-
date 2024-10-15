#define green 20
#define red 21
#define sw 7

int val = 0;

void setup() {
  // put your setup code here, to run once:
  pinMode(green, OUTPUT);
  pinMode(red, OUTPUT);
  pinMode(sw, INPUT);

  digitalWrite(green, LOW);
  digitalWrite(red, LOW);
}

void loop() {
  // put your main code here, to run repeatedly:

  if (digitalRead(sw) == 0) {

    digitalWrite(green, HIGH);
    digitalWrite(red, LOW);
  } else {
    digitalWrite(red, HIGH);
    digitalWrite(green, LOW);
  }

}
