#include <DHT.h>
#include <WiFi.h>
#include <WiFiUdp.h>

#define MINUTE 60000 // for converting milliseconds to a minute
#define SECOND 1000 // for converting milliseconds to a second

const char* ssid = "Shikhar's Hotspot";          // WiFi SSID
const char* password = "abcd1234";  // WiFi Password

const char* receiverIP = "192.168.94.194";   // Receiver's IP address (replace with actual)
const int receiverPort = 12345;            // Port to send data
WiFiUDP udp;


DHT dht1(23, DHT11);  // First DHT11 sensor connected to pin 26
DHT dht2(18, DHT11);  // Second DHT11 sensor connected to pin 27
 
int buzzer = 12;                // pin for the LED
int pirPin = 13;                // signal pin of the PIR sensor

int lastPirVal = LOW;          // the last value of the PIR sensor
int pirVal;                    // the current value of the PIR sensor
int pirval1 = 0;

int shockSensor = 2;

bool shockSensorState = 0;
int shockno = 0;

unsigned long myTime;          // number of milliseconds passed since the Arduino started running the code itself
char printBuffer[128];         
 
void setup() {  
  
  dht1.begin();
  dht2.begin();
  pinMode(buzzer, OUTPUT);    // declare LED as output
  pinMode(shockSensor, INPUT);
  pinMode(pirPin, INPUT);     // declare PIR sensor as input
  shockSensorState = digitalRead(shockSensor);
  Serial.begin(115200);
   // Connect to WiFi
  Serial.print("Connecting to WiFi...");
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nConnected to WiFi!");
  delay(1000);
}
 
void loop(){
  /*buttonState = digitalRead(ssButton);
  if(buttonState == LOW){
    isActive = true;
    Serial.println("PIR Sensor activated.");
    delay(500);
  }else{
    isActive = false;
    digitalWrite(buzzer, LOW);
    Serial.println("PIR Sensor deactivated.");
    delay(500);
  }
  */
  float temp1 = dht1.readTemperature();  // Get temperature in Celsius
  float humidity1 = dht1.readHumidity(); // Get humidity in percentage

  // Read temperature and humidity from the second sensor
  float temp2 = dht2.readTemperature();  // Get temperature in Celsius
  
  // Print data from the first sensor
  Serial.print("Sensor 1 - Temp: ");
  Serial.print(temp1);
  Serial.print(" C ");
  Serial.print("Humidity: ");
  Serial.print(humidity1);
  Serial.println(" % ");
  
  // Print data from the second sensor
  Serial.print("Sensor 2 - Temp: ");
  Serial.print(temp2);
  Serial.print(" C ");

  Serial.println(" % ");
  //if(isActive){
  pirVal = digitalRead(pirPin);  // read current input value
 
  if (pirVal == HIGH) { // movement detected  
    pirval1 = 1;
    for(int i = 1; i < 3; i++){
      digitalWrite(buzzer, HIGH);  // buzzer alert
      delay(250);
      digitalWrite(buzzer, LOW);
      delay(250);
    }
    if (lastPirVal == LOW) { // if there was NO movement before
      myTime = millis();
      sprintf(printBuffer, "%lu min %lu sec: Motion detected!", myTime/MINUTE, myTime%MINUTE/SECOND); 
      Serial.println(printBuffer);
      lastPirVal = HIGH;
    }
  } else { // no movement detected
    digitalWrite(buzzer, LOW); // turn LED off
    pirval1 = 0;
    if (lastPirVal == HIGH){ // if there was a movement before
      myTime = millis();
      sprintf(printBuffer, "%lu min %lu sec: Motion ended!", myTime/MINUTE, myTime%MINUTE/SECOND); 
      Serial.println(printBuffer);
      lastPirVal = LOW;
    }
  }
  if(shockSensorState == 1){
    shockno++;
    for(int i = 0; i < 10; i++){
      digitalWrite(buzzer, HIGH);
      delay(150);
      digitalWrite(buzzer, LOW);
      delay(150);
      if(i == 9){
        shockSensorState = 0;
      }
    }
  }
  else{
    shockSensorState = digitalRead(shockSensor);
  }
  String data = String(temp1) + "," + String(humidity1) + "," +
                String(temp2) + "," + String(pirval1) + "," + String(shockno);

  // Send data via UDP
  udp.beginPacket(receiverIP, receiverPort);
  udp.print(data);
  udp.endPacket();

  Serial.println("Data sent: " + data);

  delay(1000); // Send every second


  //}
}