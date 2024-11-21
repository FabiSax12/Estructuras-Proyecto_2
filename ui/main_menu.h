//
// Created by varga on 27/10/2024.
//

#ifndef MAIN_MENU_H
#define MAIN_MENU_H


#ifndef NOMINMAX
#define NOMINMAX         // Evita conflictos con min y max
#endif
#define WIN32_LEAN_AND_MEAN // Excluye componentes menos usados de windows.h

#include <string>
#include <windows.h>
#include <limits>

#include "../structures/Client.h"
#include "../structures/Reward.h"
#include "../structures/TravelGraph.h"
#include "../structures/utils.h"


extern HANDLE hConsole; // Declaración externa


int selectIndex(const std::string& listName, const std::string& listContent, int length);
//bool verifyNumericInput(COORD posText);
//void returnCursorPosition(COORD posText);

inline bool verifyNumericInput(COORD posText) {
    if (std::cin.fail()) {
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        moveCursor(posText.X, posText.Y, hConsole);
        deleteLine(hConsole);
        moveCursor(posText.X, posText.Y, hConsole);
        return true;
    }
    return false;
}

inline void returnCursorPosition(COORD posText) {
    moveCursor(posText.X, posText.Y, hConsole);
    deleteLine(hConsole);
    moveCursor(posText.X, posText.Y, hConsole);
}

template <typename T>
void input(T& value, DataType type, std::vector<int> range) {
    COORD posText = getCursorPosition(hConsole);
    while (true) {
        switch (type) {
            case DataType::Int: {
                int userValue;
                std::cin >> userValue;
                if (verifyNumericInput(posText)) {
                    continue;
                }
                if (!range.empty() && (userValue < range[0] || userValue > range[1])) {
                    returnCursorPosition(posText);
                    continue;
                }
                value = userValue;
                return;
            }
            case DataType::String: {
                string allowed = " .,;:-_()[]{}";
                bool error = false;
                std::string userValue;
                std::getline(std::cin, userValue);
                for (char c : userValue) {
                    if (!std::isalnum(c) && allowed.find(c) == std::string::npos) {
                        error = true;
                    }
                }
                if (!error) {
                    value = userValue;
                    return;
                }returnCursorPosition(posText);
                continue;
            }
            case DataType::Double: {
                double userValue;
                std::cin >> userValue;
                if (verifyNumericInput(posText)) {
                    continue;
                }
                if (userValue <= 0) {
                    returnCursorPosition(posText);
                    continue;
                }
                value = userValue;
                return;
            }
            default:
                std::cout<<"Error al detectar el tipo de dato";
        }
    }
}
int selectOption(vector<string> list, string defaultMessage="Selecciona la opcion",string selectionType="Opcion");
void menu(TravelGraph &graph, SimpleList<Client> &clients, SimpleList<Reward> &rewards);
void dataManagement(TravelGraph &graph, SimpleList<Client> &clients, SimpleList<Reward> &rewards);
void reports(TravelGraph &graph, SimpleList<Client> &clients, SimpleList<Reward> &rewards);
void queries(TravelGraph &graph, SimpleList<Client> &clients, SimpleList<Reward> &rewards);

#endif //MAIN_MENU_H
