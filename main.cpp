#include <iostream>
#include <thread>
#include <string>
#include <mutex>
#include <chrono>

using namespace std;

const int MAX_SPEED = 1000;

string marqueeText = "Hello World!";
int speed = 200;
bool isRunning = true;
bool marqueeActive = false;
string line;
string command;
string param;
string toPrint;
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
        cout << "Command> ";

        if (!getline(cin, line)) {
            if (cin.eof()) {
                lock_guard<mutex> lock(mtx);
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
                lock_guard<mutex> lock(mtx);
                cout << "Marquee already running!" << endl;
                continue;
            }
            marqueeActive = true;
            lock_guard<mutex> lock(mtx);
            cout << "Starting marquee..." << endl;
        } else if (command == "stop_marquee") {
            if (!marqueeActive) {
                lock_guard<mutex> lock(mtx);
                cout << "Marquee not running!" << endl;
                continue;
            }
            marqueeActive = false;
            lock_guard<mutex> lock(mtx);
            cout << "\r" << string(50, ' ') << "\rMarquee stopped." << endl;
        } else if (command == "set_text") {
            if (param.empty()) {
                lock_guard<mutex> lock(mtx);
                cout << "Missing parameter." << endl;
                continue;
            }
            marqueeText = param;
            lock_guard<mutex> lock(mtx);
            cout << "Text successfully set to " << marqueeText << endl;
        } else if (command == "set_speed") {
            try {
                if (param.empty()) {
                    lock_guard<mutex> lock(mtx);
                    cout << "Missing parameter." << endl;
                    continue;
                }
                int newSpeed = stoi(param);
                if (newSpeed <= 0 || newSpeed > MAX_SPEED) {
                    lock_guard<mutex> lock(mtx);
                    cout << "Invalid speed. Must be between 1 and " << MAX_SPEED << endl;
                    continue;
                }
                speed = newSpeed;
                lock_guard<mutex> lock(mtx);
                cout << "Speed successfully set to " << speed << endl;
            } catch (exception& e) {
                lock_guard<mutex> lock(mtx);
                cout << "Error: " << e.what() << endl;
            }
        } else if (command == "exit") {
            lock_guard<mutex> lock(mtx);
            cout << "Exiting program." << endl;
            marqueeActive = false;
            isRunning = false;
        } else if (!command.empty()) {
            lock_guard<mutex> lock(mtx);
            cout << "Command not found" << endl;
        }
    }
}

// Marquee Logic: Calculates what to display
void marqueeLogicHandler() {
    int position = 0;
    const int displayWidth = 40;
    
    while (isRunning) {
        if (marqueeActive) {
            string text = marqueeText;
            string display(displayWidth, ' ');
            
            // Create scrolling animation
            int textLen = text.length();
            if (textLen > 0) {
                for (int i = 0; i < displayWidth; i++) {
                    int textIndex = (position + i) % textLen;
                    display[i] = text[textIndex];
                }
            }
            
            // Pass formatted text to display handler
            {
                lock_guard<mutex> lock(mtx);
                toPrint = "\rMarquee: " + display;
            }
            
            position = (position + 1) % (textLen + displayWidth);
        } else {
            // Clear toPrint when marquee is inactive
            lock_guard<mutex> lock(mtx);
            toPrint.clear();
        }
        
        this_thread::sleep_for(chrono::milliseconds(MAX_SPEED - speed));
    }
}

// Display Handler: Handles actual console output
void displayHandler() {
    while (isRunning) {
        string output;
        {
            lock_guard<mutex> lock(mtx);
            output = toPrint;
        }

        if (!output.empty()) {
            lock_guard<mutex> lock(mtx);
            cout << output << flush;
        }
        
        this_thread::sleep_for(chrono::milliseconds(MAX_SPEED - speed));
    }
}

int main() {
    cout << "Welcome to CSOPESY!\n" << endl;
    cout << "Group Developer: " << endl;
    cout << "Alonzo, John Leomarc" << endl;
    cout << "Labarrete, Lance" << endl;
    cout << "Soan, Brent Jan" << endl;
    cout << "Tan, Anthony Andrei" << endl;
    cout << "Version Date: 9/30/2025" << endl;

    thread marquee_thread(marqueeLogicHandler);
    thread display_thread(displayHandler);
    thread kb_thread(keyboardHandler);

    // Wait for keyboard thread to finish
    kb_thread.join();
    marquee_thread.join();
    display_thread.join();

    return 0;
}