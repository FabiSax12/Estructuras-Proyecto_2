//
// Created by varga on 27/10/2024.
//

#include "main_menu.h"
#include <iostream>
#include <unordered_set>
#include <limits>
#include <queue>

#include "../structures/DB.h"
#include "../structures/PathStep.h"

using namespace std;

void findAllRoutes(
	Destination* current, Destination* destination, const vector<TransportMethod>& allowedMethods,
	vector<PathStep>& path, vector<vector<PathStep>>& allRoutes,
	unordered_set<Destination*>& visited
) {

	if (current == destination) {
		allRoutes.push_back(path);
		return;
	}

	visited.insert(current);

	// Recorrer las rutas desde el destino actual
	for (Route* route = current->routes; route != nullptr; route = route->next) {
		// Verificar si el medio de transporte está permitido
		if (find(allowedMethods.begin(), allowedMethods.end(), route->transportMethod) != allowedMethods.end()) {
			Destination* nextDest = route->destination;
			if (visited.find(nextDest) == visited.end()) {
				path.push_back({current, route});
				findAllRoutes(nextDest, destination, allowedMethods, path, allRoutes, visited);
				path.pop_back();
			}
		}
	}

	visited.erase(current);
}

Client* findOrCreateClient(const string& name, SimpleList<Client>& clients) {
	for (auto& client : clients) {
		if (client.name == name) {
			return &client;
		}
	}
	// Si no se encontró, crear uno nuevo
	Client newClient(name);
	clients.add(newClient);
	return clients.get(0);
}

void printPosibleRoute(TravelGraph &graph, SimpleList<Client> &clients, SimpleList<Reward> &rewards) {
	string originCountry, originEntryPoint;
	string destinationCountry, destinationEntryPoint;
	string transportMethod1, transportMethod2;

	cout << "Ingrese el pais de origen: ";
	cin.ignore(numeric_limits<streamsize>::max(), '\n');
	getline(cin, originCountry);
	cout << "Ingrese el punto de entrada de origen: ";
	getline(cin, originEntryPoint);

	cout << "Ingrese el país de destino: ";
	getline(cin, destinationCountry);
	cout << "Ingrese el punto de entrada de destino: ";
	getline(cin, destinationEntryPoint);

	cout << "Ingrese el medio de transporte (puede ingresar uno o dos, separados por coma): ";
	string transportMethodsInput;
	getline(cin, transportMethodsInput);

	vector<TransportMethod> transportMethods = parseTransportMethods(transportMethodsInput);

	Destination* origin = graph.findDestination(originCountry, originEntryPoint);
	if (!origin) {
		cout << "El destino de origen no existe en el grafo." << endl;
		return;
	}

	Destination* destination = graph.findDestination(destinationCountry, destinationEntryPoint);
	if (!destination) {
		cout << "El destino de destino no existe en el grafo." << endl;
		return;
	}

	// Preparar variables para DFS
	vector<PathStep> path;
	vector<vector<PathStep>> allRoutes;
	unordered_set<Destination*> visited;

	// Llamar a la función para encontrar todas las rutas
	findAllRoutes(origin, destination, transportMethods, path, allRoutes, visited);

	struct RouteInfo {
		vector<PathStep> path;
		double totalTime;
		double totalPoints;
	};

	vector<RouteInfo> routesInfo;

	for (const auto& routePath : allRoutes) {
		RouteInfo info;
		info.path = routePath;
		info.totalTime = 0.0;
		info.totalPoints = 0.0;

		for (const auto& step : routePath) {
			info.totalTime += step.route->travelTime;
			info.totalPoints += step.route->calculatePoints();
		}

		routesInfo.push_back(info);
	}

	// Mostrar las rutas al usuario
	if (routesInfo.empty()) {
		cout << "No se encontraron rutas entre los destinos especificados con los medios de transporte indicados." << endl;
		return;
	}

	int routeNumber = 1;
	for (const auto& info : routesInfo) {
		cout << "Ruta " << routeNumber++ << ":";
		for (const auto& step : info.path) {
			cout << "\n\tDe: " << step.origin->name << " - " << step.origin->entryPointName;
			cout << "\n\tA: " << step.route->destination->name << " - " << step.route->destination->entryPointName;
			cout << "\n\tMedio: " << step.route->getTransportMethod();
			cout << "\n\tTiempo: " << step.route->travelTime << " horas\n";
		}
		cout << "\n\tDuración total: " << info.totalTime << " horas";
		cout << "\n\tPuntos totales: " << info.totalPoints << "\n\n";
	}

	// Preguntar al usuario si desea tomar una ruta
	cout << "Desea tomar alguna de estas rutas? (si/no): ";
	string decision;
	getline(cin, decision);

	if (decision == "sí" || decision == "si" || decision == "Sí" || decision == "Si") {
		cout << "Ingrese el numero de la ruta que desea tomar: ";
		int selectedRoute;
		cin >> selectedRoute;
		cin.ignore(numeric_limits<streamsize>::max(), '\n');

		if (selectedRoute > 0 && selectedRoute <= routesInfo.size()) {
			// Registrar el viaje
			cout << "Ingrese el nombre del cliente: ";
			string clientName;
			getline(cin, clientName);

			// Buscar o crear el cliente
			Client* client = findOrCreateClient(clientName, clients);

			// Agregar los viajes al cliente
			const RouteInfo& routeInfo = routesInfo[selectedRoute - 1];
			for (const auto& step : routeInfo.path) {
				Trip trip(
						step.origin->name, step.origin->entryPointName,
						step.route->destination->name, step.route->destination->entryPointName,
						step.route->transportMethod, step.route->travelTime
				);
				client->addTrip(trip);
				step.route->traveledTimes++;
			}

			DB::saveDestinationsAndRoutes(R"(data\destinations.json)", graph);
			DB::saveClientsAndRewards(R"(data\destinations.json)", clients, rewards);
			cout << "Viaje registrado exitosamente.\n";
		} else {
			cout << "Número de ruta inválido.\n";
		}
	} else {
		cout << "No se ha tomado ninguna ruta.\n";
	}
};

