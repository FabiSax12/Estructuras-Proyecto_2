#include <iostream>
#include <vector>
#include "main_menu.h"
#include "../structures/PromptInput.h"
#include "../structures/TravelGraph.h"
#include "../structures/Client.h"
#include "../structures/Reward.h"
#include "../structures/TravelGraphSerializer.h"

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

    graph.addDestination(new Destination(countryName, ep_name, new EntryPoint(ep_type, ep_name)));
    updateJSONDestinations(countryName, ep_name, ep_type,"../data/destinations.json");
}

void deleteRoutes(TravelGraph &graph,Destination* destinationSearched) {
    for (Destination& destination : graph.destinations) {
        Route* currentRoute = destination.routes;
        Route* prevRoute = nullptr;
        while(currentRoute != nullptr) {
            if ((currentRoute->destination->name == destinationSearched->name&&currentRoute->destination->entryPointName==destinationSearched->entryPointName)||(destination.name==destinationSearched->name)) {
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
    for (Destination& destination : graph.destinations) {
        Route* currentRoute = destination.routes;
        if (currentRoute == nullptr) {cout <<"Lo sentimos, pero no hay destinos para mostrar" << endl;}
        else if (destination.name == destinationSearched->name&&destination.entryPointName==destinationSearched->entryPointName) {
            while(currentRoute != nullptr) {
                cout<<"-------------------------------------------\n";
                cout<<"\t Pais de Destino: "+currentRoute->destination->name+"\n";
                cout<<"\t Punto de entrada: "+currentRoute->destination->entryPointName+"\n";
                cout<<"\t Duracion del viaje: "+std::to_string(currentRoute->travelTime)+"\n";
                cout<<"\t Medio de transporte: "+transportMethodToString(currentRoute->transportMethod)+"\n";
                currentRoute = currentRoute->next;
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

    const auto time = promptInput<int>("Tiempo de viaje: ");
    auto transportMethod = selectIndex("Metodos de transporte: ", "{ Avion, Carro, Barco }", 3);

    TransportMethod tmType;
    if (transportMethod == 0) tmType = TransportMethod::PLANE;
    else if (transportMethod == 1) tmType = TransportMethod::CAR;
    else if (transportMethod == 2) tmType = TransportMethod::CRUISE;

    graph.addRoute(originCountry, originEntryPoint, destCountry, destEntryPoint, time, tmType);
}


void modifyRoute(TravelGraph &graph) {
    system("cls");
    cout << endl << " ================== Modificar ruta ==================" << endl;
    vector<string> strRoutes;
    vector<Route*> vctrRoutes;
    for (Destination destination : graph.destinations) {
        Route* currentRoute = destination.routes;
        cout<<"Rutas dispnibles: \n";
        while(currentRoute != nullptr) {
            strRoutes.push_back("ORIGEN: "+destination.name+" ("+destination.entryPointName+") "+"DESTINO: "+currentRoute->destination->name+" ("+currentRoute->destination->entryPointName+") ");
            cout<<"\tORIGEN: "+destination.name+" ("+destination.entryPointName+") "+"DESTINO: "+currentRoute->destination->name+" ("+currentRoute->destination->entryPointName+") "<<endl;
            vctrRoutes.push_back(currentRoute);
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
                const auto time = promptInput<int>("Tiempo de viaje: ");
                auto transportMethod = selectIndex("Metodos de transporte: ", "{ Avion, Carro, Barco }", 3);
                TransportMethod tmType;
                if (transportMethod == 0) tmType = TransportMethod::PLANE;
                else if (transportMethod == 1) tmType = TransportMethod::CAR;
                else if (transportMethod == 2) tmType = TransportMethod::CRUISE;
                if(graph.validateRoute(originCountry, originEntryPoint, destCountry, destEntryPoint, time, tmType)) {
                    graph.addRoute(originCountry, originEntryPoint, destCountry, destEntryPoint, time, tmType);
                    if (prevRoute == nullptr) {
                        destination.routes = currentRoute->next;
                    } else {
                        prevRoute->next = currentRoute->next;
                    }
                    Route* temp = currentRoute;
                    currentRoute = currentRoute->next;
                    delete temp;
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
                return;
            } else {
                tempIndex++;
                prevRoute = currentRoute;
                currentRoute = currentRoute->next;
            }
        }
    }
}

void addClient(SimpleList<Client> &clients) {
    const auto name = promptInput<string>("Nombre del cliente:", true);
    clients.add(Client(name));
}

void deleteClient(SimpleList<Client> &clients) {
    int index = selectIndex("Clientes:", clients.toString(), clients.getLength());
    clients.remove(*clients.get(index));
}

void findClient(SimpleList<Client> &clients) {
    const auto name = promptInput<string>("Nombre del cliente:", true);
    for (auto &client : clients) {
        if (client.name == name) {
            cout << "Cliente encontrado: " << client.name << ", Puntos: " << client.getPoints() << endl;
            return;
        }
    }
    cout << "Cliente no encontrado." << endl;
}

void addReward(SimpleList<Reward> &rewards) {
    const auto name = promptInput<string>("Nombre del premio:", true);
    const auto points = promptInput<int>("Puntos requeridos para canjear el premio:");
    rewards.add(Reward(name, points));
}

void modifyReward(SimpleList<Reward> &rewards) {
    int index = selectIndex("Premios:", rewards.toString(), rewards.getLength());
    auto reward = rewards.get(index);
    reward->pointsRequired = promptInput<int>("Nuevo número de puntos requeridos:");
}

void deleteReward(SimpleList<Reward> &rewards) {
    int index = selectIndex("Premios:", rewards.toString(), rewards.getLength());
    rewards.remove(*rewards.get(index));
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
        cin >> option;
        
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
                addClient(clients);
		        system("pause");
                break;
            case 7:
                system("cls");
                deleteClient(clients);
		        system("pause");
                break;
            case 8:
                system("cls");
                findClient(clients);
		        system("pause");
                break;
            case 9:
                system("cls");
                addReward(rewards);
		        system("pause");
                break;
            case 10:
                system("cls");
                modifyReward(rewards);
		        system("pause");
                break;
            case 11:
                system("cls");
                deleteReward(rewards);
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
                cout << "Opción inválida. Intente de nuevo.\n";
                system("pause");
            break;
        }

    }
}
