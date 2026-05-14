#include <Arduino.h>
#include <WiFi.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <Arduino_JSON.h>

#define FR_IN1 13
#define FR_IN2 12
#define FR_EN 16
#define FL_IN3 14
#define FL_IN4 27
#define FL_EN 17

// #define RL_IN1 26
// #define RL_IN2 25
// #define RL_EN 18
// #define RR_IN3 33
// #define RR_IN4 32
// #define RR_EN 19

// use 110 to 110 for 11.1 li-po battery and approximatly 220 for the 4.8 volt battery
int speed = 50;

// direction for the wheel will be determiend by binary, 1 is for cw and 0 is ccw of the wheel
void wheelDirection(int direction, int Motor_IN1, int Motor_IN2, int en)
{
    if (direction == 1)
    {
        digitalWrite(Motor_IN1, HIGH);
        digitalWrite(Motor_IN2, LOW);
        digitalWrite(en, HIGH);
    }
    else if (direction == 0)
    {
        digitalWrite(Motor_IN1, LOW);
        digitalWrite(Motor_IN2, LOW);
        digitalWrite(en, LOW); // <-- actually pulls EN low now
    }
    else
    {
        digitalWrite(Motor_IN1, LOW);
        digitalWrite(Motor_IN2, HIGH);
        digitalWrite(en, HIGH);
    }
}
void moveForward()
{
    wheelDirection(1, FR_IN1, FR_IN2, FR_EN);
    // wheelDirection(1, FL_IN3, FL_IN4, FL_EN);
    // wheelDirection(1, RL_IN1, RL_IN2, RL_EN);
    // wheelDirection(1, RR_IN3, RR_IN4, RR_EN);
}
void moveBackward()
{
    wheelDirection(-1, FR_IN1, FR_IN2, FR_EN);
    // wheelDirection(-1, FL_IN3, FL_IN4, FL_EN);
    // wheelDirection(-1, RL_IN1, RL_IN2, RL_EN);
    // wheelDirection(-1, RR_IN3, RR_IN4, RR_EN);
}
void moveRight()
{
    wheelDirection(1, FR_IN1, FR_IN2, FR_EN);
    // // // wheelDirection(-1, FL_IN3, FL_IN4, FL_EN);
    // wheelDirection(1, RL_IN1, RL_IN2, RL_EN);
    // wheelDirection(-1, RR_IN3, RR_IN4, RR_EN);
}
void moveLeft()
{
    wheelDirection(-1, FR_IN1, FR_IN2, FR_EN);
    // // // wheelDirection(1, FL_IN3, FL_IN4, FL_EN);
    // wheelDirection(-1, RL_IN1, RL_IN2, RL_EN);
    // wheelDirection(1, RR_IN3, RR_IN4, RR_EN);
}
void moveFR()
{
    wheelDirection(0, FR_IN1, FR_IN2, FR_EN);
    // // // wheelDirection(1, FL_IN3, FL_IN4, FL_EN);
    // wheelDirection(1, RL_IN1, RL_IN2, RL_EN);
    // wheelDirection(0, RR_IN3, RR_IN4, RR_EN);
}
void moveFL()
{
    wheelDirection(1, FR_IN1, FR_IN2, FR_EN);
    // // // wheelDirection(0, FL_IN3, FL_IN4, FL_EN);
    // wheelDirection(0, RL_IN1, RL_IN2, RL_EN);
    // wheelDirection(1, RR_IN3, RR_IN4, RR_EN);
}
void moveRR()
{
    wheelDirection(-1, FR_IN1, FR_IN2, FR_EN);
    // // // wheelDirection(0, FL_IN3, FL_IN4, FL_EN);
    // wheelDirection(0, RL_IN1, RL_IN2, RL_EN);
    // wheelDirection(-1, RR_IN3, RR_IN4, RR_EN);
}
void moveRL()
{
    wheelDirection(0, FR_IN1, FR_IN2, FR_EN);
    // // // wheelDirection(-1, FL_IN3, FL_IN4, FL_EN);
    // wheelDirection(-1, RL_IN1, RL_IN2, RL_EN);
    // wheelDirection(0, RR_IN3, RR_IN4, RR_EN);
}

void moveCW()
{
    wheelDirection(1, FR_IN1, FR_IN2, FR_EN);
    // // // wheelDirection(-1, FL_IN3, FL_IN4, FL_EN);
    // wheelDirection(-1, RL_IN1, RL_IN2, RL_EN);
    // wheelDirection(1, RR_IN3, RR_IN4, RR_EN);
}
void moveCCW()
{
    wheelDirection(-1, FR_IN1, FR_IN2, FR_EN);
    // // // wheelDirection(1, FL_IN3, FL_IN4, FL_EN);
    // wheelDirection(1, RL_IN1, RL_IN2, RL_EN);
    // wheelDirection(-1, RR_IN3, RR_IN4, RR_EN);
}

