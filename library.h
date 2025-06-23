#ifndef GET_GEO_IP_LIBRARY_H
#define GET_GEO_IP_LIBRARY_H

#ifdef __cplusplus
extern "C" {
#endif

// HTTP响应数据结构
typedef struct
{
    char* data;
    size_t size;
} HttpResponse;

// IP地理信息结构体
typedef struct
{
    char ip[16];
    char city[64];
    char region[64];
    char country[64];
    char country_code[8];
    char timezone[32];
    double latitude;
    double longitude;
    char org[128];
    int success;
} IpGeoInfo;

// 函数声明
int get_ip_info(const char* ip, IpGeoInfo* info);
void print_ip_info(const IpGeoInfo* info);

#ifdef __cplusplus
}
#endif

#endif //GET_GEO_IP_LIBRARY_H
