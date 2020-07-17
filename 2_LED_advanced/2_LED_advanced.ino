void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  pinMode(13,OUTPUT);
}

int delaytime[3]={1000,2000,3000};
int i=0;
void loop() {
  // put your main code here, to run repeatedly:
  digitalWrite(13,HIGH);
  delay(1000);
  digitalWrite(13,LOW);
  delay(1000);
}
