#include <iostream>
#include "main_menu.h"
#include "../structures/PromptInput.h"
#include "../structures/TravelGraph.h"
#include "../structures/Client.h"
#include "../structures/Reward.h"

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
}


void deleteDestination(TravelGraph &graph) {
    if (graph.destinations.getLength() < 1) {
        cout << "No hay destinos para eliminar" << endl;
        return;
    }
    const int index = selectIndex("Destinos: ", graph.destinations.toString(), graph.destinations.getLength());
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
    // cout << "Seleccione la ruta a modificar." << endl;
    // int index = selectIndex(
    //     "Rutas:",
    //     graph.routes.toString([](const Route& node) { return node->origin->name; }),
    //     graph.routes.getLength()
    // );
    //
    // auto route = graph.routes.get(index);
    // route->travelTime = promptInput<int>("Nuevo tiempo de viaje: ");
    // auto newMethodIndex = selectIndex("Nuevo metodo de transporte:", "{Avion, Carro, Barco}", 3);
    // route->transportMethod = (newMethodIndex == 0) ? TransportMethod::PLANE
    //                    : (newMethodIndex == 1) ? TransportMethod::CAR
    //                    : TransportMethod::CRUISE;
}

void deleteRoute(TravelGraph &graph) {
    // int index = selectIndex("Rutas:", graph.routes.toString([](const Route& node) { return node.origin->name; }), graph.routes.getLength());
    // graph.routes.remove(graph.routes.get(index));
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
        cout << "12. Volver\n";
        cout << "Seleccione una opcion: ";
        
        int option;
        cin >> option;
        
        switch (option) {
            case 1: addDestination(graph); break;
            case 2: deleteDestination(graph); break;
            case 3: addRoute(graph); break;
            case 4: modifyRoute(graph); break;
            case 5: deleteRoute(graph); break;
            case 6: addClient(clients); break;
            case 7: deleteClient(clients); break;
            case 8: findClient(clients); break;
            case 9: addReward(rewards); break;
            case 10: modifyReward(rewards); break;
            case 11: deleteReward(rewards); break;
            case 12: return;
            default: cout << "Opcion inválida. Intente de nuevo.\n";
        }
    }
}
