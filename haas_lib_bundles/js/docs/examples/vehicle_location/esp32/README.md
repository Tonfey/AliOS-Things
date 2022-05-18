# 车辆定位系统
&emsp;&emsp;
下图是本案例除硬件连线外的3步导学，每个步骤中实现的功能请参考图中的说明。在硬件连线完成之后我们建议您先使用“一分钟上云体验”功能预先体验本案例的实际运行效果。
<div align="center">
<img src=./../../../images/2_车辆定位系统_步骤概述.jpg width=60%/>
</div>

## 简介
&emsp;&emsp;
近年来，全球掀起了一股自动驾驶热潮，无论是传统车企，还是科技企业，甚至很多初创公司都竞相加入这一行业赛道。进入2021年，自动驾驶热度不减，且“吸金”不断，据不完全统计，从今年年初至今，自动驾驶行业投融资事件超过50起，投融资金额近1000亿元，已超过2018年，达到历年最高值。

### 背景知识

#### 卫星定位系统
&emsp;&emsp;
定位系统是以确定空间位置为目标而构成的相互关联的一个集合体或装置（部件）。这个系统可以保证在任意时刻，地球上任意一点都可以同时观测到至少4颗卫星，以保证卫星可以采集到该观测点的经纬度和高度，以便实现导航、定位、授时等功能。这项技术可以用来引导飞机、船舶、车辆以及个人，安全、准确地沿着选定的路线，准时到达目的地。
&emsp;&emsp;
目前主流定位系有除美国的GPS，中国的北斗卫星导航系统、欧盟的伽利略卫星导航系统、俄罗斯全球导航卫星系统等。

###### GPS

&emsp;&emsp;
GPS是美国第二代卫星导航系统。按目前的方案，GPS的空间部分使用24颗高度约2.02万千米的卫星组成卫星星座。24颗卫星均为近圆形轨道，运行周期约为11小时58分，分布在6个轨道面上（每轨道面4颗），轨道倾角为55度。卫星的分布使得在全球任何地方、任何时间都可观测到4颗以上的卫星，并能保持良好定位解算精度的几何图形。这就提供了在时间上连续的全球导航能力。

