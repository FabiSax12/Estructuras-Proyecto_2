//
// Created by varga on 27/10/2024.
//

#include "main_menu.h"
#include <iostream>

using namespace std;

void printPosibleRoute(TravelGraph &graph) {
	std::string originCountry, originEntryPoint;
	std::string destinationCountry, destinationEntryPoint;
	std::string transportMethod1, transportMethod2;

	std::cout << "Ingrese el país de origen: ";
	std::getline(std::cin, originCountry);
	std::cout << "Ingrese el punto de entrada de origen: ";
	std::getline(std::cin, originEntryPoint);

	std::cout << "Ingrese el país de destino: ";
	std::getline(std::cin, destinationCountry);
	std::cout << "Ingrese el punto de entrada de destino: ";
	std::getline(std::cin, destinationEntryPoint);

	std::cout << "Ingrese el medio de transporte (puede ingresar uno o dos, separados por coma): ";
	std::string transportMethodsInput;
	std::getline(std::cin, transportMethodsInput);

	std::vector<TransportMethod> transportMethods = parseTransportMethods(transportMethodsInput);

	Destination* origin = graph.findDestination(originCountry, originEntryPoint);
	if (!origin) {
		std::cout << "El destino de origen no existe en el grafo." << std::endl;
		return;
	}

	Destination* destination = graph.findDestination(destinationCountry, destinationEntryPoint);
	if (!destination) {
		std::cout << "El destino de destino no existe en el grafo." << std::endl;
		return;
	}
};
void printChooseReward(SimpleList<Client> &clients, SimpleList<Reward> &rewards){};
void printThreeMostFrecuentRoutes(TravelGraph &graph){};
void printRoutesTraveledOnce(TravelGraph &graph){};

void queries(TravelGraph &graph, SimpleList<Client> &clients, SimpleList<Reward> &rewards) {
	while (true) {
		cout << "================== Gestion de Datos ==================\n";
		cout << "1. Rutas posibles de X a Y\n";
		cout << "2. Canjear premios\n";
		cout << "3. Tres rutas mas frecuentes\n";
		cout << "4. Rutas recorridas una única vez\n";
		cout << "5. Volver\n";
		cout << "Seleccione una opcion: ";

		int option;
		cin >> option;

		switch (option) {
			case 1: printPosibleRoute(graph); break;
			case 2: printChooseReward(clients, rewards); break;
			case 3: printThreeMostFrecuentRoutes(graph); break;
			case 4: printRoutesTraveledOnce(graph); break;
			case 8: return;
			default: cout << "Opcion invalida. Intente de nuevo.\n";
		}
	}
}