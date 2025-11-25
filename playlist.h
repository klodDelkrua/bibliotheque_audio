//
// Created by lcdelcroix on 25/11/2025.
//

#ifndef BIBLIOTHEQUE_AUDIO_PLAYLIST_H
#define BIBLIOTHEQUE_AUDIO_PLAYLIST_H
#include "audio_player.h"
#include "user_account.h"

class PlaylistMenu {
private:
    AudioPlayer& audio;
    UserAccount& user;

    public:
    PlaylistMenu(AudioPlayer& p,UserAccount& u) : audio(p), user(u) {};
    int menu();
    void see_playlist();
    void add_pl();
    void delete_pl();
    void run_playlist_menu();
};
#endif //BIBLIOTHEQUE_AUDIO_PLAYLIST_H