//
// Created by Parti on 2021/3/9.
//

#include <fstream>
#include <sstream>
#include <structures/Timeline.h>
#include <thread>

using namespace structures;
using namespace std;

Timeline::Timeline(const string &configName, const std::string &timelineName) {
    ifstream config(configName), timeline(timelineName);
    if (!config.is_open()) {
        throw std::range_error("Couldn't open file: " + configName);
    }
    while (!config.eof()) {
        string tempLine;
        getline(config, tempLine);
        if (tempLine.empty()) {
            break;
        }

        istringstream tempStream(tempLine);
        string tempKeyCode, tempRealKey;
        getline(tempStream, tempKeyCode, ',');
        getline(tempStream, tempRealKey, ',');

        _config.emplace(
                static_cast<uint16_t>(strtoul(tempKeyCode.c_str(), nullptr, 10)),
                tempRealKey[0]
        );
    }

    if (!timeline.is_open()) {
        throw std::range_error("Couldn't open file: " + timelineName);
    }
    vector<pair<uint64_t, uint64_t>>
            rawTimeline;
    while (!timeline.eof()) {
        string tempLine;
        getline(timeline, tempLine);
        if (tempLine.empty()) {
            break;
        }

        istringstream tempStream(tempLine);
        string tempTimeStamp, tempKeyCode;
        getline(tempStream, tempTimeStamp, ',');
        getline(tempStream, tempKeyCode, ',');

        rawTimeline.emplace_back(
                strtoull(tempTimeStamp.c_str(), nullptr, 10),
                strtoull(tempKeyCode.c_str(), nullptr, 10)
        );
    }
    uint64_t timestamp{};
    for (const auto &pair : rawTimeline) {
        uint64_t tempTime = pair.first;

        INPUT tempKey;
        ZeroMemory(&tempKey, 1);
        tempKey.type = INPUT_KEYBOARD;
        tempKey.ki.time = 0;
        tempKey.ki.wVk = 0;
        tempKey.ki.dwExtraInfo = 0;
        tempKey.ki.dwFlags = KEYEVENTF_SCANCODE;
        tempKey.ki.wScan = MapVirtualKey(_config[pair.second], MAPVK_VK_TO_VSC);

        _timeline.emplace_back(tempTime - timestamp, tempKey);
        timestamp = tempTime;
    }
}

void Timeline::run() const {
    for (auto pair : _timeline) {
        this_thread::sleep_for(chrono::milliseconds(pair.first));

        SendInput(1, &pair.second, sizeof(INPUT));
        pair.second.ki.dwFlags = KEYEVENTF_SCANCODE | KEYEVENTF_KEYUP;
        SendInput(1, &pair.second, sizeof(INPUT));
    }
}
