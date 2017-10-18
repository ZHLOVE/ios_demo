
#ifndef __CLIENT_NET_STATISTICS_ENTRY_H_
#define __CLIENT_NET_STATISTICS_ENTRY_H_

#include <stdbool.h>
#include "Types.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * 播放器云控参数
 */
typedef struct
{
    int m_skipDetectFull; ///< 跳过完整流信息探测 默认0
    int m_bufferingType; ///< 0 缓冲时间 1 缓冲数据包 默认0
    int m_bufferingFirst; ///< 第一次缓冲时间 直播默认300ms 回放10s 数据包缓冲默认6 -1000表示不使用云控
    int m_bufferingMaxTime; ///< 最大缓冲时间 直播默认8s 回放30s -1000表示不使用云控
    int m_use_sound_speed; ///< 是否开启声音加速 -1000表示不使用云控
    int m_duration_sound_speed_1; ///< 启动阈值 -1000表示不使用云控
    float m_sound_speed_1; ///< 加速速度 0表示不使用云控
    int m_use_rtmp_delay_reconnection; ///< 是否开启rtmp延迟重连机制 -1000表示不使用云控
    int m_duration_delay_reconnection; ///< 默认2000ms -1000表示不使用云控
    int m_max_rtmp_reconnection_waittime; ///< 公有协议返回第一次数据的等待时间 -1000表示不使用云控
    int m_use_framedrop_first; ///< 首次缓冲是否要追 -1000表示不使用云控 默认0
    int m_max_buffering_delay; ///< 默认2000ms -1000表示不使用云控
    int m_gp_play_state_upload; ///< 是否开启网络抖动打点 默认0
    long m_gp_play_state_upload_size; ///< 网络抖动打点数据上传最小长度 默认1024
    int m_huikan_skip_detect_fullstreaminfo; ///< 跳过完整流信息探测,如果1则跳过，首次开屏会比较快，默认0 -1000表示不使用云控
    int m_huikan_buffering_first; ///< 首屏以及拖动后缓冲 默认300ms -1000表示不使用云控
    int m_huikan_buffering_max_time; ///< 正常观看中缓冲 默认2000ms -1000表示不使用云控
    int m_use_hw_decode; ///< 硬解总开关 默认0
    int m_use_qy265dec; ///< 是否启用金山265decoder
    int m_open_addgroup; ///< AddGroup 1表示开启（生效） 0表示关闭 默认1
} CloudControlPlayer;

/*
 * 推流相关云控参数
 */
typedef struct
{
    unsigned int m_stream_status_interval; ///< 上报流状态周期 单位秒 0表示不生效
    int m_combined_task; ///< 0表示各用各家 1表示启用我们自己的合流
} CloudControlPublisher;

/**
 * 小视频上传云控参数
 */
typedef struct
{
    char m_candidate_names[256]; ///< 上传域名列表，逗号分割
    int m_filesize_threshhold; ///< 上传模式选择门限， > 门限用分片模式上传，<= 门限用分片模式上传， 单位KB
    int m_queue_num; ///< 队列数
    int m_max_retry; ///< 单次请求重试次数
    int m_max_time; ///< 单次请求timeout值，单位秒
    bool m_isHTTPS; ///< 是否启用HTTPS
} UploadControl;

/**
 * 获取编解码信息
 * @return 编解码信息
 */
EXPORT_API const char *notify_get_cloud_codec_info(void);

/**
 * 获取播放器云控信息
 * @return 播放器云控信息
 */
EXPORT_API CloudControlPlayer notify_get_cloud_control_player(void);

/**
 * 获取推流相关云控信息
 * @return 推流相关云控信息
 */
EXPORT_API CloudControlPublisher notify_get_cloud_control_publisher(void);

/**
 * 获取小视频上传云控信息
 * @return 小视频云控信息
 */
EXPORT_API UploadControl notify_get_cloud_control_upload(void);

/**
 * 设置小视频上传的域名的默认值
 * @param domain 默认域名
 */
EXPORT_API void notify_set_upload_default_domain_names(const char *domain);

/**
 * 设置打点地址和云控地址 格式参考默认地址: http://qos.live.360.cn/vc.gif http://fb.live.360.cn/fb.gif http://sdk.live.360.cn/
 * @param url 打点地址
 * @param urlFB feedback地址
 * @param urlMicFB Mic feedback地址
 * @param urlCC 云控地址
 */
EXPORT_API void notify_set_notify_url(const char *url, const char *urlFB, const char *urlMicFB, const char *urlCC);

/**
 * 主动触发获取所有云控参数（否则只在首次调用notify_app_start时以及后续在特定时机时）
 * @param force 是否强制获取
 */
EXPORT_API void notify_trigger_cloud(bool force);

