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

std::filesystem::path getProjectRoot() {
	std::filesystem::path currentPath = std::filesystem::current_path();
	std::string buildDir = "cmake-build-debug";
	if (currentPath.filename() == buildDir) {
		currentPath = currentPath.parent_path();
	}
	return currentPath;
}

json getHoleRoute(string filePath) {
	std::filesystem::path projectRoot = getProjectRoot();
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
		int travelTime = route["horas"];

		TransportMethod tm_type;
		if (transportMode == "Avion" || transportMode == "Plane") tm_type = TransportMethod::PLANE;
		else if (transportMode == "Carro" || transportMode == "Car") tm_type = TransportMethod::CAR;
		else if (transportMode == "Crucero" || transportMode == "Cruise") tm_type = TransportMethod::CRUISE;

		graph.addRoute(originCountry, originEntryPoint, destinationCountry, destinationEntryPoint, travelTime, tm_type);
	}

	std::cout << "Destinos y rutas cargados correctamente desde JSON.\n";
}

void DB::loadClientsAndRewards(const std::string &filePath, SimpleList<Client>& clients, SimpleList<Reward>& rewards) {
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

			newClient.addTrip(trip);
		}

		for (const auto& rewardData : clientData["premios"]) {
			std::string rewardName = rewardData["nombre"];
			int pointsUsed = rewardData["puntosUsados"];
			newClient.addReward(Reward(rewardName, pointsUsed));
		}

		clients.add(newClient);
	}
}


