//
// Created by lcdelcroix on 11/11/2025.
//

#ifndef BIBLIOTHEQUE_AUDIO_USER_INTERFACE_H
#define BIBLIOTHEQUE_AUDIO_USER_INTERFACE_H
#include <istream>
#include <string>
#include <vector>
#include "audio_player.h"

class User_Interface {
private :
    AudioPlayer& audioPlay;
    explicit User_Interface(AudioPlayer& audioPlay) : audioPlay(audioPlay) {}
    int menu();
    int song_menu();
    void song_operation(AudioPlayer& player,int choice);
    void add_song_to_database();
public:
    void run_application();
};
#endif //BIBLIOTHEQUE_AUDIO_USER_INTERFACE_H