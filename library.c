#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <curl/curl.h>
#include <json-c/json.h>
#include <arpa/inet.h>

#include "library.h"
#include "utils/json_parse.h"
#include "utils/validate_ip.h"
#include "utils/resolve_hostname.h"


static size_t write_callback(const void* contents, const size_t size, size_t nmemb, HttpResponse* response)
{
    size_t total_size = size * nmemb;
    char* ptr = realloc(response->data, response->size + total_size + 1);

    if (ptr == NULL)
    {
        printf("Memory allocation failed!\n");
        return 0;
    }

    response->data = ptr;
    memcpy(&(response->data[response->size]), contents, total_size);
    response->size += total_size;
    response->data[response->size] = 0;

    return total_size;
}


bool get_ip_info(const char* ip, IpGeoInfo* info)
{
    // init struct
    memset(info, 0, sizeof(IpGeoInfo));
    char ip_str[INET_ADDRSTRLEN];

    // validate ip
    if (!validate_ip_address(ip))
    {
        fprintf(stderr, "IP invalid: %s\n", ip);
        return false;
    }
    if (!resolve_hostname(ip, ip_str))
    {
        fprintf(stderr, "Domain resolve failed: %s\n", ip);
        return false;
    }

    // init libcurl
    HttpResponse response = {0};
    CURL* curl = curl_easy_init();
    if (!curl)
    {
        fprintf(stderr, "curl init failed\n");
        return false;
    }

    // build url
    char url[256];
    snprintf(url, sizeof(url), "https://ipapi.co/%s/json/", ip_str);

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
        return false;
    }

    // check response
    long response_code;
    curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &response_code);

    if (response_code != 200)
    {
        fprintf(stderr, "HTTP response code: %ld\n", response_code);
        curl_easy_cleanup(curl);
        if (response.data) free(response.data);
        return false;
    }

    curl_easy_cleanup(curl);

    // parse response
    int parse_success = 0;
    if (response.data)
    {
        parse_success = parse_json_response(response.data, info);
        free(response.data);
    }

    if (parse_success)
    {
        info->success = true;
        return true;
    }

    return false;
}

void print_ip_info(const IpGeoInfo* info)
{
    if (!info->success)
    {
        printf("Get IP info failed\n");
        return;
    }

    printf("IP Geo Info:\n");
    printf("  IP: %s\n", info->ip);
    printf("  City: %s\n", info->city);
    printf("  Region: %s\n", info->region);
    printf("  Country: %s (%s)\n", info->country, info->country_code);
    printf("  Timezone: %s\n", info->timezone);
    printf("  Latitude: %.6f\n", info->latitude);
    printf("  Longitude: %.6f\n", info->longitude);
    printf("  Org: %s\n", info->org);
}
