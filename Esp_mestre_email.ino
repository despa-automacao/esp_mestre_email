#include <FS.h>
#include <SPI.h>
#include <WiFi.h>
#include <esp_now.h>
#include <ESP_Mail_Client.h>
#include <SD.h>
#include <EEPROM.h>

//Mac = 0C:B8:15:C3:3F:B4

#define chipSelect
#define EEPROM_SIZE 5

//Variaveis de tempo
unsigned long tempo_bt = 0;
unsigned long tempo_envio = 0;
int dados_recebidos1 = 0;
int dados_recebidos2 = 0;
int dados_recebidos3 = 0;
bool espNowInit = false;
bool envio1 = false;
bool envio2 = false;
bool envio3 = false;

//SMTP
#define WIFI_SSID "Projeto Dormentes 2.4GHz"
#define WIFI_PASSWORD "Vale135791"

//#define WIFI_SSID "M"
//#define WIFI_PASSWORD "automacao"

#define SMTP_HOST "smtp.gmail.com"
#define SMTP_PORT 465

#define AUTHOR_EMAIL "autoesp.despa@gmail.com"
#define AUTHOR_PASSWORD "pkwz kuot yfys vyfx"

#define RECIPIENT_EMAIL "ac.durae@gmail.com"

String str = "Relatório em anexo.";

SMTPSession smtp;

void smtpCallback(SMTP_Status status);

//ESP-NOW
String salva1 = "";
String salva2 = "";
String salva3 = "";

char salvaSD1[75];
char salvaSD2[75];
char salvaSD3[75];

File file;

//definindo estrutura de dados
typedef struct struct_message {
  int identificador;
  float msg_temp;
  float msg_umi;
  double msg_co2;
  int msgDia;
  int msgMes;
  int msgAno;
  int msgHora;
  int msgMin;
  int msgSeg;
} struct_message;

//criando objetos da estrutura
struct_message message;

//Variaveis para armazenar dados
float temp1, temp2, temp3;
float umi1, umi2, umi3;
double co2_1, co2_2, co2_3;
int dia_1, mes_1, ano_1, hora_1, minuto_1, segundo_1;
int dia_2, mes_2, ano_2, hora_2, minuto_2, segundo_2;
int dia_3, mes_3, ano_3, hora_3, minuto_3, segundo_3;


