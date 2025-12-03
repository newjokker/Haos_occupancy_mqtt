#include <esp_sleep.h>
#include <driver/gpio.h>
#include <Arduino.h>

// 定义唤醒引脚（ESP32-S3的RTC GPIO）
#define WAKEUP_PIN 3

// 使用RTC内存保存唤醒次数和状态
RTC_DATA_ATTR int bootCount = 0;
RTC_DATA_ATTR int lastStatus = 0; // 0表示低电平唤醒，1表示高电平唤醒

// 打印唤醒原因函数
void print_wakeup_reason() {
  esp_sleep_wakeup_cause_t wakeup_reason = esp_sleep_get_wakeup_cause();
  
  Serial.print("唤醒原因: ");
  switch(wakeup_reason) {
    case ESP_SLEEP_WAKEUP_EXT0: 
      Serial.println("外部信号 (EXT0)"); break;
    case ESP_SLEEP_WAKEUP_EXT1: 
      Serial.println("外部信号 (EXT1)"); break;
    case ESP_SLEEP_WAKEUP_TIMER: 
      Serial.println("定时器"); break;
    case ESP_SLEEP_WAKEUP_TOUCHPAD: 
      Serial.println("触摸板"); break;
    case ESP_SLEEP_WAKEUP_ULP: 
      Serial.println("ULP程序"); break;
    case ESP_SLEEP_WAKEUP_GPIO: 
      Serial.println("GPIO引脚"); break;
    case ESP_SLEEP_WAKEUP_UART: 
      Serial.println("UART活动"); break;
    default: 
      Serial.printf("重启或其他原因 (%d)\n", wakeup_reason); break;
  }
}

void setup() {
  Serial.begin(115200);
  delay(1000); // 等待串口初始化
  
  bootCount++; // 增加启动计数
  Serial.println();
  Serial.print("启动次数: ");
  Serial.println(bootCount);
  
  // 打印本次唤醒原因
  print_wakeup_reason();
  
  // 首次启动初始化状态
  if (bootCount == 1) {
    lastStatus = 0; // 初始设置为低电平唤醒
    Serial.println("首次启动，初始化状态为低电平唤醒");
  }
  
  if (lastStatus == 0) {
    // 上次是低电平唤醒，这次配置为高电平唤醒
    pinMode(WAKEUP_PIN, INPUT_PULLDOWN); // 下拉确保默认低电平
    lastStatus = 1;
    esp_sleep_enable_ext0_wakeup(static_cast<gpio_num_t>(WAKEUP_PIN), 1);
    Serial.println("配置为高电平唤醒");
    Serial.println("将GPIO" + String(WAKEUP_PIN) + "拉高进行唤醒");
  } else {
    // 上次是高电平唤醒，这次配置为低电平唤醒
    pinMode(WAKEUP_PIN, INPUT_PULLUP); // 上拉确保默认高电平
    lastStatus = 0;
    esp_sleep_enable_ext0_wakeup(static_cast<gpio_num_t>(WAKEUP_PIN), 0);
    Serial.println("配置为低电平唤醒");
    Serial.println("将GPIO" + String(WAKEUP_PIN) + "拉低进行唤醒");
  }
  
  Serial.println("配置完成，即将进入深度睡眠...");
  
  // 确保所有串口数据发送完成
  Serial.flush();
  delay(100);
  
  // 进入深度睡眠
  esp_deep_sleep_start();
}

void loop() {
  // 此处的代码永远不会执行
}