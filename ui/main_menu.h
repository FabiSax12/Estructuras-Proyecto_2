//
// Created by varga on 27/10/2024.
//

#ifndef MAIN_MENU_H
#define MAIN_MENU_H
#include <string>

#include "../structures/Client.h"
#include "../structures/Reward.h"
#include "../structures/TravelGraph.h"

int selectIndex(const std::string& listName, const std::string& listContent, int length);
int selectOption(vector<string> list);
void menu(TravelGraph &graph, SimpleList<Client> &clients, SimpleList<Reward> &rewards);
void dataManagement(TravelGraph &graph, SimpleList<Client> &clients, SimpleList<Reward> &rewards);
void reports(TravelGraph &graph, SimpleList<Client> &clients, SimpleList<Reward> &rewards);
void queries(TravelGraph &graph, SimpleList<Client> &clients, SimpleList<Reward> &rewards);

#endif //MAIN_MENU_H
