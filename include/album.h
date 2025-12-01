//
// Created by lcdelcroix on 24/11/2025.
//

#ifndef BIBLIOTHEQUE_AUDIO_ALBUM_H
#define BIBLIOTHEQUE_AUDIO_ALBUM_H
#include "audio_player.h"

class AlbumMenu {
private:
    AudioPlayer& audio;
    UserAccount& user;
    public:
    AlbumMenu(AudioPlayer& p,UserAccount& u) : audio(p),user(u) {};
    int menu();
    void display_album() const;
    void add_album() const;
    void delete_al() const;
    void run_album_menu();
};
#endif //BIBLIOTHEQUE_AUDIO_ALBUM_H