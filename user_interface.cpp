//
// Created by lcdelcroix on 11/11/2025.
//

#include "user_interface.h"
#include <limits>

int User_Interface::menu() {
    std::cout <<"1- Songs\n2- Playlist\n3- Artist\n4- Album\n5- Exit";
    std::cout<<"Choice : ";
    int choice;
    std::cin>>choice;
    while (choice < 1 || choice > 5) {
        std::cout<<"Invalid choice\n"
                    "Please choose from 1 to 4\n ";
        std::cout<<"Choice : ";
        std::cin>>choice;
    }
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    return choice;
}

int User_Interface::song_menu() {
    std::cout << "\t\t~~~Song Menu~~~\n";
    std::cout<<"1-Ajouter des chansons\n2- Liste des chansons\n3- Rechercher une chanson\n"
                "4- Rechercher une chanson\n5- Supprimer une chanson\n6- Exit";
    int choice (0);
    std::cout<<"choice : ";
    std::cin>>choice;
    while (choice < 1 || choice > 6) {
        std::cout<<"Invalid choice\n"
                    "Please choose from 1 to 6\n ";
        std::cout<<"choice : ";
        std::cin>>choice;
    }
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    return choice;
}

void User_Interface::add_song_to_database() {
    std::cout << "\t\t~~~Ajout de chanson~~~\n";
    int number(0);
    std::cout<<"number de chansons : ";
    std::cin>>number;
    std::string title;
    for (int i = 0; i < number; i++) {


    }
}

void User_Interface::song_operation(AudioPlayer &player, int choice) {
    switch (choice) {
        case 1: {

        }
    }
}

void User_Interface::run_application() {
    int choice = 0;

    do {
        choice = menu();
        switch (choice) {
            case 1 : {
                int ch = song_menu();
            }
        }
    }
}
