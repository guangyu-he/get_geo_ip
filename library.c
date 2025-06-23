#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <curl/curl.h>
#include <json-c/json.h>
#include <arpa/inet.h>

#include "library.h"
#include "utils/json_parse.h"
#include "utils/validate_ip.h"


// 回调函数，用于接收HTTP响应数据
static size_t write_callback(const void* contents, const size_t size, size_t nmemb, HttpResponse* response)
{
    size_t total_size = size * nmemb;
    char* ptr = realloc(response->data, response->size + total_size + 1);

    if (ptr == NULL)
    {
        printf("内存分配失败!\n");
        return 0;
    }

    response->data = ptr;
    memcpy(&(response->data[response->size]), contents, total_size);
    response->size += total_size;
    response->data[response->size] = 0;

    return total_size;
}


int get_ip_info(const char* ip, IpGeoInfo* info)
{
    // init struct
    memset(info, 0, sizeof(IpGeoInfo));

    // validate ip
    if (!validate_ip_address(ip))
    {
        fprintf(stderr, "IP invalid: %s\n", ip);
        return 0;
    }

    // init libcurl
    HttpResponse response = {0};
    CURL* curl = curl_easy_init();
    if (!curl)
    {
        fprintf(stderr, "curl init failed\n");
        return 0;
    }

    // build url
    char url[256];
    snprintf(url, sizeof(url), "https://ipapi.co/%s/json/", ip);

    // set url options
    curl_easy_setopt(curl, CURLOPT_URL, url);
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_callback);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response);
    curl_easy_setopt(curl, CURLOPT_USERAGENT, "gy_c_toolbox");
    curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);
    curl_easy_setopt(curl, CURLOPT_TIMEOUT, 30L);

    // execute request
    const CURLcode res = curl_easy_perform(curl);

    if (res != CURLE_OK)
    {
        fprintf(stderr, "HTTP request failed: %s\n", curl_easy_strerror(res));
        curl_easy_cleanup(curl);
        if (response.data) free(response.data);
        return 0;
    }

    // check response
    long response_code;
    curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &response_code);

    if (response_code != 200)
    {
        fprintf(stderr, "API请求失败，状态码: %ld\n", response_code);
        curl_easy_cleanup(curl);
        if (response.data) free(response.data);
        return 0;
    }

    curl_easy_cleanup(curl);

    // 解析JSON响应
    int parse_success = 0;
    if (response.data)
    {
        parse_success = parse_json_response(response.data, info);
        free(response.data);
    }

    if (parse_success)
    {
        info->success = 1;
        return 1;
    }
    else
    {
        return 0;
    }
}

// 打印IP地理信息
void print_ip_info(const IpGeoInfo* info)
{
    if (!info->success)
    {
        printf("获取IP信息失败\n");
        return;
    }

    printf("IP地理信息:\n");
    printf("  IP地址: %s\n", info->ip);
    printf("  城市: %s\n", info->city);
    printf("  地区: %s\n", info->region);
    printf("  国家: %s (%s)\n", info->country, info->country_code);
    printf("  时区: %s\n", info->timezone);
    printf("  纬度: %.6f\n", info->latitude);
    printf("  经度: %.6f\n", info->longitude);
    printf("  组织: %s\n", info->org);
}
