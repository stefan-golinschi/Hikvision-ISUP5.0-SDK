# 目录分层结构说明
## 一、 DemoApp demo实例入口
### IsupTest 示例代码入口
#### ISUP SDK基础框架功能示例代码 SdkFunctionDemo
<ol>
<li>上传图片至存储服务器(返回图片URL可以用于人脸下发) </li>
<li>取流预览模块(有预览窗口)</li>
<li>取流预览模块(保存到本地文件)</li>
<li>按时间回放模块(有回放窗口)</li>
<li>按时间回放模块(保存到本地文件) </li>
</ol>

## 二、DevModule 分产品类型的定义实现
### resources 文件说明
#### 1）audioFile 存放本地下发送给设备的音频文件
#### 2）conf 存在示例代码中业务报文信息
> 其中 ****-desc.xml文件为业务报文的字段解释，req表示字段必填，req表示字段非必填，可以根据您的实际业务需求自行删减组件请求报文内容！！！
eg：SearchFCByImage-desc.xml为查询告警事件的完整请求报文字段说明
#### 3）pics 存放发送给图片存储服务的演示图片数据
### XXX 系统相关的功能点demo演示
> 重要说明： 示例代码仅演示常见的集成功能点，如您有其他功能集成咨询，请申请并参见我司ISAPI协议文档自行补充开发 或 咨询我司对应销售/产品技术同事，感谢您的理解与配合!

## 三、 Common 通用的模块定义

## 四、SdkService sdk相关的模块实现
### AlarmService 告警服务模块
#### HCISUPAlarm.java 管理所有的告警服务的dll接口定义

### CmsService 注册管理模块
#### HCISUPCMS.java 管理所有的注册服务的dll接口定义

### SsService 存储管理模块
#### HCISUPSS.java 管理所有的存储服务的dll接口定义

### StreamService 流处理模块
#### HCISUPStream.java 管理所有的流处理的dll接口定义

## 五、 UI界面的模块定义