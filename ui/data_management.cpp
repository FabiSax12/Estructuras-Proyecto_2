#include <iostream>
#include <vector>
#include <sstream>
#include <cmath>  // Para std::floor
#include "main_menu.h"
#include "../structures/PromptInput.h"
#include "../structures/TravelGraph.h"
#include "../structures/Client.h"
#include "../structures/Reward.h"
#include "../structures/TravelGraphSerializer.h"
#include "../structures/DB.h"

using namespace std;

void addDestination(TravelGraph &graph) {
    cout << endl << " ================== Agregar destino ==================" << endl;
    string countryName;
    string ep_name;

    cout << "Nombre del pais: ";
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    getline(cin, countryName);

    auto ep_typeIndex = selectIndex("Punto de entrada a " + countryName, "{ Aeropuerto, Frontera, Puerto }", 3);

    cout << "Nombre del punto de entrada: ";
    getline(cin, ep_name);

    EntryPointType ep_type;
    if (ep_typeIndex == 0) ep_type = EntryPointType::AIRPORT;
    else if (ep_typeIndex == 1) ep_type = EntryPointType::BORDER;
    else if (ep_typeIndex == 2) ep_type = EntryPointType::PORT;

    ///////////////////////
    for (Destination& destination : graph.destinations) {
        if (destination.name == countryName&&destination.entryPointName==ep_name) {
            cout <<"El destino ya existe en el grafo" << endl;
            return;
        }
    }
    ///////////////////////

    graph.addDestination(new Destination(countryName, ep_name, new EntryPoint(ep_type, ep_name)));
    updateJSONDestinations(countryName, ep_name, ep_type,"../data/destinations.json");
}

void deleteRoutes(TravelGraph &graph,Destination* destinationSearched) {
    for (Destination& destination : graph.destinations) {
        Route* currentRoute = destination.routes;
        Route* prevRoute = nullptr;
        while(currentRoute != nullptr) {
            if ((currentRoute->destination->name == destinationSearched->name&&currentRoute->destination->entryPointName==destinationSearched->entryPointName)||(destination.name==destinationSearched->name&&destination.entryPointName==destinationSearched->entryPointName)) {
                if (prevRoute == nullptr) {
                    destination.routes = currentRoute->next;
                } else {
                    prevRoute->next = currentRoute->next;
                }
                Route* temp = currentRoute;
                currentRoute = currentRoute->next;
                delete temp;
            } else {
                prevRoute = currentRoute;
                currentRoute = currentRoute->next;
            }
        }
    }
}

void showRoutes(TravelGraph &graph) {
    system("cls");
    if (graph.destinations.getLength() < 1) {
        cout << "No hay destinos para mostrar" << endl;
        return;
    }
    vector<string> strDestinations;
    for (Destination destination : graph.destinations) {
        strDestinations.push_back("Destino: "+destination.entryPointName+" ("+destination.name+" )");
        cout<<"Destino: "+destination.entryPointName+" ("+destination.name+" )\n";
    }
    cout<<endl;
    int index=selectOption(strDestinations);
    if (index==-1){return;}
    Destination* destinationSearched = graph.destinations.get(index);
    system("cls");
    for (Destination& destination : graph.destinations) {
        Route* currentRoute = destination.routes;
        if (destination.name == destinationSearched->name&&destination.entryPointName==destinationSearched->entryPointName) {
            if (currentRoute == nullptr) {
                cout <<"Lo sentimos, pero no hay destinos para mostrar" << endl;
            }else {
                cout<<"Rutas dispnibles: \n";
                while(currentRoute != nullptr) {
                    double time = currentRoute->travelTime;
                    std::ostringstream stream;

                    // Detectar cuántos decimales usar (0, 1 o 2)
                    if (std::floor(time) == time) {
                        // Número entero
                        stream << std::fixed << std::setprecision(0) << time;
                    } else if (std::round(time * 10) == time * 10) {
                        // Un decimal significativo
                        stream << std::fixed << std::setprecision(1) << time;
                    } else {
                        // Dos decimales significativos
                        stream << std::fixed << std::setprecision(2) << time;
                    }

                    string timeString = stream.str();
                    cout<<"======================================================\n";
                    cout<<"\tPais de Destino: "+currentRoute->destination->name+"\n";
                    cout<<"\tPunto de entrada: "+currentRoute->destination->entryPointName+"\n";
                    cout<<"\tDuracion del viaje: "+timeString+" horas\n";
                    cout<<"\tMedio de transporte: "+transportMethodToString(currentRoute->transportMethod)+"\n";
                    currentRoute = currentRoute->next;
                }cout<<endl;
            }

        }
    }
}

