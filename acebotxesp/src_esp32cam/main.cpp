#include <esp_camera.h>
#include <WiFi.h>
#include <WebServer.h>

// === Wi-Fi Configuration ===
const char *ssid = "Webbaby";
const char *password = "wwwwebbaby1";

// === Web Server ===
WebServer server(80);

// === Serial Command Buffer ===
#define SERIAL_BUFFER_SIZE 64
char serialBuffer[SERIAL_BUFFER_SIZE];
uint8_t serialIndex = 0;

// === Stream Control ===
bool streamingActive = false;

// === Camera Pin Map (AI Thinker) ===
#define PWDN_GPIO_NUM 32
#define RESET_GPIO_NUM -1
#define XCLK_GPIO_NUM 0
#define SIOD_GPIO_NUM 26
#define SIOC_GPIO_NUM 27
#define Y9_GPIO_NUM 35
#define Y8_GPIO_NUM 34
#define Y7_GPIO_NUM 39
#define Y6_GPIO_NUM 36
#define Y5_GPIO_NUM 21
#define Y4_GPIO_NUM 19
#define Y3_GPIO_NUM 18
#define Y2_GPIO_NUM 5
#define VSYNC_GPIO_NUM 25
#define HREF_GPIO_NUM 23
#define PCLK_GPIO_NUM 22

// === Stream Route ===
void handleStream()
{
    WiFiClient client = server.client();
    client.println("HTTP/1.1 200 OK");
    client.println("Content-Type: multipart/x-mixed-replace; boundary=frame");
    client.println();

    while (client.connected() && streamingActive)
    {
        camera_fb_t *fb = esp_camera_fb_get();
        if (!fb)
        {
            Serial.println("ESP32:ERROR Camera capture failed");
            break;
        }

        client.print("--frame\r\n");
        client.print("Content-Type: image/jpeg\r\n\r\n");
        client.write(fb->buf, fb->len);
        client.print("\r\n");
        esp_camera_fb_return(fb);
        delay(10); // Adjust for frame rate
    }
}

// === Root Route (optional) ===
void handleRoot()
{
    server.send(200, "text/plain", "ACEBot ESP32-CAM is online.\nUse /stream to view live video.");
}

// === Initialize Camera ===
bool setupCamera()
{
    camera_config_t config;
    config.ledc_channel = LEDC_CHANNEL_0;
    config.ledc_timer = LEDC_TIMER_0;
    config.pin_d0 = Y2_GPIO_NUM;
    config.pin_d1 = Y3_GPIO_NUM;
    config.pin_d2 = Y4_GPIO_NUM;
    config.pin_d3 = Y5_GPIO_NUM;
    config.pin_d4 = Y6_GPIO_NUM;
    config.pin_d5 = Y7_GPIO_NUM;
    config.pin_d6 = Y8_GPIO_NUM;
    config.pin_d7 = Y9_GPIO_NUM;
    config.pin_xclk = XCLK_GPIO_NUM;
    config.pin_pclk = PCLK_GPIO_NUM;
    config.pin_vsync = VSYNC_GPIO_NUM;
    config.pin_href = HREF_GPIO_NUM;
    config.pin_sccb_sda = SIOD_GPIO_NUM;
    config.pin_sccb_scl = SIOC_GPIO_NUM;
    config.pin_pwdn = PWDN_GPIO_NUM;
    config.pin_reset = RESET_GPIO_NUM;
    config.xclk_freq_hz = 20000000;
    config.frame_size = FRAMESIZE_VGA;
    config.pixel_format = PIXFORMAT_JPEG;
    config.fb_location = CAMERA_FB_IN_PSRAM;
    config.jpeg_quality = 12;
    config.fb_count = 1;
    config.grab_mode = CAMERA_GRAB_WHEN_EMPTY;

    esp_err_t err = esp_camera_init(&config);
    return (err == ESP_OK);
}

// === Process Commands from UNO ===
void processSerialCommand(const char *cmd)
{
    if (strcmp(cmd, "STREAM_ON") == 0)
    {
        streamingActive = true;
        Serial.println("ESP32:STREAMING");
    }
    else if (strcmp(cmd, "STREAM_OFF") == 0)
    {
        streamingActive = false;
        Serial.println("ESP32:STREAMING_OFF");
    }
    else if (strcmp(cmd, "PING") == 0)
    {
        Serial.println("ESP32:READY");
    }
    else
    {
        Serial.println("ESP32:ERROR Unknown command");
    }
}

// === Read Serial Input from UNO ===
void handleSerialInput()
{
    while (Serial.available())
    {
        char c = Serial.read();
        if (c == '\n' || c == '\r')
        {
            serialBuffer[serialIndex] = '\0';
            if (serialIndex > 0)
            {
                processSerialCommand(serialBuffer);
                serialIndex = 0;
            }
        }
        else if (serialIndex < SERIAL_BUFFER_SIZE - 1)
        {
            serialBuffer[serialIndex++] = c;
        }
    }
}

// === Setup ===
void setup()
{
    Serial.begin(9600);
    delay(100);

    if (!setupCamera())
    {
        Serial.println("ESP32:ERROR Camera init failed");
        return;
    }

    WiFi.begin(ssid, password);
    Serial.print("ESP32:Connecting to Wi-Fi");
    while (WiFi.status() != WL_CONNECTED)
    {
        delay(500);
        Serial.print(".");
    }
    Serial.println();
    Serial.println("ESP32:Wi-Fi connected");
    Serial.print("ESP32:IP ");
    Serial.println(WiFi.localIP());

    server.on("/", handleRoot);
    server.on("/stream", handleStream);
    server.begin();
    Serial.println("ESP32:Web server started");

    Serial.println("ESP32:READY");
}

// === Main Loop ===
void loop()
{
    handleSerialInput();
    server.handleClient();
}