/**
 * app启动时调用 必须第一时间调用，且整个app的生命周期只调用一次
 * @param bid 业务ID
 * @param pid 平台端ID
 * @param ver 端版本
 * @param os 系统类型和版本
 * @param mid 机器id
 * @param model 型号
 * @param urlCC 云控地址 格式参考默认地址: http://sdk.live.360.cn/
 */
EXPORT_API void notify_app_start(const char *bid, const char *pid, const char *ver, const char *os, const char *mid, const char *model, const char *urlCC);

/**
 * 用户点击开始时调用 必须作为每个session第一个调用的SDK接口
 * @param id 会话唯一标识
 * @param uid 用户ID
 * @param cid channelID
 * @param net 网络类型
 * @param sn 资源标识
 */
EXPORT_API void notify_user_start(const char *id, const char *uid, const char *cid, const char *net, const char *sn);

/**
 * 调用notify_player_destroy或notify_pub_destroy后，没必要调用该接口
 * 否则必须调用；与这两个接口调用规则一致
 * @param id 会话唯一标识
 */
EXPORT_API void notify_user_destroy(const char *id);

/**
 * 用户点击结束时调用
 * @param id 会话唯一标识
 */
EXPORT_API void notify_user_stop(const char *id);

/**
 * 用户app切到后台时调用
 * @param id 会话唯一标识
 */
EXPORT_API void notify_user_background(const char *id);

/**
 * 用户app回到前台时调用
 * @param id 会话唯一标识
 */
EXPORT_API void notify_user_foreground(const char *id);

/**
 * 播放器初始化时调用
 * @param id 会话唯一标识
 */
EXPORT_API void notify_player_init(const char *id);

/**
 * 播放器开始工作时调用
 * @param id 会话唯一标识
 */
EXPORT_API void notify_player_open(const char *id);

/**
 * 播放器开始缓冲数据时调用
 * @param id 会话唯一标识
 */
EXPORT_API void notify_player_buffering(const char *id);

/**
 * 播放器开始播放时调用
 * @param id 会话唯一标识
 */
EXPORT_API void notify_player_playing(const char *id);

/**
 * 用户操纵播放位置时调用
 * @param id 会话唯一标识
 * @param pos 定位位置标识
 */
EXPORT_API void notify_player_seek(const char *id, unsigned int pos);

EXPORT_API void notify_player_complete(const char *id); ///< 暂时不用

/**
 * 播放器销毁时调用
 * @param id 会话唯一标识
 * @param reason 关闭原因 由业务传下来
 */
EXPORT_API void notify_player_destroy(const char *id, int reason);

/**
 * 播放器出错时调用
 * @param id 会话唯一标识
 * @param errStr 错误信息
 * @param errNo 错误码
 */
EXPORT_API void notify_player_error(const char *id, const char *errStr, int errNo);

/**
 * 播放器上报端到端延迟时调用
 * @param id 会话唯一标识
 * @param jplayer 是否直播
 * @param frameCount frame缓冲个数
 * @param currentTime 调用方当前时间
 * @param hostTime 解析出来的主播时间
 * @param diff 服务端返回的diff
 * @param diffAV 当前音视频差
 * @param cacheDuration 播放器buffer缓冲的时间
 * @param delta 端到端延迟
 */
EXPORT_API void notify_player_delay(const char *id, int jplayer, int frameCount, long long currentTime, 
                                    long long hostTime, long long diff, long long diffAV, long cacheDuration, long delta);

/**
 * 上报播放时是否启用硬解
 * @param id 会话唯一标识
 * @param isHwDecode 是否启用硬解
 * @param isFailed 上次解码是否失败
 */
EXPORT_API void notify_player_decode_type(const char *id, bool isHwDecode, bool isFailed);

/**
 * 推流开始工作时调用
 * @param id 会话唯一标识
 */
EXPORT_API void notify_pub_open(const char *id);

/** 
 * 推流销毁时调用
 * @param id 会话唯一标识
 */
EXPORT_API void notify_pub_destroy(const char *id);

/**
 * 上报feedback打点
 * @param id 会话唯一标识
 * @param deltaInMS 本次上报与上次的间隔
 * @param deltaBeginInMS 本次上报与第一次的间隔
 * @param uri 推流地址完整uri
 * @param newPublishAddr 暂时不使用
 * @param info 相关数据，请参考comn/Types.h
 */
EXPORT_API void notify_stream_status_export(const char *id, long deltaInMS, long deltaBeginInMS, const char *uri, const char *newPublishAddr, 
                                            const StreamStatus *info);

/**
 * 连麦feedback结果回调函数原型
 * @param result feedback结果 0表示成功 否则为错误码
 * @param ctx 上下文
 */
typedef void (*rtc_notify_cb)(int result, void *ctx);