void OnDataRecv(const uint8_t* mac, const uint8_t* incomingData, int len) {
  memcpy(&message, incomingData, sizeof(message));
  Serial.print("Data recieved ");
  Serial.print(len);
  Serial.print("Dados recebidos do esp ");
  Serial.println(message.identificador);

  if (message.identificador == 1) {
    temp1 = message.msg_temp;
    umi1 = message.msg_umi;
    co2_1 = message.msg_co2;
    dia_1 = message.msgDia;
    mes_1 = message.msgMes;
    ano_1 = message.msgAno;
    hora_1 = message.msgHora;
    minuto_1 = message.msgMin;
    segundo_1 = message.msgSeg;

    

    salva1 = "Temp: "; salva1 += String(temp1); salva1 += "; ";
    salva1 += "Umi: "; salva1 += String(umi1); salva1 += "; ";
    salva1 += "CO2: "; salva1 += String(co2_1); salva1 += "; ";
    salva1 += String(hora_1); salva1 += ":"; salva1 += String(minuto_1);
    salva1 += ":"; salva1 += String(segundo_1); salva1 += "; ";
    salva1 += String(dia_1); salva1 += "/"; salva1+= String(mes_1);
    salva1 += "/"; salva1 += String(ano_1); salva1+= "; ";

    Serial.println(salva1);

    salva1.toCharArray(salvaSD1, 80);
    appendFile(SD, "/d1.txt", salvaSD1);
    appendFile(SD, "/d1.txt", "\r\n");
    dados_recebidos1++;
    Serial.printf("Somador recebidos armario 1 = %d\n", dados_recebidos1);
  }

  if (message.identificador == 2) {
    temp2 = message.msg_temp;
    umi2 = message.msg_umi;
    co2_2 = message.msg_co2;
    ano_2 = message.msgAno;
    mes_2 = message.msgMes;
    dia_2 = message.msgDia;
    hora_2 = message.msgHora;
    minuto_2 = message.msgMin;
    segundo_2 = message.msgSeg;

    salva2 = "Temp: "; salva2 += String(temp2); salva2 += "; ";
    salva2 += "Umi: "; salva2 += String(umi2); salva2 += "; ";
    salva2 += "CO2: "; salva2 += String(co2_2); salva2 += "; ";
    salva2 += String(hora_2); salva2 += ":"; salva2 += String(minuto_2);
    salva2 += ":"; salva2 += String(segundo_2); salva2 += "; ";
    salva2 += String(dia_2); salva2 += "/"; salva2+= String(mes_2);
    salva2 += "/"; salva2 += String(ano_2); salva2+= "; ";

    Serial.println(salva2);

    salva2.toCharArray(salvaSD2, 80);
    appendFile(SD, "/d2.txt", salvaSD2);
    appendFile(SD, "/d2.txt", "\r\n");
    dados_recebidos2++;
    Serial.printf("Somador recebidos armario 2: %d\n", dados_recebidos2);
  }

  if (message.identificador == 3) {
    temp3 = message.msg_temp;
    umi3 = message.msg_umi;
    co2_3 = message.msg_co2;
    hora_3 = message.msgHora;
    minuto_3 = message.msgMin;
    segundo_3 = message.msgSeg;
    ano_3 = message.msgAno;
    mes_3 = message.msgMes;
    dia_3 = message.msgDia;


    salva3 = "Temp: "; salva3 += String(temp3); salva3 += "; ";
    salva3 += "Umi: "; salva3 += String(umi3); salva3 += "; ";
    salva3 += "CO2: "; salva3 += String(co2_3); salva3 += "; ";
    salva3 += String(hora_3); salva3 += ":"; salva3 += String(minuto_3);
    salva3 += ":"; salva3 += String(segundo_3); salva3 += "; ";
    salva3 += String(dia_3); salva3 += "/"; salva3+= String(mes_3);
    salva3 += "/"; salva3 += String(ano_3); salva3+= "; ";

    Serial.println(salva3);

    salva3.toCharArray(salvaSD3, 80);
    appendFile(SD, "/d3.txt", salvaSD3);
    appendFile(SD, "/d3.txt", "\r\n");
    dados_recebidos3++;
    Serial.printf("Somador recebidos camara: %d\n", dados_recebidos3);
  }
  tempo_envio = millis();
}

void setup() {
  Serial.begin(115200);

  //Iniciando a EEPROM com tamanho de 5 bytes
  EEPROM.begin(EEPROM_SIZE);

  //Iniciando cartão SD
  while (!SD.begin()) {
    Serial.println("Erro SD");
    delay(1000);
  }
  Serial.println("SD iniciado");

  if (!SD.exists("/d1.txt")) {
    writeFile(SD, "/d1.txt", "Leituras armario 1: Temperatura, Umidade, CO2\n");
  }
  if (SD.exists("/d1.txt")) {
    Serial.println("Arquivo 1 ok");
  }

  if(!SD.exists("/d2.txt")){
    writeFile(SD, "/d2.txt", "Leituras armario 2: Temperatura, Umidade, CO2\n");
  }
  if (SD.exists("/d2.txt")) {
    Serial.println("Arquivo 2 ok");
  }
  
  if(!SD.exists("/d3.txt")){
    writeFile(SD, "/d3.txt", "Leituras câmara: Temperatura, Umidade, CO2\n");
  }
  if (SD.exists("/d3.txt")) {
    Serial.println("Arquivo 3 ok");
  }

  dados_recebidos1 = EEPROM.read(0);
  dados_recebidos2 = EEPROM.read(1);
  dados_recebidos3 = EEPROM.read(2);
  
  WiFi.mode(WIFI_STA);

  if (esp_now_init() != ESP_OK) {
    Serial.println("Error initializing ESP-NOW");
    return;
  } else if (esp_now_init() == ESP_OK) {
    Serial.println("ESPNow Init Success");
    //Register callback function
    esp_now_register_recv_cb(OnDataRecv);
    espNowInit = true;
  }
  
}