void Stop()
{
    wheelDirection(0, FR_IN1, FR_IN2, FR_EN);
    // // // wheelDirection(0, FL_IN3, FL_IN4, FL_EN);
    // wheelDirection(0, RL_IN1, RL_IN2, RL_EN);
    // wheelDirection(0, RR_IN3, RR_IN4, RR_EN);
}

// --- WiFi credentials ---
const char *ssid = "Amirsiphone";
const char *password = "Amir2005";

// --- Server & WebSocket ---
AsyncWebServer server(80);
AsyncWebSocket ws("/ws");
// --- Broadcast timer ---
unsigned long lastTime = 0;
const unsigned long INTERVAL = 5000; // ms between automatic pushes

String buildPayload()
{
    JSONVar obj;
    obj["message"] = "hello from ESP32";
    obj["uptime"] = millis() / 1000; // seconds since boot — replace with real data
    return JSON.stringify(obj);
}
// Send a string to every connected client
void notifyClients(const String &payload)
{
    ws.textAll(payload);
}
// Handle a text frame arriving from React
void handleWebSocketMessage(void *arg, uint8_t *data, size_t len)
{
    AwsFrameInfo *info = (AwsFrameInfo *)arg;
    if (info->final && info->index == 0 && info->len == len && info->opcode == WS_TEXT)
    {
        String msg = String((char *)data).substring(0, len);
        Serial.printf("[WS] Received: %s\n", msg.c_str());
        // Echo back or handle commands from React here
        if (msg == "ping")
        {
            notifyClients("{\"pong\":true}");
        }
        else if (msg == "moveForward")
        {
            moveForward();
        }
        else if (msg == "moveBackward")
        {
            moveBackward();
        }
        else if (msg == "moveRight")
        {
            moveRight();
        }
        else if (msg == "moveLeft")
        {
            moveLeft();
        }
        else if (msg == "moveFR")
        {
            moveFR();
        }
        else if (msg == "moveFL")
        {
            moveFL();
        }
        else if (msg == "moveRR")
        {
            moveRR();
        }
        else if (msg == "moveRL")
        {
            moveRL();
        }
        else if (msg == "moveCW")
        {
            moveCW();
        }
        else if (msg == "moveCCW")
        {
            moveCCW();
        }
        else if (msg == "Stop")
        {
            Stop();
        }
        else
        {
            notifyClients(buildPayload());
        }
    }
}
// WebSocket event dispatcher
void onWsEvent(AsyncWebSocket *server, AsyncWebSocketClient *client,
               AwsEventType type, void *arg, uint8_t *data, size_t len)
{
    switch (type)
    {
    case WS_EVT_CONNECT:
        Serial.printf("[WS] Client #%u connected from %s\n",
                      client->id(), client->remoteIP().toString().c_str());
        // Push current state immediately on connect
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
    // 1 ----------------------- WIFI SETUP
    Serial.begin(115200);
    initWiFi();
    initWebSocket();
    // Optional: add a simple HTTP health-check endpoint
    server.on("/", HTTP_GET, [](AsyncWebServerRequest *request)
              { request->send(200, "text/plain", "ESP32 WebSocket server running"); });
    server.begin();
    Serial.println("HTTP + WebSocket server started");

    // 2 ----------------------- ROBOT SETUP
    pinMode(FR_IN1, OUTPUT);
    pinMode(FR_IN2, OUTPUT);
    pinMode(FR_EN, OUTPUT);

    // pinMode(FL_IN3, OUTPUT);
    // pinMode(FL_IN4, OUTPUT);
    // pinMode(FL_EN, OUTPUT);

    // pinMode(RL_IN1, OUTPUT);
    // pinMode(RL_IN2, OUTPUT);
    // pinMode(RL_EN, OUTPUT);

    // pinMode(RR_IN3, OUTPUT);
    // pinMode(RR_IN4, OUTPUT);
    // pinMode(RR_EN, OUTPUT);
}
void loop()
{
    // Periodic broadcast to all connected React clients
    if ((millis() - lastTime) > INTERVAL)
    {
        String payload = buildPayload();
        Serial.printf("[WS] Broadcasting: %s\n", payload.c_str());
        notifyClients(payload);
        lastTime = millis();
    }
    ws.cleanupClients(); // free memory from stale connections
}