#include "pch.h"

struct ISO_Duration
{
    uint32_t H;
    uint32_t M;
    uint32_t S;
};

ISO_Duration parse_ISO_duration(string_view duration)
{
    // example: PT1H3M13S
    ISO_Duration res{};

    if (duration[0] != 'T')
    {
        auto msg = std::format("Invalid duration: {}"sv, duration);
        throw std::runtime_error(msg);
    }

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
        date::set_install(".\\data\\tzdata2022e");
        auto& tzdb = date::get_tzdb();
        std::cout << "tzdb version: " << tzdb.version << endl;

        auto input = "2022-10-14T18:11:16Z";
        cout << "input:  " << input << endl;
        cout << "output: " << UTC_to_excel(input) << endl;

        return 0;
    }
    catch (const std::exception& e)
    {
        cout << e.what() << endl;
    }

    return 1;
}
