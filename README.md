# Get Geo IP

A C library and command-line tool for retrieving geographical information about IP addresses and domain names.

## Features

- Get geographical information for IP addresses
- Support for domain name resolution
- IP address validation
- JSON response parsing from ipapi.co
- Static library for FFI integration
- Command-line interface

## Dependencies

- libcurl
- json-c
- CMake 3.25+

## Installation

### macOS (Homebrew)

```bash
brew install curl json-c cmake
```

### Build

```bash
mkdir build && cd build
cmake ..
make
```

## Usage

### Command Line

```bash
# Get info for a specific IP
./get_geo_ip 8.8.8.8

# Get info for a domain
./get_geo_ip google.com

# Use default IP (8.8.8.8)
./get_geo_ip
```

### Library Usage

```c
#include "library.h"

IpGeoInfo info;
if (get_ip_info("8.8.8.8", &info)) {
    print_ip_info(&info);
}
```

## Output Format

```
IP Geo Info:
  IP: 8.8.8.8
  City: Mountain View
  Region: California  
  Country: United States (US)
  Timezone: America/Los_Angeles
  Latitude: 37.419200
  Longitude: -122.057400
  Org: Google LLC
```

## API

### Core Functions

- `bool get_ip_info(const char* ip, IpGeoInfo* info)` - Retrieve geo information
- `void print_ip_info(const IpGeoInfo* info)` - Print formatted output

### Utility Functions

- `bool validate_ip_address(const char* ip)` - Validate IP format
- `bool resolve_hostname(const char* hostname, char* ip_str)` - Resolve domain to IP
- `int parse_json_response(const char* json_data, IpGeoInfo* info)` - Parse API response

## Data Source

This tool uses the [ipapi.co](https://ipapi.co) API for geographical data.

## License

Open source project for educational and development purposes.