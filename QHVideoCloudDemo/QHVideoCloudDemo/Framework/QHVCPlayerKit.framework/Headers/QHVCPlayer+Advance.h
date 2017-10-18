//
//  QHVCPlayer+Advance.h
//  QHVCPlayerKit
//
//  Created by yinchaoyu on 2017/5/24.
//  Copyright © 2017年 qihoo 360. All rights reserved.
//

#import <CoreMedia/CoreMedia.h>
#import "QHVCPlayer.h"

typedef NS_ENUM(NSInteger, QHVCPlayerDecodingType)
{
    QHVCMediaInfoDecodeFormateH264    = 0,//正在解码的是h264
    QHVCMediaInfoDecodeFormateH265    = 1,//正在解码的是h265
};

/**
 播放器渲染模式
 */
typedef NS_ENUM(NSInteger, QHVCPlayerRenderMode)
{
    QHVCPlayerRenderModeIn   = 0,//默认，窗口内缩放，不填充屏幕
    QHVCPlayerRenderModeOut  = 1,//填充整个屏幕，可能丢失显示内容，但不会变形
    QHVCPlayerRenderModeFull = 2,//全屏显示，可能会变形
};

/**
 播放器播放模式
 */
typedef NS_ENUM(NSInteger, QHVCPlayMode)
{
    QHVCPlayModeFluency      = 0,//流畅模式
    QHVCPlayModeLowLatency   = 1,//低延迟模式
};

typedef NS_ENUM(NSInteger, QHVCRecorderFormat)
{
    QHVCRecorderFormat_MP4   = 0,
    QHVCRecorderFormat_MOV   = 1,
    QHVCRecorderFormat_GIF   = 2
};

typedef struct
{
    int width;
    int height;
    int video_bitrate;
    
    int audio_bitrate;
    int samplerate;
    int channels;
    
    //for gif
    int sampleInterval;//源视频多长时间(ms)挑选一帧，默认0，全部帧
    int fps;
}QHVCRecordConfig;

/**
 播放器advanceDelegate
 */
@protocol QHVCPlayerAdvanceDelegate <NSObject>

@optional

/**
 播放器实例句柄创建
 */
- (void)onPlayerHandlerCreated;

/**
 播放器销毁
 */
- (void)onPlayerDestoryed;

/**
 流量计算回调函数
 
 @param dvbps 播放器下行视频每秒字节数
 @param dabps 播放器下行音频每秒字节数
 @param dvfps 播放器下行视频每秒帧数
 @param dafps 播放器下行音频每秒帧数
 */
- (void)onPlayerNetStats:(long long)dvbps
                   dabps:(long long)dabps
                   dvfps:(long long)dvfps
                   dafps:(long long)dafps;

/**
 编码方式

 @param decodingType 编码类型
 @param sn sheduled_sn_url
 @param errorCode errorCode
 */
- (void)onPlayerDecodingType:(QHVCPlayerDecodingType)decodingType sn:(NSString *_Nullable)sn errorCode:(int)errorCode;

@end

@interface QHVCPlayer (Advance)

/**
 播放器扩展delegate
 */
@property (nonatomic, weak) _Null_unspecified id<QHVCPlayerAdvanceDelegate> playerAdvanceDelegate;

/**
 * 开始预调度
 
 * @param sn 资源标识
 * @param channelId channelID
 * @param userId 用户ID
 * @param uSign 鉴权签名
 * @return YES表示成功 NO表示失败
 */
+ (BOOL)prepareScheduling:(NSString * _Nonnull)sn
                channelId:(NSString * _Nullable)channelId
                   userId:(NSString * _Nullable)userId
                    uSign:(NSString * _Nonnull)uSign;

/**
 * 开始预调度,需要使用特定调度地址的情况
 * @param scheduleUrl 预调度地址 格式参考http://g2.live.360.cn/
 * @param sn 资源标识
 * @param channelId channelID
 * @param userId 用户ID
 * @param uSign 鉴权签名
 * @return YES表示成功 NO表示失败
 */
+ (BOOL)prepareScheduling:(NSString * _Nonnull)scheduleUrl
                       sn:(NSString * _Nonnull)sn
                channelId:(NSString * _Nullable)channelId
                   userId:(NSString * _Nullable)userId
                    uSign:(NSString * _Nonnull)uSign;

/**
 初始化播放器
 
 @param SN 要播放的SN
 @param userId 用户id
 @param uSign 验证信息
 @param optionsDict 播放器可选属性字典，可传nil，或者根据实际需要选择部分或者全部属性设置，支持的全部属性如下：
 NSDictionary *playProperty =
 {@"position", @"NSString long",//只适用于点播，设置点播开始到位置(毫秒)
 @"scheduleUrl",@"NSString"//调度地址，需要使用自定义的调度地址时使用
 @"hardDecode", @"boolValue"//播放器硬解开关
 @"renderMode", @QHVCPlayerRenderMode//播放器渲染模式
 @"playMode", @QHVCPlayMode//设置播放器播放模式，流畅/低延时
 @"mute", @"boolValue"//开始播放的时候是否静音0/1
 @"previewDuration", @"NSString int"//视频预览时长(毫秒)
 @"streamType", @"QHVCStreamType"//流类型
 }
 
 @return 播放器对象
 */
- (QHVCPlayer * _Nullable)initWithSN:(NSString * _Nonnull)SN
                           channelId:(NSString * _Nullable)channelId
                              userId:(NSString * _Nullable)userId
                               uSign:(NSString * _Nonnull)uSign
                            options:(NSDictionary * _Nullable)optionsDict;

/**
 * @abstract 视频截图
 * @return 视频帧图像
 */
- (UIImage *_Nullable)snapshotImage:(NSString * _Nonnull)path;

/**
 用于解决播放器audioSession被占用的场景
 */
+ (void)cleanAudioSession;

/**
 打开播放器流量统计

 @param intervalBySecond 统计计算周期，单位：秒
 @参见 onPlayerNetStats delegate
 */
- (void)openNetStats:(uint)intervalBySecond;

/**
 关闭播放器流量统计
 */
- (void)closeNetStats;

/**
 镜像模式开关，播放器模式状态是非镜像状态，若启用镜像模式，调用该接口

 @param bMirror 镜像：YES，否则：NO
 */
- (void)setMirror:(BOOL)bMirror;

/**
 连麦加入Group
 
 @param groupID group id
 */
- (void)addToGroup:(int)groupID;

/**
 * 通知GPS，时区信息,用于优化调度CDN，定期由业务调用（切换省市，距离较远时调用，不要频繁调用）
 * @param lon 经度
 * @param lat 纬度
 */
- (void)playerGPSZoneLonLat:(double)lon latitude:(double)lat;

/**
 * 倍速播放
 *
 * @param rate 播放速度，取值1~n（建议n<=5）
 */
- (void)setPlayBackRate:(float)rate;

/**
 * 开始录制
 * 注意：不支持暂停时录制
 *
 * @param filePath 录像存储路径（确保有读写权限）
 * @param recorderFormat 存储格式
 * @param config   配置
 * @return 0:成功  <0: 失败
 */
- (int)startRecorder:(NSString *_Nonnull)filePath recorderFormat:(QHVCRecorderFormat)recorderFormat recordConfig:(QHVCRecordConfig *_Nullable)config;

/**
 * 结束录制(异步接口)
 *
 * @return 0:成功  onInfo回调收到{@link ILiveCloudPlayer.Info.PLAYER_MSG_RECORD_SUCCESS}消息时，视频保存成功<br>
 *        <0: 失败
 */
- (int)stopRecorde;

@end
