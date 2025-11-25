//
// Created by lcdelcroix on 11/11/2025.
//

#include "user_interface.h"
#include <limits>
#include <iostream>
#include <string>
#include "song.h"
#include "album.h"
#include "playlist.h"

//std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
#ifdef _WIN32
    #include <conio.h>   // Windows only
#else
    #include <termios.h> // Linux / macOS
    #include <unistd.h>
#endif

// ----- getch() portable -----
char getch_portable() {
#ifdef _WIN32
    return _getch(); // disponible sous Windows
#else
    char buf = 0;
    struct termios old = {0};
    if (tcgetattr(STDIN_FILENO, &old) < 0)
        perror("tcgetattr()");
    old.c_lflag &= ~ICANON; // lecture caractère par caractère
    old.c_lflag &= ~ECHO;   // désactive l’affichage
    if (tcsetattr(STDIN_FILENO, TCSANOW, &old) < 0)
        perror("tcsetattr()");
    if (read(STDIN_FILENO, &buf, 1) < 0)
        perror("read()");
    old.c_lflag |= ICANON;
    old.c_lflag |= ECHO;
    if (tcsetattr(STDIN_FILENO, TCSADRAIN, &old) < 0)
        perror("tcsetattr()");
    return buf;
#endif
}

// ----- Fonction de saisie du mot de passe -----
std::string passWord() {
    std::string password;

    while (true) {
        const char ch = getch_portable();

        // 1. Gérer la fin de la saisie (Touche Entrée)
        if (ch == '\n' || ch == '\r') {
            std::cout << std::endl; // Aller à la ligne suivante
            break;
        }

        // 2. Gérer la touche "Retour arrière" (Backspace)
        // Note : Le code est différent sous Windows (8) et POSIX (127)
        bool is_backspace = false;
#ifdef _WIN32
        if (ch == 8) is_backspace = true; // Code ASCII Backspace (BS)
#else
        if (ch == 127) is_backspace = true; // Code ASCII Delete (DEL)
#endif

        if (is_backspace) {
            if (!password.empty()) {
                // Effacer le caractère de la chaîne
                password.pop_back();
                // Effacer l'astérisque de la console
                // \b = recule le curseur, ' ' = écrase, \b = recule à nouveau
                std::cout << "\b \b";
            }
        } else {
            // 3. Gérer un caractère normal
            password.push_back(ch);
            std::cout << "*";
        }
    }
    return password;
}
//fonction pour masquer la saisie du mot de passe


int UserInterface::menu_1() {
    std::cout<<"1. Se connecter\n2. Creer un compte\n";
    std::cout<<"Choix : ";
    int choice(0);
    std::cin>>choice;
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    return choice;
}

void UserInterface::handle_login() {
    std::string username;

    std::cout<<"\n--- CONNEXION ---\n";
    std::cout<<"Nom d'utilisateur : ";
    std::getline(std::cin,username);
    std::string password = passWord();

    if (auth_manager.login(username, password)) {
        const UserAccount& user = auth_manager.get_current_user();
        std::cout<<"\nBIENVENUE "<<user.get_name()<<" ! vous etes connecte \n";
    }else {
        std::cout<<"\n Echec de la connexion. Nom d'utilisateur ou mot de passe invalide.\n";
    }
}

void UserInterface::handle_register() {
    std::string username , email;
    std::cout<<"\n ---CREATION D'UN COMPTE ---"<<std::endl;

    std::cout<<"Nom d'utilisateur : ";
    std::getline(std::cin,username);

    std::cout<<"Email : ";
    std::getline(std::cin,email);

    std::cout<<"Mot de passe (8 caracteres min et 25 max) : ";
    const std::string password = passWord();

    if (username.empty() || email.empty()) {
        std::cout <<"\n le nom d'utilisateur et l'email ne peuvent pas etre vide \n";
        return;
    }

    if (auth_manager.register_user(username,password,email)) {
        const UserAccount& user = auth_manager.get_current_user();
        std::cout <<"\nLe compte a ete cree avec succes. Bienvenue "<<user.get_name()<<std::endl;
    }else {
        std::cout<<"\n Echec de la creation du compte.\n";
    }
}

void UserInterface::display_artist() const {
    const std::vector<Artist> artists = player.get_all_artists();
    std::cout<<"\t\t~~~La liste des artits\n";
    int i = 1;
    for (const auto& artist : artists) {
        std::cout <<i<<". "<< artist.get_name() << "\n";
        i++;
    }
    std::cout<< "\t\t~~~END~~~\n";
}

// void UserInterface::display_favorite() const {
//     const int  user_id = auth_manager.get_current_user().get_id();
//     const std::vector<Song> songs = player.get_liked_songs(user_id);
//     if (songs.empty()) {
//         std::cout <<"La liste des chansons preferees est vide \n";
//         return;
//     }
//     int i = 1;
//     std::cout <<"Liste des chansons preferee\n";
//     for (auto& song : songs) {
//         std::cout<<i<<". "<<song.get_title()<<std::endl;
//         i++;
//     }
//     std::cout <<"\t\t~~~END~~~\n";
// }

int UserInterface::menu_2() const {
    std::cout<<"\n--- BIBLIOTHEQUE AUDIO ["<<auth_manager.get_current_user().get_name()<<"]---\n";
    std::cout<<"1. Gerer les Chansons" <<std::endl;
    std::cout<<"2. Gerer les Artistes" <<std::endl;
    std::cout<<"3. Gerer les albums" << std::endl;
    std::cout<<"4. Gerer les Playlists" <<std::endl;
    std::cout<<"5. Se deconnecter" <<std::endl;

    int choice ;
    std::cout<<"Votre choix : ";
    while (!(std::cin>>choice) || choice < 1 || choice > 6) {
        std::cout <<"Choix invalide. Veuillez entrer un numero entre 1 et 6 : ";
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    }
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    return choice;
}

void UserInterface::run() {
    int choice = 0;
    bool running = true;

    while(running) {
        if (!auth_manager.is_logged_in()) {
            choice = menu_1();

            switch (choice) {
                case 1:
                    handle_login();
                    break;
                case 2:
                    handle_register();
                    break;
                case 3:
                    running = false;
                    break;
                default:
                    std::cout<<"Choix invalide.\n";
                    break;
            }
        }
        else {
            const int app_choice  = menu_2();

            // Raccourcis pour les references
            AudioPlayer& player_ref = player;
            const UserAccount& user_ref = auth_manager.get_current_user();
            switch (app_choice) {
                case 1:
                {
                    SongMenu song_menu(player_ref, const_cast<UserAccount&>(user_ref));
                    song_menu.run_song();
                }
                    break;
                case 2:
                    display_artist();
                    break;
                case 3 :
                {
                    AlbumMenu album_menu(player_ref, const_cast<UserAccount&>(user_ref));
                    album_menu.run_album_menu();
                    break;
                }
                case 4: {
                    PlaylistMenu playlist_menu(player_ref, const_cast<UserAccount&>(user_ref));
                    playlist_menu.run_playlist_menu();
                    break;
                }
                case 5:
                    auth_manager.logout();
                    break;
                default:
                    std::cout <<"Choix invalide.\n";
                    break;
            }
        }
    }
}