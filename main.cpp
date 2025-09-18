#include <iostream>
#include <thread>
#include <string>
#include <mutex>
#include "helper.h"

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
        std::cin>>command;

        if (command == "help") {
            showHelp();
        } else if (command == "start_marquee") {
            // TODO: start marquee
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