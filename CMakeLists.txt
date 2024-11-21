cmake_minimum_required(VERSION 3.28)

project(Proyecto_2)

set(CMAKE_CXX_STANDARD 17)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -static-libgcc -static-libstdc++")

find_package(nlohmann_json CONFIG REQUIRED)

add_executable(Proyecto_2 main.cpp
        structures/EntryPoint.h
        structures/EntryPoint.h
        structures/Destination.h
        structures/Destination.cpp
        structures/Destination.cpp
        structures/Route.cpp
        structures/TravelGraph.h
        structures/TravelGraph.cpp
        ui/queries.cpp
        ui/data_management.cpp
        ui/main_menu.cpp
        ui/reports.cpp
        structures/SimpleList.h
        structures/SimpleList.tpp
        structures/Client.h
        structures/Client.cpp
        structures/Reward.h
        structures/Reward.cpp
        structures/DB.h
        structures/DB.cpp
        structures/EntryPoint.cpp
        structures/EntryPoint.cpp
        structures/promptInput.h
        structures/ListNode.h
        structures/Trip.h
        structures/Trip.cpp
        structures/Edge.h
        structures/Edge.cpp
        structures/Edge.cpp
        main.cpp
        main.cpp
        structures/ClientSerializer.h
        structures/ClientSerializer.cpp
        structures/TravelGraphSerializer.h
        structures/TravelGraphSerializer.cpp
        main.cpp
        main.cpp
        structures/DB.cpp
        structures/DB.cpp
        structures/DB.cpp
        structures/utils.cpp
        structures/utils.cpp
)
target_link_libraries(Proyecto_2 PRIVATE nlohmann_json::nlohmann_json)
