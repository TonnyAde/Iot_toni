#include <WiFi.h>
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27, 20, 4);

// Replace with your network credentials
const char *ssid = "Delameta";
const char *password = "delameta1407";
//wifi rumah
//const char *ssid = "UDIN";
//const char *password = "18021998";

// Set web server port number to 80
WiFiServer server(80);

// Variable to store the HTTP request
String header;

// Auxiliar variables to store the current output state
String output16State = "off";
String output17State = "off";
String output18State = "off";
String output19State = "off";

// Assign output variables to GPIO pins
const int output16 = 16;
const int output17 = 17;
const int output18 = 18;
const int output19 = 19;
const int wifion = 15;
const int wifioff = 35;

// Current time
unsigned long currentTime = millis();
// Previous time
unsigned long previousTime = 0;
// Define timeout time in milliseconds (example: 2000ms = 2s)
const long timeoutTime = 2000;

void setup()
{
  lcd.init(); // initialize the lcd
  lcd.backlight();
  Serial.begin(115200);
  lcd.setCursor(3, 0);
  // print message
  lcd.print("Hello, World!");
  delay(1000);
  // clears the display to print new message
  lcd.clear();

  // Initialize the output variables as outputs

  pinMode(output16, OUTPUT);
  pinMode(output17, OUTPUT);
  pinMode(output18, OUTPUT);
  pinMode(output19, OUTPUT);
  pinMode(wifion, OUTPUT);
  pinMode(wifioff, OUTPUT);

  // set output to HIGH for turning off the lamp because we used an active low relay.
  // But, if an active high relay is used, you can set the ouput to LOW for turning off the lamp.
  digitalWrite(output16, HIGH);
  digitalWrite(output17, HIGH);
  digitalWrite(output18, HIGH);
  digitalWrite(output19, HIGH);
  //set wifi status
  digitalWrite(wifioff, LOW);
  digitalWrite(wifion, LOW);

  // Connect to Wi-Fi network with SSID and password
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
    lcd.setCursor(0, 1);
    lcd.print("Connecting to wifi :");
    lcd.setCursor(3, 2);
    lcd.print(ssid);
    digitalWrite(wifioff, HIGH);
    delay(500);
  }
  // Print local IP address and start web server
  Serial.println("");
  Serial.println("WiFi connected.");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
  server.begin();
  lcd.clear();
  lcd.setCursor(4, 0);
  lcd.print(" Your IP is: ");
  lcd.setCursor(4, 1);
  lcd.print(WiFi.localIP());
  // const char *iptmp="";
  // const char *iptmp2="";
  // sprintf(iptmp2,"%s",WiFi.localIP());
  // memcpy(iptmp, iptmp2, strlen(iptmp2)-4);
  // Serial.print("IP TMP: ");
  // Serial.println(iptmp);
  // if(memcmp(iptmp,"172.16.17",9)==0)
  //  {
  //      Serial.println("ip satu segment");
  //      digitalWrite(wifion, HIGH);
  //  }
  //  else
  //  {
  //     Serial.println("ip beda segment");
  //     while (1)
  //     {
  //       digitalWrite(wifion, HIGH);
  //       delay(500);
  //     }

  //  }
  digitalWrite(wifion, HIGH);
}

