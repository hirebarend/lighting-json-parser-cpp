#include <iostream>
#include <fstream>
#include <string>
#include <string_view>
#include <numeric>
#include <chrono>
#include "nlohmann/json.hpp"
#include "simdjson.h"

using json = nlohmann::json;

double calculate_1(std::string_view file_path)
{
    std::ifstream file(file_path.data());

    if (!file.is_open())
    {
        std::cerr << "Error opening file: " << file_path << std::endl;
        return 0.0;
    }

    double result = 0.0;

    std::string line;

    while (std::getline(file, line))
    {
        json j = json::parse(line);

        if (j.value("method", "") == "buy")
        {
            if (j.contains("params") && j["params"].is_array() && j["params"].size() > 1)
            {
                std::string amount_str = j["params"][1];
                result += std::stod(amount_str);
            }
        }
    }

    return result;
}

double calculate_2(std::string_view file_path)
{
    std::ifstream file(file_path.data());

    if (!file.is_open())
    {
        std::cerr << "Error opening file: " << file_path << std::endl;
        return 0.0;
    }

    double result = 0.0;
    std::string line;

    while (std::getline(file, line))
    {
        if (line.find("\"method\":\"buy\"") == std::string::npos)
        {
            continue;
        }

        size_t params_start_pos = line.find("\"params\":[");
        if (params_start_pos == std::string::npos)
        {
            continue;
        }

        size_t comma_pos = line.find(',', params_start_pos);
        if (comma_pos == std::string::npos)
        {
            continue;
        }

        size_t value_start_quote = line.find('"', comma_pos);
        if (value_start_quote == std::string::npos)
        {
            continue;
        }

        size_t value_end_quote = line.find('"', value_start_quote + 1);
        if (value_end_quote == std::string::npos)
        {
            continue;
        }

        std::string value_str = line.substr(value_start_quote + 1, value_end_quote - value_start_quote - 1);

        try
        {
            result += std::stod(value_str);
        }
        catch (const std::invalid_argument &e)
        {
            // ignore
        }
        catch (const std::out_of_range &e)
        {
            // ignore
        }
    }

    return result;
}

double calculate_3(std::string_view file_path)
{
    using namespace simdjson;
    dom::parser parser;
    double total_buy = 0.0;

    try
    {
        padded_string json_content = padded_string::load(file_path.data());
        for (dom::element doc : parser.parse_many(json_content))
        {
            std::string_view method;
            if (doc["method"].get_string().get(method) == SUCCESS && method == "buy")
            {
                dom::array params = doc["params"];
                std::string_view amount_str_view;
                if (params.at(1).get_string().get(amount_str_view) == SUCCESS)
                {
                    std::string amount_str(amount_str_view);
                    total_buy += std::stod(amount_str);
                }
            }
        }
    }
    catch (const simdjson::simdjson_error &e)
    {
        std::cerr << "simdjson error parsing file " << file_path << ": " << e.what() << std::endl;
        return 0.0;
    }

    return total_buy;
}

int main()
{
    {
        auto start = std::chrono::high_resolution_clock::now();
        double result = calculate_1("../data.jsonl");
        auto end = std::chrono::high_resolution_clock::now();

        std::cout << "result 1: " << result << std::endl;

        std::chrono::duration<double> duration = end - start;
        std::cout << duration.count() * 1000 << " ms" << std::endl;
        std::cout << std::endl;
    }

    {
        auto start = std::chrono::high_resolution_clock::now();
        double result = calculate_2("../data.jsonl");
        auto end = std::chrono::high_resolution_clock::now();

        std::cout << "result 2: " << result << std::endl;

        std::chrono::duration<double> duration = end - start;
        std::cout << duration.count() * 1000 << " ms" << std::endl;
        std::cout << std::endl;
    }

    {
        auto start = std::chrono::high_resolution_clock::now();
        double result = calculate_3("../data.jsonl");
        auto end = std::chrono::high_resolution_clock::now();

        std::cout << "result 3: " << result << std::endl;

        std::chrono::duration<double> duration = end - start;
        std::cout << duration.count() * 1000 << " ms" << std::endl;
        std::cout << std::endl;
    }

    return 0;
}