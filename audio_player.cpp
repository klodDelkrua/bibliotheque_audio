//
// Created by lcdelcroix on 09/11/2025.
//

#include "audio_player.h"
#include <pqxx/pqxx>
#include <memory>
//#include <pqxx/params>

//Methode de la class Song
void Song::set_id(const int id_) {
    id = id_;
}

void Song::set_title(const std::string &title_) {
    title = title_;
}

void Song::set_artist_id(const int artist_id_) {
    artist_id = artist_id_;
}

void Song::set_duration(const int duration_) {
    duration = duration_;
}

int Song::get_id() const {
    return id;
}

std::string Song::get_title() const {
    return title;
}

int Song::get_artist_id() const {
    return artist_id;
}

int Song::get_duration() const {
    return duration;
}

void Artist::set_id(const int id_) {
    id = id_;
}

void Artist::set_name(const std::string &name_) {
    name = name_;
}

int Artist::get_id() const {
    return id;
}

std::string Artist::get_name() const {
    return name;
}

void Playlist::set_id(const int id_) {
    id = id_;
}

int Playlist::get_id() const {
    return id;
}

void Playlist::set_name(const std::string &name_) {
    name = name_;
}

std::string Playlist::get_name() {
    return name;
}

void Album::set_id(const int id_) {
    id = id_;
}

void Album::set_name(const std::string &name_) {
    name = name_;
}

void Album::set_artist_id(const int artist_id_) {
    artist_id = artist_id_;
}

int Album::get_id() const {
    return  id;
}

std::string Album::get_name() const {
    return name;
}

int Album::get_artist_id() const {
    return artist_id;
}


//Methode de la class Audio_Player

std::vector<Song> AudioPlayer::get_all_songs() {

    std::vector<Song> results;
    try {
        pqxx::read_transaction R(*db_connection);

        for (auto row : R.exec("SELECT id,name, artist_id, duration FROM song")) {
            Song s;
            s.set_id(row["id"].as<int>());
            s.set_title(row["name"].as<std::string>());
            s.set_artist_id(row["artist_id"].as<int>());
            s.set_duration(row["duration"].as<int>());
            results.push_back(s);
        }
    }catch (const pqxx::sql_error& e) {
        std::cerr<<"Erreur SQL : "<<e.what()<<std::endl;
        std::cerr <<"Requete : "<<e.query()<<std::endl;
    }catch (const std::exception& e) {
        std::cerr<<"Erreur : "<<e.what()<<std::endl;
    }
    return results;
}

void AudioPlayer::add_artist(const std::string &name) {
    try {
        pqxx::work txn(*db_connection);
        pqxx::params p; //p for parametre
        p.append(name);
        txn.exec("INSERT INTO artist (name) VALUES ($1)",p);
        txn.commit();
    }catch (const pqxx::sql_error& e) {
        std::cerr<<"Erreur SQl : "<<e.what()<<std::endl;
    }
}

void AudioPlayer::add_song(const std::string &title,const int duration,const int artist_id) {
    try {
        pqxx::work txn(*db_connection);
        pqxx::params p;
        p.append(title);
        p.append(duration);
        p.append(artist_id);

        txn.exec("INSERT INTO SONG (name,duration, artist_id)"
                "VALUES ($1, $2, $3)",
                p);
        txn.commit();
        std::cout<<"Song : " <<title<<" added"<<std::endl;
    }catch (const pqxx::sql_error& e) {
        std::cerr<<"Erreur SQL : "<<e.what()<<std::endl;
        std::cerr <<"Requete : "<<e.query()<<std::endl;
    }catch (const std::exception& e) {
        std::cerr<<"Erreur : "<<e.what()<<std::endl;
    }
}

void AudioPlayer::add_playlist(const std::string &name, const std::vector<int> &song_ids,int user_id) {
    if (name.empty() || song_ids.empty()) {
        std::cerr<<"Erreur : Une playlist doit avoir un nom et au moins une chanson\n";
        return;
    }
    try {
        pqxx::work W(*db_connection);
        pqxx::params p_name;
        p_name.append(name);
        p_name.append(user_id);
        //on ajoute "RETURNING id" a la requete.
        //on utilise exec1() qui signifie "execute et attend UNE ligne en retour"
        const pqxx::result res = W.exec("INSERT INTO playlist (name,user_id) VALUES ($1,$2) RETURNING id",p_name);

        if (res.size() != 1) {
            throw std::logic_error("L'insertion de la playlist n'a pas retourne d'ID\n");
        }

        const int new_playlist_id = res[0]["id"].as<int>();

        std::cout <<"la playlist '" <<name<<"' cree avec l'ID : "<<new_playlist_id<<std::endl;
        std::cout<<"Ajout des chansons..."<<std::endl;

        for (int song_id : song_ids) {
            pqxx::params p_link;
            p_link.append(song_id);
            p_link.append(new_playlist_id);

            W.exec("INSERT INTO playlist_song (song_id, playlist_id) VALUES ($1,$2)",p_link);
        }
        W.commit();
        std::cout << "Playlist '" << name << "' ajoutee avec " << song_ids.size() << " chansons.\n";
    }catch (const std::exception& e) {
        std::cerr<<"Erreur lors de la creation de la playliste : "<<e.what()<<std::endl;
        std::cerr << "ANNULATION : la playlist n'a pas ete cree" << std::endl;
    }
}

