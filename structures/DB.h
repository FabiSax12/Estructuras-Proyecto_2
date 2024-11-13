//
// Created by varga on 29/10/2024.
//

#ifndef DB_H
#define DB_H
#include "Client.h"
#include "Reward.h"
#include "TravelGraph.h"
#include "SimpleList.h"
#include <string>
#include <filesystem>


class DB {
public:
	static void loadDestinationsAndRoutes(const std::string& filePath, TravelGraph& graph);
	static void loadClientsAndRewards(const std::string &filePath, TravelGraph& graph, SimpleList<Client>& clients, SimpleList<Reward>& rewards);

	static void saveDestinationsAndRoutes(const std::string& filePath, const TravelGraph& graph);
	static void saveClientsAndRewards(const std::string &filePath, const SimpleList<Client>& clients, const SimpleList<Reward>& rewards);


	// Funciones auxiliares existentes
	 static std::filesystem::path getProjectRoot();
private:
	// Funciones auxiliares para convertir enums a strings
	[[nodiscard]]static std::string entryPointTypeToString(EntryPointType type) ;
	[[nodiscard]]static std::string transportMethodToString(TransportMethod method) ;
};

#endif //DB_H
