#include <WiFi.h>
#include <HTTPClient.h>
#include <DHT.h>

// WiFi credentials
const char* ssid = "Nimbus1999";  // Replace with your mobile hotspot SSID
const char* password = "19991212";  // Replace with your hotspot password

// ThingSpeak details
const char* thingspeakServer = "http://api.thingspeak.com/update";
const char* apiKey = "XC8TBSVZ3JZPCB1O";  // Replace with your API Key

// Define DHT sensor pin and type
#define DHTPIN 4     // Digital pin G4 connected to the DHT sensor
#define DHTTYPE DHT11   // DHT 11
DHT dht(DHTPIN, DHTTYPE);

void setup() {
    Serial.begin(115200);
    WiFi.begin(ssid, password);

    Serial.print("Connecting to WiFi");
    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }
    Serial.println("\nConnected to WiFi!");
    Serial.print("IP Address: ");
    Serial.println(WiFi.localIP());

    dht.begin();
    Serial.println("DHT11 initialized successfully!");
}

void loop() {
    if (WiFi.status() == WL_CONNECTED) {
        HTTPClient http;
        
        // Read humidity and temperature
        float humidity = dht.readHumidity();
        float temperature = dht.readTemperature();
        
        // Check if reading failed
        if (isnan(humidity) || isnan(temperature)) {
            Serial.println("Failed to read from DHT sensor!");
            return;
        }
        
        int airPressure = (int)temperature + 850;  // Simulated air pressure
        
        Serial.print("Temperature: "); Serial.println(temperature);
        Serial.print("Humidity: "); Serial.println(humidity);
        Serial.print("Air Pressure: "); Serial.println(airPressure);

        // Construct ThingSpeak API URL
        String url = String(thingspeakServer) + "?api_key=" + apiKey +
                     "&field1=" + String(temperature) +
                     "&field2=" + String(humidity) +
                     "&field3=" + String(airPressure);
        
        http.begin(url);
        int httpResponseCode = http.GET();
        
        if (httpResponseCode > 0) {
            Serial.print("Data sent successfully. HTTP Response code: ");
            Serial.println(httpResponseCode);
        } else {
            Serial.print("Error sending data. HTTP Response code: ");
            Serial.println(httpResponseCode);
        }
        
        http.end();
    } else {
        Serial.println("WiFi not connected. Retrying...");
        WiFi.reconnect();
    }
    
    delay(5000);  // Wait 5 seconds before sending the next set of values
}
