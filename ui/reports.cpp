//
// Created by varga on 27/10/2024.
//

#include "main_menu.h"
#include <iostream>
#include <limits>

using namespace std;

/**
 * Realiza un recorrido en amplitud (BFS) del grafo de viajes y muestra los destinos visitados.
 *
 * @param graph El grafo de viajes que se recorrerá en amplitud.
 */
void amplitudeGraph(TravelGraph &graph) {
	graph.amplitude();
}


/**
 * Realiza un recorrido en profundidad (DFS) del grafo de viajes a partir de un destino inicial ingresado por el usuario.
 * Si el destino inicial no se encuentra en el grafo, se muestra un mensaje de error.
 *
 * @param graph El grafo de viajes que se recorrerá en profundidad.
 */
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

/**
 * Muestra la lista de todos los premios disponibles.
 * Si no hay premios, se informa al usuario que no hay premios para mostrar.
 *
 * @param rewards La lista de recompensas disponibles.
 */
void printRewards(const SimpleList<Reward>& rewards) {
	if (rewards.getLength() == 0) {
		std::cout << "No hay premios para mostrar." << std::endl;
		return;
	}
	std::cout << "Lista de premios:" << std::endl;
	rewards.printRecursive();
}

/**
 * Muestra la lista de todos los clientes registrados junto con sus puntos acumulados.
 * Si no hay clientes, se informa al usuario que no hay clientes para mostrar.
 *
 * @param clients La lista de clientes registrados en el sistema.
 */
void printClientsWithPoints(SimpleList<Client> &clients) {
	if (clients.getLength() == 0) {
		std::cout << "No hay clientes para mostrar." << std::endl;
		return;
	}
	std::cout << "Lista de clientes:" << std::endl;
	clients.printRecursive();
}

/**
 * Muestra los clientes que tienen viajes registrados, junto con los detalles de cada viaje.
 * Si ningún cliente tiene viajes, se informa al usuario.
 *
 * @param clients La lista de clientes registrados en el sistema.
 */
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

/**
 * Muestra los clientes que han canjeado premios, junto con los detalles de cada premio.
 * Si ningún cliente ha canjeado premios, se informa al usuario.
 *
 * @param clients La lista de clientes registrados en el sistema.
 */
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

/**
 * Muestra una lista de todos los destinos en el grafo que no han sido visitados por ningún cliente.
 * Si todos los destinos han sido visitados, se informa al usuario.
 *
 * @param graph El grafo de viajes que contiene los destinos.
 * @param clients La lista de clientes cuyos viajes se analizarán para determinar los destinos visitados.
 */
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

/**
 * Muestra el menú de reportes, permitiendo al usuario generar diferentes informes como el recorrido del grafo,
 * listas de premios, clientes y destinos no visitados. Gestiona la interacción del usuario con las distintas opciones.
 *
 * @param graph El grafo de viajes que contiene destinos y rutas.
 * @param clients La lista de clientes registrados en el sistema.
 * @param rewards La lista de recompensas disponibles en el sistema.
 */
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