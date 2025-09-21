#include <iostream>
#include <thread>
#include <string>
#include <mutex>

std::string marqueeText = "Hello World!";
int speed = 200;

void showHelp() {
    std::cout <<
        "Commands:\n"
        "   help            Show this help\n"
        "   start_marquee   Start marquee\n"
        "   stop_marquee    Stop marquee\n"
        "   set_text        Set marquee text\n"
        "   set_speed       Set marquee speed\n"
        "   exit            Exit program\n";
}

int main() {
    std::string command;
    bool isRunning = true;
    std::mutex mtx;

    std::cout<<"Welcome to CSOPESY!\n\nGroup Developer:\nVersion Date:\n"<<std::endl;
    
    //TODO: create marquee logic thread
    //TODO: create display thread
    //TODO: create keyboard handler thread

    while (isRunning) {
        std::cout<<"Command>";
        //TODO: tokenize input
        std::cin>>command;

        if (command == "help") {
            showHelp();
        } else if (command == "start_marquee") {
            //TODO: start marquee
        } else if (command == "stop_marquee") {
            // TODO: stop marquee
        } else if (command == "set_text") {
            // TODO: set text
        } else if (command == "set_speed") {
            // TODO: set speed
        } else if (command == "exit") {
            std::cout << "Exiting program." << std::endl;
            isRunning = false;
        } else {
            std::cout<<"Command not found"<<std::endl;
        }
    }
    return 0;
}