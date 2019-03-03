//Tools -> serialMonitor tam leci serial print 
//kable : 
//czerwony 5 V
//czarny masa
//zolty clock 
int lap_number = 0;
#include <Wire.h>


// idth11 **************************************
#include <idDHT11.h>
int idDHT11pin = 2; //Pin polaczenia z DHT11
int idDHT11intNumber = 1; //Numer przerwania dla DHT11 potrzebny do konstruktora

// Lib instantiate
void dht11_wrapper(); // must be declared before the lib initialization
idDHT11 DHT11(idDHT11pin,idDHT11intNumber,dht11_wrapper);

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
  Serial.begin(9600);
  delay(2000);
  
  initial_prints();
  Serial.println("11111 ");
  dht11_initialization(); //uwaga to musi byc inicjalozowane przed bh1750!! nie wiedziec czemu w odwrotnej kolejnosci dht11 nie moze przejsc linijki   while (DHT11.acquiring())
    ;
  bh1750_initialization();
  pinMode(sensor_D0, INPUT);    // for  precipitation sensor
}

void initial_prints()
{
    //dht11 ********************************************************
    Serial.println("idDHT11 ");
    Serial.print("LIB version: ");
    Serial.println(IDDHT11LIB_VERSION);
    Serial.println("---------------");  
    
}

void dht11_wrapper() {
  DHT11.isrCallback();
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
void dht11_initialization()
{
  DHT11.acquire();
  while (DHT11.acquiring())    ;
  int result = DHT11.getStatus();

  switch (result)
  {
    case IDDHTLIB_OK:
      Serial.println("OK");
      break;
    case IDDHTLIB_ERROR_CHECKSUM:
      Serial.println("Error\n\r\tChecksum error");
      break;
    case IDDHTLIB_ERROR_ISR_TIMEOUT:
      Serial.println("Error\n\r\tISR Time out error");
      break;
    case IDDHTLIB_ERROR_RESPONSE_TIMEOUT:
      Serial.println("Error\n\r\tResponse time out error");
      break;
    case IDDHTLIB_ERROR_DATA_TIMEOUT:
      Serial.println("Error\n\r\tData time out error");
      break;
    case IDDHTLIB_ERROR_ACQUIRING:
      Serial.println("Error\n\r\tAcquiring");
      break;
    case IDDHTLIB_ERROR_DELTA:
      Serial.println("Error\n\r\tDelta time to small");
      break;
    case IDDHTLIB_ERROR_NOTSTARTED:
      Serial.println("Error\n\r\tNot started");
      break;
    default:
      Serial.println("Unknown error");
      break;
  }

}

//*******************************************
//***************LOOP************************
//*******************************************

void loop() {

  lap_number++;
  Serial.println();
  Serial.print("lap_number  ");
  Serial.println(lap_number);
  Serial.print("Humidity (%): ");
  Serial.println(DHT11.getHumidity(), 2);

  Serial.print("Temperature (oC): ");
  Serial.println(DHT11.getCelsius(), 2);

  uint16_t lux = lightMeter.readLightLevel();
  Serial.print("Light: ");
  Serial.print(lux);
  Serial.println(" lx");

  wartosc_A0 = analogRead(sensor_A0);      // pobranie wartoĹ›ci z A0
  Serial.print("A0: ");
  Serial.println(wartosc_A0);
  wartosc_D0 = digitalRead(sensor_D0);     // pobranie wartoĹ›ci z D0
  Serial.print("D0: ");                    // wyĹ›wietlenie na monitorze szeregowym
  Serial.println(wartosc_D0);


  delay(2000);

}
