#include "Menu.h"

#include <fstream>
#include <iostream>
#include <stdlib.h>

void InitWsl(){
    std::ifstream inFile;
    inFile.open("/proc/version");
    if (!inFile) {
        std::cerr << "ERROR: unable to open /proc/version";
        exit(1);
    }
    std::string line;
    bool running_on_wsl = false;
    while(std::getline(inFile, line)){
        if (line.find("Microsoft") != std::string::npos){
            std::cout << "INFO: Detected WSL." << std::endl;
            std::cout << line << std::endl;
            running_on_wsl = true;

        }
    }
    if(running_on_wsl) {
        std::cout << "INFO: Initialising WSL Audio and Video." << std::endl;
        char display_key[] = "DISPLAY";
        char display_value[] = ":0";
        setenv(display_key, display_value, 1);

        char audio_key[] = "PULSE_SERVER";
        char audio_value[] = "tcp:127.0.0.1";
        setenv(audio_key, audio_value, 1);

        if (system("nc -vzw3 127.0.0.1 4713") != 0){
            std::cout << "\e[31mERROR: PulseAudio is niet gevonden! Start eerst PulseAudio vóór je het project uitvoert!\e[0m" << std::endl;
            std::cout << "\e[31mIndien je dit al gedaan hebt; herstart je computer, start PulseAudio opnieuw en probeer nogmaals.\e[0m" << std::endl;
            exit(1);
        }
        if (system(" nc -vzw3 127.0.0.1 6000") != 0){
            std::cout << "\e[31mERROR: De X server is niet gevonden! Start eerst de X Server door middel van 'XLaunch' vóór je het project uitvoert!\e[0m" << std::endl;
            exit(1);
        }
    } else {
        std::cout << "INFO: Not running on WSL. Windows initialisation skipped." << std::endl;

    }
}

int main(int argc, char** argv) {
    InitWsl();

    Allkit::Get().Init();
    
    Menu menu;

    menu.Run();

    Allkit::Get().Destroy();

    return 0;  
}
