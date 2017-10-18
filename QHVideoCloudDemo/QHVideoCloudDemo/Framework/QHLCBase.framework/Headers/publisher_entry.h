
#ifndef __CLIENT_NET_PUBLISHER_ENTRY_H_
#define __CLIENT_NET_PUBLISHER_ENTRY_H_

#include "Types.h"
#include "Stats.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef int publish_session_t;

enum ECropMode
{
    ECROP_CENTER_SCALE,   /**< 拉伸铺满 */
    ECROP_CENTER_IN,      /**< 等比缩放，宽高比不匹配留黑边 */
    ECROP_CENTER_OUT,     /**< 等比缩放，宽高比不匹配裁剪 */
};

enum EVideoCodecID
{
    V_CODEC_ID_UNKNOWN = -1,
	
    // encoded format
    V_CODEC_ID_H264,           /**< 标准H264码流，支持annexb格式（带start code）和avcC格式(NALU前面是4字节长度) */
    V_CODEC_ID_NO_VIDEO, /**< 支持声音广播类型的直播，没有视频帧 */

    //yanjia
    V_CODEC_ID_HEVC = 0x0010,           /**< 标准Hevc码流，支持annexb格式（带start code）和avcC格式(NALU前面是4字节长度) */

    // raw data format
    V_CODEC_ID_NV21 = 0x1000,  /**< Android Camera采集都支持的格式 */
	V_CODEC_ID_I420,           /**< H264编码要求的输入格式 */
};

enum EAudioCodecID
{
    A_CODEC_ID_UNKNOWN = -1,
    A_CODEC_ID_AAC,           /**< 输入的码流已经是编好的raw AAC 桢， 需要带上ADTS头 */
    A_CODEC_ID_OPUS,          /**< Opus的裸桢 */
    A_CODEC_ID_PCM = 0x1000,  /**< 标准PCM, 16bit */
};

struct media_settings_t
{
    // video parameters
    enum EVideoCodecID inputVideoFormat;   /**< 输入码流的格式, 也就是调用publish_send_frame时发送的数据格式, h264每个IDR帧的头部必须都有sps和pps */
    //yanjia
    enum EVideoCodecID outputVideoFormat; /**输出码流格式，目前只支持264，265*/
    int annexB;                       /**< 是否是h264 annexB格式，目前内部还未智能判断 */



    // 下面这组参数是外部输入原始未编码的yuv数据，需要使用内置编码时需要设置的参数
    int sourceWidth;                  /**< 指定原始yuv数据宽 */
    int sourceHeight;                 /**< 指定原始yuv数据高 */
    int codecWidth;                   /**< 指定编码时的宽，如果不匹配的话，内部会根据cropMode进行缩放和剪裁处理 */
    int codecHeight;                  /**< 指定编码时的高 */
    int rotate;                       /**< 指定旋转角度 0，90，180，270 */
    enum ECropMode cropMode;               /**< 指定缩放和剪裁模式 */
    int avgBitrate;                   /**< 平均码率 单位bps */
    int peekBitrate;                  /**< 峰值码率 单位bps */
    int fps;                          /**< 输入帧率 */
    int useHwEncoderPriorly;          /**< 是否优先启用硬件加速编码 */
    int IDRInterval;                  /**< 关键帧间隔 */

    // audio parameters
    enum EAudioCodecID inputAudioFormat;  /**< 输入音频流格式，AAC必须含有ADTS头 */
	enum EAudioCodecID outputAudioFormat; /**< 输出音频流格式 */
    int sampleRate;                  /**< 音频采样率, 例如16000， 44100 */
    int targetBitrate;               /**< 目标码率 单位bps */
    int channelConfig;               /**< 声道配置 */
    int sampleDepth;                 /**< 采样深度, 目前支持16 bit */
    bool isHEAAC;                    /**< 是否使用HE AAC */

#ifdef __cplusplus
    media_settings_t() :
    	inputVideoFormat(V_CODEC_ID_UNKNOWN),outputVideoFormat(V_CODEC_ID_H264),
        annexB(0), sourceWidth(0), sourceHeight(0), codecWidth(0), codecHeight(0),
        rotate(0), cropMode(ECROP_CENTER_SCALE), avgBitrate(0), peekBitrate(0), fps(0),
        useHwEncoderPriorly(0), IDRInterval(60), inputAudioFormat(A_CODEC_ID_UNKNOWN), outputAudioFormat(A_CODEC_ID_UNKNOWN),
        sampleRate(0), targetBitrate(0), channelConfig(0), sampleDepth(0), isHEAAC(false)
    {}
#else
    #warning "you should init the struct media_settings_t by yourself"
#endif
};

