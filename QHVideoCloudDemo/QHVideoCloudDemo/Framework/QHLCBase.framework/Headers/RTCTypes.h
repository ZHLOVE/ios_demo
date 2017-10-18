
#ifndef __CLIENT_NET_RTC_TYPES_H_
#define __CLIENT_NET_RTC_TYPES_H_

#include "Types.h"

typedef int CORE_ROLE_HANDLE;

typedef enum
{
    STATUS_AUDIO_DISABLE = 0, ///< 禁止声音
    STATUS_VIDEO_DISABLE = 1, ///< 禁止图像
    STATUS_MAX = 4
} MultiPathStatus;

typedef struct
{
    unsigned int m_cid;
    unsigned int m_status;
} StatusToUpper;

typedef struct
{
    char m_name[64];
    unsigned int m_cid;
    unsigned int m_fullStatus;
} UserOne;

///< 连麦中角色选定方式
typedef enum 
{
    ROLETYPE_SINGLE = 1, ///< 选定某一个角色
    ROLETYPE_ALL_EXCEPT_ANCHOR, ///< 选定除了主播以外的所有嘉宾
    ROLETYPE_ALL, ///< 选定主播和所有嘉宾
} RoleType;

/**
 * 视频帧回调函数原型
 * @param c 实例句柄
 * @param type 帧类型
 * @param buffer 数据
 * @param length 数据长度
 * @param ts 时间戳
 * @param context 上下文
 */
typedef void (*video_frame_callback_t)(CORE_HANDLE c, enum EFrameType type, const char *buffer, int length, unsigned int ts, void *context);

/**
 * PCM帧回调函数原型
 * @param c 实例句柄
 * @param buffer The actual 16bit audio data
 * @param len the data's length
 * @param bytes_per_sample bytes of per sample
 * @param sample_rate Sample rate in Hz
 * @param channels Number of channels in the audio data
 * @param timestamp The RTP timestamp of the first sample
 * @param context 上下文
 */
typedef void (*pcm_frame_callback_t)(CORE_HANDLE c, const short *buffer, size_t len, 
                                     size_t bytes_per_sample, int sample_rate, size_t channels, unsigned int timestamp,
                                     void *context);

typedef struct
{
    int anchor; ///< 是否是主播，1表示是 0表示不是
    int start_bitrate_kbps; // 透传WebRTC参数, 初始码率

    const char *sid; ///< 实例唯一ID
    const char *name; ///< 用户昵称
    event_callback_t eventCB; ///< 事件回调函数
    void *eventContext; ///< 事件回调函数context
} CallConfig;

typedef enum
{
    PCMU_8K = 1,
    PCMU_16K,
    PCMU_32K,
    PCMU_48K,
    OPUS_8K,
    OPUS_16K,
    OPUS_48K
} AudioType;

typedef struct
{
    // 透传WebRTC参数    
    int width;
    int height;
    int fps;
    int encoder_bitrate_kbps;
    
    int encode_from_texture;
    int ssrc_v;
    int ssrc_a;
    
#ifdef WEBRTC_ANDROID
    int use_hardware_codec;	
#endif
    AudioType audio_type;

    int enable_aec; // 0关 1开
    int aec_level;  // 0低 1中 2高
    int enable_cap_agc;
    int cap_agc_level_db; // 0 ~ 31
    int cap_agc_gain_db; // 0 ~ 90
    int enable_cap_agc_limiter;

    int enable_render_agc;
    int render_agc_level_db; // 0 ~ 31
    int render_agc_gain_db; // 0 ~ 90
    int enable_render_agc_limiter;
    
    int enable_ns;
    int ns_level; // 0低 1中 2高 3超高

    int enalbe_high_pass;

    int enable_vad; 
} SendConfig;

typedef struct
{
    void *render;
#ifdef WEBRTC_ANDROID
    int use_hardware_decoder;
#endif
    video_frame_callback_t videoFrameCB; ///< 视频帧回调函数
    void *videoFrameContext; ///< 视频帧回调函数context
    pcm_frame_callback_t pcmFrameCB; ///< PCM帧回调函数
    void *pcmFrameContext; ///< PCM帧回调函数context
} ReceiveConfig;

typedef struct {
  int key_frames;
  int delta_frames;
} RTCFrameCounts;

typedef struct {
  unsigned char fraction_lost;
  unsigned int cumulative_lost;
  unsigned int extended_max_sequence_number;
  unsigned int jitter;
} RtcpStatistics;

typedef struct
{
    bool enable;
    int codec_type;
//  std::string codec_name;
    int codec_freq;
    long long bytes_sent; // 发送字节数
    int packets_sent; // 发送包数
    int packets_lost; // 丢包数
    float cumulate_lost; // 累计丢包数
    float fraction_lost; // 周期内丢包数
    long long rtt_ms; // rtt时间
} AudioSendStatus;

typedef struct
{
    bool enable;
    bool use_hw_encoder;
    int input_frame_rate; // 输入帧率
    int encode_frame_rate; // 编码码率
    int avg_encode_time_ms; // 平均编码耗时
    unsigned int frames_encoded; // 已编码帧数
    unsigned int payload_bytes; // 已编码字节数
    int target_media_bitrate_bps; // 目标码率
    int media_bitrate_bps; // 编码码率
    int width; // 
    int height;
} VideoSendStatus;

typedef struct
{
    bool enable;
    long long bytes_rcvd; // 接收字节数
    unsigned int packets_rcvd; // 接收包数
    unsigned int packets_lost; // 丢包数
    float cumulate_lost; // 累计丢包数
    float fraction_lost; // 周期内丢包数
    int codec_type; // 编码类型
//    std::string codec_name;
    int codec_freq; // 采样率
    unsigned int delay_estimate_ms; // 延迟
} AudioRecvStatus;

typedef struct
{
    bool enable;
    bool use_hw_decoder;
    int decode_frame_rate; //
    int render_frame_rate; //
    int key_frames; // 关键帧数
    int delta_frams; // 非关键帧数量
    int frames_decoded; // 已解码数
    int total_bitrate_bps; // 接收码率
    int width; 
    int height;
    int current_delay_ms; // 延时
    int min_playout_delay_ms; // 输出延时
} VideoRecvStatus;

#endif

