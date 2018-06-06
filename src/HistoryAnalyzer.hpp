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

#pragma once

#include <string>
#include <vector>

#include <json/json.hpp>

////////////////////////////////////////////////////////////////////////////////
// Class Responsible for analyzing json price history data and returning stats
////////////////////////////////////////////////////////////////////////////////
class HistoryAnalyzer final
{
    ////////////////////////////////////////////////////////////////////////////
    // Simple data struct to represent a data point
    // 
    // - String for date, as we don't need to manipulate it
    // - float Price
    ////////////////////////////////////////////////////////////////////////////
    struct DataPoint
    {
        std::string date;
        float price;
    };

    ////////////////////////////////////////////////////////////////////////////
    // Simple data struct to represent the resulting stats:
    //
    // - Highest price in sample
    // - Lowest price in sample
    // - The mean average price 
    // - The median average price
    // - The standard deviation of the sample
    ////////////////////////////////////////////////////////////////////////////
    struct Stats
    {
        DataPoint       highestPrice;
        DataPoint       lowestPrice;
        std::uint32_t   meanPrice;
        std::uint32_t   medianPrice;
        float           standardDeviation;
    };

    public:

    // Get the stored datapoints
    const std::vector<DataPoint>& getDataPoints() const;

    // Analyze and return the stats
    const Stats analyze() const;

    // Parse the json
    // Returns false if failure
    bool parse(nlohmann::json json);

    private:
    std::vector<DataPoint> m_dataPoints = {};

};