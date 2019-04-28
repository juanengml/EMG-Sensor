#include <PubSubClient.h>
#include <ESP8266WiFi.h>

#define WIFI_AP "tapodi"
#define WIFI_PASSWORD "naolembro"
#define TOKEN "NJCnPds28cbTbSp0MTi9"


char thingsboardServer[] = "192.168.100.2";

WiFiClient wifiClient;

PubSubClient client(wifiClient);

int status = WL_IDLE_STATUS;
unsigned long lastSend;


void setup()
{
  Serial.begin(115200);
  delay(10);
  InitWiFi();
  pinMode(D5,INPUT);
  pinMode(D6,INPUT);

  client.setServer( thingsboardServer, 1883 );
  lastSend = 0;
}

void loop()
{
  if ( !client.connected() ) {
    reconnect();
  }

  if ( millis() - lastSend > 50 ) { // Update and send only after 1 seconds
    getData();
    lastSend = millis();
  }

  client.loop();
}

void getData()
{
  int t = analogRead(A0);
  //int t_dual = t*2;
  if ( isnan(t)) {
    return;
  }

  //Serial.print("EMG Sensor: ");
  Serial.println(t);
  //Serial.print("EMG Sensor Dual: ");
  //Serial.print(t_dual);

  String Status = String(t);
  //String Status_Dual = String(t_dual);
  // Just debug messages
  //Serial.print( "\nSensor de EMG Wifi [" ); 
  //Serial.print( Status );
  //Serial.print( "Sensor de EMG Wifi [" ); 
  //Serial.print( Status_Dual );
  
  //Serial.print( "]   -> \n" );

  // Prepare a JSON payload string
  String payload = "{";
  payload += "\"Status\":"; payload += Status;
  //payload += "\"Status_Dual\":"; payload += Status_Dual; payload += ",";
  
  payload += "}";

  // Send payload
  char attributes[100];
  payload.toCharArray( attributes, 100 );
  client.publish( "v1/devices/me/telemetry", attributes );
  //Serial.println( attributes );

}

void InitWiFi()
{
  Serial.println("Connecting to AP ...");
  // attempt to connect to WiFi network

  WiFi.begin(WIFI_AP, WIFI_PASSWORD);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("Connected to AP");
}


void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    status = WiFi.status();
    if ( status != WL_CONNECTED) {
      WiFi.begin(WIFI_AP, WIFI_PASSWORD);
      while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
      }
      Serial.println("Connected to AP");
    }
    Serial.print("Connecting to ThingsBoard node ...");
    // Attempt to connect (clientId, username, password)
    if ( client.connect("ESP8266 Device", TOKEN, NULL) ) {
      Serial.println( "[DONE]" );
    } else {
      Serial.print( "[FAILED] [ rc = " );
      Serial.print( client.state() );
      Serial.println( " : retrying in 5 seconds]" );
      // Wait 5 seconds before retrying
      delay( 5000 );
    }
  }
}





