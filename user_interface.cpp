//
// Created by lcdelcroix on 11/11/2025.
//

#include "user_interface.h"
#include <limits>
//std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

int UserInterface::menu_1() {
    std::cout<<"1. Se connecter\n2. Creer un compte\n";
    std::cout<<"Choix : ";
    int choice(0);
    std::cin>>choice;
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    return choice;
}
