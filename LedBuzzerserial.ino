
int POTENTIOMETER = A0; // the pontentiometer pin
int BUZZER = 5; // the buzzer pin

void setup()

{

pinMode(2, OUTPUT);
pinMode(BUZZER, OUTPUT);
pinMode(3,OUTPUT);

Serial.begin(9600);

}

void loop() {

if (Serial.available())

{

int state = Serial.read();

if (state == 45)

{
 int val = analogRead(POTENTIOMETER);
 Serial.println(val);
    // conversting the potentiometer read to 0-255
    val = map(val, 0, 1023, 0, 255);
    analogWrite(BUZZER, val);



digitalWrite(2, HIGH);
digitalWrite(3, LOW);

Serial.println("Command completed LED turned ON");

}

else if (state == 50)

{
int val = 0;
 Serial.println(val);
    // conversting the potentiometer read to 0-255
   // val = map(val, 0, 1023, 0, 255);
    analogWrite(BUZZER, 0);
digitalWrite(2, LOW);
digitalWrite(3, HIGH);

Serial.println("Command completed LED turned OFF");

}
}

}
