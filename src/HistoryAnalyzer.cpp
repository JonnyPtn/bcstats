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

#include <iostream>

#include "HistoryAnalyzer.hpp"

////////////////////////////////////////////////////////////////////////////////
bool HistoryAnalyzer::parse(nlohmann::json json)
{
    if (json.is_null() || json.is_discarded())
    {
        std::cout << "json is null, failed to parse" << std::endl;
        return false;
    }

    // Check for "bpi" array
    if (!json.count("bpi"))
    {
        std::cout << "bpi data not found in json" << std::endl;
        return false;
    }

    // Allocate memory for it
    m_dataPoints.reserve(json["bpi"].size());

    // Iterate through it, populating our data points
    for (auto& dp : json["bpi"].items())
    {
        m_dataPoints.push_back({dp.key(),dp.value()});
    }

    // Sort the datapoints by price to make analyzing easier
    std::sort(m_dataPoints.begin(), m_dataPoints.end(),
    [](DataPoint a, DataPoint b)
    {
        return a.price > b.price;
    });

    return true;
}

////////////////////////////////////////////////////////////////////////////////
const HistoryAnalyzer::Stats HistoryAnalyzer::analyze() const
{
    Stats stats;
    stats.dataSize = m_dataPoints.size();

    // Comparison function for datapoints
    auto dpCompare = [](DataPoint a, DataPoint b)
    {
        return a.price < b.price;
    };

    // Find highest and lowest
    stats.highest = *std::max_element(m_dataPoints.begin(), m_dataPoints.end(), dpCompare);
    stats.lowest = *std::min_element(m_dataPoints.begin(), m_dataPoints.end(), dpCompare);

    // Calculate mean and median
    auto sum = std::accumulate(m_dataPoints.begin(),m_dataPoints.end(), 0., 
    [](double total, DataPoint next)
    {
        return total + next.price;
    });

    stats.meanPrice = sum / m_dataPoints.size();

    auto medianIndex = (stats.dataSize + 1)/2;
    stats.medianPrice = m_dataPoints[medianIndex].price;

    // Simple std dev calculation
    double totalDev = std::accumulate(m_dataPoints.begin(), m_dataPoints.end(), 0.,
    [&stats] (double f, DataPoint next)
    {
        return f + std::fabs(std::pow(next.price - stats.meanPrice,2));
    });

    stats.standardDeviation = std::sqrt((totalDev / (stats.dataSize - 1)));

    return stats;
}

////////////////////////////////////////////////////////////////////////////////
const std::vector<HistoryAnalyzer::DataPoint>& HistoryAnalyzer::getDataPoints() const
{
    return m_dataPoints;
}
