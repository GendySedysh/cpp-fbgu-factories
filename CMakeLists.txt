CMAKE_MINIMUM_REQUIRED(VERSION 3.16)

PROJECT(project CXX)

ADD_EXECUTABLE(program src/main.cpp src/Factory.h src/Factory.cpp
               src/Warehouse.h src/Warehouse.cpp src/Product.h)