//
// Created by varga on 5/11/2024.
//

#include "Trip.h"
#include <iostream>
#include <sstream>
#include <utility>

Trip::Trip(
        const string& originCountry, const string& originEntryPoint, const string& destinationCountry,
        const string& destinationEntryPoint, TransportMethod transportMethod, double travelTime
    ): originCountry(originCountry),
      originEntryPoint(originEntryPoint),
      destinationCountry(destinationCountry),
      destinationEntryPoint(destinationEntryPoint),
      transportMethod(transportMethod),
      travelTime(travelTime),
      pointsEarned(0.0),
      next(nullptr) {
    calculatePoints();
}


void Trip::calculatePoints() {
    double factor = 0.0;
    switch (transportMethod) {
        case TransportMethod::PLANE:
            factor = 10.0;
            break;
        case TransportMethod::CAR:
            factor = 5.0;
            break;
        case TransportMethod::CRUISE:
            factor = 8.0;
            break;
        default:
            factor = 1.0;
            break;
    }
    pointsEarned = travelTime * factor;
}

std::string Trip::toString() const {
    std::ostringstream oss;
    oss << "Origen: " << originCountry << " - " << originEntryPoint
        << "\n\t\tDestino: " << destinationCountry << " - " << destinationEntryPoint
        << "\n\t\tTransporte: " << getTransportMethodString()
        << "\n\t\tDuracion: " << travelTime << " horas"
        << "\n\t\tPuntos obtenidos: " << pointsEarned;
    return oss.str();
}

std::string Trip::getTransportMethodString() const {
    switch (transportMethod) {
        case TransportMethod::PLANE:
            return "Avion";
        case TransportMethod::CAR:
            return "Carro";
        case TransportMethod::CRUISE:
            return "Crucero";
        default:
            return "Desconocido";
    }
}
