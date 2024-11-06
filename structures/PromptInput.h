//
// Created by varga on 3/11/2024.
//

#ifndef PROMPT_INPUT_H
#define PROMPT_INPUT_H

#include <iostream>
#include <limits>

using namespace std;

/**
 * @brief Solicita al usuario una entrada y la convierte al tipo especificado T.
 * @tparam T Tipo de dato esperado (int, float, string, etc.).
 * @param message Mensaje para solicitar la entrada al usuario.
 * @param allLine Si es true, utiliza getline para capturar toda la línea (solo para strings).
 * @return Entrada convertida al tipo T.
 * @author Fabian
 */
template<typename T>
T promptInput(const string& message, bool allLine = false) {
	while (true) {
		cout << message;

		if (allLine) {
			string input;
			getline(cin, input);
			if constexpr (is_same_v<T, string>) {
				return input;  // Si T es string, devuelve la entrada directamente.
			} else {
				try {
					return static_cast<T>(stoi(input));  // Convierte de string a int.
				} catch (const invalid_argument&) {
					cout << "Entrada inválida. Por favor ingrese un número válido." << endl;
				} catch (const out_of_range&) {
					cout << "Número fuera de rango. Por favor intente de nuevo." << endl;
				}
			}
		} else {
			T input;
			cin >> input;

			// Comprueba si la entrada fue exitosa
			if (cin.fail()) {
				cin.clear();  // Limpia el estado de error
				cin.ignore(numeric_limits<streamsize>::max(), '\n');  // Descartar entrada incorrecta
				cout << "Entrada inválida, por favor intente de nuevo." << endl;
				continue;  // Regresa al inicio del bucle
			}

			cin.ignore(numeric_limits<streamsize>::max(), '\n');  // Limpia el búfer después de la entrada
			return input;  // Retorna el valor ingresado
		}
	}
}


#endif //PROMPT_INPUT_H
