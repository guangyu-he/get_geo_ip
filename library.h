#ifndef GET_GEO_IP_LIBRARY_H
#define GET_GEO_IP_LIBRARY_H
#include <stdbool.h>

typedef struct
{
    char* data;
    size_t size;
} HttpResponse;

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
    bool success;
} IpGeoInfo;

bool get_ip_info(const char* ip, IpGeoInfo* info);
void print_ip_info(const IpGeoInfo* info);

#ifdef __cplusplus
}
#endif

#endif //GET_GEO_IP_LIBRARY_H