/**
 * 连麦feedback打点
 * @param id 会话唯一标识
 * @param kvList 以&和=分隔和连接的kv列表, 例如&k1=v1&k2=v2&k3=v3，v部分必须urlencode
 * @param cb feedback结果回调
 * @param ctx 上下文
 */
EXPORT_API void notify_rtc_stream_status(const char *id, const char *kvList, rtc_notify_cb cb, void *ctx);

/**
 * 连麦合流feedback打点
 * @param id 会话唯一标识
 * @param kvList 以&和=分隔和连接的kv列表, 例如&k1=v1&k2=v2&k3=v3，v部分必须urlencode
 * @param cb feedback结果回调
 * @param ctx 上下文
 */
EXPORT_API void notify_rtc_merge_stream_status(const char *id, const char *kvList, rtc_notify_cb cb, void *ctx);

/**
 * 设置打点的pro字段
 * @param id 会话唯一标识
 * @param type 值
 */
EXPORT_API void notify_set_oem(const char *id, const char *type);

/**
 * 通知wifi强度数值
 * @param quality wifi强度数值
 */
EXPORT_API void notify_wifi_quality(int quality);

/**
 * 通知cpu, gpu使用率
 * @param cpuStatus cpu使用率
 * @param gpuStatus gpu使用率
 */
EXPORT_API void notify_cpu_gpu_status(double cpuStatus, double gpuStatus);

/**
 * 通知GPS，时区信息
 * @param longtitude 经度
 * @param latitude 纬度
 */
EXPORT_API void notify_gps_zone_info(double longitude, double latitude);

/**
 * 小视频上传打点
 * @param info 打点完整信息
 */
EXPORT_API void notify_shortvideo_upload(const char *info);

typedef enum
{
    ST_FORM_UPLOAD = 1, ///< 表单上传
    ST_SLICE_REQUEST, ///< 分片Request
    ST_BLOCK_UPLOAD, ///< block上传
    ST_MEMORY_UPLOAD, ///< 内存上传
    ST_COMMIT, ///< commit
    ST_FINISH, ///< finish
    ST_CANCEL, ///< 取消上传
    ST_SPEED_TEST ///< 
} StageType;

typedef struct
{
    const char *uri; ///< 目标URI
    const char *dip; ///< 目标机器IP
    unsigned int conTime; ///< 连接时间
    unsigned int respTime; ///< 响应时间
    unsigned int avgSpeed; ///< 平均速度
    unsigned long long totalBytes; ///< 字节数
    int errCode; ///< 错误码
    int blockID; ///< block编号
    unsigned int retryCnt; ///< 重试次数
} UploadDataInfo;

/**
 * 上传过程打点
 * @param id 会话唯一标识
 * @param type stage type
 * @param info 相关信息
 */
EXPORT_API void notify_upload_data(const char *id, StageType type, const UploadDataInfo *info);

/**
 * 通用打点
 * @param id 会话唯一标识
 * @param businessSubID 业务子类型
 * @param st stage
 * @param error error
 * @param kvList 以&和=分隔和连接的kv列表, 例如&k1=v1&k2=v2&k3=v3，v部分必须urlencode
 */
EXPORT_API void notify_common_stat(const char *id, const char *businessSubID, int st, int error, const char *kvList);

/// 以下为SDK内部使用的打点接口
EXPORT_API void notify_schedule_start(const char *id);
EXPORT_API void notify_schedule_result(const char *id, const char *type, unsigned int isPub, int errCode, unsigned int dnsTime, unsigned int connectTime, 
                                       const char *ip, unsigned short port, const char *uri, const char *sn);

EXPORT_API void notify_testing_info(const char *id, const char *proto, const char *pingType, const char *ipPort, unsigned int sendSize, 
                                    unsigned int recvSize, double avgDelta);

EXPORT_API void notify_connection_start(const char *id);
EXPORT_API void notify_connection_result(const char *id, int errCode, int errDetail, unsigned int dnsTime, int type, 
                                         const char *uri, const char *ip, unsigned short port);
EXPORT_API void notify_connection_break(const char *id, int errCode, int errDetail);

EXPORT_API void notify_frames(const char *id, unsigned int type, unsigned int len, unsigned long long timestamp);

EXPORT_API void notify_pub_drop(const char *id, unsigned int audioFrames, unsigned int videoFrames);

EXPORT_API void notify_dns_refresh(const char *node, const char *service, unsigned int count);
EXPORT_API void notify_dns_dirty(void);

// 时间校正相关, SDK内部使用
EXPORT_API void set_server_local_time_diff(long long diff);
EXPORT_API int get_server_local_time_diff(long long* pdiff);

#ifdef __cplusplus
}
#endif

#endif

