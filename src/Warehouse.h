#ifndef WAREHOUSE_H
#define WAREHOUSE_H

#include "Product.h"
// #include "Truck.h"
#include <sstream>
#include <unordered_map>
#include <vector>
#include <mutex>
#include <chrono>
#include <thread>
#include <algorithm>

#define MIN_PRODUCTS 10
#define MAX_PRODUCTS 100
#define RANGE_PRODUCTS MAX_PRODUCTS - MIN_PRODUCTS + 1

class Warehouse;

class Truck
{
public:
    Truck(int max_products) 
    : max_products_(max_products) {}
    
    void CarryFromStorage(Warehouse *storage);
    void CarryPackage(char name);
    int GetMaxProducts() const;
    std::unordered_map<char, int> GetCarriedPacks() const;
private:
    int max_products_;
    std::unordered_map<char, int> carried_packages_;
};

class TruckCreator {
public:
    TruckCreator(Warehouse *storage): storage_(storage) {}
    void ObserveStorage();
    void PrintTruckStat() const;
    std::unique_ptr<Truck> CreateTruck();
private:
    Warehouse *storage_;
    std::vector<std::unique_ptr<Truck>> trucs_;
};

class Warehouse
{
public:
    Warehouse(int N)
    : max_amount_(N * 100) {}

    void AddToStorage(Product &&new_product);
    float GetFullnessPercent() const;
    void SendProductsToStore(Truck &truck);
    bool IsStorageFull() const;
    void PrintStat();
private:
    void CalculateVeights();
    char MostValuablePackage() const;

    std::unordered_map<char, std::vector<Product>> storage_;
    std::unordered_map<char, float> storage_weights_;
    std::mutex mtx_;
    int current_amount_ = 0;
    int max_amount_;
};


#endif