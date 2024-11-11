//
// Created by varga on 27/10/2024.
//

#ifndef ENUMS_H
#define ENUMS_H
#include <algorithm>
#include <string>
#include <sstream>
#include <vector>

/**
 * @brief Defines types of entry points within a destination.
 *
 * This enum classifies the entry points that can be associated with a destination.
 *
 * - BORDER: Represents a land border entry point.
 * - AIRPORT: Represents an airport entry point.
 * - PORT: Represents a port entry point.
 *
 * @see EntryPoint
 * @see Destination
 *
 * @note Used within the EntryPoint struct to specify entry point type.
 *
 * @enum EntryPointType
 * @author Fabian Vargas
 */
enum class EntryPointType {
	BORDER,
	AIRPORT,
	PORT
};

/**
 * @brief Defines types of transport methods.
 *
 * This enum classifies the modes of transportation associated with a destination.
 *
 * - PLANE: Air transportation.
 * - CRUISE: Sea transportation via cruise.
 * - CAR: Land transportation via car.
 *
 * @see Destination
 *
 * @enum TransportMethod
 * @note Used to categorize transportation modes.
 * @note Additional types may be added as required.
 *
 * @enum TransportMethod
 *
 * @param TransportMethod Type of transportation method used to reach a destination.
 *
 * @note Designed to work with different travel modes in the application.
 *
 * @see Destination
 * @note Suitable for future additions as application expands.
 * @author Fabian Vargas
 */
enum class TransportMethod {
	PLANE,
	CRUISE,
	CAR
};

inline std::string transportMethodToString(const TransportMethod method) {
	switch (method) {
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

inline TransportMethod stringToTransportMethod(const std::string& str) {
	if (str == "Avión" || str == "Avion") return TransportMethod::PLANE;
	if (str == "Carro") return TransportMethod::CAR;
	if (str == "Crucero") return TransportMethod::CRUISE;
	return TransportMethod::PLANE;
}

inline std::vector<TransportMethod> parseTransportMethods(const std::string& input) {
	std::vector<TransportMethod> methods;
	std::stringstream ss(input);
	std::string methodStr;
	while (std::getline(ss, methodStr, ',')) {
		methodStr.erase(std::remove_if(methodStr.begin(), methodStr.end(), ::isspace), methodStr.end());
		TransportMethod method = stringToTransportMethod(methodStr);
		methods.push_back(method);
	}
	return methods;
}

#endif //ENUMS_H
