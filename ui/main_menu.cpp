//
// Created by varga on 27/10/2024.
//


#include "main_menu.h"
#include "../structures/PromptInput.h"
#include <iostream>
#include <cstdlib>

using namespace std;
HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

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
/**
 * Muestra una lista de opciones y permite al usuario navegar y seleccionar una opción usando las teclas de flecha y espacio.
 *
 * @param list Lista de opciones a mostrar.
 * @param defaultMessage Mensaje predeterminado que se muestra al usuario.
 * @param selectionType Tipo de selección a realizar (por ejemplo, "Opciones").
 * @return Índice de la opción seleccionada por el usuario. Devuelve -1 si el usuario presiona ESC para salir.
 */
int selectOption(vector<string> list,string defaultMessage,string selectionType) {
	hideCursor();
	int opt = 0;
	string strOpt;
	cout << defaultMessage+":\n(Muevete con las flechas (up & down); presiona ESPACIO para seleccionar o ESC para salir)\n";
	cout << selectionType+":\n";
	COORD posText = getCursorPosition(hConsole);/*Obtiene la posicion actual del cursor*/

	bool selection = false;
	while (!selection) {
		strOpt = list[opt];
		moveCursor(20, posText.Y, hConsole);        /*Mueve el cursor a una posicion en especifico*/
		deleteLine(hConsole);                       /*Borra la linea actual en la consola*/
		moveCursor(posText.X, posText.Y, hConsole);/*Vuelve a la posicion original del cursor*/
		cout << strOpt <<endl;

		if (GetAsyncKeyState(VK_ESCAPE) & 0x8000) {
			showCursor();
			return -1;
		} else if (GetAsyncKeyState(VK_DOWN) & 0x8000) {
			opt++;
			while (GetAsyncKeyState(VK_DOWN) & 0x8000) { Sleep(50); }/*Espera que se suelte la tecla*/
		} else if (GetAsyncKeyState(VK_UP) & 0x8000) {
			opt--;
			while (GetAsyncKeyState(VK_UP) & 0x8000) { Sleep(50); }/*Espera que se suelte la tecla*/
		} else if (GetAsyncKeyState(VK_SPACE) & 0x8000) {
			while (GetAsyncKeyState(VK_SPACE) & 0x8000) { Sleep(50); }/*Espera que se suelte la tecla*/
			FlushConsoleInputBuffer(GetStdHandle(STD_INPUT_HANDLE));/*Limpia el buffer de entrada de la consola*/
			selection = true;
			break;
		}
		if (opt < 0) { opt = list.size()-1; }/*Si el indice es menor que cero, va al final de la lista*/
		else if (opt > list.size()-1) { opt = 0; } /*Si el indice supoera el tamao vuelve al inicio*/

		Sleep(10); /*Evitar la sobrecarga de la CPU*/
	}
	showCursor();
	return opt;
}
/**
 * Función principal del menú que muestra las opciones principales del sistema y gestiona la navegación entre ellas.
 * Permite al usuario acceder a la gestión de datos, reportes, consultas o salir del programa.
 *
 * @param graph Grafo de viajes que contiene destinos y rutas.
 * @param clients Lista de clientes registrados en el sistema.
 * @param rewards Lista de recompensas disponibles para los clientes.
 */
void menu(TravelGraph &graph, SimpleList<Client> &clients, SimpleList<Reward> &rewards) {
	while (true) {
		system("cls");
		cout << endl;
		cout << "============================================================" << endl;
		cout << "                Bienvenido/a a Viajes-TEC" << endl;
		cout << "1. Gestion de datos" << endl;
		cout << "2. Reportes" << endl;
		cout << "3. Consultas" << endl;
		cout << "4. Salir" << endl;
		cout << "Escoja una opcion (1, 2, 3, 4): ";

		int option = 0;
		input(option,{1,4});

		switch (option) {
			case 1:
				dataManagement(graph, clients, rewards);
				break;
			case 2:
				reports(graph, clients, rewards);
				break;
			case 3:
				queries(graph, clients, rewards);
				break;
			case 4:
				return;
			default:
				cout << "Opcion inválida\n";
				system("pause");
		}
	}
}
