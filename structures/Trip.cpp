//
// Created by varga on 5/11/2024.
//

#include "Trip.h"
#include <iostream>
#include <sstream>

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
      next(nullptr){
    calculatePoints();
}

void Trip::calculatePoints() {
    int factor;
    switch (transportMethod) {
        case TransportMethod::PLANE:
            factor = 100;
            break;
        case TransportMethod::CRUISE:
            factor = 70;
            break;
        case TransportMethod::CAR:
            factor = 25;
            break;
        default:
            factor = 1;
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
