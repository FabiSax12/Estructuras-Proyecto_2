#include "TravelGraph.h"
#include <iostream>
#include <queue>

TravelGraph::TravelGraph() {
    this->destinations = SimpleList<Destination>();
};

void TravelGraph::addDestination(Destination* dest) {
    if (findDestination(dest->name, dest->entryPointName) == nullptr) {
        destinations.add(*dest);
    } else {
        // Manejar el caso en que el destino ya existe
        std::cout << "El destino ya existe: " << dest->name << ", " << dest->entryPointName << std::endl;
    }
}


std::string normalizeString(const std::string& str) {
    std::string result = str;
    // Convertir a minúsculas y eliminar espacios en blanco al inicio y al final
    std::transform(result.begin(), result.end(), result.begin(), ::tolower);
    result.erase(result.find_last_not_of(" \n\r\t")+1);
    result.erase(0, result.find_first_not_of(" \n\r\t"));
    return result;
}

Destination* TravelGraph::findDestination(const std::string& countryName, const std::string& entryPointName) const {
    std::string normCountryName = normalizeString(countryName);
    std::string normEntryPointName = normalizeString(entryPointName);

    for (auto& dest : destinations) {
        if (normalizeString(dest.name) == normCountryName && normalizeString(dest.entryPointName) == normEntryPointName)
        {
            return &dest;
        }
    }
    return nullptr;
}


void TravelGraph::addRoute(const std::string& originCountry, const std::string& originEntryPoint,
                           const std::string& destCountry, const std::string& destEntryPoint,
                           double time, TransportMethod method) const {
    Destination* origin = findDestination(originCountry, originEntryPoint);
    Destination* destination = findDestination(destCountry, destEntryPoint);

    if (origin && destination&&time>0) {
        Route* newRoute = new Route(destination, time, method);
        newRoute->next = origin->routes;
        origin->routes = newRoute;
    } else {
        std::cout << "Origen o destino no encontrado." << std::endl;
    }
}

bool TravelGraph::validateRoute(const std::string& originCountry, const std::string& originEntryPoint,
                           const std::string& destCountry, const std::string& destEntryPoint,
                           double time, TransportMethod method) const {
    Destination* origin = findDestination(originCountry, originEntryPoint);
    Destination* destination = findDestination(destCountry, destEntryPoint);

    if (origin && destination&&time>0) {
        return true;
    } else {
        std::cout << "Origen o destino no encontrado o el tiempo tiene un formato incorrecto."<< std::endl;
        return false;
    }
}

void TravelGraph::amplitude() const {
    std::queue<Destination*> queue;
    demark();

    for (auto& dest : destinations) {
        if (!dest.visited) {
            dest.visited = true;
            queue.push(&dest);

            while (!queue.empty()) {
                Destination* current = queue.front();
                queue.pop();

                Route* route = current->routes;
                while (route != nullptr) {
                    Destination* destRoute = route->destination;

                    std::cout << "Origen: " << current->name << " ( " << current->entryPointName << " )" << std::endl;
                    std::cout << "\tDestino: " << destRoute->name << " ( " << destRoute->entryPointName << " )" << std::endl;
                    std::cout << "\tTransporte: " << route->getTransportMethod() << std::endl;
                    std::cout << "\tDuracion: " << route->travelTime << " horas" << std::endl;

                    if (!destRoute->visited) {
                        destRoute->visited = true;
                        queue.push(destRoute);
                    }
                    route = route->next;
                }

            }
        }
    }
}

void TravelGraph::depth(Destination *start) const {
    if (start == nullptr || start->visited) return;
    start->visited = true;

    Route *currentA = start->routes;
    while (currentA != nullptr) {
        cout << "Origen: " << start->name << " - " << start->entryPointName << endl;
        cout << "\tDestino: " << currentA->destination->name << " - " << currentA->destination->entryPointName << endl;
        depth(currentA->destination);
        currentA = currentA->next;
    }
}

void TravelGraph::startDepthTraversal() const {
    demark();

    std::string startCountry, startEntryPoint;
    cout << "Ingrese el pais de inicio: ";
    getline(cin, startCountry);
    cout << "Ingrese el punto de entrada de inicio: ";
    getline(cin, startEntryPoint);

    Destination* start = findDestination(startCountry, startEntryPoint);

    if (start == nullptr) {
        cout << "El destino inicial no se encontro en el grafo." << endl;
        return;
    }

    depth(start);
}

void TravelGraph::demark() const {
    for (auto& dest : destinations) {
        dest.visited = false;
    }
}

bool TravelGraph::routeExist(Destination *origin, const string& destinationEntryPointName) {
    if (origin == nullptr || origin->visited) return false;
    if (origin->entryPointName == destinationEntryPointName) return true;
    origin->visited = true;

    Route *currentA = origin->routes;
    while (currentA != nullptr) {
        if (routeExist(currentA->destination, destinationEntryPointName)) {
            return true;
        }
        currentA = currentA->next;
    }

    return false;
}

Route* TravelGraph::findRoute(
    const std::string& originCountry, const std::string& originEntryPoint,
    const std::string& destCountry, const std::string& destEntryPoint
) const {
    std::string originName = normalizeString(originCountry);
    std::string originEntry = normalizeString(originEntryPoint);
    std::string destName = normalizeString(destCountry);
    std::string destEntry = normalizeString(destEntryPoint);

    for (auto& dest : destinations) {
        if (normalizeString(dest.name) == originName && normalizeString(dest.entryPointName) == originEntry) {
            auto route = dest.routes;
            while (route != nullptr) {
                if (
                    normalizeString(route->destination->name) == destName
                    && normalizeString(route->destination->entryPointName) == destEntry
                ) return route;

                route = route->next;
            }
        }
    }
    return nullptr;
}


