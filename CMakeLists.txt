cmake_minimum_required(VERSION 3.29)
project(EnergyGradient)

set(CMAKE_CXX_STANDARD 23)

add_executable(EnergyGradient main.cpp
        Vertex/Vertex.h
        Polygon/Polygon.cpp
        Polygon/Polygon.h
        Run/Box.cpp
        Run/Box.h
        Cell/Cell.h
        Cell/Cell.cpp
        Run/Run.cpp
        Run/Run.h
        conf
        loader/loader.h
        loader/loader.cpp
        Edge/Edge.h
        Edge/Edge.cpp)
