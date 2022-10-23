#include "pch.h"

struct ISO_Duration
{
    uint32_t Y;
    uint32_t M;
    uint32_t W;
    uint32_t D;

    uint32_t H;
    uint32_t Min;
    uint32_t S;
};

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

    return res;
}

string UTC_to_excel(const std::string& input)
{
    std::istringstream ss(input);
    date::sys_seconds utc_tp; // sys_time "include" UTC timezone
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

int main()
{
    try
    {

        //date::set_install(".\\data\\tzdata2022e");
        //auto& tzdb = date::get_tzdb();
        //std::cout << "tzdb version: " << tzdb.version << endl;

        //auto input = "2022-10-14T18:11:16Z";
        //cout << "input:  " << input << endl;
        //cout << "output: " << UTC_to_excel(input) << endl;

        auto res = parse_ISO_duration("P120YT1H3M13S");
        

        return 0;
    }
    catch (const std::exception& e)
    {
        cout << e.what() << endl;
    }

    return 1;
}
