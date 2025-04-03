#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <Servo.h>
#include <OneWire.h>
#include <DallasTemperature.h>

// **WiFi Credentials**
const char* ssid = "Khaled";
const char* password = "20112002";

// **Pin Definitions**
#define FLAME_SENSOR 5  // D1 (GPIO5)
#define RAIN_SENSOR  4  // D2 (GPIO4)
#define GAS_SENSOR   12 // D6 (GPIO12)
#define TEMP_SENSOR  13 // D7 (GPIO13)
#define SERVO1_PIN   0  // D3 (GPIO0)
#define SERVO2_PIN   2  // D4 (GPIO2)
#define RELAY_PIN    14 // D5 (GPIO14)

// **Servo Motors**
Servo servo1, servo2;

// **Temperature Sensor Setup**
OneWire oneWire(TEMP_SENSOR);
DallasTemperature sensors(&oneWire);

// **Control Variables**
int servo1_angle = 0;
int servo2_angle = 0;
bool relay_state = false;
int flame_value = 0;
int rain_value = 0;
int gas_value = 0;

// **Web Server Setup**
ESP8266WebServer server(80);

// **Handle CORS (Preflight Requests)**
void handleCORS() {
    server.sendHeader("Access-Control-Allow-Origin", "*");
    server.sendHeader("Access-Control-Allow-Methods", "POST, GET, OPTIONS");
    server.sendHeader("Access-Control-Allow-Headers", "Content-Type");
    server.send(204);  // No Content response
}

// **Handle API Request to Update Servo & Relay**
void handleUpdate() {
    if (server.hasArg("servo1")) {
        int angle1 = server.arg("servo1").toInt();
        servo1_angle = angle1;  // Update servo1 angle
        servo1.write(servo1_angle);
        Serial.println("Servo1 Updated: " + String(servo1_angle));
    }

    if (server.hasArg("servo2")) {
        int angle2 = server.arg("servo2").toInt();
        servo2_angle = angle2;  // Update servo2 angle
        servo2.write(servo2_angle);
        Serial.println("Servo2 Updated: " + String(servo2_angle));
    }

    if (server.hasArg("state")) {  // Handling relay toggle
        relay_state = (server.arg("state").toInt() == 1);
        digitalWrite(RELAY_PIN, relay_state ? HIGH : LOW);
        Serial.println("Relay State: " + String(relay_state ? "ON" : "OFF"));
    }

    // **Serial Debugging**
    Serial.println("\n📤 Update Request Received:");
    Serial.println("Servo1: " + String(servo1_angle));
    Serial.println("Servo2: " + String(servo2_angle));
    Serial.println("Relay: " + String(relay_state ? "ON" : "OFF"));
    
    server.sendHeader("Access-Control-Allow-Origin", "*"); // Add CORS headers for response
    server.send(200, "text/plain", "Updated");
}

// **Handle API Request to Get Sensor Data**
void handleData() {
    // **Prepare JSON Response**
    String jsonData = "{";
    jsonData += "\"flame\":" + String(flame_value) + ",";
    jsonData += "\"rain\":" + String(rain_value) + ",";
    jsonData += "\"gas\":" + String(gas_value) + ",";
    
    // **Read Temperature**
    sensors.requestTemperatures();
    float temperature = sensors.getTempCByIndex(0);
    jsonData += "\"temperature\":" + String(temperature) + ",";
    jsonData += "\"servo1\":" + String(servo1_angle) + ",";
    jsonData += "\"servo2\":" + String(servo2_angle) + ",";
    jsonData += "\"relay\":" + String(relay_state ? "1" : "0") + "}" ;

    // **Serial Debugging**
    Serial.println("\n📨 Data Request Received:");
    Serial.println(jsonData);
    
    server.sendHeader("Access-Control-Allow-Origin", "*"); // Add CORS headers for response
    server.send(200, "application/json", jsonData);
}

void setup() {
    Serial.begin(115200);

    // **WiFi Connection**
    WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print("...");
    }
    Serial.println("\n✅ Connected to WiFi!");
    Serial.println("IP Address: " + WiFi.localIP().toString());

    // **Initialize Sensors & Actuators**
    pinMode(FLAME_SENSOR, INPUT);
    pinMode(RAIN_SENSOR, INPUT);
    pinMode(GAS_SENSOR, INPUT);
    pinMode(RELAY_PIN, OUTPUT);
    digitalWrite(RELAY_PIN, LOW);
    
    servo1.attach(SERVO1_PIN);
    servo2.attach(SERVO2_PIN);
    servo1.write(servo1_angle);
    servo2.write(servo2_angle);

    sensors.begin();

    // **CORS Preflight Routes**
    server.on("/update", HTTP_OPTIONS, handleCORS);
    server.on("/data", HTTP_OPTIONS, handleCORS);

    // **Main API Routes**
    server.on("/update", HTTP_POST, handleUpdate);
    server.on("/data", HTTP_GET, handleData);

    server.begin();
}

void loop() {
    // **Check if new data is entered via Serial**
    if (Serial.available() > 0) {
        String input = Serial.readStringUntil('\n');
        input.trim(); // Remove spaces and newlines

        // Parse new values from Serial input
        int new_flame, new_rain, new_gas;
        
        int valuesRead = sscanf(input.c_str(), "%d %d %d", 
                                &new_flame, &new_rain, &new_gas);

        if (valuesRead == 3) {
            // Update sensor values
            flame_value = new_flame;
            rain_value = new_rain;
            gas_value = new_gas;

            Serial.println("✅ Updated Sensor Data:");
            Serial.printf("Flame: %d, Rain: %d, Gas: %d\n", flame_value, rain_value, gas_value);
        } else {
            Serial.println("❌ Invalid input! Format: flame rain gas");
        }
    }

    server.handleClient();  // Handle incoming HTTP requests
}
