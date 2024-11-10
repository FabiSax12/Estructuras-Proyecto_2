//
// Created by salas on 09/11/2024.
//

#ifndef CLIENTSERIALIZER_H
#define CLIENTSERIALIZER_H

#include <nlohmann/json.hpp>
#include "../structures/Client.h"
#include "../structures/SimpleList.h"

using json = nlohmann::json;

json serializeClients(const SimpleList<Client>& clients);
void saveClientsToFile(const SimpleList<Client>& clients, const std::string& filename);

#endif //CLIENTSERIALIZER_H