void AudioPlayer::add_album(const std::string &name,const int artist_id,const std::vector<int>& songs_id) {
    if (name.empty() || songs_id.empty()) {
        std::cerr<<"Erreur : un album doit avoir un nom et au moins une chanson\n";
        return;
    }
    try {
        pqxx::work W(*db_connection);
        pqxx::params p_name;
        p_name.append(name);
        p_name.append(artist_id);

        const pqxx::result res = W.exec("INSERT INTO album (name,artist_id) VALUES ($1,$2) RETURNING id",p_name);

        if (res.size() != 1) {
            throw std::logic_error("L'insertion de l'album n'a pas retourne d'ID\n");
        }
        const int new_album_id = res[0]["id"].as<int>();

        std::cout <<"L'album " <<name<<" cree avec l'ID "<<new_album_id<<std::endl;

        for (int song_id : songs_id) {
            pqxx::params p_link;
            p_link.append(song_id);
            p_link.append(new_album_id);

            W.exec("INSERT INTO album_song (song_id, album_id) VALUES ($1,$2)",p_link);
        }
        W.commit();
        std::cout<<"L'album "<<name <<" ajoutee avec "<<songs_id.size()<<" chansons\n";
    }catch (const std::exception& e) {
        std::cerr<<"Erreur lors de la creation de la playliste : "<<e.what()<<std::endl;
        std::cerr << "ANNULATION : la playlist n'a pas ete cree" << std::endl;
    }
}

void AudioPlayer::delete_artist(const std::string& name,const int artist_id) {
    try {
        pqxx::work W(*db_connection);
        pqxx::params p_link;
        p_link.append(artist_id);

        const pqxx::result res  = W.exec("DELETE FROM artist WHERE id = ($1)",p_link);
        W.commit();
        if (res.affected_rows() > 0) {
            std::cout<<"L'artist '"<<name<<"' (ID : "<<artist_id<<") a ete supprime"<<std::endl;
        }else {
            std::cout <<"ATTENTION : Aucun artiste trouve ave l'ID "<<artist_id<<std::endl;
        }
    }catch (const pqxx::sql_error& e) {
        std::cerr<<"Erreur SQL : "<<e.what()<<std::endl;
        std::cerr <<"Requete : "<<e.query()<<std::endl;
    }catch (const std::exception& e) {
        std::cerr<<"Erreur : "<<e.what()<<std::endl;
    }
}

void AudioPlayer::delete_song(const std::string &title,const int song_id) {
    try {
        pqxx::work W(*db_connection);
        pqxx::params p_link;
        p_link.append(song_id);

        const pqxx::result res = W.exec("DELETE FROM song WHERE id = ($1)",p_link);
        W.commit();
        if (res.affected_rows() > 0) {
            std::cout <<"Le song '"<<title<<"' (ID : "<<song_id<<") a ete supprime"<<std::endl;
        }else {
            std::cout<<"ATTENTION : Aucun song de l'ID "<<song_id<<std::endl;
        }
    }catch (const pqxx::sql_error& e) {
        std::cerr<<"Erreur SQL : "<<e.what()<<std::endl;
        std::cerr <<"Requete : "<<e.query()<<std::endl;
    }catch (const std::exception& e) {
        std::cerr<<"Erreur : "<<e.what()<<std::endl;
    }
}

void AudioPlayer::delete_playlist(const std::string &name,const int playlist_id) {
    try {
        pqxx::work W(*db_connection);
        pqxx::params p_link;
        p_link.append(playlist_id);

        const pqxx::result res = W.exec("DELETE FROM playlist WHERE id = ($1)",p_link);
        W.commit();
        if (res.affected_rows() > 0) {
            std::cout <<"La playlist '" <<name<<"' (ID : "<<playlist_id<<") a ete supprime"<<std::endl;
        }else {
            std::cout<<"ATTENTION : Aucune playlist de l'ID : "<<playlist_id<<std::endl;
        }
    }catch (const pqxx::sql_error& e) {
        std::cerr<<"Erreur SQL : "<<e.what()<<std::endl;
        std::cerr <<"Requete : "<<e.query()<<std::endl;
    }catch (const std::exception& e) {
        std::cerr<<"Erreur : "<<e.what()<<std::endl;
    }
}

