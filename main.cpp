#include <iostream>
#include "user_interface.h"
#define WELCOME "WELCOME TO THE AUDIO PLAYER"

int main() {
    std::cout <<WELCOME;

    int choice = User_Interface::menu();
    switch (choice) {
        case 1: {

        }
    }
    return 0;
}