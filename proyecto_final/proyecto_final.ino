/* librerias utilizadas */
#include <DHT.h>
#include <DHT_U.h>
#include <LiquidCrystal_I2C.h>
#include <Wire.h>


//instanciando LCD
LiquidCrystal_I2C lcd(0x3f, 16, 2);

//instanciando DHT
DHT dht(DHTPin, DHTTYPE);


/* variables del sistema */
//DHT
#define DHTTYPE DHT22


/* variables para los sensores */
const int DHTPin = 4;  

/* variables para el sensor ultrasonico */
const int ECO = 2;
const int TRIG = 3;
int DURATION;
int DISTANCE;

/* pin de salida al humidificador  */
const int OUT = 8;

/* variables del sensor dht11 */
float hum;
float temp;

//VARIABLE ITERADORA
int i = 0;


void setup() {
	dht.begin();
	Serial.begin(9600);

	/* definiendo los pines del ultrasonico y salida */
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
	/* imprime en que ciclo estoy */
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

		/* evaluo si hay alguien entre 40 y 2 */
		if (DISTANCE <= 40 && DISTANCE >= 2) {
			hum = dht.readHumidity();
			temp = dht.readTemperature();
			
			Serial.println("humedad: ");
			Serial.println(hum);
			Serial.println("temperatura: ");
			Serial.println(temp);

			/* if de proteccion por si no lee bien el dht */
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
			/* encendido del humidificador */
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

		/* la temperatura es mayor a 40? */
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

	/* apago el humidificador */
	digitalWrite(OUT, LOW);

	/* validacion para volver a comenzar */
	if (i == 4) {
		i = 0;
	}
}
