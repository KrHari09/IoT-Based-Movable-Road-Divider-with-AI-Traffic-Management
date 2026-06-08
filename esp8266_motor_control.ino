/**
 * IoT-Based Movable Road Divider — ESP8266 Motor Control
 *
 * Author  : Hari Kumar (2022UGEC018)
 * Project : Implementation of Movable Road Divider Using IoT
 * College : National Institute of Technology, Jamshedpur
 *
 * Description:
 *   The ESP8266 hosts a lightweight HTTP web server over a Wi-Fi Access Point.
 *   A mobile app or browser sends HTTP GET requests with a `State` parameter
 *   (e.g., ?State=F for Forward) to control the L298N motor driver,
 *   which physically moves the road divider.
 *
 * Pin Mapping (ESP8266 GPIO → L298N):
 *   GPIO14 (D5) → ENA   (Right motor PWM speed)
 *   GPIO12 (D6) → ENB   (Left motor PWM speed)
 *   GPIO15 (D8) → IN1   (Right motor direction)
 *   GPIO13 (D7) → IN2   (Right motor direction)
 *   GPIO2  (D4) → IN3   (Left motor direction)
 *   GPIO0  (D3) → IN4   (Left motor direction)
 */

#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>

// ─── Pin Definitions ────────────────────────────────────────────────────────
#define ENA  14   // Right motor enable (PWM)  — GPIO14 (D5)
#define ENB  12   // Left motor enable (PWM)   — GPIO12 (D6)
#define IN_1 15   // Right motor direction 1   — GPIO15 (D8)
#define IN_2 13   // Right motor direction 2   — GPIO13 (D7)
#define IN_3  2   // Left motor direction 1    — GPIO2  (D4)
#define IN_4  0   // Left motor direction 2    — GPIO0  (D3)

// ─── Wi-Fi Configuration ─────────────────────────────────────────────────────
const char* ssid = "Make DIY";   // Access Point SSID (no password)

// ─── Global State ────────────────────────────────────────────────────────────
String command;           // Stores the latest command from the web server
int speedCar   = 800;     // Default PWM speed (range: 400–1023)
int speed_Coeff = 3;      // Divisor for reduced-speed side during diagonal moves

ESP8266WebServer server(80);

// ─── Setup ───────────────────────────────────────────────────────────────────
void setup() {
  // Configure motor driver pins as outputs
  pinMode(ENA, OUTPUT);
  pinMode(ENB, OUTPUT);
  pinMode(IN_1, OUTPUT);
  pinMode(IN_2, OUTPUT);
  pinMode(IN_3, OUTPUT);
  pinMode(IN_4, OUTPUT);

  Serial.begin(115200);
  Serial.println("\n[INFO] ESP8266 Motor Controller Booting...");

  // Start Wi-Fi Access Point
  WiFi.mode(WIFI_AP);
  WiFi.softAP(ssid);

  IPAddress myIP = WiFi.softAPIP();
  Serial.print("[INFO] AP IP address: ");
  Serial.println(myIP);

  // Register HTTP routes
  server.on("/", HTTP_handleRoot);
  server.onNotFound(HTTP_handleRoot);
  server.begin();

  Serial.println("[INFO] HTTP server started. Waiting for commands...");
}

// ─── Movement Functions ───────────────────────────────────────────────────────

/** Move both motors forward */
void goAhead() {
  digitalWrite(IN_1, LOW);  digitalWrite(IN_2, HIGH); analogWrite(ENA, speedCar);
  digitalWrite(IN_3, LOW);  digitalWrite(IN_4, HIGH); analogWrite(ENB, speedCar);
}

/** Move both motors backward */
void goBack() {
  digitalWrite(IN_1, HIGH); digitalWrite(IN_2, LOW);  analogWrite(ENA, speedCar);
  digitalWrite(IN_3, HIGH); digitalWrite(IN_4, LOW);  analogWrite(ENB, speedCar);
}

/** Pivot right: right motor backward, left motor forward */
void goRight() {
  digitalWrite(IN_1, HIGH); digitalWrite(IN_2, LOW);  analogWrite(ENA, speedCar);
  digitalWrite(IN_3, LOW);  digitalWrite(IN_4, HIGH); analogWrite(ENB, speedCar);
}

/** Pivot left: right motor forward, left motor backward */
void goLeft() {
  digitalWrite(IN_1, LOW);  digitalWrite(IN_2, HIGH); analogWrite(ENA, speedCar);
  digitalWrite(IN_3, HIGH); digitalWrite(IN_4, LOW);  analogWrite(ENB, speedCar);
}

/** Curve forward-right: slow down right motor */
void goAheadRight() {
  digitalWrite(IN_1, LOW);  digitalWrite(IN_2, HIGH); analogWrite(ENA, speedCar / speed_Coeff);
  digitalWrite(IN_3, LOW);  digitalWrite(IN_4, HIGH); analogWrite(ENB, speedCar);
}

/** Curve forward-left: slow down left motor */
void goAheadLeft() {
  digitalWrite(IN_1, LOW);  digitalWrite(IN_2, HIGH); analogWrite(ENA, speedCar);
  digitalWrite(IN_3, LOW);  digitalWrite(IN_4, HIGH); analogWrite(ENB, speedCar / speed_Coeff);
}

/** Curve backward-right: slow down right motor */
void goBackRight() {
  digitalWrite(IN_1, HIGH); digitalWrite(IN_2, LOW);  analogWrite(ENA, speedCar / speed_Coeff);
  digitalWrite(IN_3, HIGH); digitalWrite(IN_4, LOW);  analogWrite(ENB, speedCar);
}

/** Curve backward-left: slow down left motor */
void goBackLeft() {
  digitalWrite(IN_1, HIGH); digitalWrite(IN_2, LOW);  analogWrite(ENA, speedCar);
  digitalWrite(IN_3, HIGH); digitalWrite(IN_4, LOW);  analogWrite(ENB, speedCar / speed_Coeff);
}

/** Stop all motors */
void stopRobot() {
  digitalWrite(IN_1, LOW); digitalWrite(IN_2, LOW); analogWrite(ENA, speedCar);
  digitalWrite(IN_3, LOW); digitalWrite(IN_4, LOW); analogWrite(ENB, speedCar);
}

// ─── Main Loop ────────────────────────────────────────────────────────────────
void loop() {
  server.handleClient();

  command = server.arg("State");

  // Direction commands
  if      (command == "F") goAhead();
  else if (command == "B") goBack();
  else if (command == "L") goLeft();
  else if (command == "R") goRight();
  else if (command == "I") goAheadRight();
  else if (command == "G") goAheadLeft();
  else if (command == "J") goBackRight();
  else if (command == "H") goBackLeft();
  else if (command == "S") stopRobot();

  // Speed commands (0 = slowest, 9 = fastest)
  else if (command == "0") speedCar = 400;
  else if (command == "1") speedCar = 470;
  else if (command == "2") speedCar = 540;
  else if (command == "3") speedCar = 610;
  else if (command == "4") speedCar = 680;
  else if (command == "5") speedCar = 750;
  else if (command == "6") speedCar = 820;
  else if (command == "7") speedCar = 890;
  else if (command == "8") speedCar = 960;
  else if (command == "9") speedCar = 1023;
}

// ─── HTTP Handler ─────────────────────────────────────────────────────────────
void HTTP_handleRoot(void) {
  if (server.hasArg("State")) {
    Serial.print("[CMD] Received: ");
    Serial.println(server.arg("State"));
  }
  server.send(200, "text/html", "");
  delay(1);
}