void loop() {

  //Objetos e parametros para a sessão SMTP
  ESP_Mail_Session session;
  smtp.debug(1);
  smtp.callback(smtpCallback);


  if (millis() - tempo_bt > 5000 && espNowInit == true) {
    tempo_envio = millis();
  }
  if (dados_recebidos1 >= 250 && envio1 == false) {
    WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
    while (WiFi.status() != WL_CONNECTED) {
      Serial.print(".");
      delay(200);
    }
    Serial.println("");
    Serial.println("WiFi connected.");
    Serial.println("IP address: ");
    Serial.println(WiFi.localIP());
    Serial.println();

    //Enviando email armario 1**************
    session.server.host_name = SMTP_HOST;
    session.server.port = SMTP_PORT;
    session.login.email = AUTHOR_EMAIL;
    session.login.password = AUTHOR_PASSWORD;
    session.login.user_domain = "";

    SMTP_Message msg;
    msg.sender.name = "ESP";
    msg.sender.email = AUTHOR_EMAIL;
    msg.subject = "Relatorio Armario 1";
    msg.addRecipient("durae", RECIPIENT_EMAIL);

    msg.text.content = str.c_str();
    msg.text.charSet = "utf-8";
    msg.text.transfer_encoding = Content_Transfer_Encoding::enc_7bit;
    msg.priority = esp_mail_smtp_priority::esp_mail_smtp_priority_low;
    msg.response.notify = esp_mail_smtp_notify_success | esp_mail_smtp_notify_failure | esp_mail_smtp_notify_delay;

    //instanciando objeto do anexo
    SMTP_Attachment att;

    att.descr.filename = "d1.txt";
    att.descr.mime = "text/plain";
    att.file.path = "/d1.txt";
    att.file.storage_type = esp_mail_file_storage_type_sd;
    att.descr.transfer_encoding = Content_Transfer_Encoding::enc_base64;

    msg.addAttachment(att);

    if (!smtp.connect(&session)) {
      Serial.println("Erro email 1");
      return;
    }

    if (!MailClient.sendMail(&smtp, &msg, true)) {
      Serial.println("Error sending Email 1, " + smtp.errorReason());
    }
    //envio1 = true;
    dados_recebidos1 = 0;
    deleteFile(SD, "d1.txt");
    writeFile(SD, "/d1.txt", "Leituras armario 1: Temperatura, Umidade, CO2\n");
    EEPROM.write(0,0);
    EEPROM.write(1, dados_recebidos2);
    EEPROM.write(2, dados_recebidos3);
    EEPROM.commit();
    ESP.restart();
  }
  //final email armario 1********************************

  //Enviando email armario 2**************
  if (dados_recebidos2 >= 250 && envio2 == false) {
    WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
    while (WiFi.status() != WL_CONNECTED) {
      Serial.print(".");
      delay(200);
    }
    session.server.host_name = SMTP_HOST;
    session.server.port = SMTP_PORT;
    session.login.email = AUTHOR_EMAIL;
    session.login.password = AUTHOR_PASSWORD;
    session.login.user_domain = "";

    SMTP_Message msg2;
    msg2.sender.name = "ESP";
    msg2.sender.email = AUTHOR_EMAIL;
    msg2.subject = "Relatorio Armario 2";
    msg2.addRecipient("durae", RECIPIENT_EMAIL);

    msg2.text.content = str.c_str();
    msg2.text.charSet = "utf-8";
    msg2.text.transfer_encoding = Content_Transfer_Encoding::enc_7bit;
    msg2.priority = esp_mail_smtp_priority::esp_mail_smtp_priority_low;
    msg2.response.notify = esp_mail_smtp_notify_success | esp_mail_smtp_notify_failure | esp_mail_smtp_notify_delay;

    //instanciando objeto do anexo
    SMTP_Attachment att2;

    att2.descr.filename = "d2.txt";
    att2.descr.mime = "plain/text";
    att2.file.path = "/d2.txt";
    att2.file.storage_type = esp_mail_file_storage_type_sd;
    att2.descr.transfer_encoding = Content_Transfer_Encoding::enc_base64;

    msg2.addAttachment(att2);

    if (!smtp.connect(&session)) {
      Serial.println("Erro email 2");
      return;
    }

    if (!MailClient.sendMail(&smtp, &msg2)) {
      Serial.println("Error sending Email 2, " + smtp.errorReason());
    }
    //envio2 = true;
    dados_recebidos2 = 0;
    deleteFile(SD, "d2.txt");
    writeFile(SD, "/d2.txt", "Leituras armario 2: Temperatura, Umidade, CO2\n");
    EEPROM.write(0, dados_recebidos1);
    EEPROM.write(1, 0);
    EEPROM.write(2, dados_recebidos3);
    EEPROM.commit();
    ESP.restart();
  }
  //final email armario 2********************************

  //Enviando camara**************
  if (dados_recebidos3 >= 250 && envio3 == false) {
    WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
    while (WiFi.status() != WL_CONNECTED) {
      Serial.print(".");
      delay(200);
    }
    session.server.host_name = SMTP_HOST;
    session.server.port = SMTP_PORT;
    session.login.email = AUTHOR_EMAIL;
    session.login.password = AUTHOR_PASSWORD;
    session.login.user_domain = "";

    SMTP_Message msg3;

    msg3.sender.name = "ESP";
    msg3.sender.email = AUTHOR_EMAIL;
    msg3.subject = "Relatorio Câmara";
    msg3.addRecipient("durae", RECIPIENT_EMAIL);

    msg3.text.content = str.c_str();
    msg3.text.charSet = "utf-8";
    msg3.text.transfer_encoding = Content_Transfer_Encoding::enc_7bit;
    msg3.priority = esp_mail_smtp_priority::esp_mail_smtp_priority_low;
    msg3.response.notify = esp_mail_smtp_notify_success | esp_mail_smtp_notify_failure | esp_mail_smtp_notify_delay;

    //instanciando objeto do anexo
    SMTP_Attachment att3;

    att3.descr.filename = "d3.txt";
    att3.descr.mime = "plain/txt";
    att3.file.path = "/d3.txt";
    att3.file.storage_type = esp_mail_file_storage_type_sd;
    att3.descr.transfer_encoding = Content_Transfer_Encoding::enc_base64;

    msg3.addAttachment(att3);

    if (!smtp.connect(&session)) {
      Serial.println("Erro email 3");
      return;
    }

    if (!MailClient.sendMail(&smtp, &msg3)) {
      Serial.println("Error sending Email 3, " + smtp.errorReason());
    }
    //envio3 = true;
    dados_recebidos3 = 0;
    deleteFile(SD, "d3.txt");
    writeFile(SD, "/d3.txt", "Leituras Câmara: Temperatura, Umidade, CO2\n");
    EEPROM.write(0, dados_recebidos1);
    EEPROM.write(1, dados_recebidos2);
    EEPROM.write(2, 0);
    EEPROM.commit();
    ESP.restart();
  }
  //final camara********************************
  //  if (envio1 == true && envio2 == true && envio3 == true) {
  //    delay(1000);
  //    WiFi.disconnect();
  //    dados_recebidos1 = 0;
  //    dados_recebidos2 = 0;
  //    dados_recebidos3 = 0;
  //    espNowInit = false;
  //    tempo_bt = millis();
  //    ESP.restart();
  //  }
}
/* Callback function to get the Email sending status */
void smtpCallback(SMTP_Status status) {
  /* Print the current status */
  Serial.println(status.info());

  /* Print the sending result */
  if (status.success()) {
    Serial.println("----------------");
    ESP_MAIL_PRINTF("Message sent success: %d\n", status.completedCount());
    ESP_MAIL_PRINTF("Message sent failled: %d\n", status.failedCount());
    Serial.println("----------------\n");
    struct tm dt;

    for (size_t i = 0; i < smtp.sendingResult.size(); i++) {
      /* Get the result item */
      SMTP_Result result = smtp.sendingResult.getItem(i);
      time_t ts = (time_t)result.timestamp;
      localtime_r(&ts, &dt);

      ESP_MAIL_PRINTF("Message No: %d\n", i + 1);
      ESP_MAIL_PRINTF("Status: %s\n", result.completed ? "success" : "failed");
      ESP_MAIL_PRINTF("Date/Time: %d/%d/%d %d:%d:%d\n", dt.tm_year + 1900, dt.tm_mon + 1, dt.tm_mday, dt.tm_hour, dt.tm_min, dt.tm_sec);
      ESP_MAIL_PRINTF("Recipient: %s\n", result.recipients);
      ESP_MAIL_PRINTF("Subject: %s\n", result.subject);
    }
    Serial.println("----------------\n");
  }
}

