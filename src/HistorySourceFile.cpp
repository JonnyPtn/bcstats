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

#include <fstream>
#include <iostream>

#include "HistorySourceFile.hpp"

////////////////////////////////////////////////////////////////////////////////
HistorySourceFile::HistorySourceFile(const std::string& path) :
HistorySource(),
m_filePath(path){}

////////////////////////////////////////////////////////////////////////////////
const std::experimental::optional<nlohmann::json> HistorySourceFile::get() const
{
    // Open file
    std::ifstream file(m_filePath);

    if (!file.good() || !file.is_open())
    {
        std::cout << "Failed to open file at " + m_filePath << std::endl;
        return std::experimental::nullopt;
    }

    // Try to parse json
    nlohmann::json json;
    try
    {
        file >> json;
    }
    catch(const std::exception& e)
    {
        std::cout << e.what() << std::endl;
        std::cout << "Error parsing json file, please check file is correct" << std::endl;
    }
    
    return json.is_null() || json.is_discarded() ? std::experimental::nullopt : 
                            std::experimental::optional<nlohmann::json>(json);
}