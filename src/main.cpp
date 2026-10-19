#include <Arduino.h>
#include <WiFi.h>
#include <HTTPClient.h>
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27, 20, 4);
String pedirParaApiClima(String);

const char* nomeRedeWiFi = "SENAI EDUCACIONAL";
const char* senhaRedeWiFi = "info@134";
const String enderecoAPI = "https://wttr.in/Sao+Caetano+do+Sul?m&lang=pt&format=";

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
/*
    Serial.printf("\n\rConectado com sucesso!\n\r");
    Serial.printf("Endereço IP: %s\n\r", WiFi.localIP().toString().c_str());
    Serial.printf("Endereço MAC: %s\n\r", WiFi.macAddress().c_str());

    String clima;
    int temperatura;
    int sensacaoTermica;
    clima = pedirParaApiClima("%C");
    temperatura = pedirParaApiClima("%t").toInt();
    sensacaoTermica = pedirParaApiClima("%f").toInt();

    Serial.printf("Condição Climática para hoje: %s\n\r", clima.c_str());
    Serial.printf("Temperatura Atual %d C e sensação térmica de %d C", temperatura, sensacaoTermica);
*/
    String clima;
    int temperatura;
    int sensacaoTermica;
    clima = pedirParaApiClima("%C");
    temperatura = pedirParaApiClima("%t").toInt();
    sensacaoTermica = pedirParaApiClima("%f").toInt();
    lcd.setCursor(0,0);
    lcd.printf("Endereco IP: %s", WiFi.localIP().toString().c_str());
    lcd.setCursor(0,1);
    lcd.printf("Endereco MAC: %s", WiFi.macAddress().c_str());
    lcd.setCursor(0,2);
    lcd.printf("Cond. Climatica hoje: %s\n\r", clima.c_str());
    lcd.setCursor(0,3);
    lcd.printf("Temp. Atual %d C e sensacao term. de %d C", temperatura, sensacaoTermica);
}
void loop()
{
  /*
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

