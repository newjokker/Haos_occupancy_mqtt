# 说明

### 环境

```bash

platformio lib install "Adafruit GFX Library"

platformio lib install "Adafruit SSD1306"

platformio lib install "DHT sensor library"

platformio lib install "PubSubClient"

pio lib install "Adafruit NeoPixel"

pio pkg install --library 1076

pio pkg install --library "knolleary/PubSubClient"

pio lib install "Stepper"

pio lib install "AccelStepper"

```

### 调试代码

* 串口调试
```bash

tio -b 115200 --timestamp  /dev/cu.wchusbserial5A7B1617701 

```

* python 代码调试

```bash

python3 ./tools/read_csv.py /dev/cu.wchusbserial5A7B1617701 

```

### 信息流程图

* esp32-c3(A) 处于深度睡眠之中
* 当毫米波雷达(B) 检测到有人对 A 的 gpio8 引脚输入高电平，A激活，连接 wifi 发送信息给 mqtt，然后浅睡眠
* 




### 需求

* 测试使用一个引脚高低电平将 esp32-c3 唤醒，具体来说就是，先是高电平唤醒，显示有人，然后是低电平唤醒，显示人已经走了
    * gpio 8 显示板子是不是在工作
    * 电平的信息使用示波器进行查看


### 已经解决

* 当前使用两个板子同时通电的时候两块板子互相影响，不知道是哪里出现了问题 （应该是使用了同一个 client_id 导致的）

* 用于初始化的 boot 按键 注意 esp32S3 是 0 ， esp32c3 是 9

* 获取的 mac 地址可能是重复的，获取 WiFi 的 mac 地址，是唯一的
