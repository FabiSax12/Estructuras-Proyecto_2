#include <iostream>
#include <string>
#include "structures/SimpleList.h"
#include "structures/TravelGraph.h"
#include "structures/Client.h"
#include "structures/DB.h"
#include "structures/Reward.h"
#include "ui/main_menu.h"

using namespace std;

void loadInitialData(TravelGraph &graph, SimpleList<Client> &clients, SimpleList<Reward> &rewards) {
	DB::loadDestinationsAndRoutes(R"(data\destinations.json)", graph);
	DB::loadClientsAndRewards(R"(data\clients.json)", graph, clients, rewards);

	std::cout << "Initial data loaded successfully.\n";
}

int main() {
	auto travelGraph = new TravelGraph();
	auto clients = new SimpleList<Client>();
	auto rewards = new SimpleList<Reward>();

	// Load initial data from JSON or predefined data
	loadInitialData(*travelGraph, *clients, *rewards);
	menu(*travelGraph, *clients, *rewards);
	return 0;
}
