#include <iostream>
#include "helper.h"

int main() {
    std::string command;
    bool isRunning = true;

    std::cout<<"Welcome to CSOPESY!\n\nGroup Developer:\nVersion Date:\n"<<std::endl;
    
    //create marquee logic thread
    //create display thread
    //create keyboard handler thread

    while (isRunning) {
        std::cout<<"Command>";
        std::cin>>command;
        if (command == "help") {
            showHelp();
        } else if (command == "start_marquee") {
            // start marquee
        } else if (command == "stop_marquee") {
            // stop marquee
        } else if (command == "set_text") {
            // set text
        } else if (command == "set_speed") {
            // set speed
        } else if (command == "exit") {
            std::cout << "Exiting program." << std::endl;
            isRunning = false;
        } else {
            std::cout<<"Command not found"<<std::endl;
        }
    }
    return 0;
}