void deleteDestination(TravelGraph &graph) {
    if (graph.destinations.getLength() < 1) {
        cout << "No hay destinos para eliminar" << endl;
        return;
    }
    vector<string> strDestinations;
    for (Destination destination : graph.destinations) {
        strDestinations.push_back("Pais: "+destination.name+" Destino: "+destination.entryPointName);
        cout<<"Pais: "+destination.name+" Destino: "+destination.entryPointName<<endl;
    }
    cout<<endl;
    int index=selectOption(strDestinations);
    if (index==-1){return;}
    cout<<"En la lilsta de cadenas:"<<strDestinations[index]<<endl;
    cout<<"En la lista de destinos:"<<graph.destinations.get(index)->entryPointName<<endl;
    deleteRoutes(graph,graph.destinations.get(index));
    graph.destinations.removeByIndex(index);
    DB::saveDestinationsAndRoutes(R"(data\destinations.json)",graph);
}

void addRoute(TravelGraph &graph) {
    cout << endl << " ================== Agregar ruta ==================" << endl;
    string originCountry, originEntryPoint;
    string destCountry, destEntryPoint;

    cout << "Nombre del pais de origen: ";
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    getline(cin, originCountry);
    cout << "Nombre del punto de entrada de origen: ";
    getline(cin, originEntryPoint);

    cout << "Nombre del pais de destino: ";
    getline(cin, destCountry);
    cout << "Nombre del punto de entrada de destino: ";
    getline(cin, destEntryPoint);

    const auto time = promptInput<double>("Tiempo de viaje: ");
    auto transportMethod = selectIndex("Metodos de transporte: ", "{ Avion, Carro, Barco }", 3);

    TransportMethod tmType;
    if (transportMethod == 0) tmType = TransportMethod::PLANE;
    else if (transportMethod == 1) tmType = TransportMethod::CAR;
    else if (transportMethod == 2) tmType = TransportMethod::CRUISE;

    graph.addRoute(originCountry, originEntryPoint, destCountry, destEntryPoint, time, tmType);
    DB::saveDestinationsAndRoutes(R"(data\destinations.json)",graph);
}

void modifyRoute(TravelGraph &graph) {
    system("cls");
    cout << endl << " ================== Modificar ruta ==================" << endl;
    vector<string> strRoutes;
    vector<Route*> vctrRoutes;
    cout<<"Rutas dispnibles: \n";
    for (Destination destination : graph.destinations) {
        Route* currentRoute = destination.routes;
        while(currentRoute != nullptr) {
            strRoutes.push_back("ORIGEN: "+destination.name+" ("+destination.entryPointName+") "+"DESTINO: "+currentRoute->destination->name+" ("+currentRoute->destination->entryPointName+") ");
            cout<<"\tORIGEN: "+destination.name+" ("+destination.entryPointName+") "+"DESTINO: "+currentRoute->destination->name+" ("+currentRoute->destination->entryPointName+") "<<endl;
            vctrRoutes.push_back(currentRoute);
            currentRoute=currentRoute->next;
        }
    }cout<<endl;
    int index=selectOption(strRoutes);
    int tempIndex=0;
    if (index==-1){return;}
    for (Destination& destination : graph.destinations) {
        Route* currentRoute = destination.routes;
        Route* prevRoute = nullptr;
        while(currentRoute != nullptr) {
            if (tempIndex == index) {
                system("cls");
                cout << endl << " ================== Modificar ruta ==================" << endl;
                string originCountry = destination.name;
                string originEntryPoint = destination.entryPointName;
                string destCountry, destEntryPoint;
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                cout << "Nombre del nuevo pais de destino: ";
                getline(cin, destCountry);
                cout << "Nombre del nuevo punto de entrada de destino: ";
                getline(cin, destEntryPoint);
                const auto time = promptInput<double>("Tiempo de viaje: ");
                auto transportMethod = selectIndex("Metodos de transporte: ", "{ Avion, Carro, Barco }", 3);
                TransportMethod tmType;
                if (transportMethod == 0) tmType = TransportMethod::PLANE;
                else if (transportMethod == 1) tmType = TransportMethod::CAR;
                else if (transportMethod == 2) tmType = TransportMethod::CRUISE;
                if(graph.validateRoute(originCountry, originEntryPoint, destCountry, destEntryPoint, time, tmType)) {
                    if (prevRoute == nullptr) {
                        destination.routes = currentRoute->next;
                    } else {
                        prevRoute->next = currentRoute->next;
                    }
                    Route* temp = currentRoute;
                    currentRoute = currentRoute->next;
                    delete temp;
                    graph.addRoute(originCountry, originEntryPoint, destCountry, destEntryPoint, time, tmType);
                    DB::saveDestinationsAndRoutes(R"(data\destinations.json)",graph);
                }
                return;
            } else {
                prevRoute = currentRoute;
                tempIndex++;
                currentRoute = currentRoute->next;
            }
        }
    }
}

