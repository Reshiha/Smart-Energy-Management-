#include <Wire.h>
#include <InfluxDbClient.h>
#include <InfluxDbCloud.h>

// Define Wi-Fi credentials and InfluxDB connection details
#define WIFI_SSID "s3"
#define WIFI_PASSWORD "12345678"
#define INFLUXDB_URL "https://us-east-1-1.aws.cloud2.influxdata.com"
#define INFLUXDB_TOKEN "4RrO5LOy0QCOg_eW_R8iiCK-wdNa-0Nxbk8v9yyGN8Zk-o4wCAcT6oqq__eTTdVpJSe61Eeshc0goe2NEAtCjw=="
#define INFLUXDB_ORG "Amrita"
#define INFLUXDB_BUCKET "pzem"
#define INFLUXDB_MEASUREMENT "esp32_1_sensors"
#define TZ_INFO "IST-5:30"

const char* ssid = "s3";
const char* password = "12345678";
#define DEVICE "ESP32_1"

const int voltage = 220;
const int weeks = 4;
const int days_per_week = 3;
const int watts_per_system = 30;
const float amperage_per_system = (float)watts_per_system / voltage;


const int day_duration_ms = 30000; // Duration of each day in milliseconds
const int print_interval_ms = 2000; // Interval for printing data

InfluxDBClient client(INFLUXDB_URL, INFLUXDB_ORG, INFLUXDB_BUCKET, INFLUXDB_TOKEN, InfluxDbCloud2CACert);
Point sensor(INFLUXDB_MEASUREMENT);


void setup() {
  Serial.begin(115200);

  // Setup Wi-Fi
  WiFi.mode(WIFI_STA);
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("Connecting to WiFi");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(100);
  }
  Serial.println("WiFi connected");

  // Add tags
  // Assuming you defined DEVICE earlier or replace with appropriate value
  sensor.addTag("device", DEVICE);
}

void loop() {
  for (int week = 1; week <= weeks; ++week) {
    for (int day = 1; day <= days_per_week; ++day) {
      int systems_used;
      float amperage_per_system = (float)watts_per_system / voltage;
      String air_quality;
      int co2_level;

        switch ((week - 1) * days_per_week + day) {
          case 1: // Week 1, Day 1
            systems_used = 72;
            co2_level = random(1001, 1200); // Poor air quality range
            air_quality = "Poor Air Quality";
            break;
          case 2: // Week 1, Day 2
            systems_used = 60;
            co2_level = random(701, 1000); // Medium air quality range
            air_quality = "Medium Air Quality";
            break;
          case 3: // Week 1, Day 3
            systems_used = 15;
            co2_level = random(400, 700); // Good air quality range
            air_quality = "Good Air Quality";
            break;
          case 4: // Week 2, Day 1
            systems_used = 0;
            co2_level = random(400, 700); // Good air quality range
            air_quality = "Good Air Quality";
            break;
          case 5: // Week 2, Day 2
            systems_used = 70;
            co2_level = random(1001, 1200); // Poor air quality range
            air_quality = "Poor Air Quality";
            break;
          case 6: // Week 2, Day 3
            systems_used = 15;
            co2_level = random(400, 700); // Good air quality range
            air_quality = "Good Air Quality";
            break;
          case 7: // Week 3, Day 1
            systems_used = 25;
            co2_level = random(1001, 1200); // Poor air quality range
            air_quality = "Poor Air Quality";
            break;
          case 8: // Week 3, Day 2
            systems_used = 0;
            co2_level = random(400, 700); // Good air quality range
            air_quality = "Good Air Quality";
            break;
          case 9: // Week 3, Day 3
            systems_used = 30;
            co2_level = random(701, 1000); // Medium air quality range
            air_quality = "Medium Air Quality";
            break;
          case 10: // Week 4, Day 1
            systems_used = 40;
            co2_level = random(701, 1000); // Medium air quality range
            air_quality = "Medium Air Quality";
            break;
          case 11: // Week 4, Day 2
            systems_used = 50;
            co2_level = random(701, 1000); // Medium air quality range
            air_quality = "Medium Air Quality";
            break;
          case 12: // Week 4, Day 3
            systems_used = 20;
            co2_level = random(400, 700); // Good air quality range
            air_quality = "Good Air Quality";
            break;
          default:
            systems_used = 0;
            co2_level = 0;
            air_quality = "Unknown";
            break;
        }

        float total_power_consumed = systems_used * watts_per_system;

        unsigned long start_time = millis();

        while (millis() - start_time < day_duration_ms) {
          // Print values to serial monitor
          Serial.println("---- Data for Day " + String(day) + " of Week " + String(week) + " ----");
          Serial.print("Systems used: ");
          Serial.println(systems_used);
          Serial.print("Amperage per system: ");
          Serial.println(amperage_per_system);
          Serial.print("Air quality: ");
          Serial.println(air_quality);
          Serial.print("CO2 level: ");
          Serial.println(co2_level);
          Serial.print("Total power consumed: ");
          Serial.println(total_power_consumed);
          Serial.println("---------------------------------------");

          // Prepare data point
          Point sensor("lab_data");
          sensor.addField("systems_used", systems_used);
          sensor.addField("total_power_consumed", total_power_consumed);
          sensor.addField("co2_level", co2_level);
          sensor.addField("air_quality", air_quality);
          sensor.addField("amperage_per_system", amperage_per_system);

          // Write data to InfluxDB
          if (!client.writePoint(sensor)) {
            Serial.print("InfluxDB write failed: ");
            Serial.println(client.getLastErrorMessage());
          } else {
            Serial.println("Data successfully written to InfluxDB");
          }

          delay(print_interval_ms); // Delay between each data print
        }
      }
    }
  delay(3600000);
  }


