byte yourUID[4] = {0x0A,0xFA,0xFA,0x1A};
byte verifyUID = yourUID[0]^yourUID[1]^yourUID[2]^yourUID[3];
byte blockData[16] = {yourUID[0],yourUID[1],yourUID[2],yourUID[3],verifyUID, 0x8, 0x4, 0x0,0x62, 0x63, 0x64, 0x65,0x66, 0x67, 0x68, 0x69};
void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  Serial.print("Your verificationcode is ");
  Serial.print(blockData[4],HEX);
}

void loop() {
  // put your main code here, to run repeatedly:

}
