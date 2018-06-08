////////////////////////////////////////////////////////////////////////////////
// MIT License
// 
// Copyright (c) 2018 Jonny Paton
// 
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
// 
// The above copyright notice and this permission notice shall be included in 
// all copies or substantial portions of the Software.
// 
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.
////////////////////////////////////////////////////////////////////////////////

#include <regex>

#include <cxxopts/cxxopts.hpp>

#include "HistorySourceFile.hpp"
#include "HistorySourceHTTP.hpp"
#include "HistoryAnalyzer.hpp"

using std::operator ""s;

int main(int argc, const char *argv[])
{
    // Set up the program options
    cxxopts::Options options(argv[0], "A tool for generating stats based on bitcoin price history");

    options
    .show_positional_help()
    .add_options()
    ("h,help", "Show this help")
    ("v,verbose", "Verbose output")
    ("f,file", "JSON file containing history data to analyze", cxxopts::value<std::string>())
    ("r,range", "Date range to analyze data for [FROM TO] (YYYY-MM-DD)", cxxopts::value<std::vector<std::string>>()) ;

    try
    {
        options.parse_positional("range");
        auto result = options.parse(argc,argv);
        // Help option
        if (result.count("help"))
        {
            std::cout << options.help();
            return 0;
        }

        // Determine which source to use
        std::unique_ptr<HistorySource> source;

        if (result.count("file"))
        {
            source = std::make_unique<HistorySourceFile>(result["file"].as<std::string>());
        }
        else
        {
            // If no file provided, it's an HTTP request
            auto host = "api.coindesk.com"s;
            auto query = "/v1/bpi/historical/close.json"s;

            // Check for range date param
            if (result.count("range"))
            {
                // Validate Dates
                auto& dates = result["range"].as<std::vector<std::string>>();

                if (dates.size() !=2)
                {
                    std::cout << "Please provide 2 dates for range, in the format \"YYYY-MM-DD YYYY-MM-DD\"" << std::endl;
                    return 1;
                }

                // Set query params
                query.append("?start=" + dates[0]);
                query.append("&end=" + dates[1]);
            }
            else
            {
                // No range provided, API will return previous 31 days by default
                std::cout << "Using data from previous 31 days" << std::endl;
            }
            source = std::make_unique<HistorySourceHTTP>(host, query);
        }

        auto data = source->get();

        if (!data)
        {
            std::cout << "Failed to get source data" << std::endl;
            return 1;
        }

        HistoryAnalyzer analyzer;
        analyzer.parse(*data);

        // If verbose output, spit out the raw data
        if (result.count("verbose"))
        {
            auto& rawData = analyzer.getDataPoints();
            for (auto& p : rawData)
            {
                std::cout << p.date << ": " << p.price << std::endl;
            }
        }
        // Output stats
        auto stats = analyzer.analyze();
        std::cout << "Stats for data:" << std::endl

        << "Total samples: " << stats.dataSize << std::endl

        << "Highest price was $" << stats.highest.price 
        << " on " << stats.highest.date << std::endl

        << "Lowest price was $" << stats.lowest.price
        << " on " << stats.lowest.date << std::endl

        << "Mean price was $" << stats.meanPrice << std::endl

        << "Median price was $" << stats.medianPrice << std::endl

        << "Standard deviation of $" << stats.standardDeviation << std::endl;
    }
    catch(cxxopts::OptionException& e)
    {
        std::cout << e.what() << std::endl;;
        return 1;
    }
    return 0;
}
