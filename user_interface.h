//
// Created by lcdelcroix on 11/11/2025.
//

#ifndef BIBLIOTHEQUE_AUDIO_USER_INTERFACE_H
#define BIBLIOTHEQUE_AUDIO_USER_INTERFACE_H
#include <istream>
#include <string>
#include <vector>
#include "audio_player.h"
#include "authManager.h"

class UserInterface {
private :
    AudioPlayer& player;
    AuthManager auth_manager;

    int menu_1();
    void handle_login();
    void handle_register();
    int menu_2() const;
public:
    explicit UserInterface(AudioPlayer& p) : player(p), auth_manager(p) {};
    void run();
};
#endif //BIBLIOTHEQUE_AUDIO_USER_INTERFACE_H