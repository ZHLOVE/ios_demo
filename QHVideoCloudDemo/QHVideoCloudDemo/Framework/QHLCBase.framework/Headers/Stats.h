
#ifndef __CLIENT_NET_STATS_H_
#define __CLIENT_NET_STATS_H_

/// 详细统计信息
struct stat_info_t
{
    unsigned int audio_frames_sent; ///< 音频帧发送数量
    unsigned int video_Iframes_sent; ///< IDR帧发送数量
    unsigned int video_Bframes_sent; ///< B帧发送数量
    unsigned int video_Pframes_sent; ///< P帧发送数量
    unsigned int seqFramesSent;

    unsigned int audio_frames_queued; ///< 音频帧入队列数量
    unsigned int video_Iframes_queued; ///< IDR帧入队列数量
    unsigned int video_Bframes_queued; ///< B帧入队列数量
    unsigned int video_Pframes_queued; ///< P帧入队列数量
    unsigned int seqFramesQueued;

    unsigned int audio_frames_dropped; ///< 音频帧丢帧数量
    unsigned int video_Iframes_dropped; ///< IDR帧丢帧数量
    unsigned int video_Bframes_dropped; ///< B帧丢帧数量
    unsigned int video_Pframes_dropped; ///< P帧丢帧数量
    unsigned int seqFramesDropped;

    unsigned int audio_frames_recv; ///< 音频帧接收数量
    unsigned int video_Iframes_recv; ///< IDR帧接收数量
    unsigned int video_Bframes_recv; ///< B帧接收数量
    unsigned int video_Pframes_recv; ///< P帧接收数量

    unsigned long long bytes_sent; ///< 发送的字节数
    unsigned long long bytes_queued; ///< 入队列的字节数
    unsigned long long bytes_dropped; ///< 总丢帧字节数
    unsigned long long bytes_dropped_in5sec; ///< 5秒内丢帧字节数
    unsigned long long bytes_recv; ///< 接收的字节数

    unsigned long long start_frame_ab_ts; ///< 首帧绝对时间
    unsigned long long start_frame_pts; ///< 首帧时间戳
    unsigned long long last_frame_pts; ///< 最后一帧时间戳
    unsigned int queue_length; ///< 队列长度
    unsigned long long queue_bytes; ///< 队列数据大小

    unsigned int greater100ms; ///< 帧间隔大于100ms的间隔次数
    unsigned int greater200ms; ///< 帧间隔大于200ms的间隔次数
    unsigned int greater300ms; ///< 帧间隔大于300ms的间隔次数
    unsigned int conTag; ///< 连接标记

    char ip[256];
    int port;
};

// 一些简要信息
struct StatisticsBriefInfo
{
    enum Status
    {
        BREAK_OFF = 0, ///< 连接已经断开
        GOOD = 1, ///< 码率正常且无丢帧和堆积
        NOT_GOOD, ///< 无丢帧和堆积但码率较低
        AWFUL ///< 有丢帧或堆积
    } status;
    double averageFPS; ///< 平均帧率（总发送的帧数/以帧的时间戳计算的时间间隔）
    double absoluteBPS; ///< 平均码率（总发送的字节数/绝对时间间隔）
};

#endif

