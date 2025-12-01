//
// Created by lcdelcroix on 24/11/2025.
//

#include "song.h"
#include "audio_player.h"
#include <iostream>
#include <string>
#include <vector>

int SongMenu::menu() {
    int choice;
    std::cout<<"1. Voir la liste des chansons\n"
                "2. Ajouter une chanson\n"
                "3. Supprimer une chanson\n"
                "4. Liker une chanson\n"
                "5. Voir mes chansons aimee\n"
                "6. Rechercher une chanson\n"
                "7. Retour aux menu principale\n";
    while (!(std::cin >> choice) || choice < 0 || choice > 7) {
        std::cout << "Choix invalide. Veuillez entrer un numéro entre 1 et " << 7 << " : ";

        // Si l'entree n'etait pas un nombre :
        if (std::cin.fail()) {
            std::cin.clear(); // Reinitialise le flag d'erreur
        }
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // Vide le buffer
    }
    return choice;
}

void SongMenu::see_songs() const {
    const std::vector<Song> songs = audio.get_all_songs();
    int i = 1;
    std::cout<<"\t\t~~~SONGS~~~\n";
    for ( const auto& s : songs ) {
        std::cout<<i<<". "<<s.get_title()<<std::endl;
        i++;
    }
    std::cout<<"\t\t~~~END~~~\n";
}

int SongMenu::select_or_create_artist() {
    std::cout << "\n--- Selection de l'Artiste ---\n";

    const std::vector<Artist> artists = audio.get_all_artists();

    // 2. Les afficher
    if (artists.empty()) {
        std::cout << "Aucun artiste trouve.\n";
    } else {
        for (size_t i = 0; i < artists.size(); ++i) {
            std::cout << (i + 1) << ". " << artists[i].get_name() << "\n";
        }
    }
    std::cout << "0. Creer un nouvel artiste\n";

    // 3. Demander le choix
    int choice;
    std::cout << "Votre choix (Numero ou 0) : ";
    while (!(std::cin >> choice) || choice < 0 || choice > static_cast<int>(artists.size())) {
        std::cout << "Choix invalide. Reessayez : ";
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    }
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

    if (choice == 0) {
        std::string new_name;
        std::cout << "Nom du nouvel artiste : ";
        std::getline(std::cin, new_name);
        return audio.add_artist(new_name);
    } else {
        return artists[choice - 1].get_id();
    }
}

void SongMenu::add_song() {
    std::cout << "\n--- AJOUT D'UNE CHANSON ---\n";

    const int artist_id = select_or_create_artist();

    if (artist_id == -1 || artist_id == 0) {
        std::cout << "Erreur lors de la selection de l'artiste. Annulation.\n";
        return;
    }

    std::string title;
    int duration;

    std::cout << "Titre de la chanson : ";
    std::getline(std::cin, title);

    std::cout << "Durée (en secondes) : ";
    std::cin >> duration;
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

    audio.add_song(title, duration, artist_id);

    std::cout << "La chanson a été ajoutée avec succès !\n";
}

void SongMenu::delete_song() {
    const std::vector<Song> songs = audio.get_all_songs();

    if (songs.empty()) {
        std::cout << "Il n'y a aucune chanson a supprimer.\n";
        return;
    }

    std::cout << "\n--- SUPPRESSION D'UNE CHANSON ---\n";
    int i = 1;
    for (const auto& s : songs) {
        std::cout << i << ". " << s.get_title() << std::endl;
        i++;
    }

    int choice;
    std::cout << "Entrez le numero de la chanson a supprimer (ou 0 pour annuler) : ";

    while (!(std::cin >> choice) || choice < 0 || choice > static_cast<int>(songs.size())) {
        std::cout << "Choix invalide. Veuillez entrer un numero entre 1 et " << songs.size() << " : ";
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    }
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

    if (choice == 0) {
        std::cout << "Suppression annulee.\n";
        return;
    }

    const int index = choice - 1;
    const int song_id_to_delete = songs[index].get_id();
    const std::string song_title = songs[index].get_title();

    std::cout << "Etes-vous sur de vouloir supprimer la chanson '" << song_title << "' ? (O/N) : ";
    char confirmation;
    std::cin >> confirmation;
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

    if (confirmation == 'o' || confirmation == 'O') {
        // 5. Suppression BDD
        if (audio.delete_song(song_id_to_delete)) {
            std::cout << "La chanson '" << song_title << "' a ete supprimée definitivement.\n";
        } else {
            std::cout << "Echec de la suppression de la chanson.\n";
        }
    } else {
        std::cout << "Suppression annulee.\n";
    }
}

