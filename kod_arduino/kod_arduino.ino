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
BH1750 lightMeter(0x23);
// address 0x23

//YL-83 precipitation sensor  *****************
int sensor_A0 = A0;    // analog pin
int wartosc_A0;        // zmienna dla wartoĹ›ci A0
int sensor_D0 = 3;
int wartosc_D0;

// bmp180 //ZAWSZE DO 3,3 V !!!!!!!!!!!!!!!!!!!!!!!!!!
//address 0x77
#include <SFE_BMP180.h>
SFE_BMP180 pressure;
char status;
double T, P;

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

  bmp180_initialization();

}
//*******************************************
//***************SETUP END*******************
//*******************************************

void bmp180_initialization()
{
  if (pressure.begin())
    Serial.println("BMP180 init success");
  else
  {
    Serial.println("BMP180 init fail\n\n");
    while (1);
  }
}


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

void bh1750_initialization() {
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
  DHT22.read(DHT22PIN);
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

  //bmp180
  // Print out the measurement:
  status = pressure.startTemperature();
  if (status != 0)
  {
    // Wait for the measurement to complete:
    delay(status);
    status = pressure.getTemperature(T);
    if (status != 0)
    {
      Serial.print("temperature in BMP180: ");
      Serial.print(T, 2);
      Serial.println(" deg C, ");

      status = pressure.startPressure(3);
      if (status != 0)
      {
        delay(status);

        status = pressure.getPressure(P, T);
        if (status != 0)
        {
          // Print out the measurement:
          Serial.print("absolute pressure: ");
          Serial.print(P, 2);
          Serial.println(" mb, ");
        } else
        {
          Serial.println("error retrieving pressure measurement\n");
        }
      } else
      {
        Serial.println("error starting pressure measurement\n");
      }
    } else
    {
      Serial.println("error retrieving temperature measurement\n");
    }
  }
  else
  {
    Serial.println("error starting temperature measurement\n");
  }
  delay(4000);

}
//*******************************************
//***************LOOP END********************
//*******************************************

