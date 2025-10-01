#include <iostream>
#include <thread>
#include <string>
#include <mutex>
#include <chrono>
#include <windows.h>
#include <conio.h>   // added

using namespace std;

const int MAX_SPEED = 500;          // upper bound used in delay formula
const int DEFAULT_SPEED = 250;
const int INACTIVE_SLEEP = 100;
const int MIN_DELAY = 20;           // minimum allowed delay (ms) to avoid 0 / busy loop

string marqueeText = "Hello World!";
int speed = DEFAULT_SPEED;
bool isRunning = true;
bool marqueeActive = false;
string line;
string command;
string param;
string toPrint;
mutex mtx;

int windowColumns;
int windowRows;
CONSOLE_SCREEN_BUFFER_INFO csbi;

string inputBuffer;        // live typed characters
size_t prevInputLen = 0;   // track previous rendered input length

void promptCommand() {
    cout << "Command> ";
}

void printGreeting() {
    cout << "Welcome to CSOPESY!\n" << endl;
    cout << "Group Developer: " << endl;
    cout << "Alonzo, John Leomarc" << endl;
    cout << "Labarrete, Lance Desmond" << endl;
    cout << "Soan, Brent Jan" << endl;
    cout << "Tan, Anthony Andrei\n" << endl;
    cout << "Version Date: 10/01/2025\n" << endl;
}

void getConsoleSize() {
    while(isRunning) {
        GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi);

        windowColumns = csbi.srWindow.Right - csbi.srWindow.Left + 1;
        windowRows = csbi.srWindow.Bottom - csbi.srWindow.Top + 1;
    }
} 

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
        if (_kbhit()) {
            int ch = _getch();

            if (ch == 13) { // Enter
                {
                    lock_guard<mutex> lock(mtx);
                    cout << "\n";
                    line = inputBuffer;
                    inputBuffer.clear();
                    prevInputLen = 0; // reset so prompt redraws clean
                }
                processLine();

                if (command == "help") {
                    showHelp();
                } else if (command == "start_marquee") {
                    if (marqueeActive) {
                        cout << "Marquee already running!" << endl;
                    }
                    marqueeActive = true;
                    cout << "Starting marquee..." << endl;
                } else if (command == "stop_marquee") {
                    if (!marqueeActive) {
                        cout << "Marquee not running!" << endl;
                    }
                    marqueeActive = false;
                    cout << "Marquee stopped." << endl;
                } else if (command == "set_text") {
                    if (param.empty()) {
                        cout << "Missing parameter." << endl;
                    }
                    marqueeText = param;
                    cout << "Text successfully set to " << marqueeText << endl;
                } else if (command == "set_speed") {
                    try {
                        if (param.empty()) {
                            cout << "Missing parameter." << endl;
                        }
                        //get first token before whitespace
                        size_t spacePos = param.find_first_of(" \t");
                        if (spacePos != string::npos) {
                            param = param.substr(0, spacePos);
                        }
                        int newSpeed = stoi(param);
                        int maxUserSpeed = MAX_SPEED - MIN_DELAY;
                        if (newSpeed < 1 || newSpeed > maxUserSpeed) {
                            cout << "Invalid speed. Must be between 1 and " << maxUserSpeed << endl;
                        }
                        speed = newSpeed;
                        cout << "Speed successfully set to " << speed << endl;
                    } catch (exception& e) {
                        cout << "Error: " << e.what() << endl;
                    }
                } else if (command == "exit") {
                    cout << "Exiting program." << endl;

                    marqueeActive = false;
                    isRunning = false;
                } else if (!command.empty()) {
                    cout << "Command not found" << endl;
                }

            } else if (ch == 8) { // Backspace
                lock_guard<mutex> lock(mtx);
                if (!inputBuffer.empty()) {
                    inputBuffer.pop_back();
                }
            } else if (isprint(ch)) {
                lock_guard<mutex> lock(mtx);
                inputBuffer.push_back(static_cast<char>(ch));
            }
        }
        Sleep(10);
    }
}

// Marquee Logic: Calculates what to display
void marqueeLogicHandler() {
    while(isRunning) {
        if(marqueeActive) {
            int j = 0;
            string currentText = marqueeText; // Snapshot the text
            int marqueeTextLen = static_cast<int>(currentText.length());

            for(int i = windowColumns; i > 0 - marqueeTextLen && isRunning && marqueeActive; i--) {
                {
                    lock_guard<mutex> lock(mtx);
                    toPrint = "";

                    for(int k = i; k > 0; k--) {
                        toPrint.push_back(' ');
                    }

                    if(i > windowColumns - marqueeTextLen) {
                        toPrint += currentText.substr(0, j);
                        j++;
                    }
                    else if(i >= 0) {
                        toPrint += currentText;
                    }
                    else if(i < 0) {
                        toPrint += currentText.substr(j, marqueeTextLen - j);
                        j++;
                    }

                    if(i == 0) {
                        j = 0;
                    }
                }

                Sleep(MAX_SPEED - speed);
                
                // Break if text changed during animation
                if(currentText != marqueeText) {
                    break;
                }
            }
        }
        else {
            Sleep(INACTIVE_SLEEP);
        }
    }
}

// Display Handler: Handles actual console output
void displayHandler() {
    while(isRunning) {
        if(marqueeActive) {
            system("cls");
            printGreeting();
            string textToPrint = toPrint;

            if(!textToPrint.empty()) {
                cout << textToPrint << endl;
            }
            
            {
                lock_guard<mutex> lock(mtx);
                size_t curLen = inputBuffer.size();
                cout << "\nCommand> " << inputBuffer;
                if (curLen < prevInputLen) {
                    cout << string(prevInputLen - curLen, ' '); // clear leftovers
                    // cursor is already after spaces; reposition to end of current buffer:
                    cout << "\r" << "Command> " << inputBuffer;
                }
                prevInputLen = curLen;
                cout << flush;
            }

            Sleep(MAX_SPEED - speed);
        } else {
            {
                lock_guard<mutex> lock(mtx);
                size_t curLen = inputBuffer.size();
                cout << "\rCommand> " << inputBuffer;
                if (curLen < prevInputLen) {
                    cout << string(prevInputLen - curLen, ' ');
                    cout << "\rCommand> " << inputBuffer;
                }
                prevInputLen = curLen;
                cout << flush;
            }
            Sleep(INACTIVE_SLEEP);
        }
    }
}

int main() {
    printGreeting();
    thread get_console_size_thread(getConsoleSize);

    thread marquee_thread(marqueeLogicHandler);
    thread display_thread(displayHandler);
    thread kb_thread(keyboardHandler);

    // Wait for keyboard thread to finish
    kb_thread.join();
    marquee_thread.join();
    display_thread.join();

    return 0;
}
