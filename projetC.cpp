// projetC.cpp : Ce fichier contient la fonction 'main'. L'exécution du programme commence et se termine à cet endroit.
//

#include <Windows.h>
#include <iostream>
#include "stdlib.h"
#include <filesystem>
#include <sstream>
#include <chrono>
#include <iomanip>
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

std::string convertir_en_date_lisible(const fs::file_time_type& ftime) {
    auto sctp = std::chrono::time_point_cast<std::chrono::system_clock::duration>(
        ftime - fs::file_time_type::clock::now() + std::chrono::system_clock::now());

    std::time_t temps = std::chrono::system_clock::to_time_t(sctp);
    std::tm tm_result;
    localtime_s(&tm_result, &temps);

    std::stringstream ss;
    ss << std::put_time(&tm_result, "%Y-%m-%d %H:%M:%S");
    return ss.str();
}

/*std::string convertir_en_date_lisible(const FILETIME& filetime) {
    // Convertir FILETIME en un nombre de nanosecondes depuis l'epoch Unix
    ULARGE_INTEGER ull;
    ull.LowPart = filetime.dwLowDateTime;
    ull.HighPart = filetime.dwHighDateTime;

    // Convertir en nanosecondes depuis 1601
    auto ns_since_1601 = ull.QuadPart * 100;

    // L'epoch Unix (1er janvier 1970) est 11644473600 secondes après 1601
    constexpr uint64_t UNIX_EPOCH = 11644473600ULL;
    auto ns_since_unix_epoch = ns_since_1601 - UNIX_EPOCH * 10000000ULL;

    // Convertir en time_point basé sur system_clock
    auto tp = std::chrono::system_clock::time_point(std::chrono::nanoseconds(ns_since_unix_epoch));

    // Formater la date en chaîne lisible
    std::time_t temps = std::chrono::system_clock::to_time_t(tp);
    std::tm tm_result;
    localtime_s(&tm_result, &temps);

    std::stringstream ss;
    ss << std::put_time(&tm_result, "%Y-%m-%d %H:%M:%S");
    return ss.str();
}

std::string obtenir_creation_time(const std::string& chemin) {
    HANDLE hFile = CreateFile(
        chemin.c_str(),
        GENERIC_READ,
        FILE_SHARE_READ,
        NULL,
        OPEN_EXISTING,
        FILE_ATTRIBUTE_NORMAL,
        NULL
    );

    if (hFile == INVALID_HANDLE_VALUE) {
        return "Erreur : Impossible d'ouvrir le fichier";
    }

    FILETIME creationTime, lastAccessTime, lastWriteTime;
    if (GetFileTime(hFile, &creationTime, &lastAccessTime, &lastWriteTime)) {
        CloseHandle(hFile);
        return convertir_en_date_lisible(creationTime);
    }

    CloseHandle(hFile);
    return "Erreur : Impossible de lire la date de création";
}*/

uintmax_t taille_dossier(const fs::path& dossier) {
    uintmax_t taille_totale = 0;
    for (const auto& entry : fs::recursive_directory_iterator(dossier)) {
        if (fs::is_regular_file(entry)) {
            taille_totale += fs::file_size(entry);
        }
    }
    return taille_totale;
}

void lister_fichiers_et_dossiers(const std::string& chemin) {
    // Vérifier si le répertoire existe
    if (!fs::exists(chemin)) {
        std::cerr << "Le répertoire spécifié n'existe pas : " << chemin << std::endl;
        return;
    }

    //std::ofstream fichier("C:/Users/ngiroud0/Documents/resultat.json");
    
    std::ofstream fichier;
    fichier.open("C:/Users/ngiroud0/Documents/GitHub/resultat.json", ios_base::out | ios_base::binary);
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
            element["taille"] = taille_dossier(entry.path());
            std::cout << "[Dossier] " << entry.path().filename().string() << std::endl;
        }

        //element["creation"] = convertir_en_date_lisible(fs::creation_time(entry));
        element["creation"] = convertir_en_date_lisible(fs::last_write_time(entry));
        element["modification"] = convertir_en_date_lisible(fs::last_write_time(entry));

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