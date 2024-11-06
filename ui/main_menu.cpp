//
// Created by varga on 27/10/2024.
//

#include "main_menu.h"
#include "../structures/PromptInput.h"
#include <iostream>

using namespace std;

/**
 * @brief Muestra una lista y solicita al usuario que seleccione un índice.
 * @param listName Nombre de la lista a mostrar.
 * @param listContent Contenido de la lista a seleccionar.
 * @param length Tamaño de la lista
 * @return Índice seleccionado por el usuario.
 * @author Fabian
 */
int selectIndex(const string& listName, const string& listContent, const int length) {
	cout << listName << ": " << listContent << endl;
	return promptInput<int>("Escoja mediante el indice (0 - " + to_string(length-1) + "): ");
}

void menu(TravelGraph &graph, SimpleList<Client> &clients, SimpleList<Reward> &rewards) {
	while (true) {
		cout << endl;
		cout << "============================================================" << endl;
		cout << "                Bienvenido/a a Viajes-TEC" << endl;
		cout << "1. Gestion de datos" << endl;
		cout << "2. Reportes" << endl;
		cout << "3. Consultas" << endl;
		cout << "4. Salir" << endl;
		cout << "Escoja una opcion (1, 2, 3, 4): ";

		int option = 0;
		cin >> option;

		if (option == 1) dataManagement(graph, clients, rewards);
		else if (option == 2) reports(graph, clients, rewards);
		else if (option == 3) queries(graph, clients, rewards);
		else if (option == 4) break;
		else cout << "Opcion inválida";
	}
}