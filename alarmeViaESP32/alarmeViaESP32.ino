#include <WiFi.h>
#include <WebServer.h>

WebServer server (80);

// Nome e senha da rede
const char* ssid = "NOME_DA_REDE_WIFI";
const char* password = "SENHA_DA_REDE_WIFI";

// Token de autenticação, para evitar erro e acesso indesejados
const char* AUTH_TOKEN = "A7OmEetdXwYtEJ6b1eVccngugO89WxPm";

// Armazena as condições sendo verificada
bool danificado = false;

// Pinos do ESP32 conectados aos dispositivos
const int LED = 26;
const int PINO_BUZZER = 33;
const int BUTTON = 18;

void setup() {
  Serial.begin(115200);

  connectWifi();  // Conectar ao Wifi
  serverBegin();  // Inicia o servidor web

  // Configuração dos pinos para saída (OUTPUT) e entrada (INPUT)
  pinMode(PINO_BUZZER, OUTPUT);
  pinMode(LED, OUTPUT);
  pinMode(BUTTON, INPUT_PULLUP);

  Serial.println("Configuração completa.");
}


void loop() {

  // Mantém o servidor ativo
  server.handleClient(); 

  // Caso perca a conexão com o WiFi, tenta reconectar
  if (WiFi.status() != WL_CONNECTED) {
    Serial.print("Conectando a: ");
    Serial.println(ssid);
    connectWifi();
  }

  // Se as condições forem atendidas, o alarme despara e acende o LED e toca o buzzer
  if (danificado) {
    digitalWrite(LED, HIGH);
    playSound();
  }else{
    digitalWrite(LED, LOW);
  }

  Serial.print("danificado: ");
  Serial.println(danificado);

  delay(1000);
  reset();  // Verifica se o botão foi pressionado para resetar o estado
}

// Conecta ao WiFi
void connectWifi() {
  WiFi.enableSTA(true); // Habilita modo estação, sendo capaz de conectar a um roteador
  delay(2000);

  // Conecta na rede
  WiFi.begin(ssid, password, 6);

  // Aguarda até estar conectado de fato
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("Conectado ao Wi-Fi");
  Serial.print("Endereço IP: ");
  Serial.println(WiFi.localIP()); // Mostra o IP para acessar
}

// Inicializa o servidor web e define a rota raiz
void serverBegin() {
  server.on("/", handleRoot); // Quando acessar a raiz, chama handleeRoot()
  server.on("/status", handleStatus);
  server.begin();
  Serial.println("Servidor HTTP inicializado.");
}

// É chamada quando alguém acessa o endereço do ESP32
void handleRoot() {
  // Verifica se existe o argumento token na requisição
  if (server.hasArg("token")){
    // Verifica se o token recebido é igual ao token de autenticação
    if (server.arg("token") == AUTH_TOKEN){
      danificado = !danificado;  // Alterna o estado para teste
      //danificado = true   // Qualquer acesso à raiz ativa o estado
      server.send(200, "text/html", getPag()); 
    }else{
      // Caso, o token for incorreto devolve
      server.send(401, "text/html", "Acesso nao autorizado: Token invalido.");
    }
  }else{
    // Se tentar acessar sem o token na requisição devolve
    server.send(400, "text/html", "Acesso nao autorizado: Token faltando.");
  }
}

void handleStatus() {
  if (server.hasArg("token") && server.arg("token") == AUTH_TOKEN) {
    String jsonResponse = "{\"danificado\": " + String(danificado ? true : false) + "}";
    server.send(200, "aplication/json", jsonResponse);
  }else{
    server.send(401, "text/html", "Acesso não autorizado: Token inválido.");
  }
}

// Verifica se o botão foi pressionado para resetar o estado
void reset() {
  int buttonState = digitalRead(BUTTON);

  if (buttonState == LOW) {
    //danificado = !danificado; // Aqui é para teste, fica alternando false/true
    danificado = false;   // Desliga o alarme
    delay(200);           // Debounce simples
  }
}

// Toca o buzzer por 1 segundo
void playSound() {
  digitalWrite(PINO_BUZZER, HIGH);
  delay(1000);
  digitalWrite(PINO_BUZZER, LOW);
}

// Gera a página HTML exibida no navegador
String getPag() {
  String pag = "<html lang=en-EN><head><meta http-equiv='refresh' content='60'/>";
  pag += "<title>ESP32 WebServer - Alarm Controller</title>";
  pag += "<style> body { background-color: #fffff; font-family: Arial, Helvetica, Sans-Serif; Color: #000000; }</style>";
  pag += "</head><body><h1>Alarm Controller</h1>";
  pag += "<h3>Embalagem danificada detectada.</h3>"; 
  pag += "<h3>";
  pag += danificado;
  pag += "</h3>";
  pag += "</form>";
  pag += "</body></html>";
  return pag;
}
