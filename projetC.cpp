// projetC.cpp : Ce fichier contient la fonction 'main'. L'exécution du programme commence et se termine à cet endroit.
//

#include <iostream>
#include "stdlib.h"
#include <filesystem>
#include <fstream>
#include <vector>
#include <locale>
#include <Windows.h>
#include <nlohmann/json.hpp>

// Exécuter le programme : Ctrl+F5 ou menu Déboguer > Exécuter sans débogage
// Déboguer le programme : F5 ou menu Déboguer > Démarrer le débogage

// Astuces pour bien démarrer : 
//   1. Utilisez la fenêtre Explorateur de solutions pour ajouter des fichiers et les gérer.
//   2. Utilisez la fenêtre Team Explorer pour vous connecter au contrôle de code source.
//   3. Utilisez la fenêtre Sortie pour voir la sortie de la génération et d'autres messages.
//   4. Utilisez la fenêtre Liste d'erreurs pour voir les erreurs.
//   5. Accédez à Projet > Ajouter un nouvel élément pour créer des fichiers de code, ou à Projet > Ajouter un élément existant pour ajouter des fichiers de code existants au projet.
//   6. Pour rouvrir ce projet plus tard, accédez à Fichier > Ouvrir > Projet et sélectionnez le fichier .sln.

namespace fs = std::filesystem;

void lister_fichiers_et_dossiers(const std::string& chemin) {
    // Vérifier si le répertoire existe
    if (!fs::exists(chemin)) {
        std::cerr << "Le répertoire spécifié n'existe pas : " << chemin << std::endl;
        return;
    }

    std::ofstream fichier("resulat.json");
    fichier << "[" << std::endl;
    bool premier = true;

    // Parcourir le répertoire
    std::cout << "\nContenu du répertoire \"" << chemin << "\":" << std::endl;

    for (const auto& entry : fs::directory_iterator(chemin)) {
        nlohmann::json element; // JSON pour chaque élément
        element["nom"] = entry.path().filename().string();

        if (fs::is_regular_file(entry)) {
            element["type"] = "Fichier";
            element["taille"] = fs::file_size(entry);
            // Afficher le fichier
            std::cout << "[Fichier] " << entry.path().filename().string() << std::endl;
        }
        else if (fs::is_directory(entry)) {
            // Afficher le dossier
            element["type"] = "Dossier";
            std::cout << "[Dossier] " << entry.path().filename().string() << std::endl;
        }

        if (!premier) {
            fichier << "," << std::endl;
        }
        premier = false;
        fichier << "  " << element.dump(4);
    }

    fichier << std::endl << "]" << std::endl;
    fichier.close();
}

int main() {
    std::string chemin;
    std::cout << "Entrez le chemin du répertoire à explorer : ";
    std::cin >> chemin;

    lister_fichiers_et_dossiers(chemin);

    return 0;
}