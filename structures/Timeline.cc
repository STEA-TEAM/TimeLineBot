//
// Created by Parti on 2021/3/9.
//

#include <iostream>
#include <fstream>
#include <sstream>
#include <structures/Timeline.h>
#include <thread>

using namespace structures;
using namespace std;

Timeline::Timeline(const string &configName, const std::string &timelineName) {
    ifstream config(configName), timeline(timelineName);
    if (!config.is_open()) {
        throw range_error("Couldn't open file: " + configName);
    }
    while (!config.eof()) {
        string tempLine;
        getline(config, tempLine);
        if (tempLine.empty()) {
            continue;
        }

        istringstream tempStream(tempLine);
        string tempPosition, tempKeyName;
        getline(tempStream, tempPosition, ',');
        getline(tempStream, tempKeyName, ',');

        _config.emplace(
                static_cast<uint16_t>(strtoul(tempPosition.c_str(), nullptr, 10)),
                tempKeyName[0]
        );
    }

    if (!timeline.is_open()) {
        throw range_error("Couldn't open file: " + timelineName);
    }
    vector<tuple<uint64_t, uint64_t, bool>>
            rawTimeline;
    while (!timeline.eof()) {
        string tempLine;
        getline(timeline, tempLine);
        if (tempLine.empty()) {
            continue;
        }

        istringstream tempStream(tempLine);
        string tempTimeStamp, tempPosition, tempKeyState;
        getline(tempStream, tempTimeStamp, ',');
        getline(tempStream, tempPosition, ',');
        getline(tempStream, tempKeyState, ',');

        rawTimeline.emplace_back(
                strtoull(tempTimeStamp.c_str(), nullptr, 10),
                strtoull(tempPosition.c_str(), nullptr, 10),
                tempKeyState == "1"
        );
    }

    for (const auto &[timestamp, position, isKeyUp]: rawTimeline) {
        INPUT tempKey;
        ZeroMemory(&tempKey, 1);
        tempKey.type = INPUT_KEYBOARD;
        tempKey.ki.time = 0;
        tempKey.ki.wVk = 0;
        tempKey.ki.dwExtraInfo = 0;
        tempKey.ki.dwFlags = KEYEVENTF_SCANCODE | (isKeyUp ? KEYEVENTF_KEYUP : 0);
        tempKey.ki.wScan = MapVirtualKey(_config[position], MAPVK_VK_TO_VSC);

        _timeline.emplace_back(timestamp, tempKey);
    }
}

void Timeline::run() const {
    auto startTime = chrono::high_resolution_clock::now();
    for (auto pair: _timeline) {
        auto [timestamp, key] = pair;
        while (chrono::high_resolution_clock::now() - startTime < chrono::milliseconds(timestamp)) {
            this_thread::sleep_for(chrono::microseconds(100));
        }
        SendInput(1, &key, sizeof(INPUT));
        cout << "[" << timestamp << "]" << key.ki.wScan << endl;
    }
}
