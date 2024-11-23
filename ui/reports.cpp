//
// Created by varga on 27/10/2024.
//

#include "main_menu.h"
#include <iostream>
#include <limits>

using namespace std;

void amplitudeGraph(TravelGraph &graph) {
	graph.amplitude();
}

void depthGraph(TravelGraph &graph) {
	graph.demark();

	std::string startCountry, startEntryPoint;
	cout << "Ingrese el pais de inicio: ";
	cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
	getline(cin, startCountry);
	cout << "Ingrese el punto de entrada de inicio: ";
	getline(cin, startEntryPoint);


	Destination* start = graph.findDestination(startCountry, startEntryPoint);

	if (start == nullptr) {
		cout << "El destino inicial no se encontro en el grafo." << endl;
		return;
	}
	graph.depth(start);
}

void printRewards(const SimpleList<Reward>& rewards) {
	if (rewards.getLength() == 0) {
		std::cout << "No hay premios para mostrar." << std::endl;
		return;
	}
	std::cout << "Lista de premios:" << std::endl;
	rewards.printRecursive();
}

void printClientsWithPoints(SimpleList<Client> &clients) {
	if (clients.getLength() == 0) {
		std::cout << "No hay clientes para mostrar." << std::endl;
		return;
	}
	std::cout << "Lista de clientes:" << std::endl;
	clients.printRecursive();
}

void printClientsWithTrips(SimpleList<Client> &clients) {
	bool anyClientWithTrips = false;
	for (const auto& client : clients) {
		if (client.trips->length > 0) {
			anyClientWithTrips = true;
			std::cout << "Nombre: " << client.name<< std::endl;
			std::cout << "Viajes:" << std::endl;
			for (auto& trip : *(client.trips)) {
				std::cout << "\t" << trip.toString() << std::endl;
			}
		}
	}
	if (!anyClientWithTrips) {
		std::cout << "No hay clientes con viajes para mostrar." << std::endl;
	}
}

void printClientsWithRewards(SimpleList<Client> &clients) {
	bool anyClientWithRewards = false;
	for (const auto& client : clients) {
		if (client.rewardsEarned->length > 0) {
			anyClientWithRewards = true;
			std::cout << "Nombre: " << client.name << std::endl;
			std::cout << "Premios:" << std::endl;
			for (const auto& reward : *(client.rewardsEarned)) {
				std::cout << "\t" << reward.toString() << std::endl;
			}
		}
	}
	if (!anyClientWithRewards) {
		std::cout << "No hay clientes con premios para mostrar." << std::endl;
	}
}


void printNonVisitedDestinations(TravelGraph &graph, SimpleList<Client> &clients) {
	for (auto& dest : graph.destinations) {
		dest.visited = false;
	}

	for (const auto& client : clients) {
		for (const auto& trip : *(client.trips)) {
			Destination* dest = graph.findDestination(trip.destinationCountry, trip.destinationEntryPoint);
			if (dest != nullptr) {
				dest->visited = true;
			}
		}
	}
	bool anyNonVisited = false;
	for (const auto& dest : graph.destinations) {
		if (!dest.visited) {
			anyNonVisited = true;
			std::cout << "Destino no visitado: " << dest.name << " - " << dest.entryPointName << std::endl;
		}
	}

	if (!anyNonVisited) {
		std::cout << "Todos los destinos han sido visitados." << std::endl;
	}
}


void reports(TravelGraph &graph, SimpleList<Client> &clients, SimpleList<Reward> &rewards) {
	while (true) {
		system("cls");
		cout << "================== Reportes ==================\n";
		cout << "1. Grafo en amplitud\n";
		cout << "2. Grafo en profundidad\n";
		cout << "3. Lista de premios\n";
		cout << "4. Lista de clientes (nombre y puntos)\n";
		cout << "5. Lista de clientes (nombre y viajes)\n";
		cout << "6. Lista de clientes (nombre y premios)\n";
		cout << "7. Destinos sin visitar\n";
		cout << "8. Volver\n";
		cout << "Seleccione una opcion: ";

		int option;
		input(option,{1,8});

		switch (option) {
		    case 1:
		        system("cls");
		        amplitudeGraph(graph);
		        system("pause");
		        break;
		    case 2:
		        system("cls");
		        depthGraph(graph);
		        system("pause");
		        break;
		    case 3:
		        system("cls");
		        printRewards(rewards);
		        system("pause");
		        break;
		    case 4:
		        system("cls");
		        printClientsWithPoints(clients);
		        system("pause");
		        break;
		    case 5:
		        system("cls");
		        printClientsWithTrips(clients);
		        system("pause");
		        break;
		    case 6:
		        system("cls");
		        printClientsWithRewards(clients);
		        system("pause");
		        break;
		    case 7:
		        system("cls");
		        printNonVisitedDestinations(graph, clients);
		        system("pause");
		        break;
		    case 8:
		        return;
		    default:
		        cout << "Opción inválida. Intente de nuevo.\n";
		        system("pause");
		        break;
		}
	}
}