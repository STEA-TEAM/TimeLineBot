//
// Created by Parti on 2021/3/8.
//

#include <iostream>
#include <processors/WindowProcessor.h>
#include <stdexcept>
#include <thread>

using namespace processors;
using namespace std;
using namespace structures;

WindowProcessor::WindowProcessor(const string &windowName) {
    _targetWindow = FindWindow(windowName.c_str(), nullptr);
    if (!_targetWindow) {
        _targetWindow = FindWindow(nullptr, windowName.c_str());
        if (!_targetWindow) {
            throw runtime_error("Couldn't find window: " + windowName);
        }
    }
}

bool WindowProcessor::keepState() {
    bool result;
    do {
        SetWindowPos(_targetWindow, HWND_TOPMOST, 100, 100, 1280, 720, SWP_SHOWWINDOW);
        result = SetForegroundWindow(_targetWindow);
    } while (!result);
    return true;
}

void WindowProcessor::getInfo() const {
    char tempString[255];
    GetWindowText(_targetWindow, tempString, 255);
    cout << "Window Handle: " << _targetWindow << endl
         << "Window Name: " << tempString << endl;
}

void WindowProcessor::load(const string &configName, const string &timelineName) {
    _timeline = make_unique<Timeline>(configName, timelineName);
}

void WindowProcessor::start() const {
    thread([this]() {
        this->_timeline->run();
    }).join();
}
