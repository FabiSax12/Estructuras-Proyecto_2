#ifndef ENTRYPOINT_H
#define ENTRYPOINT_H

#include <string>

#include "Enums.h"

struct  EntryPoint {
	EntryPointType type;
	std::string name;

	EntryPoint(EntryPointType type, std::string name);
};

#endif // ENTRYPOINT_H
