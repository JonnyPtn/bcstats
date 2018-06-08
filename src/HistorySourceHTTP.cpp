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

#include "HistorySourceHTTP.hpp"

#include <http/httplib.hpp>

////////////////////////////////////////////////////////////////////////////////
HistorySourceHTTP::HistorySourceHTTP(const std::string& host, const std::string& query) :
HistorySource(),
m_host(host),
m_query(query) {}

////////////////////////////////////////////////////////////////////////////////
const optional<nlohmann::json> HistorySourceHTTP::get() const
{
    // Make the http request
    httplib::Client client(m_host.c_str(), 80);

    auto res = client.Get(m_query.c_str());

    nlohmann::json json;

    if (res && res->status == 200)
    {
        try
        {
            json = nlohmann::json::parse(res->body);
        }
        catch (nlohmann::json::exception e)
        {
            std::cout << e.what() << std::endl;
            std::cout << "HTTP returned " << res->body << std::endl;
            return {};
        }
        return json;
    }
    else
    {
        if (res)
        {
            std::cout << "HTTP Error: " << res->status << " " << res->body << std::endl;
        }
        else
        {
            std::cout << "Bad HTTP request" << std::endl;
        }
        return {};
    }
    
}