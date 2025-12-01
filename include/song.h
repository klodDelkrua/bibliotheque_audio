//
// Created by lcdelcroix on 24/11/2025.
//

#ifndef BIBLIOTHEQUE_AUDIO_SONG_H
#define BIBLIOTHEQUE_AUDIO_SONG_H
#include "audio_player.h"
#include "user_account.h"

class SongMenu {
private:
    AudioPlayer& audio;
    UserAccount& user;

    int select_or_create_artist();

public:
    explicit SongMenu(AudioPlayer &audio_,UserAccount &user_)
        : audio(audio_), user(user_) {
    }

    int menu();
    void see_songs() const;
    void add_song();
    void delete_song();
    void liker_song();
    void run_song();
    void see_liked_songs() const;
    void search_song_ui();

};
#endif //BIBLIOTHEQUE_AUDIO_SONG_H