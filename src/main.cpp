#include <Arduino.h>
#include <esp_sleep.h> 

#define WAKEUP_PIN   3  // 唤醒引脚
#define STATUS_PIN   8  // 状态指示灯引脚

// 全局变量，记录下一次应该等待的唤醒条件
bool nextWakeupHigh = true;  // true=等待高电平, false=等待低电平

void handlePresence();
void handleAbsence();
void setupNextWakeup();

void setup() {
  Serial.begin(115200);
  
  pinMode(STATUS_PIN, OUTPUT);
  pinMode(WAKEUP_PIN, INPUT_PULLUP);
  
  digitalWrite(STATUS_PIN, HIGH); // LED亮起表示活跃状态
  
  // 显示当前唤醒原因和引脚状态
  esp_sleep_wakeup_cause_t wakeup_reason = esp_sleep_get_wakeup_cause();
  
  if(wakeup_reason == ESP_SLEEP_WAKEUP_GPIO) {
    Serial.println("🔔 GPIO唤醒事件");
  } else {
    Serial.println("🚀 首次启动或复位唤醒");
  }
  
  Serial.print("当前引脚状态: ");
  Serial.println(digitalRead(WAKEUP_PIN) == HIGH ? "HIGH" : "LOW");
  
  // 根据当前引脚状态执行相应任务
  if (digitalRead(WAKEUP_PIN) == HIGH) {
    Serial.println("🔴 检测到高电平状态");
    handlePresence();
  } else {
    Serial.println("🟢 检测到低电平状态"); 
    handleAbsence();
  }
  
  setupNextWakeup();
}

void loop() {
  // 不会执行
}

void handlePresence() {
  Serial.println("执行高电平状态任务...");
  delay(1000); 
  
  // 固定设置下一次为低电平唤醒
  Serial.println("📋 设置下次唤醒条件: 低电平");
  nextWakeupHigh = false;
}

void handleAbsence() {
  Serial.println("执行低电平状态任务...");
  delay(1000); 
  
  // 固定设置下一次为高电平唤醒
  Serial.println("📋 设置下次唤醒条件: 高电平");
  nextWakeupHigh = true;
}

void setupNextWakeup() {
  // 根据全局变量设置唤醒条件
  esp_sleep_enable_gpio_wakeup();
  
  if(nextWakeupHigh) {
    Serial.println("⏰ 等待高电平唤醒...");
    gpio_wakeup_enable((gpio_num_t)WAKEUP_PIN, GPIO_INTR_HIGH_LEVEL);
  } else {
    Serial.println("⏰ 等待低电平唤醒...");
    gpio_wakeup_enable((gpio_num_t)WAKEUP_PIN, GPIO_INTR_LOW_LEVEL);
  }
  
  Serial.println("进入深度睡眠...");
  Serial.flush();
  digitalWrite(STATUS_PIN, LOW); // LED熄灭
  esp_deep_sleep_start();
}