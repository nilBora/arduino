#include <ESP8266WiFi.h>
 
const char* ssid = "INET_EPTA";  // для SSID точки доступа
const char* password = "brdnlsrg21111990";  // для пароля к точке доступа
IPAddress ip(192,168,0,21);  //Node static IP
IPAddress gateway(192,168,0,1);
IPAddress subnet(255,255,255,0); 
int ledPin = 2; // контакт GPIO2 на ESP8266
WiFiServer server(80);  //  порт веб-сервера
 
void setup() {
  Serial.begin(115200);
  delay(10);
 
  pinMode(ledPin, OUTPUT);
  digitalWrite(ledPin, LOW);
   
  // подключаемся к WiFi-сети:
  Serial.println();
  Serial.println();
  Serial.print("Connecting to ");  //  "Подключение к "
  Serial.println(ssid);
   
  WiFi.begin(ssid, password);
  WiFi.config(ip, gateway, subnet);
    
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");  //  "Подключение к WiFi
                                        //  выполнено"
   
  // запускаем сервер:
  server.begin();
  Serial.println("Server started");  //  "Сервер запущен"
 
  // печатаем IP-адрес:
  Serial.print("Use this URL to connect: ");  //  "Используем этот URL
                                              //  для подключения: "
  Serial.print("http://");
  Serial.print(WiFi.localIP());
  Serial.println("/");
}
 
void loop() {
   if (WiFi.status() != WL_CONNECTED) {
     delay(500);
     ESP.reset();
     return;
  } 
  // проверяем, подключен ли клиент:
  WiFiClient client = server.available();
  if (!client) {
    return;
  }

 
  // ждем, когда клиент отправит какие-нибудь данные:
  Serial.println("new client");  //  "новый клиент"
  while(!client.available()){
    delay(1);
  }
   
  // считываем первую строчку запроса:
  String request = client.readStringUntil('\r');
  Serial.println(request);
  client.flush();
   
   // обрабатываем запрос:
   int value = LOW;
  if (request.indexOf("/LED=ON") != -1) {
    digitalWrite(ledPin, HIGH);
    value = HIGH;
  }
  if (request.indexOf("/LED=OFF") != -1){
    digitalWrite(ledPin, LOW);
    value = LOW;
  }
 
  // выставляем значение на ledPin в соответствии с запросом:
  //digitalWrite(ledPin, value);
   
  // возвращаем ответ:
  client.println("HTTP/1.1 200 OK");
  client.println("Content-Type: text/html");  //  "Тип контента:
                                              //  text/html "
  client.println("");  //  не забываем это...
  client.println("<!DOCTYPE HTML>");
  client.println("<html>");
   
  client.print("Led pin is now: ");  //  "Контакт светодиода теперь
                                     //  в состоянии: "
   
  if(value == HIGH) {
    client.print("On");   //  "Вкл"
  } else {
    client.print("Off");  //  "Выкл"
  }
  client.println("<br><br>");
  client.println("Click <a href=\"/LED=ON\">here</a> turn the LED on pin 2 ON<br>");  //  "Кликните тут, чтобы включить светодиод
                 //  на контакте 2"
  client.println("Click <a href=\"/LED=OFF\">here turn the LED on pin 2 OFF<br>");     //  "Кликните тут, чтобы выключить светодиод
                 //  на контакте 2"
 
  client.println("</html>");
 
  delay(1);
  Serial.println("Client disconnected");  //  "Клиент отключен"
  Serial.println("");
}
