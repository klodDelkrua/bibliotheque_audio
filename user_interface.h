//
// Created by lcdelcroix on 11/11/2025.
//

#ifndef BIBLIOTHEQUE_AUDIO_USER_INTERFACE_H
#define BIBLIOTHEQUE_AUDIO_USER_INTERFACE_H
#include <istream>
#include <string>
#include <vector>
#include "audio_player.h"

class UserInterface {
private :
    AudioPlayer& player;

public:
    static int menu_1();
};
#endif //BIBLIOTHEQUE_AUDIO_USER_INTERFACE_H