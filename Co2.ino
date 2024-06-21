#define MQ_PIN A0 // Define analog input pin for MQ135 sensor
#define RL_VALUE 10 // Define the load resistance on the board, in kilo ohms
#define RO_CLEAN_AIR_FACTOR 9.83 // Define the Clean Air Ratio constant

// Define thresholds for air quality levels
#define GOOD_THRESHOLD 100 // Customize these thresholds as needed
#define AVERAGE_THRESHOLD 300
#define POOR_THRESHOLD 500

float MQResistanceCalculation(int raw_adc) {
  return ((float)RL_VALUE * (1023 - raw_adc) / raw_adc);
}

float MQCalibration(int mq_pin) {
  int i;
  float val = 0;

  for (i = 0; i < 100; i++) { //take multiple samples
    val += MQResistanceCalculation(analogRead(mq_pin));
    delay(100);
  }
  val = val / 100; //average them out
  val = val / RO_CLEAN_AIR_FACTOR; //divided by RO_CLEAN_AIR_FACTOR yields clean air's value
  return val;
}

float MQRead(int mq_pin) {
  return MQResistanceCalculation(analogRead(mq_pin)) / MQCalibration(mq_pin);
}

void setup() {
  Serial.begin(115200);
}

void loop() {
  float airQuality = MQRead(MQ_PIN);

  Serial.print("Air Quality: ");
  Serial.println(airQuality);

  // Categorize air quality
  if (airQuality < GOOD_THRESHOLD) {
    Serial.println("Air Quality: Good");
  } else if (airQuality < AVERAGE_THRESHOLD) {
    Serial.println("Air Quality: Average");
  } else {
    Serial.println("Air Quality: Poor");
  }

  delay(2000);
}