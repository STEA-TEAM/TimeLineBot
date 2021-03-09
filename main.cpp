#include <processors/WindowProcessor.h>
#include <iostream>

using namespace processors;
using namespace std;

int main(int argc, char *argv[]) {
    if (argc != 4) {
        cout << "Usage: TimeLineBot <processName> <configFile.csv> <timelineFile.csv>" << endl;
        return -1;
    }

    try {
        WindowProcessor windowProcessor(argv[1]);
        windowProcessor.getInfo();
        windowProcessor.keepState();
        windowProcessor.load(argv[2], argv[3]);
        windowProcessor.start();
        return 0;
    } catch (std::exception &e) {
        cout << "Error: " << e.what() << endl;
        return 1;
    }
}

