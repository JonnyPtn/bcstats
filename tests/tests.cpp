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

#define CATCH_CONFIG_MAIN
#include <catch/catch-2.hpp>

#include <json/json.hpp>

#include "HistoryAnalyzer.hpp"
#include "HistorySourceFile.hpp"
#include "HistorySourceHTTP.hpp"

namespace
{
    // Example json given
    nlohmann::json exampleJson =
    {
        {"bpi",
        {
            {"2018-01-01", 13412.44},
            {"2018-01-02", 14740.7563},
            {"2018-01-03", 15134.6513},
            {"2018-01-04", 15155.2263},
            {"2018-01-05", 16937.1738},
            {"2018-01-06", 17135.8363},
            {"2018-01-07", 16178.495},
            {"2018-01-08", 14970.3575},
            {"2018-01-09", 14439.4738},
            {"2018-01-10", 14890.7225},
            {"2018-01-11", 13287.26},
            {"2018-01-12", 13812.715},
            {"2018-01-13", 14188.785},
            {"2018-01-14", 13619.0288},
            {"2018-01-15", 13585.9013},
            {"2018-01-16", 11348.02},
            {"2018-01-17", 11141.2488},
            {"2018-01-18", 11250.6475},
            {"2018-01-19", 11514.925},
            {"2018-01-20", 12759.6413}
        }},
        {"disclaimer",
         "This data was produced from the CoinDesk Bitcoin Price Index. BPI value data returned as USD."},
        {"time", 
        {
            {"updated", "Jan 21, 2018 00:03:00 UTC"},
            {"updatedISO", "2018-01-21T00:03:00+00:00"}
        }}
    };
}

// HistoryAnalyzer tests
TEST_CASE("History json is analyzed correctly")
{
    HistoryAnalyzer analyzer;

    REQUIRE(analyzer.parse(exampleJson));
    
    SECTION("Data points populate correctly")
    {
        auto dataPoints = analyzer.getDataPoints();

        for (auto& p : dataPoints)
        {
            REQUIRE(exampleJson["bpi"][p.date] == p.price);
        }
    }

    SECTION("Stats are as expected")
    {
        auto stats = analyzer.analyze();
        REQUIRE(stats.dataSize == 20);
        REQUIRE(stats.highest.date ==  "2018-01-06");
        REQUIRE(stats.highest.price == 17135.8363);
        REQUIRE(stats.lowest.date ==  "2018-01-17");
        REQUIRE(stats.lowest.price == 11141.2488);
        REQUIRE(stats.meanPrice == Approx(13975.165275));
    }

    SECTION("Improper json is handled properly")
    {
        nlohmann::json json = "boop";
        REQUIRE(!analyzer.parse(json));

        json = {};
        REQUIRE(!analyzer.parse(json));

        json = {"beep", 12};
        REQUIRE(!analyzer.parse(json));
    }
}

// HistorySourceHTTP tests
TEST_CASE("Get history from http request")
{   
    SECTION("HTTP request succeeds with valid URI")
    {
        HistorySourceHTTP source("api.coindesk.com",
            "/v1/bpi/historical/close.json?start=2018-01-01&end=2018-01-20");

        auto data = source.get();
        REQUIRE_FALSE(!data);
        REQUIRE(*data == exampleJson);
    }

    SECTION("HTTP request fails with invalid URI")
    {
        HistorySourceHTTP source("http://beep.boop", "queery");
        REQUIRE_FALSE(!!source.get());

        HistorySourceHTTP source2("apo.beep.boop", "queery");
        REQUIRE_FALSE(!!source2.get());
    }
}

// HistorySourceFile tests
TEST_CASE("Get history from file")
{   
    // Dump the example json into a file for test purposes
    const std::string testFilePath("test.json");
    std::ofstream testFile(testFilePath, std::ios::trunc);
    testFile << exampleJson;
    testFile.close();

    SECTION("Valid file loads correctly")
    {
        HistorySourceFile source(testFilePath);
        auto data = source.get();
        REQUIRE(*source.get() == exampleJson);
    }

    SECTION("Invalid file fails cleanly")
    {
        HistorySourceFile source("");
        REQUIRE_FALSE(!!source.get());
    }
    SECTION("Invalid json fails cleanly")
    {
        testFile.open(testFilePath);
        testFile << "boop";
        testFile.close();

        HistorySourceFile source(testFilePath);
        REQUIRE_FALSE(!!source.get());
    }
}