void SongMenu::liker_song() {
    const std::vector<Song> songs = audio.get_all_songs();

    if (songs.empty()) {
        std::cout << "Il n'y a aucune chanson a liker.\n";
        return;
    }

    int choice = -1;

    std::cout << "\n--- LIKER UNE CHANSON ---\n";
    std::cout << "Quelle chanson voulez-vous liker?\n";
    int i = 1;
    for (const auto& s : songs) {
        std::cout << i << ". " << s.get_title() << std::endl;
        i++;
    }

    do {
        std::cout << "Choix (1 à " << songs.size() << ") : ";

        if (!(std::cin >> choice)) {
            std::cout << "Choix invalide. Veuillez entrer un NUMÉRO valide.\n";
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            choice = -1;
            continue;
        }

        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

        if (choice < 1 || choice > static_cast<int>(songs.size())) {
            std::cout << "Choix invalide. Veuillez entrer un numéro entre 1 et " << songs.size() << ".\n";
        }

    } while (choice < 1 || choice > static_cast<int>(songs.size()));

    const int user_id = user.get_id();
    const int song_id = songs[choice - 1].get_id();

    audio.like_song(user_id, song_id, true);
    std::cout << "Chanson '" << songs[choice - 1].get_title() << "' a ete ajoutee a vos favoris.\n";
}
void SongMenu::see_liked_songs() const {
    const int user_id = user.get_id();
    const std::vector<Song> liked_songs = audio.get_liked_songs(user_id);

    std::cout << "\n--- VOS CHANSONS AIMEES [" << user.get_name() << "] ---\n";

    if (liked_songs.empty()) {
        std::cout << "Vous n'avez pas encore aime de chansons.\n";
        return;
    }

    int i = 1;
    for (const auto& s : liked_songs) {
        std::cout << i << ". " << s.get_title() << " (Artiste ID: " << s.get_artist_id() << ")\n";
        i++;
    }
    std::cout << "-------------------------------------------------------\n";
}

void SongMenu::search_song_ui() {
    std::cout << "\n--- RECHERCHE DE CHANSON ---\n";
    std::cout << "Entrez le titre (ou une partie du titre) : ";
    std::string query;
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    std::getline(std::cin, query);

    // Appel à la fonction chronométrée
    std::vector<Song> results = audio.search_song_by_title(query);

    if (results.empty()) {
        std::cout << "Aucune chanson trouvee.\n";
    } else {
        std::cout << "\n--- RESULTATS ---\n";
        int i = 1;
        for (const auto& s : results) {
            std::cout << i << ". " << s.get_title() << " (" << s.get_duration() << "s)\n";
            i++;
        }
    }
}

void SongMenu::run_song() {
    bool running = true;
    while (running) {
        switch (const int choice = menu()) {
            case 1:
                see_songs();
                break;
            case 2:
                add_song();
                break;
            case 3:
                delete_song();
                break;
            case 4:
                liker_song();
                break;
            case 5:
                see_liked_songs();
                break;
            case 6 :
                search_song_ui();
                break;
            case 7:
                running = false;
                break;
            default:
                std::cout << "Choix invalide.\n";
                break;
        }
    }
}
