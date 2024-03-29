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
            "published_at," << "timezone," << "url" << endl;

        auto const& channel_name = json["title"].get<string>();

        for (auto const& item : json["items"])
        {
            auto video_title = item["title"].get<string>();
            std::replace(video_title.begin(), video_title.end(),
                         '"', ' ');

            auto const video_id = item["videoId"].get<string>();
            auto const duration = parse_ISO_duration(item["duration"].get<string>());
            auto const duration_min = duration.to_min();
            auto [published_day, published_at, timezone] = UTC_to_excel(item["publishedAt"].get<string>());

            {
                // just some mumbo jumbo, don't mind me
                std::tm t{};
                std::istringstream is(published_at);
                //is.imbue(std::locale("de_DE.utf-8"));
                is >> std::get_time(&t, "%H:%M");
                
                t.tm_min = (t.tm_min / 10) * 10; // truncate

                std::ostringstream os;
                os << std::put_time(&t, "%H:%M");
                
                published_at = os.str();
            }

            if (duration_min > 1 && duration_min < 30)
            {
                bool parliamone_video =
                    video_title.find("Parliamone") != string::npos;

                if (not parliamone_video)
                {
                    auto csv_line = std::format(R"("{}","{}",{},{},{},{},https://www.youtube.com/watch?v={})",
                                                channel_name, video_title,
                                                duration_min, published_day,
                                                published_at, timezone, video_id);
                    ofs << csv_line << endl;
                }
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
