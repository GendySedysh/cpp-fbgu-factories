#ifndef FACTORY_H
#define FACTORY_H

#include "Warehouse.h"
#include <chrono>
#include <thread>
#include <memory>

#define SECONDS_IN_HOUR 3600
#define MULIPLYER_INCREASE 0.1

class Factory
{
public:
    // N - кол-во продукции выпускаемой в час
    Factory(Warehouse *storage, char name, int N)
    : storage_(storage), name_(name), timeout_(SECONDS_IN_HOUR / N) {}

    void StartProduce();
private:
    Warehouse *storage_;
    char name_;
    std::chrono::milliseconds timeout_;
    PackageType type_ = static_cast<PackageType>(rand() % 4);
};

class FactoryCreator {
public:
    FactoryCreator(int product_per_hour): product_per_hour_(product_per_hour) {}
    std::unique_ptr<Factory> CreateFactory(Warehouse *storage);
private:
    int product_per_hour_;
};

#endif