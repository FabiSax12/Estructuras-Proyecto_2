//
// Created by salas on 09/11/2024.
//

#include "TravelGraphSerializer.h"
#include <fstream>
#include <nlohmann/json.hpp>
#include <iostream>

using json = nlohmann::json;

bool InvalidCharacters(const std::string& text) {
    for (unsigned char c : text) {
        if (c >= 128) {
            return true;
        }
    }
    return false;
}

void updateJSONDestinations(const std::string& countryName, const std::string& ep_name, EntryPointType ep_type, const std::string& filename) {
    if (InvalidCharacters(countryName) || InvalidCharacters(ep_name)) {
        std::cerr << "Error:\n\tEl nombre del pais o del punto de entrada contiene caracteres no permitidos\n\t(tildes u otros caracteres especiales).\n\tPor favor, use solo letras sin acentos.\n\n";
        system("pause");
        return;
    }

    std::ifstream file(filename);
    json j_graph;

    if (file.is_open()) {
        file >> j_graph;
        file.close();
    }

    bool countryExists = false;
    for (auto& j_country : j_graph["paises"]) {
        if (j_country["nombre"] == countryName) {
            countryExists = true;

            json j_entryPoint;
            j_entryPoint["nombre"] = ep_name;
            j_entryPoint["tipo"] = (ep_type == EntryPointType::AIRPORT) ? "Airport" : (ep_type == EntryPointType::BORDER) ? "Border" : "Port";
            j_country["puntos_de_entrada"].push_back(j_entryPoint);

            break;
        }
    }

    if (!countryExists) {
        json j_newCountry;
        j_newCountry["nombre"] = countryName;

        json j_entryPoint;
        j_entryPoint["nombre"] = ep_name;
        j_entryPoint["tipo"] = (ep_type == EntryPointType::AIRPORT) ? "Airport" : (ep_type == EntryPointType::BORDER) ? "Border" : "Port";
        j_newCountry["puntos_de_entrada"] = {j_entryPoint};

        j_graph["paises"].push_back(j_newCountry);
    }

    std::ofstream outputFile(filename);
    if (outputFile.is_open()) {
        outputFile << j_graph.dump(2);
        outputFile.close();
        std::cout << "Destino guardado correctamente!.\n";
    } else {
        std::cerr << "Error al guardar los datos.: " <<std::endl;
    }

}

void deleteJSONDestination(const std::string& countryName, const std::string& ep_name, const std::string& filename) {
    if (InvalidCharacters(countryName) || InvalidCharacters(ep_name)) {
        std::cerr << "Error:\n\tEl nombre del país o del punto de entrada contiene caracteres no permitidos\n\t(tildes u otros caracteres especiales).\n\tPor favor, use solo letras sin acentos.\n\n";
        system("pause");
        return;
    }

    std::ifstream file(filename);
    json j_graph;

    if (file.is_open()) {
        try {
            file >> j_graph;
        } catch (const json::parse_error& e) {
            std::cerr << "Error al analizar el JSON: " << e.what() << std::endl;
            file.close();
            return;
        }
        file.close();
    }

    bool countryExists = false;
    bool entryPointDeleted = false;

    // Buscar el país y eliminar el punto de entrada si existe
    for (auto& j_country : j_graph["paises"]) {
        if (j_country["nombre"] == countryName) {
            countryExists = true;

            // Buscar y eliminar el punto de entrada
            for (auto it = j_country["puntos_de_entrada"].begin(); it != j_country["puntos_de_entrada"].end(); ++it) {
                if ((*it)["nombre"] == ep_name) {
                    j_country["puntos_de_entrada"].erase(it);
                    entryPointDeleted = true;
                    std::cout << "Punto de entrada '" << ep_name << "' eliminado del país '" << countryName << "'.\n";
                    break;
                }
            }

            // Eliminar el país si no tiene más puntos de entrada
            if (j_country["puntos_de_entrada"].empty()) {
                j_graph["paises"].erase(std::remove(j_graph["paises"].begin(), j_graph["paises"].end(), j_country), j_graph["paises"].end());
                std::cout << "País '" << countryName << "' eliminado porque no tiene más puntos de entrada.\n";
            }

            break;
        }
    }

    if (!countryExists) {
        std::cerr << "El país '" << countryName << "' no se encontró en el archivo JSON.\n";
    } else if (!entryPointDeleted) {
        std::cerr << "El punto de entrada '" << ep_name << "' no se encontró en el país '" << countryName << "'.\n";
    }

    // Guardar el archivo JSON actualizado
    std::ofstream outputFile(filename);
    if (outputFile.is_open()) {
        outputFile << j_graph.dump(2);
        outputFile.close();
        std::cout << "Archivo JSON actualizado.\n";
    } else {
        std::cerr << "Error al guardar los datos." << std::endl;
    }
    system("pause");
}
