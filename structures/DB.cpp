//
// Created by varga on 29/10/2024.
//

#include "DB.h"
#include <fstream>
#include <iostream>
#include <nlohmann/json.hpp>
#include <filesystem>
#include "TravelGraph.h"

using namespace std;
using json = nlohmann::json;

std::vector<std::string> split(const std::string& s, const std::string& delimiter) {
	size_t pos_start = 0, pos_end, delim_len = delimiter.length();
	std::string token;
	std::vector<std::string> res;

	while ((pos_end = s.find(delimiter, pos_start)) != std::string::npos) {
		token = s.substr(pos_start, pos_end - pos_start);
		res.push_back(token);
		pos_start = pos_end + delim_len;
	}

	res.push_back(s.substr(pos_start));
	return res;
}

std::ifstream openFile(const std::string& filePath) {
	std::ifstream file(filePath);
	if (!file.is_open()) {
		throw std::runtime_error("Error opening file: " + filePath);
	}

	return file;
}

std::filesystem::path DB::getProjectRoot() {
	std::filesystem::path currentPath = std::filesystem::current_path();
	std::string buildDir = "cmake-build-debug";
	std::string buildDir2 = "cmake-build-debug-prueba";

	if (currentPath.filename()== buildDir || currentPath.filename() == buildDir2) {
		currentPath = currentPath.parent_path();
	}
	return currentPath;
}

json getHoleRoute(string filePath) {
	std::filesystem::path projectRoot = DB::getProjectRoot();
	std::filesystem::path fullPath = projectRoot / filePath;
	// std::cout << "Esta es la ruta obtenida: " << fullPath << std::endl;
	json data;
	std::ifstream file(fullPath);
	file >> data;
	return data;
}

void DB::loadDestinationsAndRoutes(const std::string& filePath, TravelGraph& graph) {
	json data = getHoleRoute(filePath);

	for (const auto& country : data["paises"]) {
		std::string countryName = country["nombre"];

		for (const auto& entryPoint : country["puntos_de_entrada"]) {
			std::string entryName = entryPoint["nombre"];
			std::string entryTypeString = entryPoint["tipo"];

			EntryPointType entryType;
			if (entryTypeString == "Aeropuerto" || entryTypeString == "Airport") entryType = EntryPointType::AIRPORT;
			else if (entryTypeString == "Frontera" || entryTypeString == "Border") entryType = EntryPointType::BORDER;
			else if (entryTypeString == "Puerto" || entryTypeString == "Port") entryType = EntryPointType::PORT;

			auto destination = new Destination(countryName, entryName, new EntryPoint(entryType, entryName));
			graph.addDestination(destination);
		}
	}

	for (const auto& route : data["rutas"]) {
		std::string originCountry = route["origen"]["pais"];
		std::string originEntryPoint = route["origen"]["punto_de_entrada"];
		std::string destinationCountry = route["destino"]["pais"];
		std::string destinationEntryPoint = route["destino"]["punto_de_entrada"];
		std::string transportMode = route["transporte"];
		double travelTime = route["horas"];

		TransportMethod tm_type;
		if (transportMode == "Avion" || transportMode == "Plane") tm_type = TransportMethod::PLANE;
		else if (transportMode == "Carro" || transportMode == "Car") tm_type = TransportMethod::CAR;
		else if (transportMode == "Crucero" || transportMode == "Cruise") tm_type = TransportMethod::CRUISE;

		graph.addRoute(originCountry, originEntryPoint, destinationCountry, destinationEntryPoint, travelTime, tm_type);
	}

	std::cout << "Destinos y rutas cargados correctamente desde JSON.\n";
}

void DB::loadClientsAndRewards(const std::string &filePath, TravelGraph& graph, SimpleList<Client>& clients, SimpleList<Reward>& rewards) {
	json data = getHoleRoute(filePath);

	for (const auto& rewardData : data["premiosDisponibles"]) {
		std::string rewardName = rewardData["nombre"];
		int pointsRequired = rewardData["puntosRequeridos"];
		rewards.add(Reward(rewardName, pointsRequired));
	}

	for (auto& clientData : data["clientes"]) {
		std::string name = clientData["nombre"];
		int points = clientData["puntos"];

		Client newClient(name);
		newClient.addPoints(points);

		for (auto& tripData : clientData["viajes"]) {
			std::string origin = tripData["origen"];
			std::string destination = tripData["destino"];
			std::string transportStr = tripData["transporte"];
			double hours = tripData["horas"];

			auto originParts = split(origin, " - ");
			auto destParts = split(destination, " - ");

			TransportMethod transport = stringToTransportMethod(transportStr);

			auto trip = Trip(
					originParts[0], originParts[1],
					destParts[0], destParts[1],
					transport,
					hours
			);

			if (!graph.routeExist(
				graph.findDestination(originParts[0], originParts[1]),
				destParts[1]
			)) continue;

			auto route = graph.findRoute(originParts[0], originParts[1], destParts[0], destParts[1]);
			newClient.addTrip(trip);
			route->traveledTimes++;
		}

		for (const auto& rewardData : clientData["premios"]) {
			std::string rewardName = rewardData["nombre"];
			int pointsUsed = rewardData["puntosUsados"];
			newClient.addReward(Reward(rewardName, pointsUsed));
		}

		clients.add(newClient);
	}
}

