//
// Created by Parti on 2021/3/9.
//

#pragma once

#include <unordered_map>
#include <string>
#include <vector>
#include <Windows.h>

namespace structures {
    class Timeline {
    public:
        explicit Timeline(const std::string &configName, const std::string &timelineName);

        void run() const;

    private:
        std::unordered_map<uint16_t, unsigned char> _config;
        std::vector<std::pair<uint64_t, INPUT>> _timeline{};
    };
}

