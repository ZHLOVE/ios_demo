//
//  QHVCPlayer.h
//  QHVCPlayerKit
//
//  Created by yinchaoyu on 2017/5/24.
//  Copyright © 2017年 qihoo 360. All rights reserved.
//

#import <Foundation/Foundation.h>
#import <UIKit/UIKit.h>

/**
 播放器播放视频的类型
 */
typedef NS_ENUM(NSInteger, QHVCPlayType)
{
    QHVCPlayTypeLive     = 0,//直播
    QHVCPlayTypeVod      = 1,//点播
};

//直播用
typedef NS_ENUM(NSInteger, QHVCStreamType)
{
    QHVCStreamTypeOnlyAudio = 0,//仅视频
    QHVCStreamTypeOnlyVideo = 1,//仅音频
    QHVCStreamTypeDefault = 2,//默认方式（音、视频）
};

/**
 定义播放器错误信息
 */
typedef NS_ENUM(NSInteger, QHVCPlayerError)
{
    QHVCPlayerErrorPlayerInitFailed  = 1,//播放器初始化失败
    QHVCPlayerErrorConnectFailed     = 2,//播放器连接失败，比如网络连接
    QHVCPlayerErrorFormatNotSupport  = 3,//文件格式不支持
    QHVCPlayerErrorFileNotOpen       = 4,//文件打开失败
};

/**
 播放器详细错误信息
 */
typedef NS_ENUM(NSInteger, QHVCPlayerErrorDetailedInfo)
{
    //播放器内部错误
    QHVCPlayerErrorDetailedInfoUnknow                    = 0,//未知消息
    QHVCPlayerErrorDetailedInfoIOException               = 1,//I/O异常
    QHVCPlayerErrorDetailedInfoConnectRefused            = 2,//拒绝连接
    QHVCPlayerErrorDetailedInfoInvalidData               = 3,//无效的数据
    QHVCPlayerErrorDetailedInfoExit                      = 4,//退出，可能注册了无效的过滤器
    QHVCPlayerErrorDetailedInfoNotBitstream              = 5,//找不到Bitstream
    QHVCPlayerErrorDetailedInfoNotDecoder                = 6,//找不到解码器
    QHVCPlayerErrorDetailedInfoNotDemuxer                = 7,//找不到demuxer
    QHVCPlayerErrorDetailedInfoNotFilter                 = 8,//找不到Filter
    QHVCPlayerErrorDetailedInfoNotProtocol               = 9,//找不到Protocol
    QHVCPlayerErrorDetailedInfoNotStream                 = 10,//找不到Stream
    QHVCPlayerErrorDetailedInfoServer                    = 11,//server 错误
    QHVCPlayerErrorDetailedInfoEOF                       = 12,//EOF
    //SDK层错误
    QHVCPlayerErrorDetailedInfoHandleError               = 13,//播放器初始化handle为空
    QHVCPlayerErrorDetailedInfoNotReadyToPlay            = 14,//prepare failed
};

/**
 播放器状态
 */
typedef NS_ENUM(NSInteger, QHVCPlayerStatus)
{
    QHVCPlayerStatusUnknown              = 0,//未知类型
    QHVCPlayerStatusPlaying              = 1,//播放
    QHVCPlayerStatusPaused               = 2,//暂停
    QHVCPlayerStatusStoped               = 3,//会话关闭
};

/**
  日志级别
 */
typedef NS_ENUM(NSInteger, QHVCPlayerLogLevel)
{
    QHVCPlayerLogLevelTrace = 0,//trace
    QHVCPlayerLogLevelDebug = 1,//debug
    QHVCPlayerLogLevelInfo  = 2,//info
    QHVCPlayerLogLevelWarn  = 3,//warn
    QHVCPlayerLogLevelError = 4,//error
    QHVCPlayerLogLevelAlarm = 5,//alarm
    QHVCPlayerLogLevelFatal = 6,//fatal
};

/**
 播放器状态delegate
 */
@protocol QHVCPlayerDelegate <NSObject>

@required
/**
 播放器首次加载缓冲准备完毕，在此回调中调用play开始播放
 */
- (void)onPlayerPrepared;

/**
 播放器首屏渲染，可以显示第一帧画面
 */
- (void)onPlayerFirstFrameRender:(NSDictionary *_Nullable)mediaInfo;

/**
 播放结束回调
 */
- (void)onPlayerFinish;

@optional
/**
 * 视频大小变化通知
 *
 * @param width  视频宽度
 * @param height 视频高度
 */
- (void)onPlayerSizeChanged:(int)width height:(int)height;

/**
 开始缓冲(buffer为空，触发loading)
 */
- (void)onPlayerBufferingBegin;

/**
 * 缓冲进度(buffer loading进度)
 *
 * @param progress 缓冲进度，progress==0表示开始缓冲， progress==100表示缓冲结束
 */
- (void)onPlayerBufferingUpdate:(int)progress;

/**
 缓冲完成(buffer loading完成，可以继续播放)
 */
- (void)onPlayerBufferingComplete;

/**
 播放进度回调

 @param progress 播放进度
 */
- (void)onPlayerPlayingProgress:(CGFloat)progress;

/**
 测试用

 @param mediaInfo 视频详细参数
 */
- (void)onplayerPlayingUpdatingMediaInfo:(NSDictionary *_Nullable)mediaInfo;

/**
 * 拖动操作缓冲完成
 */
- (void)onPlayerSeekComplete;

/**
 * 播放器错误回调
 *
 * @param error       错误类型
 * @param extraInfo   额外的信息
 */
- (void)onPlayerError:(QHVCPlayerError) error extra:(QHVCPlayerErrorDetailedInfo)extraInfo;

