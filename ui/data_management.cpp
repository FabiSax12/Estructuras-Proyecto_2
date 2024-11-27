#include <iostream>
#include <vector>
#include <sstream>
#include <cmath>
#include "main_menu.h"
#include "../structures/TravelGraph.h"
#include "../structures/Client.h"
#include "../structures/Reward.h"
#include "../structures/TravelGraphSerializer.h"
#include "../structures/DB.h"

using namespace std;
/**
 * Muestra un mensaje de advertencia acerca de los caracteres no admitidos en la entrada.
 */
void warningMessage() {
    std::cout << "ADEVERTENCIA:\nCaracteres no admitidos: !@#$%^&*+=~\\|\"'<>?/`" << std::endl;
}
/**
 * Agrega un nuevo destino al grafo de viajes, solicitando el nombre del país, el punto de entrada y el tipo de entrada.
 * Si el destino ya existe, se notifica al usuario.
 *
 * @param graph El grafo de viajes donde se agregará el destino.
 */
void addDestination(TravelGraph &graph) {
    string countryName;
    string ep_name;
    int ep_typeIndex;

    cout << endl << " ================== Agregar destino ==================" << endl;
    warningMessage();

    cout << "Nombre del pais: ";
    input(countryName,{});
    cout<<"Punto de entrada a " + countryName + ":{ Aeropuerto, Frontera, Puerto }\nEscoja mediante el indice (0 - 2): " ;
    input(ep_typeIndex,{0,2});
    cout << "Nombre del punto de entrada: ";
    input(ep_name,{});

    EntryPointType ep_type;
    if (ep_typeIndex == 0) ep_type = EntryPointType::AIRPORT;
    else if (ep_typeIndex == 1) ep_type = EntryPointType::BORDER;
    else if (ep_typeIndex == 2) ep_type = EntryPointType::PORT;

    for (Destination& destination : graph.destinations) {
        if (destination.name == countryName&&destination.entryPointName==ep_name) {
            cout <<"El destino ya existe en el grafo" << endl;
            return;
        }
    }

    graph.addDestination(new Destination(countryName, ep_name, new EntryPoint(ep_type, ep_name)));
    updateJSONDestinations(countryName, ep_name, ep_type,"../data/destinations.json");
}

/**
 * Elimina todas las rutas que tienen un destino específico.
 *
 * @param graph El grafo de viajes que contiene las rutas.
 * @param destinationSearched El destino cuyas rutas se eliminarán.
 */
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
/**
 * Muestra todas las rutas disponibles en el grafo de viajes.
 * Si no hay destinos, se muestra un mensaje indicando que no hay destinos para mostrar.
 *
 * @param graph El grafo de viajes del que se mostrarán las rutas.
 */