struct publish_settings_t
{
    struct BaseSettings baseSettings;

    const char *rtmpPushAddr;    ///> 直接指定rtmp推流地址, 如果设置了合法的url，内部不再访问调度服务器
    const char *mp4FileName;     ///> 指定采集时，录制到本地文件的文件名
    bool isOnlyToFile;           ///> 是否单独录制到本地
    bool needMp4Optimize;           ///> 是否需要mp4优化
    encoded_callback_t encoded_cb; ///> 编码后数据回调函数
    void *encoded_context; ///> 编码后数据回调函数上下文

#ifdef __cplusplus
    publish_settings_t() : rtmpPushAddr(NULL), mp4FileName(NULL), isOnlyToFile(false),needMp4Optimize(false), encoded_cb(NULL), encoded_context(NULL)
    {
        // BaseSettings is a pure C structure, so we have to give the default value here
        baseSettings.sid = nullptr;
        baseSettings.event_cb = nullptr;
        baseSettings.event_context = nullptr;
        baseSettings.frame_cb = nullptr;
        baseSettings.frame_context = nullptr;
        baseSettings.retry_max = 0;
    }
#else
    #warning "you should init the struct publish_settings_t by yourself"
#endif
};

/**
 * 推流模块全局初始化
 * @return 错误码 0表示成功 否则表示失败 具体请参考Types.h
 */
EXPORT_API int publish_initialize(void);

/**
 * 推流模块全局清理
 * @return 错误码 0表示成功 否则表示失败 具体请参考Types.h
 */
EXPORT_API int publish_cleanup(void);

/**
 * 创建推流模块实例
 * @param settings 配置项
 * @return 实例句柄 <=0表示创建失败 >0表示创建成功
 */
EXPORT_API publish_session_t publish_new_session(const struct publish_settings_t *settings);

/**
 * 销毁推流模块实例
 * @param c 实例句柄
 * @param reason 关闭原因 由业务传下来
 * @param isSwitch 表示本次销毁的原因是否是由于切换
 * @return 错误码 0表示成功 否则表示失败 具体请参考Types.h
 */
EXPORT_API int publish_free_session(publish_session_t c, int reason, bool isSwitch);

/**
 * 配置推流实例中码流相关参数
 * @param c 实例句柄
 * @param settings 配置项
 * @return 错误码 0表示成功 否则表示失败 具体请参考Types.h
 */
EXPORT_API int publish_media_settings(publish_session_t c, struct media_settings_t *settings);

/**
 * 获取统计信息
 * @param c 实例句柄
 * @param stats 统计信息结构 请参考Stats.h
 * @return 错误码 0表示成功 否则表示失败 具体请参考Types.h
 */
EXPORT_API int publish_get_stats(publish_session_t c, struct stat_info_t *stats);

/**
 * 获取简要统计信息
 * @param c 实例句柄
 * @param stats 简要统计信息结构 请参考Stats.h
 * @return 错误码 0表示成功 否则表示失败 具体请参考Types.h
 */
EXPORT_API int publish_get_brief_stats(publish_session_t c, struct StatisticsBriefInfo *stats);

/**
 * 获取资源标识SN
 * @param c 实例句柄
 * @return 资源标识SN NULL表示失败
 */
EXPORT_API const char *publish_get_sn(publish_session_t c);

/**
 * 连接服务器
 * @param c 实例句柄
 * @return 错误码 0表示成功 否则表示失败 具体请参考Types.h
 */
EXPORT_API int publish_session_connect(publish_session_t c);

EXPORT_API int publish_set_water_mark_state(publish_session_t c, bool open);

EXPORT_API int publish_add_water_mark(publish_session_t c, char *wnv, int wnvLength, char *wa, int waLength, int x, int y, int w, int h);

