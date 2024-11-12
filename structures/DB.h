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
	static void loadClientsAndRewards(const std::string &filePath, SimpleList<Client>& clients, SimpleList<Reward>& rewards);

	void saveDestinationsAndRoutes(const std::string& filePath, const TravelGraph& graph);
	void saveClientsAndRewards(const std::string &filePath, const SimpleList<Client>& clients, const SimpleList<Reward>& rewards);

private:
	// Funciones auxiliares existentes
	std::filesystem::path getProjectRoot();

	// Funciones auxiliares para convertir enums a strings
	[[nodiscard]]std::string entryPointTypeToString(EntryPointType type) const;
	[[nodiscard]]std::string transportMethodToString(TransportMethod method) const;
};

#endif //DB_H
