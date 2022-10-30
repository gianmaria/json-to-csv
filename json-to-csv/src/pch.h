#pragma once

#include <filesystem>
#include <fstream>
#include <iostream>
#include <string>
#include <string_view>
#include <vector>
#include <chrono>
#include <exception>
#include <format>
#include <regex>

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