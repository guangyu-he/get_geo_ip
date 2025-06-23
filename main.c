#include <stdio.h>
#include <stdlib.h>
#include <curl/curl.h>
#include "library.h"

int main(int argc, char* argv[])
{
    // 初始化libcurl
    curl_global_init(CURL_GLOBAL_DEFAULT);

    const char* test_ip = "8.8.8.8";
    if (argc > 1)
    {
        test_ip = argv[1];
    }

    printf("正在查询IP: %s\n", test_ip);

    IpGeoInfo info;
    if (get_ip_info(test_ip, &info))
    {
        print_ip_info(&info);
    }
    else
    {
        printf("查询失败\n");
    }

    // 清理libcurl
    curl_global_cleanup();

    return 0;
}