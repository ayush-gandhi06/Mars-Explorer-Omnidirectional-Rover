#include <Arduino.h>
#include <WiFi.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <Arduino_JSON.h>

// --- Motor Pins ---
#define FR_IN1 13
#define FR_IN2 12
#define FR_EN 16
#define FL_IN3 4
#define FL_IN4 2
#define FL_EN 17

#define RL_IN1 26
#define RL_IN2 25
#define RL_EN 18
#define RR_IN3 33
#define RR_IN4 32
#define RR_EN 19

// --- LED Pins ---
#define LED1_PIN 21
#define LED2_PIN 22

int speed = 50;

// --- LED Timing Variables (Non-blocking) ---
unsigned long lastLedTime = 0;
bool ledState = false; // false = OFF, true = ON

// direction for the wheel will be determined by binary, 1 is for cw and 0 is ccw
void wheelDirection(int direction, int Motor_IN1, int Motor_IN2)
{
    if (direction == 1)
    {
        digitalWrite(Motor_IN1, HIGH);
        digitalWrite(Motor_IN2, LOW);
    }
    else if (direction == 0)
    {
        digitalWrite(Motor_IN1, LOW);
        digitalWrite(Motor_IN2, LOW);
    }
    else
    {
        digitalWrite(Motor_IN1, LOW);
        digitalWrite(Motor_IN2, HIGH);
    }
}

// --- Movement Functions ---
void moveForward() {
    wheelDirection(1, FR_IN1, FR_IN2);
    wheelDirection(1, FL_IN3, FL_IN4);
    wheelDirection(-1, RL_IN1, RL_IN2);
    wheelDirection(1, RR_IN3, RR_IN4);
}
void moveBackward() {
    wheelDirection(-1, FR_IN1, FR_IN2);
    wheelDirection(-1, FL_IN3, FL_IN4);
    wheelDirection(1, RL_IN1, RL_IN2);
    wheelDirection(-1, RR_IN3, RR_IN4);
}
void moveRight() {
    wheelDirection(1, FR_IN1, FR_IN2);
    wheelDirection(-1, FL_IN3, FL_IN4);
    wheelDirection(-1, RL_IN1, RL_IN2);
    wheelDirection(-1, RR_IN3, RR_IN4);
}
void moveLeft() {
    wheelDirection(-1, FR_IN1, FR_IN2);
    wheelDirection(1, FL_IN3, FL_IN4);
    wheelDirection(1, RL_IN1, RL_IN2);
    wheelDirection(1, RR_IN3, RR_IN4);
}
void moveFR() {
    wheelDirection(1, FR_IN1, FR_IN2);
    wheelDirection(0, FL_IN3, FL_IN4);
    wheelDirection(-1, RL_IN1, RL_IN2);
    wheelDirection(0, RR_IN3, RR_IN4);
}
void moveFL() {
    wheelDirection(0, FR_IN1, FR_IN2);
    wheelDirection(1, FL_IN3, FL_IN4);
    wheelDirection(0, RL_IN1, RL_IN2);
    wheelDirection(1, RR_IN3, RR_IN4);
}
void moveRR() {
    wheelDirection(-1, FR_IN1, FR_IN2);
    wheelDirection(0, FL_IN3, FL_IN4);
    wheelDirection(1, RL_IN1, RL_IN2);
    wheelDirection(0, RR_IN3, RR_IN4);
}
void moveRL() {
    wheelDirection(0, FR_IN1, FR_IN2);
    wheelDirection(-1, FL_IN3, FL_IN4);
    wheelDirection(0, RL_IN1, RL_IN2);
    wheelDirection(-1, RR_IN3, RR_IN4);
}
void moveCW() {
    wheelDirection(1, FR_IN1, FR_IN2);
    wheelDirection(-1, FL_IN3, FL_IN4);
    wheelDirection(-1, RL_IN1, RL_IN2);
    wheelDirection(-1, RR_IN3, RR_IN4);
}
void moveCCW() {
    wheelDirection(-1, FR_IN1, FR_IN2);
    wheelDirection(1, FL_IN3, FL_IN4);
    wheelDirection(1, RL_IN1, RL_IN2);
    wheelDirection(1, RR_IN3, RR_IN4);
}
void Stop() {
    wheelDirection(0, FR_IN1, FR_IN2);
    wheelDirection(0, FL_IN3, FL_IN4);
    wheelDirection(0, RL_IN1, RL_IN2);
    wheelDirection(0, RR_IN3, RR_IN4);
}

// --- WiFi credentials ---
const char *ssid = "Shran";
const char *password = "2259818803";

// --- Server & WebSocket ---
AsyncWebServer server(80);
AsyncWebSocket ws("/ws");

unsigned long lastTime = 0;
const unsigned long INTERVAL = 2000;

// Payload updated to exclude distance and servo angle
String buildPayload()
{
    JSONVar obj;
    obj["message"] = "hello from ESP32";
    obj["uptime"] = millis() / 1000;
    return JSON.stringify(obj);
}

