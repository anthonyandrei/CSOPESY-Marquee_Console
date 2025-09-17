#include <iostream>
#include "helper.h"

int main() {
    std::string command;
    
    std::cout<<"Welcome to CSOPESY!"<<std::endl;

    std::cout<<"Group Developer:"<<std::endl;

    std::cout<<"Version Date:"<<std::endl;
    
    while (true) {
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
            return 0;
        } else {
            std::cout<<"Command not found"<<std::endl;
        }
    }
    return 0;
}