void showRoutes(TravelGraph &graph) {
    system("cls");
    if (graph.destinations.getLength() < 1) {
        cout << "No hay destinos para mostrar" << endl;
        return;
    }
    vector<string> strDestinations;
    for (Destination destination : graph.destinations) {
        strDestinations.push_back("Destino: "+destination.entryPointName+" ("+destination.name+")");
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
/**
 * Elimina un destino del grafo de viajes. Si no hay destinos, muestra un mensaje indicando que no hay destinos para eliminar.
 *
 * @param graph El grafo de viajes del que se eliminará el destino.
 */
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
    deleteRoutes(graph,graph.destinations.get(index));
    graph.destinations.removeByIndex(index);
    DB::saveDestinationsAndRoutes(R"(data\destinations.json)",graph);
}
/**
 * Agrega una nueva ruta entre dos destinos, solicitando la información necesaria sobre los países, puntos de entrada,
 * tiempo de viaje y método de transporte.
 *
 * @param graph El grafo de viajes donde se agregará la ruta.
 */
void addRoute(TravelGraph &graph) {
    cout << endl << " ================== Agregar ruta ==================" << endl;
    warningMessage();
    string originCountry, originEntryPoint;
    string destCountry, destEntryPoint;
    double time;
    int transportMethod;

    cout << "Nombre del pais de origen: ";
    input(originCountry,{});
    cout << "Nombre del punto de entrada de origen: ";
    input(originEntryPoint,{});
    cout << "Nombre del pais de destino: ";
    input(destCountry,{});
    cout << "Nombre del punto de entrada de destino: ";
    input(destEntryPoint,{});
    cout<<"Tiempo de viaje: ";
    input(time,{});
    cout<<"Metodos de transporte: {Avion, Carro, Barco}\nEscoja mediante el indice (0 - 2): " ;
    input(transportMethod,{0,2});

    TransportMethod tmType;
    if (transportMethod == 0) tmType = TransportMethod::PLANE;
    else if (transportMethod == 1) tmType = TransportMethod::CAR;
    else if (transportMethod == 2) tmType = TransportMethod::CRUISE;

    graph.addRoute(originCountry, originEntryPoint, destCountry, destEntryPoint, time, tmType);
    DB::saveDestinationsAndRoutes(R"(data\destinations.json)",graph);
}
/**
 * Modifica una ruta existente, permitiendo al usuario cambiar los destinos, tiempo de viaje y método de transporte.
 * Si la ruta es válida, la ruta original se elimina y se agrega la nueva.
 *
 * @param graph El grafo de viajes donde se modificará la ruta.
 */
void modifyRoute(TravelGraph &graph) {
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
                warningMessage();
                string originCountry = destination.name;
                string originEntryPoint = destination.entryPointName;
                string destCountry, destEntryPoint;
                double time;
                int transportMethod;
                cout << "Nombre del nuevo pais de destino: ";
                input(destCountry,{});
                cout << "Nombre del nuevo punto de entrada de destino: ";
                input(destEntryPoint,{});
                cout<<"Nuevo tiempo de viaje: ";
                input(time,{});
                cout<<"Nuevo metodo de transporte: {Avion, Carro, Barco}\nEscoja mediante el indice (0 - 2): " ;
                input(transportMethod,{0,2});
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

/**
 * Elimina una ruta existente entre dos destinos. Si no hay rutas, se muestra un mensaje indicando que no hay rutas para eliminar.
 *
 * @param graph El grafo de viajes del que se eliminará la ruta.
 */
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

/**
 * Agrega un nuevo cliente, verificando que el nombre no esté vacío y que el cliente no exista ya en la lista.
 *
 * @param clients La lista de clientes a la que se agregará el nuevo cliente.
 * @param rewards La lista de recompensas asociadas a los clientes.
 */
void addClient(SimpleList<Client> &clients,SimpleList<Reward> &rewards) {
    cout << endl << " ================== Agregar cliente ================== " << endl;
    warningMessage();
    string strName;
    cout << "Nombre del cliente: ";
    input(strName,{});
    if(!strName.empty()) {
        for (auto &client : clients) {
            if (client.name == strName) {
                cout << "El cliente ya ha sido registrado, ingrese uno nuevo" << endl;
                return;
            }
        }
        clients.add(Client(strName));
        DB::saveClientsAndRewards(R"(data\clients.json)",clients,rewards);
    }else {
        cout<<"Error: El nombre no puede estar vacio!\n";
    }
}
/**
 * Elimina un cliente de la lista de clientes. Si no hay clientes registrados, se muestra un mensaje de advertencia.
 *
 * @param clients La lista de clientes de la que se eliminará el cliente.
 * @param rewards La lista de recompensas asociadas a los clientes.
 */
void deleteClient(SimpleList<Client> &clients,SimpleList<Reward> &rewards) {
    if (clients.getLength()== 0) {
        cout<<"No hay clientes resgistrados\n";
        return;
    }
    vector<string> strClients;
    Client* currentClient = clients.get(0);
    while(currentClient != nullptr) {
        cout<<"Cliente: "+currentClient->name+" Puntos Acumulados: "+std::to_string(currentClient->accumulatedPoints)<<endl;
        strClients.push_back("Cliente: "+currentClient->name+" Puntos Acumulados: "+std::to_string(currentClient->accumulatedPoints));
        currentClient=currentClient->next;
    }cout<<endl;
    int index=selectOption(strClients);
    if (index==-1){return;}
    clients.remove(*clients.get(index));
    DB::saveClientsAndRewards(R"(data\clients.json)",clients,rewards);
}
/**
 * Busca un cliente por nombre y muestra sus puntos acumulados si se encuentra.
 *
 * @param clients La lista de clientes donde se buscará al cliente.
 * @param rewards La lista de recompensas asociadas a los clientes.
 */
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

/**
 * Agrega una nueva recompensa, solicitando el nombre y los puntos requeridos. Si ya existe, se muestra un mensaje de advertencia.
 *
 * @param clients La lista de clientes a la que se asociarán las recompensas.
 * @param rewards La lista de recompensas donde se agregará la nueva recompensa.
 */
void addReward(SimpleList<Client> &clients,SimpleList<Reward> &rewards) {
    string strName;
    int points;
    cout << "Nombre del premio: ";
    input(strName,{});
    cout<<"Puntos requeridos para canjear el premio: ";
    input(points,{});
    if(points>0&&!strName.empty()) {
        for (auto &reward : rewards) {
            if (reward.name == strName) {
                cout << "El premio ya ha sido registrado, ingrese uno nuevo" << endl;
                return;
            }
        }
        rewards.add(Reward(strName,points));
        DB::saveClientsAndRewards(R"(data\clients.json)",clients,rewards);
    }else {
        cout<<"Error: Datos incorrectos!\n";
    }
}
/**
 * Modifica una recompensa existente, permitiendo cambiar su nombre y los puntos requeridos.
 *
 * @param clients La lista de clientes a la que se asociarán las recompensas.
 * @param rewards La lista de recompensas donde se modificará la recompensa.
 */
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
    input(strName,{});
    cout<<"Nueva cantidad de puntos requeridos: ";
    input(points,{});
    if(points>0&&!strName.empty()) {
        reward->name=strName;
        reward->pointsRequired=points;
        DB::saveClientsAndRewards(R"(data\clients.json)",clients,rewards);
    }else {
        cout<<"Error: Datos incorrectos!\n";
    }
}
/**
 * Elimina una recompensa existente de la lista de recompensas. Si no hay recompensas disponibles, se muestra un mensaje de advertencia.
 *
 * @param clients La lista de clientes a la que se asociarán las recompensas.
 * @param rewards La lista de recompensas donde se eliminará la recompensa.
 */
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
/**
 * Función principal para gestionar los datos de destinos, rutas, clientes y recompensas, presentando un menú al usuario.
 * El menú permite agregar, eliminar, modificar y mostrar destinos, rutas, clientes y recompensas.
 *
 * @param graph El grafo de viajes que contiene los destinos y rutas.
 * @param clients La lista de clientes registrados.
 * @param rewards La lista de recompensas disponibles.
 */
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

        input(option,{1,13});

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
