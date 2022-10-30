#include "pch.h"
#include "json_to_csv.h"

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
