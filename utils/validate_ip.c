//
// Created by Guangyu He on 23.06.25.
//
#include <arpa/inet.h>

#include "validate_ip.h"


// 验证IP地址格式
int validate_ip_address(const char* ip)
{
    struct sockaddr_in sa;
    struct sockaddr_in6 sa6;

    // 检查IPv4
    if (inet_pton(AF_INET, ip, &(sa.sin_addr)) == 1)
    {
        return 1;
    }

    // 检查IPv6
    if (inet_pton(AF_INET6, ip, &(sa6.sin6_addr)) == 1)
    {
        return 1;
    }

    return 0;
}
