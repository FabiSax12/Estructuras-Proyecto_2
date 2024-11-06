//
// Created by varga on 29/10/2024.
//

#ifndef DB_H
#define DB_H
#include "Client.h"
#include "Reward.h"
#include "TravelGraph.h"


class DB {
public:
	static void loadDestinationsAndRoutes(const std::string& filePath, TravelGraph& graph);
	static void loadClientsAndRewards(const std::string &filePath, SimpleList<Client>& clients, SimpleList<Reward>& rewards);
};

#endif //DB_H