void deleteRoute(TravelGraph &graph) {
    system("cls");
    cout << endl << " ================== Eliminar ruta ==================" << endl;
    vector<string> strRoutes;
    vector<Route*> vctrRoutes;
    for (Destination destination : graph.destinations) {
        Route* currentRoute = destination.routes;
        while(currentRoute != nullptr) {
            strRoutes.push_back("ORIGEN: "+destination.name+" ("+destination.entryPointName+") "+"DESTINO: "+currentRoute->destination->name+" ("+currentRoute->destination->entryPointName+") ");
            cout<<"ORIGEN: "+destination.name+" ("+destination.entryPointName+") "+"DESTINO: "+currentRoute->destination->name+" ("+currentRoute->destination->entryPointName+") "<<endl;
            vctrRoutes.push_back(currentRoute);
            currentRoute=currentRoute->next;
        }
    }cout<<endl;
    int index=selectOption(strRoutes);
    int tempIndex=0;
    if (index==-1){return;}
    for (Destination& destination : graph.destinations) {
        Route* currentRoute = destination.routes;
        Route* prevRoute = nullptr;
        while(currentRoute != nullptr) {
            if (tempIndex == index) {
                if (prevRoute == nullptr) {
                    destination.routes = currentRoute->next;
                } else {
                    prevRoute->next = currentRoute->next;
                }
                Route* temp = currentRoute;
                currentRoute = currentRoute->next;
                delete temp;
                DB::saveDestinationsAndRoutes(R"(data\destinations.json)",graph);
                return;
            } else {
                tempIndex++;
                prevRoute = currentRoute;
                currentRoute = currentRoute->next;
            }
        }
    }
}

void addClient(SimpleList<Client> &clients,SimpleList<Reward> &rewards) {
    cout << endl << " ================== Agregar cliente ==================" << endl;
    string strName;
    cout << "Nombre del cliente: ";
    getline(cin, strName);
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    if(!strName.empty()) {
        clients.add(Client(strName));
        DB::saveClientsAndRewards(R"(data\clients.json)",clients,rewards);
    }else {
        cout<<"Error: El nombre no puede estar vacio!\n";
    }
}

void deleteClient(SimpleList<Client> &clients,SimpleList<Reward> &rewards) {
    if (clients.getLength()== 0) {
        cout<<"No hay clientes resgistrados\n";
        return;
    }
    vector<string> strClients;
    Client* currentClient = clients.get(0);
    while(currentClient != nullptr) {
        cout<<"Cliente: "+currentClient->name+" Puntos Acumulados: "+std::to_string(currentClient->accumulatedPoints)<<endl;
        currentClient=currentClient->next;
    }cout<<endl;
    int index=selectOption(strClients);
    if (index==-1){return;}
    clients.remove(*clients.get(index));
    DB::saveClientsAndRewards(R"(data\clients.json)",clients,rewards);
}

void findClient(SimpleList<Client> &clients,SimpleList<Reward> &rewards) {
    string name;
    cout << "Nombre del cliente a buscar: ";
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    getline(cin, name);
    for (auto &client : clients) {
        if (client.name == name) {
            cout << "Cliente encontrado: " << client.name << ", Puntos: " << client.getPoints() << endl;
            return;
        }
    }
    cout << "Cliente no encontrado." << endl;
}

void addReward(SimpleList<Client> &clients,SimpleList<Reward> &rewards) {
    string strName;
    int points;
    cout << "Nombre del premio: ";
    getline(cin, strName);
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    points = promptInput<int>("Puntos requeridos para canjear el premio:: ");
    if(points>0&&!strName.empty()) {
        rewards.add(Reward(strName,points));
        DB::saveClientsAndRewards(R"(data\clients.json)",clients,rewards);
    }else {
        cout<<"Error: Datos incorrectos!\n";
    }
}

