int trig1 = 2;
int echo1 = 3;
int trig2 = 4;
int echo2 = 5;
long duration1;
long distance1;
long duration2;
long distance2;
void setup() {
  // put your setup code here, to run once:
pinMode(trig1, OUTPUT);
pinMode(echo1, INPUT);
pinMode(trig2, OUTPUT);
pinMode(echo2, INPUT);
Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
digitalWrite(trig1, LOW);
delayMicroseconds(2);
digitalWrite(trig1, HIGH);
delayMicroseconds(10);
digitalWrite(trig1, LOW);
duration1 = pulseIn(echo1, HIGH);
distance1 = duration1 * 0.034 / 2;
Serial.print("Distance 1 :");
Serial.println(distance1);
}