void AudioPlayer::delete_album(const std::string &name, int album_id) {
    try {
        pqxx::work W(*db_connection);
        pqxx::params p_link;
        p_link.append(album_id);

        const pqxx::result res = W.exec("DELETE FROM album WHERE id = ($1)",p_link);
        W.commit();
        if (res.affected_rows() > 0) {
            std::cout <<"L'album '"<<name<<"' (ID : "<<album_id<<") a ete supprime"<<std::endl;
        }else {
            std::cout <<"ATTENTION : Aucun album de l'ID : "<<album_id<<std::endl;
        }
    }catch (const pqxx::sql_error& e) {
        std::cerr<<"Erreur SQL : "<<e.what()<<std::endl;
        std::cerr <<"Requete : "<<e.query()<<std::endl;
    }catch (const std::exception& e) {
        std::cerr<<"Erreur : "<<e.what()<<std::endl;
    }
}

std::vector<Playlist> AudioPlayer::get_playlist_by_user(const int user_id){
    std::vector<Playlist> playlist;
    try {
        pqxx::read_transaction W(*db_connection);
        pqxx::params p_link;
        p_link.append(user_id);
        for (auto row : W.exec("SELECT id,name FROM playlist WHERE user_id = $1",p_link)) {
            Playlist s;
            s.set_id(row["id"].as<int>());
            s.set_name(row["name"].as<std::string>());
            playlist.push_back(s);
        }
        W.commit();
    }catch (const pqxx::sql_error& e) {
        std::cerr<<"Erreur SQL : "<<e.what()<<std::endl;
        std::cerr <<"Requete : "<<e.query()<<std::endl;
    }catch (const std::exception& e) {
        std::cerr<<"Erreur : "<<e.what()<<std::endl;
    }
    return playlist;
}

std::vector<Artist> AudioPlayer::get_all_artists() {
    std::vector<Artist> artists;
    try {
        pqxx::read_transaction R(*db_connection);
        for (auto row : R.exec("SELECT id,name FROM artist")) {
            Artist a;
            a.set_id(row["id"].as<int>());
            a.set_name(row["name"].as<std::string>());
            artists.push_back(a);
        }
        R.commit();
    }catch (const pqxx::sql_error& e) {
        std::cerr<<"Erreur SQL : "<<e.what()<<std::endl;
        std::cerr <<"Requete : "<<e.query()<<std::endl;
    }catch (const std::exception& e) {
        std::cerr<<"Erreur : "<<e.what()<<std::endl;
    }
    return std::move(artists);
}

std::vector<Album> AudioPlayer::get_all_albums() {
    std::vector<Album> albums;
    try {
        pqxx::read_transaction R(*db_connection);
        for (auto row : R.exec("SELECT id,name,artist_id FROM album")) {
            Album a;
            a.set_id(row["id"].as<int>());
            a.set_name(row["name"].as<std::string>());
            a.set_artist_id(row["artist_id"].as<int>());
            albums.push_back(a);
        }
        R.commit();
    }catch (const pqxx::sql_error& e) {
        std::cerr<<"Erreur SQL : "<<e.what()<<std::endl;
        std::cerr <<"Requete : "<<e.query()<<std::endl;
    }catch (const std::exception& e) {
        std::cerr<<"Erreur : "<<e.what()<<std::endl;
    }
    return albums;
}

int AudioPlayer::add_user(const std::string &username, const std::string &hash, const std::string &email) {
    if (username.empty() || hash.empty() || email.empty()) {
        std::cerr<<"Erreur aucune information ne doit etre vide\n";
        return -1;
    }
    int new_id = -1;
    try {
        pqxx::work W(*db_connection);
    pqxx::params p;
        p.append(username);
        p.append(hash);
        p.append(email);

        const pqxx::result res = W.exec("INSERT INTO user_account (username,password_hash,email) VALUES ($1) RETURNING id",p );
        new_id = res[0]["id"].as<int>();
        W.commit();
        std::cout<<" Ajout de l'utilisateur << '"<<username<<"' cree avec l'ID : "<<new_id<<std::endl;

    }catch (const pqxx::sql_error& e) {
        std::cerr<<"Erreur SQL : "<<e.what()<<std::endl;
        std::cerr <<"Requete : "<<e.query()<<std::endl;
    }catch (const std::exception& e) {
        std::cerr<<"Erreur : "<<e.what()<<std::endl;
    }
    return new_id;
}

std::unique_ptr<UserAccount> AudioPlayer::get_user_by_username(const std::string &username) {
    try {
        pqxx::read_transaction R(*db_connection);
        pqxx::params p;
        p.append(username);

        const pqxx::result res = R.exec("SELECT id, username, password_hash, email "
            "FROM user_account WHERE username = $1",p);

        if (res.empty()) {
            std::cout<<"Utilisateur non trouve \n";
            return nullptr;
        }
        const pqxx::row row = res[0];

        return std::make_unique<UserAccount>(
            row["id"].as<int>(),
            row["username"].as<std::string>(),
            row["password_hash"].as<std::string>(),
            row["email"].as<std::string>());
    }catch (const std::exception& e) {
        // En cas d'erreur BDD (connexion perdue, etc.), on signale l'échec
        // en retournant aussi un pointeur nul.
        std::cerr << "Erreur BDD [get_user_by_username]: " << e.what() << std::endl;
        return nullptr;
    }
}