void loop()
{
  WiFiClient client = server.available(); // Listen for incoming clients

  if (client)
  { // If a new client connects,
    currentTime = millis();
    previousTime = currentTime;
    Serial.println("New Client."); // print a message out in the serial port
    String currentLine = "";       // make a String to hold incoming data from the client
    while (client.connected() && currentTime - previousTime <= timeoutTime)
    { // loop while the client's connected
      currentTime = millis();
      if (client.available())
      {                         // if there's bytes to read from the client,
        char c = client.read(); // read a byte, then
        Serial.write(c);        // print it out the serial monitor
        header += c;
        if (c == '\n')
        { // if the byte is a newline character
          // if the current line is blank, you got two newline characters in a row.
          // that's the end of the client HTTP request, so send a response:
          if (currentLine.length() == 0)
          {
            // HTTP headers always start with a response code (e.g. HTTP/1.1 200 OK)
            // and a content-type so the client knows what's coming, then a blank line:
            client.println("HTTP/1.1 200 OK");
            client.println("Content-type:text/html");
            client.println("Connection: close");
            client.println();

            // turns the GPIOs on and off
            if (header.indexOf("GET /16/on") >= 0)
            {
              Serial.println("GPIO 16 on");
              output16State = "on";
              digitalWrite(output16, LOW);
              lcd.clear();
              lcd.setCursor(4, 1);
              lcd.print("Lampu 1 ON");
            }
            else if (header.indexOf("GET /16/off") >= 0)
            {
              Serial.println("GPIO 16 off");
              output16State = "off";
              digitalWrite(output16, HIGH);
              lcd.clear();
              lcd.setCursor(4, 1);
              lcd.print("Lampu 1 OF");
            }
            else if (header.indexOf("GET /17/on") >= 0)
            {
              Serial.println("GPIO 17 on");
              output17State = "on";
              digitalWrite(output17, LOW);
              lcd.clear();
              lcd.setCursor(4, 1);
              lcd.print("Lampu 2 ON");
            }
            else if (header.indexOf("GET /17/off") >= 0)
            {
              Serial.println("GPIO 17 off");
              output17State = "off";
              digitalWrite(output17, HIGH);
              lcd.clear();
              lcd.setCursor(4, 1);
              lcd.print("Lampu 2 Off");
            }
            else if (header.indexOf("GET /18/on") >= 0)
            {
              Serial.println("GPIO 18 on");
              output18State = "on";
              digitalWrite(output18, LOW);
              lcd.clear();
              lcd.setCursor(4, 1);
              lcd.print("Lampu 3 ON");
            }
            else if (header.indexOf("GET /18/off") >= 0)
            {
              Serial.println("GPIO 18 off");
              output18State = "off";
              digitalWrite(output18, HIGH);
              lcd.clear();
              lcd.setCursor(4, 1);
              lcd.print("Lampu 3 OFF");
            }
            else if (header.indexOf("GET /19/on") >= 0)
            {
              Serial.println("GPIO 19 on");
              output19State = "on";
              digitalWrite(output19, LOW);
              lcd.clear();
              lcd.setCursor(4, 1);
              lcd.print("Lampu 4 ON");
            }
            else if (header.indexOf("GET /19/off") >= 0)
            {
              Serial.println("GPIO 19 off");
              output19State = "off";
              digitalWrite(output19, HIGH);
              lcd.clear();
              lcd.setCursor(4, 1);
              lcd.print("Lampu 4 OFF");
            }

            // Display the HTML web page
            client.println("<!DOCTYPE html><html>");
            client.println("<head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">");
            client.println("<link rel=\"icon\" href=\"data:,\">");
            // CSS to style the on/off buttons
            // Feel free to change the background-color and font-size attributes to fit your preferences
            client.println("<style>html { font-family: Helvetica; display: inline-block; margin: 0px auto; text-align: center;}");
            client.println(".button { background-color: #4CAF50; border: none; color: white; padding: 16px 40px;");
            client.println("text-decoration: none; font-size: 30px; margin: 2px; cursor: pointer;}");
            client.println(".button2 {background-color: #555555;}</style></head>");

            // Web Page Heading
            client.println("<body><h1>ESP32 Dashboard Test to controlling Lamp</h1>");

            // Display current state, and ON/OFF buttons for GPIO 26
            client.println("<p>GPIO 16 - State " + output16State + "</p>");
            // If the output16State is off, it displays the ON button
            if (output16State == "off")
            {
              client.println("<p><a href=\"/16/on\"><button class=\"button\">ON</button></a></p>");
            }
            else
            {
              client.println("<p><a href=\"/16/off\"><button class=\"button button2\">OFF</button></a></p>");
            }

            // Display current state, and ON/OFF buttons for GPIO 27
            client.println("<p>GPIO 17 - State " + output17State + "</p>");
            // If the output17State is off, it displays the ON button
            if (output17State == "off")
            {
              client.println("<p><a href=\"/17/on\"><button class=\"button\">ON</button></a></p>");
            }
            else
            {
              client.println("<p><a href=\"/17/off\"><button class=\"button button2\">OFF</button></a></p>");
            }
            // Display current state, and ON/OFF buttons for GPIO 26
            client.println("<p>GPIO 18 - State " + output18State + "</p>");
            // If the output16State is off, it displays the ON button
            if (output18State == "off")
            {
              client.println("<p><a href=\"/18/on\"><button class=\"button\">ON</button></a></p>");
            }
            else
            {
              client.println("<p><a href=\"/18/off\"><button class=\"button button2\">OFF</button></a></p>");
            }

            // Display current state, and ON/OFF buttons for GPIO 27
            client.println("<p>GPIO 19 - State " + output19State + "</p>");
            // If the output17State is off, it displays the ON button
            if (output19State == "off")
            {
              client.println("<p><a href=\"/19/on\"><button class=\"button\">ON</button></a></p>");
            }
            else
            {
              client.println("<p><a href=\"/19/off\"><button class=\"button button2\">OFF</button></a></p>");
            }
            client.println("</body></html>");

            // The HTTP response ends with another blank line
            client.println();
            // Break out of the while loop
            break;
          }
          else
          { // if you got a newline, then clear currentLine
            currentLine = "";
          }
        }
        else if (c != '\r')
        {                   // if you got anything else but a carriage return character,
          currentLine += c; // add it to the end of the currentLine
        }
      }
    }
    // Clear the header variable
    header = "";
    // Close the connection
    client.stop();
    Serial.println("Client disconnected.");
    Serial.println("");
  }
}
