#include <stdio.h>
#include <stdlib.h>
#include <curl/curl.h>
#include "library.h"

int main(const int argc, char* argv[])
{
    // init libcurl
    curl_global_init(CURL_GLOBAL_DEFAULT);

    const char* test_ip = "8.8.8.8";
    if (argc > 1)
    {
        test_ip = argv[1];
    }

    IpGeoInfo info;
    if (get_ip_info(test_ip, &info))
    {
        print_ip_info(&info);
    }
    else
    {
        fprintf(stderr, "Check IP failed\n");
    }

    // Cleanup
    curl_global_cleanup();

    return 0;
}
