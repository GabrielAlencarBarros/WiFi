#include <Arduino.h>
#include <WiFi.h>
#include <HTTPClient.h>
#include <time.h>
#include <LiquidCrystal_I2C.h>
#include "esp_sntp.h"

LiquidCrystal_I2C lcd(0x27, 20, 4);
String pedirParaApiClima(String);

const char* nomeRedeWiFi = "SENAI EDUCACIONAL";
const char* senhaRedeWiFi = "info@134";
const String enderecoAPI = "https://wttr.in/Sao+Caetano+do+Sul?m&lang=pt&format=";

//? ----------------------- SERVIDORES NTP -----------------------
const char* NTP1 = "a.st1.ntp.br";
const char* NTP2 = "pool.ntp.org";
const char* FUSO = "<-03>3";
const char* DIAS[] = {"Domingo", "Segunda", "Terca", "Quarta", "Quinta", "Sexta", "Sabado"};

void setup() 
{
    Serial.begin(115200);
    lcd.init();
    lcd.backlight();
    WiFi.begin(nomeRedeWiFi, senhaRedeWiFi);

    Serial.print("\n\rIniciando conexão WiFi\n\r");
    while(WiFi.status() != WL_CONNECTED)
    {
        Serial.print("#");
        delay(500);
    }

    sntp_set_sync_interval(24*60*60*1000); //? intervalo de atualização da hora na internet padrao 1*60*60*1000

    //? Configuração de fuso e servidores NTP e inicia a sincronização
    configTzTime(FUSO, NTP1, NTP2);

    struct tm agora;
    Serial.print("\n\r Sincronizando hora\n\r");
    while(!getLocalTime(&agora)); //? o parametro é o endereço da estrutura tipo tm
    {
        Serial.print(".");
        delay(500);
    }
    Serial.print("\n\r Hora Sincronizada!");
/*
    Serial.printf("\n\rConectado com sucesso!\n\r");
    Serial.printf("Endereço IP: %s\n\r", WiFi.localIP().toString().c_str());
    Serial.printf("Endereço MAC: %s\n\r", WiFi.macAddress().c_str());

    ? Configuração de fuso e servidores NTP e inicia a sincronização
    configTzTime(FUSO, NTP1, NTP2);

    struct tm agora;
    Serial.print("\n\r Sincronizando hora\n\r")
    while(!getLocalTime(&agora)); //? o parametro é o endereço da estrutura tipo tm
    {
        Serial.print(".");
        delay(500);
    }
    Serial.print("\n\r Hora Sincronizada!");

    String clima;
    int temperatura;
    int sensacaoTermica;
    clima = pedirParaApiClima("%C");
    temperatura = pedirParaApiClima("%t").toInt();
    sensacaoTermica = pedirParaApiClima("%f").toInt();

    Serial.printf("Condição Climática para hoje: %s\n\r", clima.c_str());
    Serial.printf("Temperatura Atual %d C e sensação térmica de %d C", temperatura, sensacaoTermica);
*/

    
}
void loop()
{
    struct tm agora;

    String clima;
    int temperatura;
    int sensacaoTermica;
    clima = pedirParaApiClima("%C");
    temperatura = pedirParaApiClima("%t").toInt();
    sensacaoTermica = pedirParaApiClima("%f").toInt();
    lcd.setCursor(0,0);
    lcd.printf("Sao Caetano do Sul");
    if(getLocalTime(&agora))
    {
        lcd.setCursor(0,1);
        lcd.printf("Hoje e: %s, %02d:%02d", DIAS[agora.tm_wday], agora.tm_hour, agora.tm_min);
    }

    lcd.setCursor(0,2);
    lcd.printf("Cond. Climatica hoje"); //, clima.c_str()
    lcd.setCursor(0,3);
    lcd.printf("Temp. Atual: %d C  ", temperatura);
    delay(3600);
    lcd.setCursor(0,3);
    lcd.printf("sens. term. de %d C", sensacaoTermica);
    delay(3600);
   // lcd.printf("Temp. Atual %d C e sensacao term. de %d C", temperatura, sensacaoTermica); 


/*
    struct tm agora;
    if(getLocalTime(&agora))
    {
        Serial.printf("Hoje é: %d, %02d:%02d:%02d\n\r", DIAS[agora.tm_wday], agora.tm_hour, agora.tm_min, agora.tm_sec);
    }
    delay(1000);
  
    String clima;
    int temperatura;
    int sensacaoTermica;
    clima = pedirParaApiClima("%C");
    temperatura = pedirParaApiClima("%t").toInt();
    sensacaoTermica = pedirParaApiClima("%f").toInt();
    lcd.setCursor(0,0);
    lcd.printf("Endereço IP: %s", WiFi.localIP().toString().c_str());
    lcd.setCursor(0,1);
    lcd.printf("Endereço MAC: %s", WiFi.macAddress().c_str());
    lcd.setCursor(0,2);
    lcd.printf("Cond. Climática hoje: %s\n\r", clima.c_str());
    lcd.setCursor(0,3);
    lcd.printf("Temp. Atual %d C e sensação térm. de %d C", temperatura, sensacaoTermica);
  */
}

String pedirParaApiClima(String codigo)
{
    HTTPClient http;
    http.begin(enderecoAPI + codigo);

    int codigoHTTP = http.GET(); //? Retorna o código HTTP ex.: 200 --> OK ou 404 --> pagina não encontrada

    String resposta;

    if(codigoHTTP == 200)
    {
        resposta = http.getString(); //? Retorna o conteúdo HTML da página
    }

    else
    {
        Serial.printf("Deu erro. Código: %d", codigoHTTP);
    }
    http.end();
    return resposta;
}

