#ifndef TRAVELGRAPH_H
#define TRAVELGRAPH_H

#include <string>
#include "Destination.h"
#include "Route.h"
#include "SimpleList.h"

struct TravelGraph {
	SimpleList<Destination> destinations;

	TravelGraph();
	void addDestination(Destination *dest);
	[[nodiscard]] Destination* findDestination(const std::string& countryName, const std::string& entryPointName) const;
	void addRoute(
		const std::string& originCountry, const std::string& originEntryPoint,
		const std::string& destCountry, const std::string& destEntryPoint,
		double time, TransportMethod method
	) const;
	bool validateRoute(const std::string& originCountry, const std::string& originEntryPoint,
						   const std::string& destCountry, const std::string& destEntryPoint,
						   double time, TransportMethod method) const;
	void amplitude() const;
	void depth(Destination *start) const;
	void startDepthTraversal() const;
	void demark() const;
	bool routeExist(Destination *origin, const string& destination);
};

#endif // TRAVELGRAPH_H
