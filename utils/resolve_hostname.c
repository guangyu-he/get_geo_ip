#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <ctype.h>
#include <stdbool.h>

#include "resolve_hostname.h"


bool resolve_hostname(const char* hostname, char* ip)
{
    // Only support IPv4
    struct addrinfo hints, *result;

    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;

    if (getaddrinfo(hostname, NULL, &hints, &result)) return false;

    const struct sockaddr_in* ipv4 = (struct sockaddr_in*)result->ai_addr;
    inet_ntop(AF_INET, &(ipv4->sin_addr), ip, INET_ADDRSTRLEN);
    freeaddrinfo(result);

    return true;
}

bool is_valid_domain(const char* domain)
{
    const size_t len = strlen(domain);

    if (len == 0 || len > 253) return false;
    if (domain[0] == '.' || domain[len - 1] == '.') return false;

    int label_len = 0;
    int has_alpha = 0;

    for (int i = 0; i < len; i++)
    {
        const char c = domain[i];

        if (c == '.')
        {
            if (label_len == 0 || label_len > 63) return false;
            label_len = 0;
            has_alpha = 0;
        }
        else if (isalnum(c) || c == '-')
        {
            label_len++;
            if (isalpha(c)) has_alpha = 1;

            if (c == '-' && (i == 0 || domain[i + 1] == '.' || i == len - 1))
            {
                return false;
            }
        }
        else
        {
            return false;
        }
    }

    if (label_len == 0 || label_len > 63) return false;
    if (has_alpha == 0) return false;

    return true;
}
