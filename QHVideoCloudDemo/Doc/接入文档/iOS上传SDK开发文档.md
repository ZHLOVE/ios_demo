# iOS上传SDK开发文档

## 介绍

直播云以SDK形式提供上传服务，可以帮助开发者快速上传文件并进行管理。SDK包含framework、demo及开发文档。

## 功能说明

上传SDK提供两种上传形式：本地文件、内存数据，根据实际业务情况使用，上传过程中可取消，设置代理接收进度、上传状态等信息
## 系统范围

| 系统特性 | 支持范围     |
| -------- | ------------ |
| 系统版本 | iOS8+        |
| 系统架构 | armv7、armv7s、arm64 |

## 业务流程

![image](http://p1.qhimg.com/d/inn/dd65852f/t01365f0273df5e605c.png)

详细信息请参考：[云存储-用户手册-编程模型](https://live.360.cn/index/doc?type=s3&id=189)。

## SDK集成
### 下载SDK

下载链接：[https://github.com/360livecloud/ios_demo.git](https://github.com/360livecloud/ios_demo.git)

### 配置说明

1. 上传功能提供两个framework：

	QHVCUploadKit.framework该库为静态库（Build Phases->Link Binary With Libraries-> +）

	QHLCBase.framework该库为动态库（Build Phases->Embed Frameworks-> +）




2. 实际开发中#import `<QHVCUploadKit/QHVCUploader.h>`头文件调用相关接口。


## 接口说明

### 上传相关

创建上传对象

` _uploader =  [[QHVCUploader alloc]init];`

   ` [_uploader setUploaderDelegate:self];`
    
    

```

/**
 *  @功能 获取上传类型，目前有表单和分片两种形式，具体使用哪种形式由服务器返回的配置信息决定
 *  如果是分片上传，需要调用parallelQueueNum获得队列数，用于计算token
 *  如果是表单上传，无需调用parallelQueueNum，计算token不需要此参数
 *  @参数 size 待上传任务数据大小，单位：字节
 *  @返回值 详见QHVCUploadTaskType
 */
- (QHVCUploadTaskType)uploadTaskType:(uint64_t)size;

/**
 *  @功能 获取分片上传队列数，用于业务计算token
 *  @返回值 分片上传队列数
 */
- (NSInteger)parallelQueueNum;

/**
 *  @功能 两种上传方式，数据在本地uploadFile:，数据在内存中uploadData:
 *  @参数 filePath 待上传文件本地路径
 *  @参数 data    待上传内存数据
 *  @参数 fileName    本地文件/内存数据上传到服务器后的文件名
 *  @参数 token 表单/分片任务计算规则略有差别
 */
- (void)uploadFile:(NSString *)filePath fileName:(NSString *)fileName token:(NSString *)token;
- (void)uploadData:(NSData *)data fileName:(NSString *)fileName token:(NSString *)token;

/**
 *  @功能 取消当前上传任务
 */
- (void)cancel;

直播云上传域名由云控参数确定，业务方可以调用以下接口修改上传域名。
/**
 *  @功能 第三方设置上传域名，上传前设置
 *  @参数 domain 有效的域名
 */
+ (void)setUploadDomain:(NSString *)domain;
```

### 日志相关
开发阶段辅助开发调试，根据实际情况使用

```
/**
 * 打开上传日志
 * @参数 level 日志等级
 */
+ (void)openLogWithLevel:(QHVCUploadLogLevel)level;

/**
 * 设置日志输出callback
 * @参数 callback 回调block
 */
+ (void)setLogOutputCallBack:(void(^)(int loggerID, QHVCUploadLogLevel level, const char *data))callback
```
###统计相关
```
//统计相关，请正确设置，利于排查线上问题，在上传前设置
/**
 *  @功能 用户id
 *  @参数 userId  第三方用户id
 */
+ (void)setUserId:(NSString *)userId;

/**
 *  @功能 设置第三方渠道号
 *  @参数 channelId   渠道号
 */

+ (void)setChannelId:(NSString *)channelId;
/**
 *  @功能 设置第三方业务版本号
 *  @参数 appVersion   版本号
 */
+ (void)setAppVersion:(NSString *)appVersion;

/**
 *  @功能 设置设备id
 *  @参数 deviceId   设备id
 */
+ (void)setDeviceId:(NSString *)deviceId;
```
###回调
```
/**
 *  @功能 回调上传状态 成功、失败
 *  @参数 uploader
 *  @参数 status 上传状态
 */
- (void)didUpload:(QHVCUploader *)uploader status:(QHVCUploadStatus)status error:(nullable NSError *)error;

@optional
/**
 *  @功能 上传进度
 *  @参数 uploader
 *  @参数 progress 上传进度（0.0-1.0）
 */
- (void)didUpload:(QHVCUploader *)uploader progress:(float)progress;
```

## 错误码说明

|状态码|含义|
|:--:|:--|
|-105|文件不存在|
|-108|不支持文件夹|
|-111|Token为空|
|-112|上传的内存数据为空|
|-113|上传的文件是0字节|