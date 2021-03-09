//
// Created by Parti on 2021/3/8.
//

#pragma once

#include <memory>
#include <structures/Timeline.h>

namespace processors {
    class WindowProcessor {
    public:
        explicit WindowProcessor(const std::string &processName);

        bool keepState();

        void getInfo() const;

        void load(const std::string &configName, const std::string &timelineName);

        void start() const;

    private:
        std::unique_ptr<structures::Timeline> _timeline{};
        HWND _targetWindow;
    };
}
