#include <iostream>
#include <pqxx/pqxx> // Bibliothèque pour la connexion à PostgreSQL

// Inclure les classes de la couche logique et d'interface
#include "audio_player.h"
#include "authManager.h"
#include "user_interface.h"

// Définitions de la connexion BDD (à ajuster selon votre configuration)
// IMPORTANT : N'exposez jamais les vrais mots de passe ici dans un environnement de production!
const std::string DB_CONN_STRING =
    "dbname=audio_player "
    "user=lounou "
    "password=clemence15lcd "
    "host=localhost "
    "port=5432";

int main() {
    std::cout << "--- Démarrage de l'Application AudioPlayer ---\n";

    try {
        // 1. Initialisation de la Connexion à la Base de Données
        // La connexion est passée par référence à toutes les couches qui en ont besoin.
        std::unique_ptr<pqxx::connection> db_conn = std::make_unique<pqxx::connection>(DB_CONN_STRING);

        if (db_conn->is_open()) {
            std::cout << "Connexion à la base de données réussie.\n";
        } else {
            std::cerr << "Échec de la connexion à la base de données.\n";
            return 1;
        }

        // 2. Initialisation des Couches de Service
        // L'AudioPlayer est le cœur des requêtes BDD (CRUD sur Chansons, Albums, etc.).
        AudioPlayer audio_player(std::move(db_conn));

        // 3. Initialisation du Gestionnaire d'Authentification
        // L'AuthManager utilise AudioPlayer (ou directement la BDD) pour gérer les utilisateurs.
        AuthManager auth_manager(audio_player);

        // 4. Initialisation de l'Interface Utilisateur
        // L'UI est le contrôleur principal, il reçoit les deux autres managers.
        UserInterface ui(audio_player,auth_manager);

        // 5. Lancement de la Boucle Principale de l'Application
        std::cout << "Lancement de l'interface utilisateur...\n";
        ui.run();

        std::cout << "\n--- Application Arrêtée. Au revoir. ---\n";

    } catch (const pqxx::sql_error& e) {
        std::cerr << "\nERREUR SQL: " << e.what() << " (Query: " << e.query() << ")\n";
        return 1;
    } catch (const std::exception& e) {
        std::cerr << "\nERREUR CRITIQUE: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}