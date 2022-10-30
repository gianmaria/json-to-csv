#include "pch.h"

struct ISO_Duration
{
    uint32_t Y;
    uint32_t Month;
    uint32_t W;
    uint32_t D;

    uint32_t H;
    uint32_t Min;
    uint32_t S;

    uint32_t to_min()
    {
        return Min +
            H * 60 +
            D * 1440 +
            W * 10080 +
            Month * 43800 +
            Y * 525960;
    }

    string to_str()
    {
        string res;
        res += std::to_string(H) + "H ";
        res += std::to_string(Month) + "Min ";
        res += std::to_string(S) + "S";
        return res;
    }
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

int main()
{
    try
    {
        date::set_install(".\\data\\tzdata2022e");
        //auto& tzdb = date::get_tzdb();
        //std::cout << "tzdb version: " << tzdb.version << endl;

        //auto input = "2022-10-14T18:11:16Z";
        //cout << "input:  " << input << endl;
        //cout << "output: " << UTC_to_excel(input) << endl;

        //auto res = parse_ISO_duration("P120YT1H3M13S");

        auto filepath = "data\\breakingitaly.json";
        std::ifstream ifs(filepath);
        if (not ifs.is_open())
        {
            throw std::runtime_error(std::format("[ERROR] Cannot open file '{}' for reading", filepath));
        }

        auto json = njson::parse(ifs);

        auto dest_filepath = "breakingitaly.csv";
        std::ofstream ofs(dest_filepath, std::ofstream::trunc);
        if (not ofs.is_open())
        {
            throw std::runtime_error(std::format("[ERROR] Cannot open file '{}' for writing", dest_filepath));
        }

        ofs << "channel_name," <<
            "video_title," <<
            "duration_min," <<
            "published_day," <<
            "published_at," <<
            "timezone" << endl;

        auto const& channel_title = json["title"].get<string>();

        for (auto const& item : json["items"])
        {
            auto const& video_title = item["title"].get<string>();
            auto duration = parse_ISO_duration(item["duration"].get<string>());
            auto duration_min = duration.to_min();
            auto published_at = UTC_to_excel(item["publishedAt"].get<string>());

            if (duration_min > 1 && duration_min < 30)
                ofs << channel_title << "," << video_title << "," << duration_min << "," << published_at << endl;
            else
                cout << std::format("[INFO] Skipped: <{}> duration {}", video_title, duration.to_str()) << endl; // skip longer / shorter video
        }

        return 0;
    }
    catch (const std::exception& e)
    {
        cout << e.what() << endl;
    }

    return 1;
}
