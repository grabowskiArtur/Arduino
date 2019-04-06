// link to libraries :
// https://drive.google.com/drive/folders/1x29EhA_QyLCDvwt1uYwJfcXbnrRn8K5H?usp=sharing

//Tools -> serialMonitor tam leci serial print
//wires :
//red 5 V
//orange 3,3 V
//black ground
//yellow clock
//green SDA
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

// JSON **************************************
#include <ArduinoJson.h>
StaticJsonDocument<200> doc;

//YL-83 precipitation sensor  *****************
int RainAnalogPin = 0;    // analog pin
int RainAnalogPinValue;
int RainDigitalPin = 3;    //digital pin
int RainDigitalPinValue;

// bmp180 // 3,3 V always! !!!!!!!!!!!!!!!!!!!!!!!!!!
//address 0x77
#include <SFE_BMP180.h>
SFE_BMP180 pressure;
char status;
double T, P;

//*******************************************
//***************SETUP***********************
//*******************************************
void setup() {
  Serial.begin(115200);
  delay(2000);
  //check_dht22_status();
  //Serial.println("11111 ");

  bh1750_initialization();
  pinMode(RainDigitalPin, INPUT);    // for  precipitation sensor

  bmp180_initialization();
}
//*******************************************
//***************SETUP END*******************
//*******************************************

void bmp180_initialization()
{
  pressure.begin();
  //  if (pressure.begin())
  //    Serial.println("BMP180 init success");
  //  else
  //  {
  //    Serial.println("BMP180 init fail\n\n");
  //    while (1);
  //  }
}


void check_dht22_status()
{
  int check_status = DHT22.read(DHT22PIN);
  Serial.print("Sensor status : ");
  switch (check_status)
  {
    case DHTLIB_OK:
      Serial.print("OK\t");
      break;
    case DHTLIB_ERROR_CHECKSUM:
      Serial.println("Checksum error");
      break;
    case DHTLIB_ERROR_TIMEOUT:
      Serial.println("End of waiting time - no answer");
      break;
    default:
      Serial.println("Unknown error");
      break;
  }
}

void bh1750_initialization() {
  //BH1750 light sensor*******************************************
  //Serial.println("11112 ");
  Wire.begin();
  // On esp8266 you can select SCL and SDA pins using Wire.begin(D4, D3);
  //Serial.println("11113 ");
  if (lightMeter.begin()) {
    //Serial.println("BH1750 initialised");
  }
  else {
    //Serial.println("Error initialising BH1750");
  }
  //Serial.println("11114 ");
}

int getPresseure()
{
  status = pressure.startTemperature();
  if (status != 0)
  {
    // Wait for the measurement to complete:
    delay(status);
    status = pressure.getTemperature(T);
    if (status != 0)
    {
      status = pressure.startPressure(3);
      if (status != 0)
      {
        delay(status);

        status = pressure.getPressure(P, T);
        if (status != 0)
        {
          return (P);
        }
      }
    }
  }


}

String rainFunction(int value)
{
  if (value > 900)
  {
    return "NO_RAIN";
  }
  else if (value < 900 && value > 600) {
    return "SMALL_RAIN";
  } else if (value < 600 && value > 400) {
    return "MEDIUM_RAIN";
  } else if (value < 400) {
    return "STRONG_RAIN";
  }
  return "ERROR";
}


void printInDebugMode()
{
  lap_number++;
  Serial.println();
  Serial.print("lap_number  ");
  Serial.println(lap_number);

  //DHT22 data :
  DHT22.read(DHT22PIN);
  Serial.print("Wilgotnosc (%): ");
  Serial.println((float)DHT22.humidity, 2);
  Serial.print("Temperatura (C): ");
  Serial.println((float)DHT22.temperature, 2);

  // BH 1750 data
  uint16_t lux = lightMeter.readLightLevel();
  Serial.print("Light: ");
  Serial.print(lux);
  Serial.println(" lx");

  //Rain sensor data :
  RainAnalogPinValue = analogRead(RainAnalogPin);
  Serial.print("A0: ");
  Serial.println(RainAnalogPinValue);
  RainDigitalPinValue = digitalRead(RainDigitalPin);
  Serial.print("D0: ");
  Serial.println(RainDigitalPinValue);

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

void printInJson()
{
  //DHT22 data :
  DHT22.read(DHT22PIN);
  doc["Humidity"] = DHT22.humidity;
  doc["Temperature"] = DHT22.temperature;

  // BH 1750 data
  //uint16_t lux = lightMeter.readLightLevel();
  int lux = lightMeter.readLightLevel();
  doc["Light"] = lux;
  //String lightString = (String)lux;
  //lightString += "lx";
  //doc["Light"] = lightString;

  //Rain sensor data :
  doc["Rain"] = rainFunction(analogRead(RainAnalogPin));

  doc["Pressure"] = getPresseure();

  //Serial.println();
  serializeJsonPretty(doc, Serial);
  Serial.println(",");
  delay(4000);
  //clear console command
  //  Serial.write(27);       // ESC command
  //  Serial.print("[2J");    // clear screen command
  //  Serial.write(27);
  //  Serial.print("[H");     // cursor to home command
}

void printInCSV()
{
  //DHT22 data :
  DHT22.read(DHT22PIN);
  Serial.print((int)DHT22.humidity);
  Serial.print(",");

  Serial.print((int)DHT22.temperature);
  Serial.print(",");

  // BH 1750 data
  Serial.print(lightMeter.readLightLevel());
  Serial.print(",");

  //Rain sensor data :
  Serial.print(rainFunction(analogRead(RainAnalogPin)));
  Serial.print(",");

  Serial.println(getPresseure());

  delay(4000);
}


//*******************************************
//***************LOOP************************
//*******************************************

void loop() {

  printInCSV();
  //printInJson();
  //printInDebugMode();
}
//*******************************************
//***************LOOP END********************
//*******************************************

