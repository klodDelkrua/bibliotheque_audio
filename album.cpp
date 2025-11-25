//
// Created by lcdelcroix on 24/11/2025.
//

#include "album.h"
#include <iostream>
#include <vector>
#include <string>
#include <limits>
#include "audio_player.h"

int AlbumMenu::menu() {
    std::cout <<"\n\t\t~~~ ALBUM MENU ~~~\n";
    std::cout <<"1. Voir la liste des albums\n"
                "2. Ajouter un album\n"
                "3. Supprimer un album\n"
                "4. Retourner au menu principal\n";
    int choice;
    do {
        std::cout << "Choix (1 a 4) : ";

        if (!(std::cin >> choice)) {
            std::cout << "Choix invalide. Veuillez entrer un NUMERO valide.\n";
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            choice = -1;
            continue;
        }

        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

        if (choice < 1 || choice > 4) {
            std::cout << "Choix invalide. Veuillez entrer un numero entre 1 et 4.\n";
        }

    } while (choice < 1 || choice > 4);
    return choice;
}

void AlbumMenu::display_album() const {
    const std::vector<Album> albums = audio.get_all_albums();

    if (albums.empty()) {
        std::cout << "Aucun album enregistre.\n";
        return;
    }

    std::cout<<"\n\t\t~~~ La liste des albums ~~~\n";
    int i = 1;
    for (const auto& album : albums) {
        std::cout << i << ". " << album.get_name() << " (ID: " << album.get_id() << ")\n";
        i++;
    }
    std::cout<< "\t\t~~~ END ~~~\n";
}

void AlbumMenu::add_album() const {
    std::cout<<"\n~~~ AJOUT D'ALBUM ~~~\n";

    std::cout <<"Nom de l'album : ";
    std::string name;
    std::getline(std::cin, name);

    std::vector<Artist> artists = audio.get_all_artists();
    if (artists.empty()) {
        std::cout << "Erreur: Aucun artiste disponible. Creez un artiste d'abord.\n";
        return;
    }

    std::cout <<"\t\t~~~ Liste des Artistes ~~~\n";
    for (size_t k = 0; k < artists.size(); ++k) {
        std::cout << (k + 1) << ". " << artists[k].get_name() << "\n";
    }

    int artist_choice = -1;
    std::cout<<"Choix de l'artiste : ";

    while (!(std::cin >> artist_choice) || artist_choice < 1 || artist_choice > static_cast<int>(artists.size())) {
        std::cout <<"Choix invalide. Entrez un numero valide : ";
        if (std::cin.fail()) std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    }
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

    int artist_id = artists[artist_choice - 1].get_id();

    std::vector<Song> songs = audio.get_all_songs();
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

    audio.add_album(name, artist_id, songs_id_to_add);
    std::cout << "Album cree avec succes !\n";
}

void AlbumMenu::delete_al() const {
    const std::vector<Album> albums = audio.get_all_albums();

    if (albums.empty()) {
        std::cout << "Aucun album a supprimer.\n";
        return;
    }

    std::cout<<"\t\t~~~ Liste des Albums ~~~\n";
    int i = 1;
    for (const auto& album : albums) {
        std::cout << i << ". " << album.get_name() << "\n";
        i++;
    }

    int choice = -1;
    std::cout<<"Choix de l'album a supprimer (0 pour annuler) : ";

    while (!(std::cin >> choice) || choice < 0 || choice > static_cast<int>(albums.size())) {
        std::cout <<"Choix invalide : ";
        if (std::cin.fail()) std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    }
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

    if (choice == 0) return;

    const int album_id = albums[choice-1].get_id();
    const std::string name = albums[choice-1].get_name();
    audio.delete_album(name, album_id);

    std::cout << "Album supprime.\n";
}

void AlbumMenu::run_album_menu() {
    bool running = true;
    while (running) {
        switch (const int choice = menu()) {
            case 1:
                display_album();
                break;
            case 2:
                add_album();
                break;
            case 3:
                delete_al();
                break;
            case 4:
                running = false;
                break;
            default:
                break;
        }
    }
}