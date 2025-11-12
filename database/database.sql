CREATE TABLE artist(
    id INTEGER GENERATED ALWAYS AS IDENTITY PRIMARY KEY,
    name VARCHAR(255) NOT NULL UNIQUE
);

CREATE TABLE album(
    id INTEGER GENERATED ALWAYS AS IDENTITY PRIMARY KEY,
    name VARCHAR(255) NOT NULL,
    artist_id INTEGER NOT NULL,
    CONSTRAINT fk_album_artist FOREIGN KEY (artist_id) REFERENCES artist(id) ON DELETE CASCADE
);

CREATE TABLE song(
    id INTEGER GENERATED ALWAYS AS IDENTITY PRIMARY KEY,
    name VARCHAR(255) NOT NULL,
    duration INTEGER ,
    artist_id INTEGER NOT NULL,
    CONSTRAINT fk_song_artist FOREIGN KEY (artist_id) REFERENCES artist(id) ON DELETE CASCADE
);

CREATE TABLE user_account(
    id INTEGER GENERATED ALWAYS AS IDENTITY PRIMARY KEY,
    username VARCHAR(255) NOT NULL UNIQUE,
    password_hash TEXT NOT NULL,
    email VARCHAR(255) NOT NULL UNIQUE
);

CREATE TABLE playlist(
    id INTEGER GENERATED ALWAYS AS IDENTITY PRIMARY KEY,
    name VARCHAR(255) NOT NULL,
    user_id INTEGER NOT NULL,
    CONSTRAINT fk_playlist_user FOREIGN KEY (user_id) REFERENCES user_account(id) ON DELETE CASCADE
);

CREATE TABLE playlist_song(
    song_id INTEGER NOT NULL,
    playlist_id INTEGER NOT NULL,
    CONSTRAINT fk_play_song_song FOREIGN KEY (song_id) REFERENCES song(id) ON DELETE CASCADE,
    CONSTRAINT fk_play_song_playlist FOREIGN KEY (playlist_id) REFERENCES playlist(id) ON DELETE CASCADE,
    PRIMARY KEY(song_id, playlist_id)
);

CREATE TABLE album_song(
    album_id INTEGER NOT NULL,
    song_id INTEGER NOT NULL,
    CONSTRAINT fk_album_song_album FOREIGN KEY (album_id) REFERENCES album(id) ON DELETE CASCADE,
    CONSTRAINT fk_album_song_song FOREIGN KEY (song_id) REFERENCES song(id) ON DELETE CASCADE,
    PRIMARY KEY(album_id, song_id)
);

CREATE TABLE song_featuring(
    song_id INTEGER NOT NULL,
    artist_id INTEGER NOT NULL,
    CONSTRAINT fk_song_featuring_song FOREIGN KEY (song_id) REFERENCES song (id) ON DELETE CASCADE,
    CONSTRAINT fk_song_featuring_artist FOREIGN KEY (artist_id) REFERENCES artist(id) ON DELETE CASCADE,
    PRIMARY KEY(song_id, artist_id)
);

CREATE TABLE genre(
    id INTEGER GENERATED ALWAYS AS IDENTITY PRIMARY KEY,
    name VARCHAR(255) NOT NULL UNIQUE
);

CREATE TABLE song_genre(
    song_id INTEGER NOT NULL,
    genre_id INTEGER NOT NULL,
    CONSTRAINT fk_song_genre_song FOREIGN KEY (song_id) REFERENCES song(id) ON DELETE CASCADE,
    CONSTRAINT fk_song_genre_genre FOREIGN KEY (genre_id) REFERENCES genre(id) ON DELETE CASCADE,
    PRIMARY KEY(song_id, genre_id)
);

CREATE TABLE user_song_interaction(
    user_id INTEGER NOT NULL,
    song_id INTEGER NOT NULL,
    is_liked BOOLEAN DEFAULT FALSE,
    rating INTEGER DEFAULT NULL,
    CONSTRAINT fk_user_song_interaction_user FOREIGN KEY (user_id) REFERENCES user_account(id) ON DELETE CASCADE,
    CONSTRAINT fk_user_song_interaction_song Foreign Key (song_id) REFERENCES song(id) ON DELETE CASCADE,
    PRIMARY KEY(user_id, song_id)
);

CREATE TABLE listen_history(
    id INTEGER GENERATED ALWAYS AS IDENTITY PRIMARY KEY,
    user_id INTEGER NOT NULL,
    song_id INTEGER NOT NULL,
    listen_time TIMESTAMPTZ DEFAULT NOW(),
    CONSTRAINT fk_listen_history_user FOREIGN KEY (user_id) REFERENCES user_account(id) ON DELETE CASCADE,
    CONSTRAINT fk_listen_history_song FOREIGN KEY (song_id) REFERENCES song(id) ON DELETE CASCADE
);