void notifyClients(const String &payload)
{
    ws.textAll(payload);
}

void handleWebSocketMessage(void *arg, uint8_t *data, size_t len)
{
    AwsFrameInfo *info = (AwsFrameInfo *)arg;
    if (info->final && info->index == 0 && info->len == len && info->opcode == WS_TEXT)
    {
        String msg = String((char *)data).substring(0, len);
        Serial.printf("[WS] Received: %s\n", msg.c_str());
        
        if (msg == "ping") {
            notifyClients("{\"pong\":true}");
        }
        else if (msg == "moveForward")  { moveForward(); }
        else if (msg == "moveBackward") { moveBackward(); }
        else if (msg == "moveRight")    { moveRight(); }
        else if (msg == "moveLeft")     { moveLeft(); }
        else if (msg == "moveFR")       { moveFR(); }
        else if (msg == "moveFL")       { moveFL(); }
        else if (msg == "moveRR")       { moveRR(); }
        else if (msg == "moveRL")       { moveRL(); }
        else if (msg == "moveCW")       { moveCW(); }
        else if (msg == "moveCCW")      { moveCCW(); }
        else if (msg == "Stop")         { Stop(); }
        else {
            notifyClients(buildPayload());
        }
    }
}

void onWsEvent(AsyncWebSocket *server, AsyncWebSocketClient *client,
               AwsEventType type, void *arg, uint8_t *data, size_t len)
{
    switch (type)
    {
    case WS_EVT_CONNECT:
        Serial.printf("[WS] Client #%u connected from %s\n",
                      client->id(), client->remoteIP().toString().c_str());
        client->text(buildPayload());
        break;
    case WS_EVT_DISCONNECT:
        Serial.printf("[WS] Client #%u disconnected\n", client->id());
        break;
    case WS_EVT_DATA:
        handleWebSocketMessage(arg, data, len);
        break;
    case WS_EVT_PONG:
    case WS_EVT_ERROR:
        break;
    }
}

void initWiFi()
{
    WiFi.mode(WIFI_STA);
    WiFi.begin(ssid, password);
    Serial.print("Connecting to WiFi");
    while (WiFi.status() != WL_CONNECTED)
    {
        Serial.print('.');
        delay(1000);
    }
    Serial.printf("\nConnected! IP: %s\n", WiFi.localIP().toString().c_str());
}

void initWebSocket()
{
    ws.onEvent(onWsEvent);
    server.addHandler(&ws);
}

void setup()
{
    Serial.begin(460800);
    
    initWiFi();
    initWebSocket();
    
    server.on("/", HTTP_GET, [](AsyncWebServerRequest *request)
              { request->send(200, "text/plain", "ESP32 WebSocket server running"); });
    server.begin();
    Serial.println("HTTP + WebSocket server started");

    // --- Robot Motor Pin Setup ---
    pinMode(FR_IN1, OUTPUT);
    pinMode(FR_IN2, OUTPUT);
    pinMode(FR_EN, OUTPUT);

    pinMode(FL_IN3, OUTPUT);
    pinMode(FL_IN4, OUTPUT);
    pinMode(FL_EN, OUTPUT);

    pinMode(RL_IN1, OUTPUT);
    pinMode(RL_IN2, OUTPUT);
    pinMode(RL_EN, OUTPUT);

    pinMode(RR_IN3, OUTPUT);
    pinMode(RR_IN4, OUTPUT);
    pinMode(RR_EN, OUTPUT);

    // --- LED Pin Setup ---
    pinMode(LED1_PIN, OUTPUT);
    pinMode(LED2_PIN, OUTPUT);
    digitalWrite(LED1_PIN, LOW);
    digitalWrite(LED2_PIN, LOW);
}

void loop()
{
    unsigned long currentMillis = millis();

    // --- Non-blocking LED Blinking State Machine ---
    if (ledState) 
    {
        // LEDs are ON. Check if 1 second (1000ms) has passed to turn them OFF.
        if (currentMillis - lastLedTime >= 1000) 
        {
            ledState = false;
            digitalWrite(LED1_PIN, LOW);
            digitalWrite(LED2_PIN, LOW);
            lastLedTime = currentMillis;
        }
    } 
    else 
    {
        // LEDs are OFF. Check if 2 seconds (2000ms) have passed to turn them ON.
        if (currentMillis - lastLedTime >= 2000) 
        {
            ledState = true;
            digitalWrite(LED1_PIN, HIGH);
            digitalWrite(LED2_PIN, HIGH);
            lastLedTime = currentMillis;
        }
    }

    // --- Existing WebSocket Broadcast Interval ---
    if ((currentMillis - lastTime) > INTERVAL)
    {
        String payload = buildPayload();
        Serial.printf("[WS] Broadcasting: %s\n", payload.c_str());
        notifyClients(payload);
        lastTime = currentMillis;
    }
    
    ws.cleanupClients();
}