void modifyReward(SimpleList<Client> &clients,SimpleList<Reward> &rewards) {
    if (rewards.getLength() == 0) {
        cout<<"No hay premios disponibles\n";
        return;
    }
    vector<string> strRewards;
    Reward* currentReward = rewards.get(0);
    while(currentReward != nullptr) {
        strRewards.push_back("Premio: "+currentReward->name+" Puntos Requeridos: "+std::to_string(currentReward->pointsRequired));
        cout<<"Premio: "+currentReward->name+" Puntos Requeridos: "+std::to_string(currentReward->pointsRequired)<<endl;
        currentReward=currentReward->next;
    }cout<<endl;
    int index=selectOption(strRewards);
    if (index==-1){return;}
    auto reward = rewards.get(index);

    string strName;
    int points;
    cout << "Nuevo nombre del premio: ";
    getline(cin, strName);
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    points = promptInput<int>("Nuevo numero de puntos requeridos: ");
    if(points>0&&!strName.empty()) {
        reward->name=strName;
        reward->pointsRequired=points;
        DB::saveClientsAndRewards(R"(data\clients.json)",clients,rewards);
    }else {
        cout<<"Error: Datos incorrectos!\n";
    }
}

void deleteReward(SimpleList<Client> &clients,SimpleList<Reward> &rewards) {
    if (rewards.getLength() == 0) {
        cout<<"No hay premios disponibles\n";
        return;
    }
    vector<string> strRewards;
    Reward* currentReward = rewards.get(0);
    while(currentReward != nullptr) {
        strRewards.push_back("Premio: "+currentReward->name+" Puntos Requeridos: "+std::to_string(currentReward->pointsRequired));
        cout<<"Premio: "+currentReward->name+" Puntos Requeridos: "+std::to_string(currentReward->pointsRequired)<<endl;
        currentReward=currentReward->next;
    }cout<<endl;
    int index=selectOption(strRewards);
    if (index==-1){return;}
    //int index = selectIndex("Premios:", rewards.toString(), rewards.getLength());
    rewards.remove(*rewards.get(index));

    DB::saveClientsAndRewards(R"(data\clients.json)",clients,rewards);

}

// Menú principal de gestión de datos
void dataManagement(TravelGraph &graph, SimpleList<Client> &clients, SimpleList<Reward> &rewards) {
    while (true) {
        system("cls");
        cout << "================== Gestion de Datos ==================\n";
        cout << "1. Agregar destino\n";
        cout << "2. Eliminar destino\n";
        cout << "3. Agregar ruta\n";
        cout << "4. Modificar ruta\n";
        cout << "5. Eliminar ruta\n";
        cout << "6. Agregar cliente\n";
        cout << "7. Eliminar cliente\n";
        cout << "8. Buscar cliente\n";
        cout << "9. Agregar premio\n";
        cout << "10. Modificar premio\n";
        cout << "11. Eliminar premio\n";
        cout << "12. Mostrar rutas\n";
        cout << "13. Volver\n";
        cout << "Seleccione una opcion: ";

        int option;
        cin>>option;

        if (cin.fail()) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Entrada no válida. Por favor, ingrese un número entre 1 y 4.\n";
            system("pause");
            continue;
        }


        switch (option) {
            case 1:
                system("cls");
                addDestination(graph);
                system("pause");
                break;
            case 2:
                system("cls");
                deleteDestination(graph);
                system("pause");
                break;
            case 3:
                system("cls");
                addRoute(graph);
                system("pause");
                break;
            case 4:
                system("cls");
                modifyRoute(graph);
                system("pause");
                break;
            case 5:
                system("cls");
                deleteRoute(graph);
                system("pause");
                break;
            case 6:
                system("cls");
                addClient(clients, rewards);
                system("pause");
                break;
            case 7:
                system("cls");
                deleteClient(clients, rewards);
                system("pause");
                break;
            case 8:
                system("cls");
                findClient(clients, rewards);
                system("pause");
                break;
            case 9:
                system("cls");
                addReward(clients, rewards);
                system("pause");
                break;
            case 10:
                system("cls");
                modifyReward(clients, rewards);
                system("pause");
                break;
            case 11:
                system("cls");
                deleteReward(clients, rewards);
                system("pause");
                break;
            case 12:
                system("cls");
                showRoutes(graph);
                system("pause");
                break;
            case 13:
                return;
            default:
                cout << "Opción no válida. Por favor, elija un número entre 1 y 13.\n";
                system("pause");
                break;
        }
    }
}