EXPORT_API int publish_remove_all_water_mark(publish_session_t c);

/**
 * 对硬编而言，avcC格式输入的解码extraData,目前只对第三方编码起作用，android系统硬编输入已经是annexB格式
 * iOS系统需要调用publish_update_ios_avcC_extra_data
 * 首次调用publish_send_frame编码之前调用该函数或者分辨率改变之后调用该函数
 * @param c 实例句柄
 * @param data avcC格式输入的解码extraData
 * @param len extraData的长度
 * @return 错误码 0表示成功 否则表示失败 具体请参考Types.h
 */
EXPORT_API int publish_update_avcC_extra_data(publish_session_t c,void* data,int len);
EXPORT_API int publish_update_ios_avcC_extra_data(publish_session_t c,void* sps,int sps_len,void* pps,int pps_len,void* vps = 0,int vps_len = 0);

/**
 * 动态调整视频分辨率
  * @param c 实例句柄
 * @param width
 * @param height
 * @return 错误码 0表示成功 否则表示失败 具体请参考Types.h
 */
EXPORT_API int publish_update_resolution(publish_session_t c,int width,int height);
/**
 * 发送数据
 * @param c 实例句柄
 * @param frame_type 帧类型 请参考Types.h
 * @param buffer 数据
 * @param length 数据长度
 * @param pts 显示时间戳
 * @param dts 解码时间戳
 * @return 错误码 0表示成功 否则表示失败 具体请参考Types.h
 */
typedef void (*buffer_free_callback_t)(void *ptr);
EXPORT_API int publish_send_frame(publish_session_t c, int type, char *buffer, int length, unsigned long long pts, unsigned long long dts, 
                                  buffer_free_callback_t cb);
EXPORT_API int publish_send_frame2(publish_session_t c, int type, char *src_y, int src_stride_y,char *src_u, int src_stride_u,char *src_v, int src_stride_v,unsigned long long pts, buffer_free_callback_t cb);

/**
 * 启动编码后数据回调，在该函数内部会直接回调AudioSpecificConfig信息
 * @param c 实例句柄
 */
EXPORT_API void publish_start_encoded_callback(publish_session_t c);

/**
 * 停止编码后数据回调，返回后不会再有回调函数调用
 * @param c 实例句柄
 */
EXPORT_API void publish_stop_encoded_callback(publish_session_t c);

/**
 * 公有推流强制使用新的IP和端口重连
 * @param c 实例句柄
 * @param sid 会话唯一ID(应该由业务来传)
 * @param ip 新的IP
 * @param port 新的端口
 */
EXPORT_API void publish_force_reconnect_with_new(publish_session_t c, const char *sid, const char *ip, unsigned short port);

/**
 * 崩溃后使用之前落盘的信息恢复推流
 * @param c 实例句柄
 * @param info 恢复推流需要的信息
 * @param len 信息的长度
 */
EXPORT_API int publish_session_respawn(publish_session_t c, const unsigned char *info, size_t len);

/**
 * 设置自适应开关
 * @param c 实例句柄
 * @param open true表示打开 false表示关闭(默认)
 * @return 错误码 0表示成功 否则表示失败 具体请参考Types.h
 */
EXPORT_API int publish_set_autoadjust_state(publish_session_t c, bool open);

/**
 * 获取自适应调节后的码率
 * @param c 实例句柄
 * @return 码率 0表示失败 否则成功 表示自适应调节后的码率
 */
EXPORT_API int publish_get_adjust_bitrate(publish_session_t c);

/**
 * 推流暂停时调用
 * @param c 实例句柄
 */
EXPORT_API int publish_session_pause(publish_session_t c);

/**
 * 推流恢复时调用
 * @param c 实例句柄
 */
EXPORT_API int publish_session_resume(publish_session_t c);

/**
 * 发送用户自定义数据
 * @param c 实例句柄
 * @param data 数据
 * @param length 数据长度
 * @param immediate true直接发送 false同视频数据一起发送
 */
EXPORT_API int publish_session_send_userdata(publish_session_t c, char* data, int length, bool immediate);

#ifdef __cplusplus
}
#endif
#endif

