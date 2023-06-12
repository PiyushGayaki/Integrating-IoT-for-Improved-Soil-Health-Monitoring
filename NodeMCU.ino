#include <SoftwareSerial.h>
#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ThingSpeak.h>

SoftwareSerial arduino(5, 4);  // RX, TX pins of NodeMCU
const char* ssid = "ROG";
const char* password = "Piyush@03";
const char* server = "api.thingspeak.com";
const char* apiKey = "YJY1GUS6G2AOONK9";

WiFiClient client;

void setup() {
  Serial.begin(9600);
  arduino.begin(9600);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }
  ThingSpeak.begin(client);
}

void loop() {
  while (arduino.available()) {
    String data = arduino.readStringUntil('\n');
    Serial.println(data);
  
    String delimiter = ","; // delimiter used to split the string
    String values[3]; // array to store the substrings

    int index = 0; // current index in the array
    int start = 0; // starting position of the substring
    int end = data.indexOf(delimiter); // ending position of the substring

    while (end >= 0) {
      // extract the substring and store it in the array
      values[index] = data.substring(start, end);

      // update the start and end positions for the next substring
      start = end + delimiter.length();
      end = data.indexOf(delimiter, start);

      index++; // increment the index in the array
    }

    // extract the final substring (after the last delimiter)
    values[index] = data.substring(start);

    float lm35 = values[0].toFloat();
    float fc28 = values[1].toFloat();
    float ph = values[2].toFloat();

    Serial.print("LM35: ");
    Serial.print(lm35);
    Serial.print(", FC28: ");
    Serial.print(fc28);
    Serial.print(", pH: ");
    Serial.println(ph);

    ThingSpeak.setField(1, lm35);
    ThingSpeak.setField(2, fc28);
    ThingSpeak.setField(3, ph);
    if(lm35>21&&lm35<35&&fc28>=70&&fc28<85&&ph>=5&&ph<7)
    {
      Serial.println("Rice is the Suitable Crop");
    }
    else if(lm35>12&&lm35<18&&fc28>=60&&fc28<80&&ph>=5&&ph<7)
    {
      Serial.println("Potato and Corn are the Suitable Crop");
    }
    else if(lm35>25&&lm35<35&&fc28>=15&&fc28<68&&ph>=5&&ph<7)
    {
      Serial.println("Sugarcane is Suitable Crop");
    }
    else
    {
      Serial.println("No Crop Suitable");
    }
    int status = ThingSpeak.writeFields(1, apiKey);
    if (status == 200) {
      Serial.println("Data sent to ThingSpeak successfully!");
    } else {
      Serial.println("Error sending data to ThingSpeak!");
    }
    delay(20000);
  }
}
