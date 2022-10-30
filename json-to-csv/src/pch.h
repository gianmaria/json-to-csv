#pragma once

#include <algorithm>   
#include <chrono>
#include <exception>
#include <filesystem>
#include <format>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <regex>
#include <string_view>
#include <string>
#include <vector>

using std::cout;
using std::endl;
using std::string;
using std::string_view;
using namespace std::string_view_literals;
using namespace std::string_literals;

namespace fs = std::filesystem;

#include <date/date.h>
#include <date/tz.h>

#define JSON_USE_IMPLICIT_CONVERSIONS 0
#include <nlohmann/json.hpp>

using njson = nlohmann::json;