/**
 * 播放状态回调
 *
 * @param info  参见状态信息枚举
 * @param extraInfo 扩展信息
 */
- (void)onPlayerInfo:(QHVCPlayerStatus)info extra:(NSString * _Nullable)extraInfo;

/**
 码率切换成功

 @param index 播放index
 */
- (void)onPlayerSwitchResolutionSuccess:(int)index;

/**
 码率切换失败

 @param errorMsg errorMsg description
 */
- (void)onPlayerSwitchResolutionFailed:(NSString *_Nullable)errorMsg;

/**
 主播切入后台
 */
- (void)onPlayerAnchorInBackground;

@end

@interface QHVCPlayer : NSObject

/**
 播放器状态delegate
 */
@property (nonatomic, weak) _Null_unspecified id<QHVCPlayerDelegate> playerDelegate;

/**
 播放器状态
 */
@property (nonatomic, assign) QHVCPlayerStatus playerStatus;

/**
 初始化播放器
 
 @param URL 需要播放到URL
 @param channelId 渠道ID，使用者从平台申请，eg:live_huajiao_v2
 @param userId 用户ID，用户标识，唯一标识（需要详细说明）
 @param playType 播放类型，直播、点播、本地
 @return 成功：播放器对象, 失败：nil
 */
- (QHVCPlayer * _Nullable)initWithURL:(NSURL * _Nonnull)URL
                            channelId:(NSString * _Nullable)channelId//内部默认值
                               userId:(NSString * _Nullable)userId//内部默认值
                             playType:(QHVCPlayType)playType;

/**
 初始化播放器(若需要设置解码类型、流类型用如下初始化接口，更多设置请用Advance内部初始化接口)
 
 @param URL 需要播放到URL
 @param channelId 渠道ID，使用者从平台申请，eg:live_huajiao_v2
 @param userId 用户ID，用户标识，唯一标识（需要详细说明）
 @param playType 播放类型，直播、点播、本地
 @param options @{@"streamType":@"QHVCStreamType",@"hardDecode":@"boolValue"}
 @return 成功：播放器对象, 失败：nil
 */
- (QHVCPlayer * _Nullable)initWithURL:(NSURL * _Nonnull)URL
                            channelId:(NSString * _Nullable)channelId//内部默认值
                               userId:(NSString * _Nullable)userId//内部默认值
                             playType:(QHVCPlayType)playType
                              options:(NSDictionary *_Nullable)options;


/**
 初始化播放器(需要切换码率时用如下初始化接口)

 @param urlArray 多分辨播放源
 @param playIndex 初始播放索引
 @param channelId 渠道ID，使用者从平台申请，eg:live_huajiao_v2
 @param userId 用户ID，用户标识，唯一标识（需要详细说明）
 @param playType 播放类型，直播、点播、本地
 @param options @{@"streamType":@"QHVCStreamType",@"bUseHW":@"boolValue"}
 @return 成功：播放器对象, 失败：nil
 */
- (QHVCPlayer * _Nullable)initWithUrlArray:(NSArray<NSURL *> *_Nullable)urlArray
                                 playIndex:(int)playIndex
                                 channelId:(NSString * _Nullable)channelId//内部默认值
                                    userId:(NSString * _Nullable)userId//内部默认值
                                  playType:(QHVCPlayType)playType
                                   options:(NSDictionary *_Nullable)options;


/**
 切换码率

 @param index 索引
 @return success or not
 */
- (BOOL)switchResolutionWithIndex:(int)index;

/**
 自动切换码率

 @param isAutomatically yes or no
 */
- (void)setAutomaticallySwitchResolution:(BOOL)isAutomatically;

/**
 创建播放器渲染playerView(add在传入的view上)

 @param view playerView
 */
- (void)createPlayerView:(UIView *_Nonnull)view;

/**
 释放player时候是否移除playerView

 @param remove 默认移除
 */
- (void)removePlayerViewWhenPlayerRelease:(BOOL)remove;

/**
 播放器准备播放，准备完毕后回调onPrepared
 */
- (void)prepare;

/**
 播放器准备完成后调用该接口开始播放，调用时机说明：播放器准备完成后会回调QHVCPlayerDelegate中的onPrepared方法，在该方法中调用play开始播放
 */
- (void)play;

/**
 点播视频暂停播放, 直播场景调用无效，暂停后继续播放使用play
 */
- (void)pause;

/**
 播放器停止播放
 */
- (void)stop;

/**
 播放过程中改变进度操作,直播场景无效
 *  @param positionByMS 点播视频位置，单位毫秒(millisecond)
 *  @return 成功：YES，失败：NO
 */
- (BOOL)seekTo:(NSTimeInterval)positionByMS;

/**
 点播视频当前播放时间

 @return 点播视频场景下获取当前播放时间，单位毫秒
 */
- (NSTimeInterval)getCurrentPosition;

/**
 点播视频总时长

 @return 点播视频总时长，直播时调用无效，单位毫秒
 */
- (NSTimeInterval)getDuration;

/**
 获取播放器回看缓冲下载进度
 
 @return <0失败， >0成功
 */
- (long)getDownloadProgress;

/**
 设置音量

 @param volume 音量范围 0.0~1.0 （1.0最大）
 @return YES:成功， NO:失败
 */
- (BOOL)setVolume:(float)volume;

/**
 获取播放器当前音量

 @return 音量范围 0.0~1.0 （1.0最大）
 */
- (float)getVolume;

/**
 设置日志级别

 @param level 日志级别
 */
+ (void)setLogLevel:(QHVCPlayerLogLevel)level;

/**
 设置日志输出block

 @param logOutput 接收日志block
 */
+ (void)setLogOutputBlock:(void (^_Nonnull)(int loggerID, QHVCPlayerLogLevel level, const char * _Nonnull data))logOutput;

@end
