#include <iostream>
#include <thread>
#include <string>
#include <mutex>
#include <chrono>

using namespace std;

string marqueeText = "Hello World!";
int speed = 200;
bool isRunning = true;
bool marqueeActive = false;
string line;
string command;
string param;
mutex mtx;

void showHelp() {
    cout <<
        "Commands:\n"
        "   help                  Show this help\n"
        "   start_marquee         Start marquee\n"
        "   stop_marquee          Stop marquee\n"
        "   set_text <text>       Set marquee text\n"
        "   set_speed <speed>     Set marquee speed\n"
        "   exit                  Exit program\n";
}
void processLine() {
    command.clear();
    param.clear();

    size_t start = line.find_first_not_of(" \t\r\n");
    if (start == string::npos) 
        return;

    size_t space = line.find_first_of(" ", start);
    if (space == string::npos) {
        command = line.substr(start);
        return;
    }

    command = line.substr(start, space - start);
    size_t param_start = line.find_first_not_of(" ", space);
    if (param_start != string::npos) {
        param = line.substr(param_start);
        size_t param_end = param.find_last_not_of(" \t\r\n");
        if (param_end != string::npos)
            param.erase(param_end + 1);
        else
            param.clear();
    }
}

void keyboardHandler() {
    while (isRunning) {

        if (marqueeActive) {
            cout << "\nCommand> ";
        } else {
            cout << "Command> ";
        }

        if (!getline(cin, line)) {
            if (cin.eof()) {
                cout << "\nEnd of input detected. Exiting program." << endl;
                marqueeActive = false;
                isRunning = false;
                break;
            }
            continue;
        }

        processLine();

        if (command == "help") {
            lock_guard<mutex> lock(mtx);
            showHelp();
        } else if (command == "start_marquee") {
            if (marqueeActive) {
                cout << "Marquee already running!" << endl;
                continue;
            }
            marqueeActive = true;
            lock_guard<mutex> lock(mtx);
            cout << "Starting marquee..." << endl;
        } else if (command == "stop_marquee") {
            if (!marqueeActive) {
                cout << "Marquee not running!" << endl;
                continue;
            }
            lock_guard<mutex> lock(mtx);
            cout << "Stopping marquee..." << endl;
            marqueeActive = false;
            cout << "Marquee stopped." << endl;
        } else if (command == "set_text") {
            lock_guard<mutex> lock(mtx);

            if (param.empty()) {
                cout << "Missing parameter." << endl;
                continue;
            }

            marqueeText = param;
            cout << "Text successfully set to " << marqueeText << endl;
        } else if (command == "set_speed") {
            try {
                lock_guard<mutex> lock(mtx);
                if (param.empty()) {
                    cout << "Missing parameter." << endl;
                    continue;
                }
                speed = stoi(param);
                if (speed <= 0) {
                    cout << "Invalid speed. Must be a positive integer." << endl;
                    continue;
                }
                cout << "Speed successfully set to " << speed << endl;
            } catch (exception& e) {
                cout << "Error: " << e.what() << endl;
            }
        } else if (command == "exit") {
            lock_guard<mutex> lock(mtx);
            cout << "Exiting program." << endl;
            marqueeActive = false;
            isRunning = false;
        } else {
            lock_guard<mutex> lock(mtx);
            cout << "Command not found" << endl;
        }
    }
}

int main() {
    cout<<"Welcome to CSOPESY!\n\nGroup Developer:\nVersion Date:\n"<<endl;

    //TODO: create marquee logic thread
    //TODO: create display thread
    thread kb_thread(keyboardHandler);

    // Wait for keyboard thread to finish
    kb_thread.join();

    return 0;
}