#include <json-c/json.h>
#include <stdio.h>
#include <string.h>

#include "../library.h"
#include "json_parse.h"


int parse_json_response(const char* json_data, IpGeoInfo* info)
{
    json_object* root = json_tokener_parse(json_data);
    if (root == NULL)
    {
        fprintf(stderr, "Json parse failed\n");
        return 0;
    }

    json_object* obj;

    if (json_object_object_get_ex(root, "ip", &obj))
    {
        const char* ip = json_object_get_string(obj);
        if (ip)
            snprintf(info->ip, sizeof(info->ip), "%s", ip);
    }

    if (json_object_object_get_ex(root, "city", &obj))
    {
        const char* city = json_object_get_string(obj);
        if (city)
            snprintf(info->city, sizeof(info->city), "%s", city);
    }

    if (json_object_object_get_ex(root, "region", &obj))
    {
        const char* region = json_object_get_string(obj);
        if (region)
            snprintf(info->region, sizeof(info->region), "%s", region);
    }

    if (json_object_object_get_ex(root, "country_name", &obj))
    {
        const char* country = json_object_get_string(obj);
        if (country)
            snprintf(info->country, sizeof(info->country), "%s", country);
    }

    if (json_object_object_get_ex(root, "country_code", &obj))
    {
        const char* country_code = json_object_get_string(obj);
        if (country_code)
            snprintf(info->country_code, sizeof(info->country_code), "%s", country_code);
    }

    if (json_object_object_get_ex(root, "timezone", &obj))
    {
        const char* timezone = json_object_get_string(obj);
        if (timezone)
            snprintf(info->timezone, sizeof(info->timezone), "%s", timezone);
    }

    if (json_object_object_get_ex(root, "latitude", &obj))
    {
        info->latitude = json_object_get_double(obj);
    }

    if (json_object_object_get_ex(root, "longitude", &obj))
    {
        info->longitude = json_object_get_double(obj);
    }

    if (json_object_object_get_ex(root, "org", &obj))
    {
        const char* org = json_object_get_string(obj);
        if (org)
            snprintf(info->org, sizeof(info->org), "%s", org);
    }

    if (json_object_object_get_ex(root, "error", &obj))
    {
        const char* error = json_object_get_string(obj);
        const char* reason = json_object_get_string(json_object_object_get(root, "reason"));
        if (error && reason)
        {
            fprintf(stderr, "API error: %s\n", reason);
            json_object_put(root);
            return 0;
        }
    }

    json_object_put(root);
    return 1;
}
