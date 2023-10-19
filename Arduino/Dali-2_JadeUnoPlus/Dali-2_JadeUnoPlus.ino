const int DALI_TX_PIN = 2;
const int DALI_RX_PIN = 3;
const int DALI_BIT_DELAY = 416;
long daliTimeout = 20000; //us, DALI response timeout

#define BROADCAST_C 0b11111111
#define ON_C 0b00000101
#define OFF_C 0b00000000

void setup() {
  Serial.begin(115200);
  help(); //Show help
  setupDali();
}

void help() {
  Serial.println("Enter one of the following commands in serial monitor:");
  Serial.println("help -  command list");
  Serial.println("on - broadcast on 100%");
  Serial.println("off -  broadcast off 0%");
  Serial.println();
}

////////////DALI

void setupDali(){
  pinMode(DALI_TX_PIN, OUTPUT);
  digitalWrite(DALI_TX_PIN, LOW); 
}

void startDali(){
    digitalWrite(DALI_TX_PIN, LOW);
}

void startBit(){
  sendBit(1);
}

void stopBits() {
  digitalWrite(DALI_TX_PIN, LOW);
  delayMicroseconds(4 * DALI_BIT_DELAY);
}

void sendBit(int pBit) {
  if(pBit) //send 1
  {
    digitalWrite(DALI_TX_PIN, HIGH);
    delayMicroseconds(DALI_BIT_DELAY);
    digitalWrite(DALI_TX_PIN, LOW);
    delayMicroseconds(DALI_BIT_DELAY);
    }
  else{
    digitalWrite(DALI_TX_PIN, LOW);
    delayMicroseconds(DALI_BIT_DELAY);
    digitalWrite(DALI_TX_PIN, HIGH);
    delayMicroseconds(DALI_BIT_DELAY);
  }
}

void sendByte(uint8_t b) {
  for (int i = 7; i >= 0; i--) 
  {
    sendBit((b >> i) & 1);
  }
}

void transmit(uint8_t cmd1, uint8_t cmd2) { // transmit commands to DALI bus (address byte, command byte)
  startBit();
  sendByte(cmd1);
  sendByte(cmd2);
  stopBits();
}

void loop() {
  int i;
  int cmd1;
  int cmd2;
  String comMsg;

  // Read command from port
  delay(1000);

  while (Serial.available()) {
    comMsg = comMsg + (char)(Serial.read());
  }; // read data from serial

  if (comMsg == "on") {
   transmit(BROADCAST_C, ON_C);
  }; // broadcast, 100%

  if (comMsg == "off") {
    transmit(BROADCAST_C, OFF_C);
  }; // broadcast, 0%

  if (comMsg == "help") {
    help();
  }; //help
  
}
