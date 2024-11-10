//
// Created by salas on 09/11/2024.
//

#include "ClientSerializer.h"
#include <fstream>

json serializeClients(const SimpleList<Client>& clients) {
    json j_clients;
    for (const auto& client : clients) {
        json j_client;
        j_client["nombre"] = client.name;
        j_client["puntos"] = client.getPoints();

        for (const auto& t : client.getTrips()) {
            json j_travel;
            j_travel["origen"] = t.originCountry;
            j_travel["destino"] = t.destinationCountry;
            j_travel["transporte"] = t.transportMethod;
            j_travel["horas"] = t.travelTime;
            j_client["viajes"].push_back(j_travel);
        }

        for (const auto& reward : client.getRewards()) {
            json j_reward;
            j_reward["nombre"] = reward.name;
            j_reward["puntosUsados"] = reward.pointsRequired;
            j_client["premios"].push_back(j_reward);
        }

        j_clients["clientes"].push_back(j_client);
    }
    return j_clients;
}

void saveClientsToFile(const SimpleList<Client>& clients, const std::string& filename) {
    json j_clients = serializeClients(clients);
    std::ofstream file(filename);
    file << j_clients.dump(4);
    file.close();
}