//funcoes do cartao sd
void listDir(fs::FS &fs, const
             char * dirname, uint8_t levels) {
  Serial.printf("Listing directory: %s\n", dirname);

  File root = fs.open(dirname);
  if (!root) {
    Serial.println("Failed to open directory");
    return;
  }
  if (!root.isDirectory()) {
    Serial.println("Not a directory");
    return;
  }

  File file = root.openNextFile();
  while (file) {
    if (file.isDirectory()) {
      Serial.print("  DIR : ");
      Serial.println(file.name());
      if (levels) {
        listDir(fs, file.name(), levels - 1);
      }
    } else {
      Serial.print("  FILE: ");
      Serial.print(file.name());
      Serial.print("  SIZE: ");
      Serial.println(file.size());
    }
    file = root.openNextFile();
  }
}

void createDir(fs::FS &fs, const char * path) {
  Serial.printf("Creating Dir: %s\n", path);
  if (fs.mkdir(path)) {
    Serial.println("Dir created");
  } else {
    Serial.println("mkdir failed");
  }
}

void removeDir(fs::FS &fs, const char * path) {
  Serial.printf("Removing Dir: %s\n", path);
  if (fs.rmdir(path)) {
    Serial.println("Dir removed");
  } else {
    Serial.println("rmdir failed");
  }
}

