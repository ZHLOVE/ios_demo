
#ifndef __CLIENT_NET_CORE_ENTRY_H_
#define __CLIENT_NET_CORE_ENTRY_H_

#include "Types.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * 通知SDK app启动
 * 必须在app启动的第一时间调用，且在整个app生命周期只调用一次
 * @param bid 业务ID
 * @param pid 平台端ID
 * @param ver 端版本
 * @param os 系统类型和版本
 * @param mid 机器id
 * @param model 型号
 * @param urlCC 云控地址 格式参考默认地址: http://sdk.live.360.cn/
 */
EXPORT_API void core_on_app_start(const char *bid, const char *pid, const char *ver, const char *os, const char *mid, const char *model, 
                                  const char *urlCC);
typedef enum
{
    NC_TYPE_TOWIFI, /**< 移动网络变为WIFI */
    NC_TYPE_OTHERS /**< 其他网络变化类型 */
} NetworkChangeType;

/**
 * 网络变化时通知传输层进行相应处理
 * @param type 网络变化类型
 */
EXPORT_API void core_network_change(NetworkChangeType type);

EXPORT_API int core_getaddrinfo(const char *node, const char *service,
                                const struct addrinfo *hints,
                                struct addrinfo_dns *addr);

#ifdef __cplusplus
}
#endif

#endif

