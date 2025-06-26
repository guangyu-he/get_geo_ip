#ifndef RESOLVE_HOSTNAME_H
#define RESOLVE_HOSTNAME_H
#include <stdbool.h>

int resolve_hostname(const char* hostname, char* ip);
bool is_valid_domain(const char* domain);

#endif //RESOLVE_HOSTNAME_H