void readFile(fs::FS &fs, const char * path) {
  Serial.printf("Reading file: %s\n", path);

  File file = fs.open(path);
  if (!file) {
    Serial.println("Failed to open file for reading");
    return;
  }

  Serial.print("Read from file: ");
  while (file.available()) {
    Serial.write(file.read());
  }
  file.close();
}

void writeFile(fs::FS &fs, const char * path, const char * message) {
  Serial.printf("Writing file: %s\n", path);

  File file = fs.open(path, FILE_WRITE);
  if (!file) {
    Serial.println("Failed to open file for writing");
    return;
  }
  if (file.print(message)) {
    Serial.println("File written");
  } else {
    Serial.println("Write failed");
  }
  file.close();
}

void appendFile(fs::FS &fs, const char * path, const char * message) {
  Serial.printf("Appending to file: %s\n", path);

  File file = fs.open(path, FILE_APPEND);
  if (!file) {
    Serial.println("Failed to open file for appending");
    return;
  }
  if (file.print(message)) {
    Serial.println("Message appended");
  } else {
    Serial.println("Append failed");
  }
  file.close();
}

void renameFile(fs::FS &fs, const char * path1, const char * path2) {
  Serial.printf("Renaming file %s to %s\n", path1, path2);
  if (fs.rename(path1, path2)) {
    Serial.println("File renamed");
  } else {
    Serial.println("Rename failed");
  }
}

void deleteFile(fs::FS &fs, const char * path) {
  Serial.printf("Deleting file: %s\n", path);
  if (fs.remove(path)) {
    Serial.println("File deleted");
  } else {
    Serial.println("Delete failed");
  }
}

void testFileIO(fs::FS &fs, const char * path) {
  File file = fs.open(path);
  static uint8_t buf[512];
  size_t len = 0;
  uint32_t start = millis();
  uint32_t end = start;
  if (file) {
    len = file.size();
    size_t flen = len;
    start = millis();
    while (len) {
      size_t toRead = len;
      if (toRead > 512) {
        toRead = 512;
      }
      file.read(buf, toRead);
      len -= toRead;
    }
    end = millis() - start;
    Serial.printf("%u bytes read for %u ms\n", flen, end);
    file.close();
  } else {
    Serial.println("Failed to open file for reading");
  }


  file = fs.open(path, FILE_WRITE);
  if (!file) {
    Serial.println("Failed to open file for writing");
    return;
  }

  size_t i;
  start = millis();
  for (i = 0; i < 2048; i++) {
    file.write(buf, 512);
  }
  end = millis() - start;
  Serial.printf("%u bytes written for %u ms\n", 2048 * 512, end);
  file.close();
}
