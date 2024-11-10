//
// Created by salas on 09/11/2024.
//

#ifndef TRAVELGRAPHSERIALIZER_H
#define TRAVELGRAPHSERIALIZER_H

#include <nlohmann/json.hpp>
#include "../structures/TravelGraph.h"

using json = nlohmann::json;

bool InvalidCharacters(const std::string& texto);

void updateJSONDestinations(const std::string& countryName, const std::string& ep_name, EntryPointType ep_type, const std::string& filename = "travel_graph.json");

#endif //TRAVELGRAPHSERIALIZER_H
