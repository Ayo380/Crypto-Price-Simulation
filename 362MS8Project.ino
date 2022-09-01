#include <LiquidCrystal.h>
#include <ESP8266WiFi.h> //needed for wifi connection
#include <ArduinoJson.h> //to use json files
// initialize the library by associating any needed LCD interface pin
// with the arduino pin number it is connected to
const int rs = 12, en = 11, d4 = 5, d5 = 4, d6 = 10, d7 = 8;
const int green = 7, red = 9; //green and red leds
const char* ssid = "UIC-WiFi";
const char* password; //user input using serial
#define CMC_API "/v1/ticker/bitcoin/" //API 
#define CMC_URL "api.coinmarketcap.com" //website url
int master = 0;
int POTENTIOMETER = A1; // the pontentiometer pin
//int BUZZER = 6; // the buzzer pin
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);
long int Bitcoin = 40000, Ethereum = 3000,currLargeCoin; //base Bitcoin and Ethereum prices
float Doge = 0.1465253, Duinocoin = 0.0000504, currSmallCoin; //base Dogecoin and Duinocoin prices
String BTC, ETH, DOGE, DUCO,currL,currS; //short hand abbreviations for screen 
float randNum, randPercent, changeLarge,changeSmall; //variables that simulate stock prices. Explained further in updatecoin function
//int buttonState = 0;
int flag_small = 0; //to change small coin
int flag_large = 0; //to change large coin
const char* fingerprint = "EF 9D 44 BA 1A 91 4C 42 06 B1 6A 25 71 26 58 61 BA DA FA B9";
static char respBuffer[4096];
void setup() {
  // set up the LCD's number of columns and rows:
  Serial.begin(115200);
  Serial.setDebugOutput(true);
  Serial.println();
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);
  Serial.print("Please enter password for ", ssid);
  password = Serial.read();
  WiFi.begin(ssid,password);
  while(WiFi.status() != WL_CONNECTED){ //if wifi not connected
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
  pinMode(green,OUTPUT);
  pinMode(red,OUTPUT);
  //flag_small = 0;
  //pinMode(2,INPUT_PULLUP);
  //pinMode(3,INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(2), DOGECOIN, CHANGE);
  attachInterrupt(digitalPinToInterrupt(3), UNOCOIN, CHANGE);
  lcd.begin(16, 2);
  lcd.print("Current Coin: ");
}
void loop() {
  /getData();
  if(flag_small == 0)
  {
    DOGECOIN();
  } 
  else {
     UNOCOIN();
  }
  if(flag_large == 0){
    BITCOIN();
  }
  else{
    ETHEREUM();
  }
   delay(3000);
  
}
String getData(){
  const char request[] =
    "GET " CMC_API "HTTP/1.1\r\n"
    "User-Agent: ESP8266/0.1\r\n"
    "Accept: */*\r\n"
    "Host: " CMC_URL "\r\n"
    "Connection: close\r\n"
    "\r\n";
   Serial.print("Requesting URL: ");
   Serial.println(CMC_URL);

   delay(100);

   if (!client.connect(CMC_URL,443)){ //if connection fails
    Serial.println("Connection failed";
    return;
   }

   if (client.verify(fingerprint,CMC_URL)){
    Serial.println("Certificate matches");
   }
   else{
    Serial.println("Certificate doesn't match!");
   }

   client.print(request);
   client.flush();
   delay(1000);

   uint16_t index = 0;
   char c;
   while(client.connected()){
    if(client.available()){
      c = client.read();
      respBuffer[index++] = c;
      delay(1);
    }
   }
   client.stop();
   char* json = strchr(respBuffer,'{');
   Serial.println(json);
   DynamicJsonBuffer jBuffer;
   JsonOBject& root = jBuffer.parseObject(json);

   Serial.println("JsonObject: ");
   root.prettyPrintTo(Serial);
   Serial.println();

   String price_usd = root["price_usd"];
   return price_usd;
   
}
float updateSmallCoin(float currCoin){
  randNum = random(-5,5);
  randPercent = float(randNum / 100) + 1;
  changeSmall = currCoin * randPercent;
  return changeSmall;
}

void DOGECOIN()
{
    changeSmall = updateSmallCoin(Doge);
    DOGE = String(currSmallCoin,7); //convert Bitcoin to string
    lcd.clear();
    lcd.print("Current Coin: ");
    lcd.setCursor(0,1);
    lcd.print("DOGE: $"+ DOGE);
    //delay(2000);
    currSmallCoin = changeSmall; //change after
    Serial.print("current value: ");
    Serial.println(currSmallCoin);
    if (currSmallCoin > Doge * 1.03 )
    {
    Serial.println("Hig");
    digitalWrite(red,LOW);
    Serial.write(50);
    digitalWrite(green,HIGH);
  }
  else if (currSmallCoin < Doge * 0.97 )
  {
    //int val = analogRead(POTENTIOMETER);
    Serial.println("Low");
    //Serial.println(val);
    // conversting the potentiometer read to 0-255
    //val = map(val, 0, 1023, 0, 255);
    //analogWrite(BUZZER, val);
    digitalWrite(green,LOW);
    Serial.write(45);
    digitalWrite(red,HIGH);
  }
  else{
    Serial.flush();
    digitalWrite(green,LOW);
    digitalWrite(red,LOW);
  }
  flag_small = 0;
  Serial.println("i am in doge");
}

void UNOCOIN()
{
    changeSmall = updateSmallCoin(Duinocoin);
    DUCO = String(currSmallCoin,7);
    lcd.clear();
    lcd.print("Current Coin: ");
    lcd.setCursor(0,1);
    lcd.print("DUCO: $"+DUCO);
    //delay(2000);
    currSmallCoin = changeSmall;
    Serial.print("current value: ");
    Serial.println(currSmallCoin);
    if(currSmallCoin > Duinocoin * 1.03 )
    {
    Serial.println("High");
    digitalWrite(red,LOW);
    digitalWrite(green,HIGH);
    Serial.write(50);
  }
  else if (currSmallCoin < Duinocoin * 0.97 )
  {
    //int val = analogRead(POTENTIOMETER);
    Serial.println("Low");
    //Serial.println(val);
    // conversting the potentiometer read to 0-255
    //val = map(val, 0, 1023, 0, 255);
    //analogWrite(BUZZER, val);
    Serial.write(45);
    digitalWrite(green,LOW);
    digitalWrite(red,HIGH);
  }
  else{
    Serial.flush();
    digitalWrite(green,LOW);
    digitalWrite(red,LOW);
  }
  flag_small = 1;
  Serial.println("i am in DuinoCoin");
}

// update function constantly generate randome numbers between -5 to 5
// and calculate the percentage to simulate the change in crypto currency prices 
// We could not connect the wifi to get the live prices of the crypto prices this our best option
long int updateLargeCoin(long int currCoin){
  randNum = random(-5,5);
  randPercent = float(randNum / 100) + 1;
  changeLarge = currCoin * randPercent;
  return changeLarge;
}

// This function prints the Bitcoin prices on to the LCD and it called when a button is pressed
void BITCOIN()
{
    changeLarge = updateLargeCoin(Bitcoin);
    BTC = String(currLargeCoin); //convert Bitcoin to string
    lcd.clear();
    lcd.print("Current Coin: ");
    lcd.setCursor(0,1);
    lcd.print("BTC: $"+ BTC);
    delay(3000);
    currLargeCoin = changeLarge; //change after
    Serial.print("current value: ");

    // checking if the current prices I greater than $4120 then the green light goes on
    // Indicating the crypto price is rising
    if (currLargeCoin > Bitcoin * 1.03 )
    {
    Serial.println(currLargeCoin);
    Serial.println("High");
//    digitalWrite(red,LOW);
//    digitalWrite(green,HIGH);
    Serial.write(50);
  }
  else if (currLargeCoin < Bitcoin * 0.97 )
  {
    Serial.println(currLargeCoin);
    //int val = analogRead(POTENTIOMETER);
    Serial.println("Low");
   // val = map(val, 0, 1023, 0, 255);
    //analogWrite(BUZZER, val);
    //digitalWrite(green,LOW);
    //digitalWrite(red,HIGH);
    Serial.write(45);
   
  }
  else{
     Serial.flush();
//    digitalWrite(green,LOW);
//    digitalWrite(red,LOW);
    Serial.println(currLargeCoin);
  }
  flag_large = 0;
  Serial.println("i am in bitcoin");
}


// This code is for ethereum to update the prices for ethereum and print in the LCD
void ETHERIUM()
{
  // gets the update price from Update function
    changeLarge = updateLargeCoin(Ethereum);
    ETH = String(currLargeCoin);
    lcd.clear();
    lcd.print("Current Coin: ");
     lcd.setCursor(0,1);
    lcd.print("ETH: $"+ETH);
    delay(3000);
    currLargeCoin = changeLarge;
    Serial.print("current value: ");
    // compares the current ethereum price to the base prices * 1.03.
    // and sends signal to the second ardino to let it know when to turns the green light.
    if(currLargeCoin > Ethereum * 1.03 )
    {
    Serial.println(currLargeCoin);
    Serial.println("High");
//    digitalWrite(red,LOW);
//    digitalWrite(green,HIGH);
    Serial.write(50);
  }
  // turns red light when it is less
  // and activates the buzzer and also send signal through serial write to the second ardino so it know when to turn on the light
  else if (currLargeCoin < Ethereum * 0.97 )
  {
    Serial.println(currLargeCoin);
    //int val = analogRead(POTENTIOMETER);
    Serial.println("Low");
    Serial.println(val);
//    val = map(val, 0, 1023, 0, 255);
//    analogWrite(BUZZER, val);
      Serial.write(45);
//    digitalWrite(green,LOW);
//    digitalWrite(red,HIGH);
  }
  else{
    Serial.println(currLargeCoin);
    Serial.flush();
    digitalWrite(green,LOW);
    digitalWrite(red,LOW);
  }
  
  flag_large =1;
}