void printChooseReward(SimpleList<Client> &clients, SimpleList<Reward> &rewards) {
	cout << "Nombre completo del cliente: ";
	string clientName;
	cin.ignore(numeric_limits<streamsize>::max(), '\n');
	getline(cin, clientName);

	auto client = findOrCreateClient(clientName, clients);

	cout << "\n" << client->name << " tiene " << client->accumulatedPoints << " puntos." << endl;
	cout << "\nPremios que puede canjear: ";
	for (auto& reward : rewards) {
		if (reward.canRedeem(client->accumulatedPoints)) {
			cout << "\n\t" << reward.name << " por " << reward.pointsRequired << " puntos.";
		}
	}

	cout << "\nDesea canjear algun premio? (si/no): ";
	string decision;
	getline(cin, decision);

	if (decision == "sí" || decision == "si" || decision == "Sí" || decision == "Si") {
		cout << "Ingrese el numero premio que desea canjear: ";
		int selectedReward;
		cin >> selectedReward;
		cin.ignore(numeric_limits<streamsize>::max(), '\n');

		if (selectedReward > 0 && selectedReward <= rewards.length) {
			auto reward = rewards.get(selectedReward - 1);
			client->addReward(*reward);

			DB::saveClientsAndRewards(R"(data\clients.json)", clients, rewards);
			cout << "Premio canjeado exitosamente.\n";
		} else {
			cout << "Número de premio invalido.\n";
		}
	} else {
		cout << "No se ha canjeado ningun premio.\n";
	}
};

void printThreeMostFrecuentRoutes(TravelGraph &graph){};

void printRoutesTraveledOnce(TravelGraph &graph) {
	system("cls");
	cout << "================== Rutas recorridas una sola vez ==================\n";

	queue<Destination*> queue;
	graph.demark();

	for (auto& dest : graph.destinations) {
		if (!dest.visited) {
			dest.visited = true;
			queue.push(&dest);

			while (!queue.empty()) {
				Destination* current = queue.front();
				queue.pop();

				Route* route = current->routes;
				while (route != nullptr) {
					if (route->traveledTimes != 1) {
						route = route->next;
						continue;
					};

					Destination* destRoute = route->destination;

					std::cout << "Origen: " << current->name << " ( " << current->entryPointName << " )" << std::endl;
					std::cout << "\tDestino: " << destRoute->name << " ( " << destRoute->entryPointName << " )" << std::endl;
					std::cout << "\tTransporte: " << route->getTransportMethod() << std::endl;
					std::cout << "\tDuracion: " << route->travelTime << " horas" << std::endl;

					if (!destRoute->visited) {
						destRoute->visited = true;
						queue.push(destRoute);
					}
					route = route->next;
				}

			}
		}
	}
};

void queries(TravelGraph &graph, SimpleList<Client> &clients, SimpleList<Reward> &rewards) {
	while (true) {
    system("cls");
		cout << "================== Consultas ==================\n";
		cout << "1. Rutas posibles de X a Y\n";
		cout << "2. Canjear premios\n";
		cout << "3. Tres rutas mas frecuentes\n";
		cout << "4. Rutas recorridas una unica vez\n";
		cout << "5. Volver\n";
		cout << "Seleccione una opcion: ";

		int option;
		cin >> option;
		switch (option) {
			case 1:
				system("cls");
				printPosibleRoute(graph,clients, rewards);
		        system("pause");
				break;
			case 2:
				system("cls");
				printChooseReward(clients, rewards);
		        system("pause");
				break;
			case 3:
				system("cls");
				printThreeMostFrecuentRoutes(graph);
		        system("pause");
				break;
			case 4:
				system("cls");
				printRoutesTraveledOnce(graph);
		        system("pause");
				break;
			case 5:
				return;
			default:
				cout << "Opción inválida. Intente de nuevo.\n";
				system("pause");
			break;
		}
	}
}