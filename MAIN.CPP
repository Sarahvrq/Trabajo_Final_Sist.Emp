#include <WiFi.h>
#include <WebServer.h>
#include <uri/UriBraces.h>
#include <WiFiClient.h>

#define WIFI_SSID "wifiRafa"       // Cambia por tu red WiFi
#define WIFI_PASSWORD "iPad0000"   // Cambia por tu contraseña
#define LED1 4
#define LED2 5
#define LED3 6
#define LED4 7
#define POT 8
#define BOOT 0
#define SAMPFREQ                1000
#define CONVERSIONS_PER_MEASURE 100

bool led1State = false;
bool led2State = false;
bool led3State = false;
bool led4State = false;
bool BootState = false;

uint8_t adc_pins[]            = {8}; 
adc_continuous_data_t *result = NULL; 

hw_timer_t * timer = NULL;

volatile bool flag_conversion_done = false;
volatile bool flag_Boot_pressed = false;
volatile int lastBootPress=0;
int duty = 0; 

void IRAM_ATTR Boot_Isr() {
  flag_Boot_pressed = true;         // marcar evento
}

void ARDUINO_ISR_ATTR myISR_ADC ()
{ 
  flag_conversion_done = true;
}

WebServer server(80); // Servidor HTTP en el puerto 80

void sendHtml() {
  Serial.println("/GET ");
  String response = R"(
    <!DOCTYPE html><html>
      <head>
        <title>ESP32 Web Server Demo</title>
        <meta name="viewport" content="width=device-width, initial-scale=1">
        <style>
          html { font-family: sans-serif; text-align: center; }
          body { display: inline-flex; flex-direction: column; }
          h1 { margin-bottom: 1.2em; } 
          h2 { margin: 0; }
          div { display: grid; grid-template-columns: 1fr 1fr; grid-template-rows: auto auto; grid-auto-flow: column; grid-gap: 1em; }
          .btn { background-color: #5B5; border: none; color: #fff; padding: 0.5em 1em;
                  font-size: 2em; text-decoration: none }
          .btn.OFF { background-color: #333; }
        </style>
      </head>
            
      <body>
        <h1>ESP32 Web Server</h1>

        <div>
          
          <h2>LED 1</h2>
          <a href="/toggle/1" class="btn LED1_TEXT">LED1_TEXT</a>
          
          <h2>LED 2</h2>
          <a href="/toggle/2" class="btn LED2_TEXT">LED2_TEXT</a>
          
          <h2>LED 3</h2>
          <a href="/toggle/3" class="btn LED3_TEXT">LED3_TEXT</a>
          
          <h2>LED 4</h2>
          <a href="/toggle/4" class="btn LED4_TEXT">LED4_TEXT</a>

        </div>
      </body>
    </html>
  )";

  response.replace("LED1_TEXT", led1State ? "ON" : "OFF");
  response.replace("LED2_TEXT", led2State ? "ON" : "OFF");
  response.replace("LED3_TEXT", led3State ? "ON" : "OFF");
  response.replace("LED4_TEXT", led4State ? "ON" : "OFF");
  server.send(200, "text/html", response);
}

void setup() {
  Serial.begin(115200);
  pinMode(BOOT, INPUT_PULLUP);
  attachInterrupt(BOOT, Boot_Isr, FALLING);

  ledcAttach(LED1, 1000, 12);
  ledcWrite(LED1, 0);
  ledcAttach(LED2, 1000, 12);
  ledcWrite(LED2, 0);
  ledcAttach(LED3, 1000, 12);
  ledcWrite(LED3, 0);
  ledcAttach(LED4, 1000, 12);
  ledcWrite(LED4, 0);
  pinMode(POT, ANALOG);

  analogContinuousSetWidth(12);
  analogContinuousSetAtten(ADC_11db); 
  analogContinuous(adc_pins, 1, CONVERSIONS_PER_MEASURE, SAMPFREQ, &myISR_ADC); 
  analogContinuousStart();
  Serial.println();
  Serial.println("Conectando a WiFi...");

  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("\nWiFi conectado!");
  Serial.print("IP asignada: ");
  Serial.println(WiFi.localIP());

  server.on("/", sendHtml);
  server.on(UriBraces("/toggle/{}"), []() {
    String led = server.pathArg(0);
    Serial.print("Toggle LED #");
    Serial.println(led);

    switch (led.toInt()) {
      case 1:
        led1State = !led1State;
        if(led1State!=true){
          ledcWrite(LED1, 0);
        }
        break;
      case 2:
        led2State = !led2State;
        if(led2State!=true){
          ledcWrite(LED2, 0);
        }
        break;
      case 3:
        led3State = !led3State;
        if(led3State!=true){
          ledcWrite(LED3, 0);
        }
        break;
      case 4:
        led4State = !led4State;
        if(led4State!=true){
          ledcWrite(LED4, 0);
        }
        break;
    }

    sendHtml();
  });
  delay(10);
  server.begin();
  Serial.println("Servidor web iniciado.");
}

void loop() {
  server.handleClient();
  if(!BootState){
    if (flag_conversion_done) {
      flag_conversion_done = false;
      if(analogContinuousRead(&result, 0)){
        int raw_ADC = result[0].avg_read_raw;
        if(led1State){
          ledcWrite(LED1, raw_ADC);
        }
        if(led2State){
          ledcWrite(LED2, raw_ADC);
        }
        if(led3State){
          ledcWrite(LED3, raw_ADC);
        }
        if(led4State){
          ledcWrite(LED4, raw_ADC);
        }
      }
    }
  }else{
    ledcWrite(LED1, 0);
    ledcWrite(LED2, 0);
    ledcWrite(LED3, 0);
    ledcWrite(LED4, 0);
  }
  if (flag_Boot_pressed) {
    if(millis() - lastBootPress > 1000){
      lastBootPress=millis();
      flag_Boot_pressed = false;
      BootState=!BootState;
    }
  }

  delay(10);
}

