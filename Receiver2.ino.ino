#include <WiFi.h>
#include <WiFiUdp.h>

const char* ssid = "Shikhar's Hotspot";          // WiFi SSID
const char* password = "abcd1234";  // WiFi Password

const int localPort = 12345;                // Port to listen for data
WiFiUDP udp;
char incomingPacket[255];                   // Buffer for incoming packets

void setup() {
  Serial.begin(115200);

  // Connect to WiFi
  Serial.print("Connecting to WiFi...");
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nConnected to WiFi!");

  // Start UDP listener
  udp.begin(localPort);
  Serial.printf("Listening on port %d\n", localPort);
}

void loop() {
  // Check for incoming packets
  int packetSize = udp.parsePacket();
  if (packetSize) {
    int len = udp.read(incomingPacket, 255);
    if (len > 0) {
      incomingPacket[len] = '\0'; // Null-terminate the string
    }
    Serial.printf("Received packet: %s\n", incomingPacket);
  }
 // Parse the received data (comma-separated values)
    String data = String(incomingPacket);
    float temperature = data.substring(0, data.indexOf(',')).toFloat();
    data = data.substring(data.indexOf(',') + 1);
    float humidity = data.substring(0, data.indexOf(',')).toFloat();
    data = data.substring(data.indexOf(',') + 1);
    int flameSensor = data.substring(0, data.indexOf(',')).toInt();
    int motionDetected = data.substring(data.indexOf(',') + 1).toInt();

    // Display separated data
    Serial.printf("Temperature: %.2f\n", temperature);
    Serial.printf("Humidity: %.2f\n", humidity);
    Serial.printf("Flame Sensor: %d\n", flameSensor);
    Serial.printf("Motion Detected: %d\n", motionDetected);
  delay(4900); // Reduce CPU usage
}
