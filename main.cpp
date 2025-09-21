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
string command;
string param;
mutex mtx;

void showHelp() {
    cout <<
        "Commands:\n"
        "   help            Show this help\n"
        "   start_marquee   Start marquee\n"
        "   stop_marquee    Stop marquee\n"
        "   set_text        Set marquee text\n"
        "   set_speed       Set marquee speed\n"
        "   exit            Exit program\n";
}

void keyboardHandler() {
    while (isRunning) {

        if (marqueeActive) {
                cout << "\nCommand> ";
        } else {
            cout << "Command> ";
        }

        cin >> command;

        if (command == "help") {
            lock_guard<mutex> lock(mtx);
            showHelp();
        } else if (command == "start_marquee") {
            if (marqueeActive) {
                cout << "Marquee already running!" << endl;
                continue;
            }
            lock_guard<mutex> lock(mtx);
            cout << "Starting marquee..." << endl;
            //TODO: start marquee
        } else if (command == "stop_marquee") {
            if (!marqueeActive) {
                cout << "Marquee not running!" << endl;
                continue;
            }
            lock_guard<mutex> lock(mtx);
            cout << "Stopping marquee..." << endl;
            // TODO: stop marquee
        } else if (command == "set_text") {
            lock_guard<mutex> lock(mtx);
            cout << "Enter new text: ";
            cin.ignore();
            getline(cin, param);
            marqueeText = param;
            cout << "Text successfully set to " << marqueeText << endl;
            // might need to restart marquee here to avoid issues
        } else if (command == "set_speed") {
            lock_guard<mutex> lock(mtx);
            cout << "Enter new speed: ";
            cin >> param;
            int newSpeed = stoi(param);
            speed = newSpeed;
            cout << "Speed successfully set to " << speed << endl;
            // might need to restart marquee here to avoid issues
        } else if (command == "exit") {
            lock_guard<mutex> lock(mtx);
            cout << "Exiting program." << endl;
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