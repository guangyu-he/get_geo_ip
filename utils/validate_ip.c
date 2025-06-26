#include <arpa/inet.h>
#include <stdbool.h>

#include "validate_ip.h"
#include "resolve_hostname.h"


bool validate_ip_address(const char* ip)
{
    struct sockaddr_in sa;
    struct sockaddr_in6 sa6;

    // Check IPv4
    if (inet_pton(AF_INET, ip, &(sa.sin_addr)) == 1)
    {
        return true;
    }

    // Check IPv6
    if (inet_pton(AF_INET6, ip, &(sa6.sin6_addr)) == 1)
    {
        return true;
    }

    // Check if it is a domain
    if (is_valid_domain(ip))
    {
        return true;
    }

    return false;
}
