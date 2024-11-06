#include "EntryPoint.h"

EntryPoint::EntryPoint(EntryPointType type, std::string name)
		: type(type), name(std::move(name)) {}
