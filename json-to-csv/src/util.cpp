#include "pch.h"
#include "util.h"

uint32_t ISO_Duration::to_min()
{
    return Min +
        H * 60 +
        D * 1440 +
        W * 10080 +
        Month * 43800 +
        Y * 525960;
}

string  ISO_Duration::to_str()
{
    string res;
    res += std::to_string(H) + "H ";
    res += std::to_string(Month) + "Min ";
    res += std::to_string(S) + "S";
    return res;
}

ISO_Duration parse_ISO_duration(const string& duration)
{
    using namespace std::regex_constants;

    auto p1 = R"((?:(\d+)Y)?(?:(\d+)M)?(?:(\d+)W)?(?:(\d+)D)?)"s;
    auto p2 = R"((?:(\d+)H)?(?:(\d+)M)?(?:(\d+)S)?)"s;
    auto r = std::format("P(?:{})?(?:T{})?", p1, p2);
    std::regex regex(r);

    ISO_Duration res{};

    std::smatch match{};
    if (not std::regex_search(duration, match, regex) or
        match.size() != 8)
    {
        auto msg = std::format("Invalid duration: '{}'"sv, duration);
        throw std::runtime_error(msg);
    }

    res.Y = match[1].matched ? std::stoul(match[1]) : 0;
    res.Month = match[2].matched ? std::stoul(match[2]) : 0;
    res.W = match[3].matched ? std::stoul(match[3]) : 0;
    res.D = match[4].matched ? std::stoul(match[4]) : 0;

    res.H = match[5].matched ? std::stoul(match[5]) : 0;
    res.Min = match[6].matched ? std::stoul(match[6]) : 0;
    res.S = match[7].matched ? std::stoul(match[7]) : 0;

    return res;
}

string UTC_to_excel(const std::string& input)
{
    std::istringstream ss(input);
    date::sys_seconds utc_tp; // sys_time associated with UTC timezone
    string timezone_name;

    const auto& stream = date::from_stream(ss, "%FT%T%Z", utc_tp, &timezone_name);
    assert(not stream.fail());
    assert(timezone_name == "Z"sv);

    auto current_zone_time = date::make_zoned(date::current_zone(), utc_tp);

    /*
    cout << "utc_time:  " << input << endl;
    cout << "rome_time: " << current_zone_time << endl;
    */

    return date::format("%d/%m/%Y,%H:%M:%S,%Z", current_zone_time);
}