&emsp;&emsp;
想了解GPS详细信息请参考[文章]((https://baike.baidu.com/item/%E5%85%A8%E7%90%83%E5%AE%9A%E4%BD%8D%E7%B3%BB%E7%BB%9F/1240960?fromtitle=GPS&fromid=214654&fr=aladdin))。

##### 北斗卫星导航系统

&emsp;&emsp;
中国北斗卫星导航系统（英文名称：BeiDou Navigation Satellite System，简称BDS）是中国自行研制的全球卫星导航系统，也是继GPS、GLONASS之后的第三个成熟的卫星导航系统。北斗卫星导航系统（BDS）和美国GPS、俄罗斯GLONASS、欧盟GALILEO，是联合国卫星导航委员会已认定的供应商。

&emsp;&emsp;
北斗卫星导航系统由空间段、地面段和用户段三部分组成，可在全球范围内全天候、全天时为各类用户提供高精度、高可靠定位、导航、授时服务，并且具备短报文通信能力，已经初步具备区域导航、定位和授时能力，定位精度为分米、厘米级别，测速精度0.2米/秒，授时精度10纳秒。

&emsp;&emsp;
全球范围内已经有137个国家与北斗卫星导航系统签下了合作协议。随着全球组网的成功，北斗卫星导航系统未来的国际应用空间将会不断扩展。

&emsp;&emsp;
想了解北斗卫星导航系统详细信息请参考[文章]((https://baike.baidu.com/item/%E5%8C%97%E6%96%97%E5%8D%AB%E6%98%9F%E5%AF%BC%E8%88%AA%E7%B3%BB%E7%BB%9F/10390403?fr=aladdin))

#### 基站定位

&emsp;&emsp;
除了像GPS这种卫星定位之外，我们身边的设备如：智能手机、智能手表、穿戴设备等能联网的设备都可以用来定位，通过连接运营商(电信、联通、移动)基站来定位的方式称之为基站定位，或者又称之为移动位置服务(LBS--Location Based Service)。

## 准备

&emsp;&emsp;
需要准备如下硬件：

* ESP32开发板  1套
* 杜邦线若干
* GNSS定位模块 HT2828Z3G5L

&emsp;&emsp;
其中，HT2828Z3G5L与ESP32的连接如下：

| 定位模块 |                                                   |    ESP32     |          |          |
| -------- | ------------------------------------------------- | :----------: | -------- | -------- |
| 引脚名   | 引脚功能                                          |  连接到引脚  | NODEMCU32S引脚序号 | M5STACKCORE2引脚序号 |
| VBAT/PPS | 备用电池 / 时间标准脉冲输出                       | NC（不连接） | ——       | ——       |
| VCC      | 模块供电，可选3.3V至5V                            |     3.3V     | 2        | 3.3V        |
| TX       | 串口发送端                                        |     RXD2     | GPIO16   | G13   |
| RX       | 串口接收端                                        |     TXD2     | GPIO17   | G14   |
| GND      | 接地                                              |     GND      | 30       | GND   |
| EN       | 模块使能，高电平/悬空使能模组工作，低电平模组关闭 | NC（不连接） | ——       |

注意：以上引脚顺序是NODEMCU32S硬件。M5StackCore2硬件上的TX/RX分别为G13/G14。

如下图实物图所示：
<div align="center">
<img src=./../../../images/5_3_车辆定位硬件连接_esp32.jpg width=90%/>
</div>

<br>

## 一分钟上云体验

- 打开“支付宝”扫描下图二维码

<div align="center">
<img src=../../../images/案例-GNSS定位系统（JavaScript）.png width=40%/>
</div>

- 在完成上面的“硬件连线”步骤之后，点击“体验案例”按钮，即可立即体验本案例的实际运行效果。
<div align="center">
<img src=../../../images/1_一分钟上云_step2.png width=80%/>
</div>


## 物联网平台开发
&emsp;&emsp;
整个过程包含以下4个步骤：

1. 开通公共实例
2. 创建产品（设备模型）
3. 定义产品功能（物模型）
4. 创建设备及获取三元组

<br>

### 开通公共实例
&emsp;&emsp;
对于第一次使用物联网平台的读者，需要开通实例以使用物联网平台的功能。这里可以使用免费的公共实例进行开发。

&emsp;&emsp;
在[物联网平台](https://iot.console.aliyun.com/lk/summary/new)中，左上角选择“华东2-上海”，点击“公共实例”，即可开通。
<div align="center">
<img src=./../../../images/5_3_开通公共实例.png
 width=100%/>
</div>

### 创建产品（设备模型）
&emsp;&emsp;
点击上图中的“公共实例”，即可进入[控制台](https://iot.console.aliyun.com/lk/summary/new)进行产品创建。然后，点击“创建产品”按钮，即可进入[新建产品页面](https://iot.console.aliyun.com/product)。
<div align="center">
<img src=./../../../images/5_3_公共实例控制台.png
 width=100%/>
</div>

&emsp;&emsp;
进入[新建产品页面](https://iot.console.aliyun.com/product)后，设定“产品名称”，这里我们命名为“**车辆定位器**”，读者也可以根据自己的喜好来命名。

&emsp;&emsp;
在“所属品类”中，选择“标准品类”。“标准品类”收录了一些常用的设备模型，其会为我们定义好后续的产品功能（物模型）。我们也可以选择“自定义品类”，来自己定义物模型。在右侧栏中搜索并选择“定位器”。

<div align="center">
<img src=./../../../images/5_3_创建标准品类产品.png
 width=100%/>
</div>

&emsp;&emsp;
产品的节点类型选择“直连设备”，数据格式选择“ICA标准数据格式”，检验类型和认证方式选择默认设定即可。开发者可根据自己的需求在“产品描述”页面添加针对此产品的描述。

&emsp;&emsp;
对于 ESP32 等搭载 Wi-Fi 的设备而言，联网方式可以选择“Wi-Fi”。

<div align="center">
    <img src=./../../../images/5_3_创建WIFI产品.png
    width=40%/>
</div>

&emsp;&emsp;
选择之后，点击“确认”按钮，即可完成产品创建。返回“产品”页面之后可以看到产品类表中会出现刚刚创建的“车辆定位器”的产品，如下图所示。

<div align="center">
<img src=./../../../images/5_3_产品列表车辆定位器.png width=100%/>
</div>
<br>

### 定义产品功能（物模型）
&emsp;&emsp;
点击上图中的“查看”按钮，即可看到产品信息，Topic列表，功能定义，数据解析等跟产品相关功能的设定。点开“功能定义”标签页，可以看到设备物模型定义。

<div align="center">
<img src=./../../../images/5_3_车辆定位器产品详情.png width=100%/>
</div>

&emsp;&emsp;
因为在创建产品的过程中选择了标准的产品品类，这里会出现标准品类中自带的物模型设定，即“地理位置”。点击“查看”，我们可以看到该属性的详情。可以看到，该属性的数据类型是一个结构体(struct),其成员描述了设备的精度（浮点）、维度（浮点）、海拔（浮点）、坐标系统（枚举量，1：WGS_84/2:GCJ_02）

<div align="center">
<img src=./../../../images/5_3_属性详情.png width=100%/>
</div>

&emsp;&emsp;
定义好物模型后，记得点击“发布”以使变更生效。
<div align="center">
<img src=./../../../images/5_3_发布物模型.png width=100%/>
</div>

&emsp;&emsp;
产品及其物模型创建完成后就可以创建这个产品的设备了。

<br>

### 创建设备及获取三元组
&emsp;&emsp;
点击左侧栏中“产品“，回到产品列表。在产品列表页面中，点击“管理设备”，就会进到设备管理页面。

<div align="center">
<img src=./../../../images/5_3_产品列表管理设备.png width=100%/>
</div>

&emsp;&emsp;
在“设备”页面点击“批量添加”按钮，如下图所示。填入需要生成的设备数量。例如图中物联网平台会为我们生成3台设备，以及随机的deviceName。
<div align="center">
<img src=./../../../images/5_3_批量添加.png width=100%/>
</div>

&emsp;&emsp;
批量生成的设备如下
<div align="center">
<img src=./../../../images/5_3_设备列表.png width=100%/>
</div>

&emsp;&emsp;
设备添加完成后，点击“前往查看”按钮，就可以看到此设备的详细信息了。
<div align="center">
<img src=./../../../images/5_3_设备端详细信息.png width=100%/>
</div>

<br>

#### 获取设备三元组
&emsp;&emsp;
如下图所示，点击“查看”按钮，就可以看到设备的三元组信息，三元组是物联网设备端和物联网云端设备相关联的唯一标识符，在设备端连接云端的时候会使用三元组信息和云端进行鉴权，鉴权通过之后云端会认为设备已激活并上线。

<div align="center">
<img src=./../../../images/5_3_三元组信息.png width=100%/>
</div>

<br>


## 设备端开发

### 开发环境
&emsp;&emsp;
在进行下一步之前请确保以下工作已经搭建完毕。详情请参考[esp32开发环境](../../../startup/ESP32-quick-start.md)的说明。
<br>
&emsp;&emsp;
(1) 烧录支持HaaS-JS开发的"ESP32 HaaS固件"。
<br>
&emsp;&emsp;
(2) 已安装ESP32 USB串口驱动。
<br>
&emsp;&emsp;
(2) 已下载HaaS-JS轻应用推送工具amp。
<br>

### 下载示例代码

&emsp;&emsp;
下载《[ESP32 vehicle_location示例](https://gitee.com/alios-things/amp/tree/rel_3.3.0/example-js/haaseduk1/fs)》，示例代码目录如下所示：
```bash
vehicle_location
├── app.json      # 板级配置
└── app.js        # 应用代码
└── gps.js        # gps数据解析
```

### 修改wifi ssid和password

&emsp;&emsp;
修改vehicle_location工程里app.js中wifiSsid和wifiPassword的值为读者实际要连接的路由器的名称及密码（请注意名称和密码都需要放在""符号中间）。

```javascript
  var wifiOptions = {
    ssid: "请填写您的路由器名称",
    password: "请填写您的路由器密码",
    bssid: '',
    timeout_ms: 18000
  };
```

### 修改设备端三元组

&emsp;&emsp;
修改fire_detector工程里app.js中productKey、deviceName和deviceSecret的值为读者创建的物联网设备的三元组信息，如下图所示：

```javascript
var iotOptions = {
    productKey: "请填写您在阿里云物联网平台申请的产品PK",
    deviceName: "请填写您在阿里云物联网平台申请的产品DN",
    deviceSecret: "请填写您在阿里云物联网平台申请的产品DS",
    region: 'cn-shanghai',
    keepaliveSec: 60
  };
```
<br>

### 推送脚本至设备
&emsp;&emsp;
 注意: ESP32在Windows环境下执行amp-win工具下载JS脚本之前，需要先把ESP32的GPIO_0拉高（和3.3v脚短接），这样才能正常推送。Mac环境下则无此问题，不需该步骤。

&emsp;&emsp;
ESP32板子GPIO_0 硬件连线如下表所示：
|板子|板级pin脚|说明|
|-----|----|----|
| NODEMCU-32 | P0 | GPIO_0对应管脚 |
| M5StackCore2 | G0 | GPIO_0对应管脚 |

&emsp;&emsp;
执行以下amp命令。
> ./amp-clitool/dist/amp-macos/amp serialput ./fire_detector /dev/cu.wchusbserial537A003351 -m quickjs -e 0 -b 115200

&emsp;&emsp;
注意：amp工具路径和ESP32的串口ID请以实际为准。
<br>

## 运行结果
### 本地查看
&emsp;&emsp;
推送此脚本到ESP32之后，串口会输出获取到的定位信息，同时按照脚本中的逻辑，当定位信息发生变化时，会将更新的定位信息上报至物联网平台。如下面这段串口日志所示。
```log
network connected
net ip_addr is: 192.168.124.5
net mac is: 01-23-45-67-89-ab
create IotDeviceClient
iot platform connected
reportLocationData: {
    "lat": 31.188156,
    "lon": 121.60450216666666,
    "alt": 2.4
}
...
```
&emsp;&emsp;
出现此段日志时，说明定位信息上报成功，读者此时可以前往物联网平台查看设备的实时定位数据。


### 在物联网平台上查看设备数据
&emsp;&emsp;
当设备启动成功并第一次连接到物联网平台之后，物联网平台上对应的设备状态会从”未激活状态“变为”上线“。点击“物模型数据”标签页，能够看到设备上报到物联网平台的属性值。

<div align="center">
    <img src=./../../../images/5_3_LP显示定位数据.png width=100%/>
</div>

&emsp;&emsp;
点击卡片上的“查看数据”，可以看到完整的历史数据。
<div align="center">
    <img src=./../../../images/5_3_LP显示定位数据详情.png width=100%/>
</div>

&emsp;&emsp;
此时，如果设备上报新的地理位置信息，则物联网平台上能够实时显示。至此，定位数据上云完成。

<br>
&emsp;&emsp;
## 物联网应用开发（定位数据可视化）

&emsp;&emsp;
在前面的章节中，我们已经成功将地位信息实时上传到了云端。本节中，我们将介绍如何快速实现将设备的位置实时显示在地图中。

&emsp;&emsp;
IoT Studio 提供了应用快速开发的能力，可以很方便地与物联网平台进行联动。本节的开发工作也将围绕 IoT Studio展开。
<br>

### 新建“普通项目”
&emsp;&emsp;
打开[IoT Studio官网](https://studio.iot.aliyun.com/)，在项目管理中新建一个空白项目，如下图所示，将此项目命名为“车辆实时定位系统”。
<div align="center">
<img src=./../../../images/5_3_普通项目.png width=100%/>
<img src=./../../../images/5_3_创建空白项目.png width=100%/>
</div>

<br>

### 关联产品
&emsp;&emsp;
为了使本项目能够获取到目标设备的定位信息，我们首先需要将该项目和我们在前一节创建的产品“车辆定位器”绑定。

&emsp;&emsp;
在项目控制台，点击左侧的“产品”，点击“关联物联网平台产品”。此时可以看见我们创建的“车辆定位器”。点击选中，并勾选“关联产品同时关联其下所有设备”，以便该项目可以访问到所有设备的定位信息。
<div align="center">
<img src=./../../../images/5_3_关联产品.png width=100%/>
</div>

### 创建“Web应用”
&emsp;&emsp;
在项目控制台，点击左侧的“主页”，在新项目的首页新建一个Web应用，命名为“车辆实时定位监控”。
<div align="center">
<img src=./../../../images/5_3_Web应用.png width=100%/>
</div>

&emsp;&emsp;
Web应用创建成功后自动跳转到应用界面设计页面。点击左侧栏中的“组件”按钮图标，就可以看到可用的组件列表。各组件的说明请参考[IoT Studio组件说明](https://help.aliyun.com/document_detail/125196.html)。

&emsp;&emsp;
为了能够将设备显示在地图上，我们选用“设备地图”组件。鼠标悬停在该组件上以查看详细信息。
<div align="center">
<img src=./../../../images/5_3_设备地图.png width=100%/>
</div>

&emsp;&emsp;
将“设备地图”组件拖拽至画布，并调整大小。此时，地图上还未能正确显示设备。我们需要点击右侧“编辑设备地图”。
<div align="center">
<img src=./../../../images/5_3_界面布局.png width=100%/>
</div>

&emsp;&emsp;
点击右侧“编辑设备地图”后，我们可以在右侧“选择设备”中，选择我们刚刚和项目绑定的“车辆定位器”。点击确定。
<div align="center">
<img src=./../../../images/5_3_选择产品.png width=100%/>
</div>

&emsp;&emsp;
此时，地图上已经出现了定位点，正是我们之前上线的设备。在右侧选择“车辆定位器”，可以看到绑定的所有设备。点击对应设备可以在地图上定位到每个设备的具体位置。至此，“设备地图“配置完成，点击页面右上角”√“保存配置。
<div align="center">
<img src=./../../../images/5_3_选择设备.png width=100%/>
</div>

<br>
​
### 发布上线
<div align="center">
<img src=./../../../images/5_3_发布和上线.png width=100%/>
</div>

&emsp;&emsp;
回到WEB应用页面，可以看到地图上的设备定位已经正常显示，可以进行发布。在发布之前可以点击上图的“预览”查看应用的实际运行效果。实际运行效果如下图所示：

<div align="center">
<img src=./../../../images/5_3_预览.png width=100%/>
</div>

<br>

&emsp;&emsp;
这样我们就完成了一个车辆实时定位系统从设备端定位信息获取、物联网云平台开发及物联网应用开发全链路的开发。

&emsp;&emsp;
如果想要学习车辆定位系统案例更详细的操作步骤，请参考“[车辆定位系统详解](https://gitee.com/haasedu/haasedu/blob/release_2.0/5-%E6%99%BA%E6%85%A7%E5%9F%8E%E5%B8%82/%E5%9C%BA%E6%99%AF3-%E8%BD%A6%E8%BE%86%E5%AE%9A%E4%BD%8D%E7%B3%BB%E7%BB%9F/README.md)”中的说明。
