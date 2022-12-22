/* How to use the DHT-22 sensor with Arduino uno
   Temperature and humidity sensor
*/

//Libraries
#include <Fuzzy.h>
#include <Adafruit_Sensor.h>
#include <DHT.h>

//Constants
#define DHTPIN 7     // what pin we're connected to
#define DHTTYPE DHT22   // DHT 22  (AM2302)
DHT dht(DHTPIN, DHTTYPE); //// Initialize DHT sensor for normal 16mhz Arduino

Fuzzy *fuzzy = new Fuzzy();

//Variables
int chk;
float hum;  //Stores humidity value
float temp; //Stores temperature value

void setup()
{
  Serial.begin(115200);
  dht.begin();
  //suhu
  FuzzyInput *temperature = new FuzzyInput(1);
  FuzzySet *cold_temp = new FuzzySet(0, 15, 20, 25);
  temperature->addFuzzySet(cold_temp);
  FuzzySet *warm_temp = new FuzzySet(20, 23, 26, 30);
  temperature->addFuzzySet(warm_temp);
  FuzzySet *hot_temp = new FuzzySet(25, 30, 35, 40);
  temperature->addFuzzySet(hot_temp);
  //kelembapan
  FuzzyOutput *rotation = new FuzzyOutput(1);
  FuzzySet *slow = new FuzzySet(43, 64, 85, 106);
  rotation->addFuzzySet(slow);
  FuzzySet *medium = new FuzzySet(85, 106 ,170, 191);
  rotation->addFuzzySet(medium);
  FuzzySet *fast = new FuzzySet(170, 190, 210, 255);
  rotation->addFuzzySet(fast);
  //rules1
  FuzzyRuleAntecedent *iftemperaturecold = new FuzzyRuleAntecedent();
  iftemperaturecold->joinSingle(cold_temp);
  FuzzyRuleConsequent *thenrotationslow = new FuzzyRuleConsequent();
  thenrotationslow->addOutput(slow);
  FuzzyRule *fuzzyRule01 = new FuzzyRule(1, iftemperaturecold, thenrotationslow);
  fuzzy->addFuzzyRule(fuzzyRule01);
  //rules2
  FuzzyRuleAntecedent *iftemperaturewarm = new FuzzyRuleAntecedent();
  iftemperaturewarm->joinSingle(warm_temp);
  FuzzyRuleConsequent *thenrotationmedium = new FuzzyRuleConsequent();
  thenrotationmedium->addOutput(medium);
  FuzzyRule *fuzzyRule02 = new FuzzyRule(1, iftemperaturewarm, thenrotationmedium);
  fuzzy->addFuzzyRule(fuzzyRule02);
  //rules3
  FuzzyRuleAntecedent *iftemperaturehot = new FuzzyRuleAntecedent();
  iftemperaturehot->joinSingle(hot_temp);
  FuzzyRuleConsequent *thenrotationfast = new FuzzyRuleConsequent();
  thenrotationfast->addOutput(fast);
  FuzzyRule *fuzzyRule03 = new FuzzyRule(1, iftemperaturehot, thenrotationfast);
  fuzzy->addFuzzyRule(fuzzyRule03);
}

void loop()
{
    delay(2000);
    //Read data and store it to variables hum and temp
    hum = dht.readHumidity();
    temp= dht.readTemperature();
    //Print temp and humidity values to serial monitor
    Serial.print("Humidity: ");
    Serial.print(hum);
    Serial.print(" %, Temp: ");
    Serial.print(temp);
    Serial.println(" Celsius");
    delay(10000); //Delay 2 sec.
    fuzzy->setInput(1, temp);
    fuzzy->fuzzify();
    float output = fuzzy->defuzzify(1);
    Serial.print(output);
}

   