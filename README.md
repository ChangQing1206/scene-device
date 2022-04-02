# ToOneNetByMqtt

**简介：** ESP8266通过MQTT协议将数据传输至OnenNet云平台


#### 3. 硬件环境：

1. stm32f103 c8t6
2. GPS模块
3. rfid
4. DS18B20
5. ESP8266-01S无线模块

#### 4. 云平台环境配置：

1. **云平台配置：**
- OneNET控制台—全部产品服务（多协议接入，选MQTT旧版）—添加产品—进入产品（记住产品ID）—设备列表—添加设备（记住鉴权信息）—创建完成（记住设备ID）
  
2. **云平台应用设置：**
- 添加应用—编辑应用—组件库中添加折线图和开关
  
- 折线图（数据上传成功后进行配置）: 选择数据流 — 选择设备—数据流选择要显示的数据（这里选择温度，temperature）
   - 开关（数据上传成功后进行配置）: 选择数据流 — 选择设备—数据流选择要显示的数据（这里选择温度，ledFlag）—开关开值（LEDON）—开关关值（LEDOFF）——EDP不填

#### 5. 接线：

1. ESP8266-01S（5根线）

   - PA2     RX
   - PA3     TX
   - PB1    复位
   - 3V3     VCC
   - GND   GND

3. LED（最小系统自带）
   - PC13    LED1

4.一些常用操作
接线：
I.ch340 <----> stm32    (串口1通信 用于调试显示)
PA9 <----> RXD
PA10 <----> TXD


III.电脑要连热点

IV.stm32  <---->  GPS     (串口3通信)
GND  <---->  GND
3.3V <---->  VCC
PB10 <---->  RXD
PB11 <---->  TXD
有源天线接上放在室外闲置几分钟

#### 6. 功能展示：

1. **数据流**![image-20201223113457360](./images/data.png)

2. **APP应用管理**

   ![image-20201223113558021](./images/APP.png)

3. **串口数据**

   ![image-20201223113700277](./images/serial.png)

4. **源码简要解析**

   ![image-20201223113912384](./images/code.png)

   - stm32f10x_it.c：中断处理函数
   - usart1.c：与上位机通信
   - usart2.c：与ESP8266通信，串口2接收中断处理上位机发送来的数据
   - timer2.c：定时器2中断用来发送GPS数据，体温，30S
   - timer3.c：定时器3中断用来发送心跳包（ping，用于保持和服务器连接，长时间没给服务器发送数据会被踢下线），2s和30s两种模式
   - timer4.c：将串口2接收到的服务器数据依次存放在MQTT接收缓存数组中，50ms
   - control.c：控制LED灯，并且发送LED灯开关信号

#### 7. 要修改的的地方：

1. mqtt.h ：

   ```c
   #define  PRODUCTID      "412380"          //自己的产品ID
   #define  DEVICEID       "702197187"       //自己的设备ID 
   #define  AUTHENTICATION  "SHT30"         //自己的鉴权信息（创建设备的时候设置）
   ```
   
   ![./image-20201223112140673](./images/__MQTT_H.png)
   
2. wifi.h

   ```c
    #define SSID   "Redmi K30 Ultra"                     //路由器SSID名称
    #define PASS   "stm32f407"               //路由器密码
   ```

   ![__WIFI_H](./images/__WIFI_H.png)

3. mqtt.c

   ```c
   sprintf(ServerIP,"%s","183.230.40.39");              //构建服务器域名（ONENET的）
   ServerPort = 6002;                                   //服务器端口号6002
   ```
  sprintf(ServerIP,"%s","47.106.235.154/");              //构建服务器域名（自己的）
   ServerPort = 1883;                                   //服务器端口号1883


   根据平台地址列表填写，这里是mqtt协议

   ![image-20201223113142935](./images/address.png)

   ![image-20201223112902671](./images/__MQTT_C.png)

   

