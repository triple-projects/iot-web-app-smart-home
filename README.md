# **Smart Home Monitoring & Control with ESP8266**

This project is a **web-based IoT application** that integrates an **ESP8266** microcontroller to monitor and control various smart home elements. It features real-time monitoring of **flame, rain, gas, and temperature sensors**, along with control mechanisms for **a door, a window (via servo motors), and a gas valve (via a relay switch)**.

## **Features**  
✔️ **Real-time Monitoring**: Displays live sensor data for flame, rain, gas concentration, and temperature.  
✔️ **Remote Control**: Operate the door, window, and gas valve from a web-based interface.  
✔️ **ESP8266 Integration**: Uses WiFi communication for seamless connectivity.  
✔️ **Web-Based Dashboard**: Interactive and user-friendly interface for monitoring and control.  
✔️ **Safety Automation**: Automatically closes the gas valve if a gas leak is detected.  

## **Tech Stack**  
- **ESP8266** (NodeMCU) for IoT connectivity  
- **HTML, CSS, JavaScript** for the web dashboard  
- **AJAX/WebSockets** for real-time data updates  
- **Arduino IDE** for ESP8266 programming  

## **How It Works**  
1. Sensors collect environmental data and send it to the ESP8266.  
2. The web app fetches and displays real-time data from ESP8266.  
3. Users can remotely control the **servo motors** (door & window) and **relay switch** (gas valve).  
4. Safety mechanisms trigger automatic actions in case of emergencies (e.g., gas leakage).  

## **Setup Instructions**  
1. Flash the ESP8266 with the provided Arduino code.  
2. Deploy the web app on a local or cloud server.  
3. Connect the ESP8266 to your WiFi network.  
4. Access the web dashboard and start monitoring & controlling!  

---

**Contributions & Issues**  
Feel free to submit issues, fork the project, and contribute improvements!
