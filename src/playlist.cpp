//
// Created by lcdelcroix on 25/11/2025.
//

#include "playlist.h"
#include "audio_player.h"
#include "user_account.h"
#include <vector>
#include <string>
#include <iostream>


int PlaylistMenu::menu() {
    std::cout<<"1. Voir la liste des playlist\n"
                "2. Ajouter une playlist\n"
                "3. Supprimer une playlist\n"
                "4. Retourner au menu principale\n";
    int choice;
    std::cout <<"Choix : ";
    std::cin >> choice;
    while (choice < 1 || choice > 4) {
        std::cout<<"Invalide choice\n";
        std::cout <<"Choix : ";
        std::cin >> choice;
    }
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    return choice;
}

void PlaylistMenu::see_playlist() {
    const int user_id = user.get_id();
    std::vector<Playlist> playlists = audio.get_playlist_by_user(user_id);
    std::cout<<"\t\t~~~Liste des playlist~~~\n";
    int i = 1;
    for (auto& playlist : playlists) {
        std::cout<<i<<". "<<playlist.get_name()<<std::endl;
        i++;
    }
    std::cout<<"\t\t~~~END~~~\n";
}

void PlaylistMenu::add_pl() {
    std::cout <<"Nom de la playlist : ";
    std::string name ;
    std::getline(std::cin, name);
    const std::vector<Song> songs = audio.get_all_songs();
    if (songs.empty()) {
        std::cout << "Aucune chanson disponible. Album cree vide.\n";
    } else {
        std::cout <<"\t\t~~~ Liste des Chansons ~~~\n";
        for (size_t k = 0; k < songs.size(); ++k) {
            std::cout << (k + 1) << ". " << songs[k].get_title() << "\n";
        }
    }

    std::vector<int> songs_id_to_add;
    std::cout <<"\nChoisissez les chansons a ajouter (0 pour terminer)\n";

    int song_choice = -1;
    int count = 1;

    while (true) {
        std::cout << "Chanson n" << static_cast<char>(176) << count << " (0 pour finir) : ";

        if (!(std::cin >> song_choice)) {
            std::cout << "Entree invalide.\n";
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            continue;
        }
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

        if (song_choice == 0) {
            break;
        }

        if (song_choice < 1 || song_choice > static_cast<int>(songs.size())) {
            std::cout << "Numero de chanson incorrect.\n";
            continue;
        }

        int id = songs[song_choice - 1].get_id();
        songs_id_to_add.push_back(id);
        count++;
    }
    const int user_id = user.get_id();
    audio.add_playlist(name,songs_id_to_add,user_id);
    std::cout << "Playlist added !\n";
    std::cout <<"~~~END~~~\n";
}

void PlaylistMenu::delete_pl() {
    const int user_id = user.get_id();
    std::vector<Playlist> pl = audio.get_playlist_by_user(user_id);
    int i = 1;
    for (int k = 0; k < pl.size(); ++k) {
        std::cout<<i<<". "<<pl[k].get_name()<<std::endl;
        i++;
    }
    std::cout<<"choisissez la playliste a supprimee : ";
    int choice;
    std::cin >> choice;
    while (choice < 1 || choice > pl.size()) {
        std::cout<<"Invalide choice\n";
        std::cout <<"Choix : ";
        std::cin >> choice;
    }
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    const std::string playlist_name = pl[choice-1].get_name();
    const int playlist_id = pl[choice-1].get_id();
    audio.delete_playlist(playlist_name,playlist_id);
    std::cout << "Playlist deleted !\n";
    std::cout<<"~~~END~~~\n";
}

void PlaylistMenu::run_playlist_menu() {
    bool running = true;
    while (running) {
        switch (const int choice = menu()) {
            case 1:
                see_playlist();
                break;
            case 2:
                add_pl();
                break;
            case 3:
                delete_pl();
                break;
            case 4:
                running = false;
                break;
            default:
                std::cout << "choix invalide\n";
                break;
        }
    }
}
