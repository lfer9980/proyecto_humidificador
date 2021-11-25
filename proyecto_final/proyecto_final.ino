/* librerias utilizadas */
#include <DHT.h>
#include <DHT_U.h>
#include <LiquidCrystal_I2C.h>
#include <Wire.h>

LiquidCrystal_I2C lcd(0x3f, 16, 2);


/* variables del sistema */
//DHT
#define DHTTYPE DHT22

/* variables para los sensores */
const int DHTPin = 4;  
const int ECO = 2;
const int TRIG = 3;
const int OUT = 8;
int DURATION;
int DISTANCE;
int i = 0;
float hum;
float temp;

//instanciando DHT
DHT dht(DHTPin, DHTTYPE);


void setup() {
	dht.begin();
	Serial.begin(9600);
	pinMode(TRIG, OUTPUT);
	pinMode(ECO, INPUT);
	pinMode(OUT, OUTPUT);


	lcd.init();
	lcd.backlight();
	lcd.clear();
	lcd.setCursor(0, 0);
	lcd.print("HOLA!");
	lcd.setCursor(0, 1);
	lcd.print("==> LISTO <");
}


void loop() {
	Serial.println("ciclo: ");
	Serial.println(i);


	while (!i) {
		/* PRIMER PULSO DEL ULTRASONICO */
		digitalWrite(TRIG, HIGH);
		delay(1);
		digitalWrite(TRIG, LOW);

		DURATION = pulseIn(ECO, HIGH);
		DISTANCE = DURATION / 58.2; 
		delay(200);
		
		Serial.println("Distancia: ");
		Serial.println(DISTANCE);
		delay(200);

		if (DISTANCE <= 40 && DISTANCE >= 2) {
			hum = dht.readHumidity();
			temp = dht.readTemperature();
			
			Serial.println("humedad: ");
			Serial.println(hum);
			Serial.println("temperatura: ");
			Serial.println(temp);

			if (isnan(hum) || isnan(temp)) {
				lcd.clear();
				lcd.setCursor(0, 0);
				lcd.print("Fallo DHT!");
				return;
			}

			lcd.clear();
			lcd.setCursor(0, 0);
			lcd.display();
			lcd.print("Encendiendo...");
			delay(200);
			lcd.noDisplay();
			delay(200);
			
			lcd.clear();
			lcd.display();
			lcd.print("encendido...");
			delay(200);

			lcd.clear();
			lcd.print("Hum: ");
			lcd.setCursor(6,0);
			lcd.print(hum);
			lcd.setCursor(0,1);
			lcd.print("temp: ");
			lcd.setCursor(6,1);
			lcd.print(temp);
			delay(100);
			digitalWrite(OUT, HIGH);
			Serial.println("ON");
			delay(10000);
	
			i++;
		}	
	} 

	while(i <= 3) {
		Serial.println("ciclo: ");
		Serial.println(i);
		hum = dht.readHumidity();
		temp = dht.readTemperature();
		
		Serial.println("humedad: ");
		Serial.println(hum);
		Serial.println("temperatura: ");
		Serial.println(temp);

		
		if (hum <= 40) {
			lcd.clear();
			lcd.setCursor(0, 0);
			lcd.display();
			lcd.print("Encendiendo...");
			delay(200);
			lcd.noDisplay();
			delay(200);

			lcd.clear();
			lcd.display();
			lcd.print("encendido...");
			delay(200);

			lcd.clear();
			lcd.print("Hum: ");
			lcd.setCursor(6,0);
			lcd.print(hum);
			lcd.setCursor(0,1);
			lcd.print("temp: ");
			lcd.print(temp);
			lcd.setCursor(6,1);

			digitalWrite(OUT, HIGH);
			Serial.println("ON");
			delay(5000);
		}
		delay(5000);
		i++;
	}

	digitalWrite(OUT, LOW);

	if (i == 4) {
		i = 0;
	}
}