std::string DB::entryPointTypeToString(EntryPointType type) {
    switch (type) {
        case EntryPointType::AIRPORT:
            return "Airport";
        case EntryPointType::PORT:
            return "Port";
        case EntryPointType::BORDER:
            return "Border";
        default:
            return "Unknown";
    }
}

std::string DB::transportMethodToString(TransportMethod method) {
    switch (method) {
        case TransportMethod::PLANE:
            return "Avion";
        case TransportMethod::CAR:
            return "Carro";
        case TransportMethod::CRUISE:
            return "Crucero";
        default:
            return "Unknown";
    }
}

void DB::saveDestinationsAndRoutes(const std::string& filePath, const TravelGraph& graph) {
    json data;

    std::map<std::string, std::vector<const Destination*>> countriesMap;
    for (const auto& dest : graph.destinations) {
        countriesMap[dest.name].push_back(&dest);
    }

    for (const auto& [countryName, destinations] : countriesMap) {
        json countryJson;
        countryJson["nombre"] = countryName;

        json entryPointsJson = json::array();
        for (const auto& destination : destinations) {
            json entryJson;
            entryJson["nombre"] = destination->entryPointName;
            entryJson["tipo"] = entryPointTypeToString(destination->entryPoint->type);
            entryPointsJson.push_back(entryJson);
        }

        countryJson["puntos_de_entrada"] = entryPointsJson;
        data["paises"].push_back(countryJson);
    }

    json routesJson = json::array();
    for (const auto& dest : graph.destinations) {
        const Route* route = dest.routes;
        while (route != nullptr) {
            json routeJson;
            routeJson["origen"]["pais"] = dest.name;
            routeJson["origen"]["punto_de_entrada"] = dest.entryPointName;
            routeJson["destino"]["pais"] = route->destination->name;
            routeJson["destino"]["punto_de_entrada"] = route->destination->entryPointName;
            routeJson["transporte"] = transportMethodToString(route->transportMethod);
            routeJson["horas"] = route->travelTime;

            routesJson.push_back(routeJson);

            route = route->next;
        }
    }
    data["rutas"] = routesJson;

    try {
        std::filesystem::path projectRoot = getProjectRoot();
        std::filesystem::path fullPath = projectRoot / filePath;

        std::ofstream file(fullPath);
        if (!file.is_open()) {
            throw std::runtime_error("Error abriendo el archivo para escribir: " + fullPath.string());
        }

        file << data.dump(2);
        file.close();

        std::cout << "Destinos y rutas guardados correctamente en JSON.\n";
    }
    catch (const std::exception& e) {
        std::cerr << "Error al guardar destinos y rutas: " << e.what() << std::endl;
    }
}

void DB::saveClientsAndRewards(const std::string &filePath, const SimpleList<Client>& clients, const SimpleList<Reward>& rewards) {
    json data;

    json rewardsJson = json::array();
    for (const auto& reward : rewards) {
        json rewardJson;
        rewardJson["nombre"] = reward.name;
        rewardJson["puntosRequeridos"] = reward.pointsRequired;
        rewardsJson.push_back(rewardJson);
    }
    data["premiosDisponibles"] = rewardsJson;

    json clientsJson = json::array();
    for (const auto& client : clients) {
        json clientJson;
        clientJson["nombre"] = client.name;
        clientJson["puntos"] = client.getPoints();

        json tripsJson = json::array();
        for (const auto& trip : client.getTrips()) {
            json tripJson;
            tripJson["origen"] = trip.originCountry + " - " + trip.originEntryPoint;
            tripJson["destino"] = trip.destinationCountry + " - " + trip.destinationEntryPoint;
            tripJson["transporte"] = transportMethodToString(trip.transportMethod);
            tripJson["horas"] = trip.travelTime;

            tripsJson.push_back(tripJson);
        }
        clientJson["viajes"] = tripsJson;

        json usedRewardsJson = json::array();
        for (const auto& usedReward : client.getRewards()) {
            json usedRewardJson;
            usedRewardJson["nombre"] = usedReward.name;
            usedRewardJson["puntosUsados"] = usedReward.pointsRequired;
            usedRewardsJson.push_back(usedRewardJson);
        }
        clientJson["premios"] = usedRewardsJson;

        clientsJson.push_back(clientJson);
    }
    data["clientes"] = clientsJson;

    try {
        std::filesystem::path projectRoot = getProjectRoot();
        std::filesystem::path fullPath = projectRoot / filePath;

        std::ofstream file(fullPath);
        if (!file.is_open()) {
            throw std::runtime_error("Error abriendo el archivo para escribir: " + fullPath.string());
        }

        file << data.dump(2);
        file.close();

        std::cout << "Clientes y premios guardados correctamente en JSON.\n";
    }
    catch (const std::exception& e) {
        std::cerr << "Error al guardar clientes y premios: " << e.what() << std::endl;
    }
}
