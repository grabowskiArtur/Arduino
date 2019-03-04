//Tools -> serialMonitor tam leci serial print 
//kable : 
//czerwony 5 V
//czarny masa
//zolty clock 
int lap_number = 0;
#include <Wire.h>


//dht22 ******************************************
#include <dht.h>
dht DHT22;
#define DHT22PIN 7

// BH1750 **************************************
#include <BH1750.h>
BH1750 lightMeter;
//function declarations

//YL-83 precipitation sensor  *****************
int sensor_A0 = A0;    // analog pin
int wartosc_A0;        // zmienna dla wartoĹ›ci A0
int sensor_D0 = 3; 
int wartosc_D0; 

//*******************************************
//***************SETUP***********************
//*******************************************
void setup() {
  Serial1.begin(9600);
  Serial.begin(115200);
  delay(2000);
  check_dht22_status();
  Serial.println("11111 ");
  
  bh1750_initialization();
  pinMode(sensor_D0, INPUT);    // for  precipitation sensor
}
//*******************************************
//***************SETUP END*******************
//*******************************************

void check_dht22_status()
{
  int check_status = DHT22.read(DHT22PIN);
  Serial.print("Stan sensora: ");
  switch (check_status)
  {
    case DHTLIB_OK: 
    Serial.print("OK\t"); 
    break;
    case DHTLIB_ERROR_CHECKSUM: 
    Serial.println("Błąd sumy kontrolnej"); 
    break;
    case DHTLIB_ERROR_TIMEOUT: 
    Serial.println("Koniec czasu oczekiwania - brak odpowiedzi"); 
    break;
    default: 
    Serial.println("Nieznany błąd"); 
    break;
  }
}

void bh1750_initialization(){
  //BH1750 light sensor*******************************************
  Serial.println("11112 ");
    Wire.begin();
    // On esp8266 you can select SCL and SDA pins using Wire.begin(D4, D3);
  Serial.println("11113 ");
  if (lightMeter.begin()) {
  Serial.println("BH1750 initialised");
  }
  else {
  Serial.println("Error initialising BH1750");
  }
  Serial.println("11114 ");
}

//*******************************************
//***************LOOP************************
//*******************************************

void loop() {

  lap_number++;
  Serial.println();
  Serial.print("lap_number  ");
  Serial.println(lap_number);

  //DHT22 data : 

  Serial.print("Wilgotnosc (%): ");              //wyświetlenie wartości wilgotności
  Serial.println((float)DHT22.humidity, 2);
  Serial.print("Temperatura (C): ");           //wyświetlenie temperatury
  Serial.println((float)DHT22.temperature, 2);  

  // BH 1750 data 
  uint16_t lux = lightMeter.readLightLevel();
  Serial.print("Light: ");
  Serial.print(lux);
  Serial.println(" lx");

  //Rain sensor data : 
  wartosc_A0 = analogRead(sensor_A0);      // pobranie wartosci z A0
  Serial.print("A0: ");
  Serial.println(wartosc_A0);
  wartosc_D0 = digitalRead(sensor_D0);     // pobranie wartosci z D0
  Serial.print("D0: ");                    // wyswietlenie na monitorze szeregowym
  Serial.println(wartosc_D0);

  delay(2000);

}
//*******************************************
//***************LOOP END********************
//*******************************************

