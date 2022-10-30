#include "pch.h"
#include "json_to_csv.h"

[[noreturn]] void test()
{
    auto filepath = "data\\breakingitaly.json";
    std::ifstream ifs(filepath);
    if (not ifs.is_open())
    {
        throw std::runtime_error(std::format("[ERROR] Cannot open file '{}' for reading", filepath));
    }

    auto json = njson::parse(ifs);

    [[maybe_unused]] auto const& channel_title = json["title"].get<string>();

    for (auto const& item : json["items"])
    {
        auto const& video_title = item["title"].get<string>();
        auto duration = parse_ISO_duration(item["duration"].get<string>());
        [[maybe_unused]] auto duration_min = duration.to_min();
        auto published_at = UTC_to_excel(item["publishedAt"].get<string>());

        cout << std::format("[INFO] <{}> duration {}", video_title, duration.to_str()) << endl;
        [[maybe_unused]] int stop = 0;
    }

    std::exit(0);
}

int main()
{
    try
    {
        date::set_install(".\\data\\tzdata2020a");
        //date::get_tzdb().version;
        
        //test();

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

        ofs << "channel_name," << "video_title," <<
            "duration_min," << "published_day," <<
            "published_at," << "timezone" << endl;

        auto const& channel_name = json["title"].get<string>();

        for (auto const& item : json["items"])
        {
            auto const& video_title = item["title"].get<string>();
            auto const duration = parse_ISO_duration(item["duration"].get<string>());
            auto const duration_min = duration.to_min();
            auto const [published_day, published_at, timezone] = UTC_to_excel(item["publishedAt"].get<string>());

            if (duration_min > 1 && duration_min < 30)
            {
                auto csv_line = std::format(R"("{}","{}",{},{},{},{})",
                                            channel_name, video_title, 
                                            duration_min, published_day,
                                            published_at, timezone);
                ofs << csv_line << endl;
            }
            else
            {
                cout << std::format("[INFO] Skipped: <{}> duration {}", video_title, duration.to_str()) << endl; // skip longer / shorter video
            }
        }

        return 0;
    }
    catch (const std::exception& e)
    {
        cout << e.what() << endl;
    }

    return 1;
}
