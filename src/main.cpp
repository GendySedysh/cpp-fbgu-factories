#include "Factory.h"

int CalculateAddedStorageAmount(int num_of_factories, int base_production_rate) {
    int added_amount = 0;
    float muliplyer = 1.0;

    for (int i = 0; i < num_of_factories; ++i) {
        added_amount += (i * MULIPLYER_INCREASE + muliplyer) * base_production_rate;
    }
    return added_amount;
}

int main(int argc, char ** ) {
    int num_of_factories = 5, base_production_rate = 100, base_storage_amount = 1;

    std::cout << "Enter number of factories(min 3, max 26): ";
    std::cin >> num_of_factories;

    if (num_of_factories < 3 && num_of_factories > 26) {
        std::cerr << "Wrong number of factories" << std::endl;
        exit(1);
    }

    std::cout << "Enter basic production rate(min 50): ";
    std::cin >> base_production_rate;

    if (base_production_rate < 50) {
        std::cerr << "Wrong basic production rate" << std::endl;
        exit(1);
    }

    std::cout << "Enter basic storage amount(min 100): ";
    std::cin >> base_storage_amount;

    if (base_storage_amount < 100) {
        std::cerr << "Wrong basic storage amount" << std::endl;
        exit(1);
    }

    Warehouse storage(base_storage_amount * CalculateAddedStorageAmount(num_of_factories, base_production_rate));
    FactoryCreator factory_creator(base_production_rate);
    TruckCreator truck_creator(&storage);
        
    std::vector<std::unique_ptr<Factory>> factories;
    for (int i = 0; i < num_of_factories; ++i) {
        factories.push_back(std::move(factory_creator.CreateFactory(&storage)));
    }

    std::cout << "Factories created.." << std::endl;

    std::vector<std::thread> threads;
    for (int i = 0; i < num_of_factories; ++i) {
        threads.push_back(std::thread(&Factory::StartProduce, factories[i].get()));
    }

    std::cout << "Production started.." << std::endl << std::endl;
    std::thread stat_thread(&Warehouse::PrintStat, &storage);
    std::thread truck_thread(&TruckCreator::ObserveStorage, &truck_creator);


    getchar();
    getchar();
    isProducing = false;
    
    for (auto &thread: threads) {
        thread.join();
    }
    stat_thread.join();
    truck_thread.join();

    truck_creator.PrintTruckStat();

    return 0;
}