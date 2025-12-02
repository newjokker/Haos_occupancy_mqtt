#include <Arduino.h>
#include <esp_sleep.h>
#include <driver/rtc_io.h>

#define WAKE_PIN GPIO_NUM_2   // 推荐 GPIO2 做人体感应输入
#define LED_PIN  GPIO_NUM_8   // SuperMini 的板载LED通常在 GPIO8（高亮、低灭）

void setup() {
  Serial.begin(115200);
  delay(200);

  pinMode(WAKE_PIN, INPUT);
  pinMode(LED_PIN, OUTPUT);

  // 显示唤醒原因
  esp_sleep_wakeup_cause_t cause = esp_sleep_get_wakeup_cause();
  Serial.printf("Wakeup reason: %d\n", cause);

  int level = digitalRead(WAKE_PIN);
  Serial.printf("Current pin level: %d\n", level);

  if(level == HIGH) {
    digitalWrite(LED_PIN, LOW);   // 低亮灯
    Serial.println("Detected HIGH (有人)");
  } else {
    digitalWrite(LED_PIN, HIGH);  // 高灭灯
    Serial.println("Detected LOW (无人)");
  }

  delay(1000);

  Serial.println("Config wakeup on HIGH...");
  
  // 对于 ESP32-C3，使用 gpio_wakeup_enable 和 esp_sleep_enable_gpio_wakeup
  esp_err_t result = gpio_wakeup_enable(WAKE_PIN, GPIO_INTR_HIGH_LEVEL);
  if (result != ESP_OK) {
    Serial.printf("Error enabling gpio wakeup: %d\n", result);
  }
  
  result = esp_sleep_enable_gpio_wakeup();
  if (result != ESP_OK) {
    Serial.printf("Error enabling sleep wakeup: %d\n", result);
  }

  Serial.println("Going to deep sleep now...");
  Serial.flush();
  delay(50);
  esp_deep_sleep_start();
}

void loop() {}