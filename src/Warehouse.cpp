#include "Warehouse.h"

/* ------------------------ Truck ------------------------ */

void Truck::CarryFromStorage(Warehouse *storage) {
    storage->SendProductsToStore(*this);
}

int Truck::GetMaxProducts() const {
    return max_products_;
}

void Truck::CarryPackage(char name) {
    carried_packages_[name] += max_products_;
}

std::unordered_map<char, int> Truck::GetCarriedPacks() const {
    return carried_packages_;
}

/* ------------------------ TruckCreator ------------------------ */

std::unique_ptr<Truck> TruckCreator::CreateTruck() {
    return std::make_unique<Truck>(rand() % RANGE_PRODUCTS + MIN_PRODUCTS);
}

void TruckCreator::PrintTruckStat() const {
    std::unordered_map<char, int> stats;

    for (auto &truck: trucs_) {
        auto truck_data = truck->GetCarriedPacks();
        for (const auto &[name, amount]: truck_data) {
            stats[name] += amount;
        }
    }

    int carried_summ = 0;
    std::cout << "Carried products by trucs:" << std::endl;
    for (const auto &[name, amount]: stats) {
        std::cout << name << ": " << amount << std::endl;
        carried_summ += amount;
    }

    std::cout << std::endl << "Percent of carried products from each factory:" << std::endl;
    for (const auto &[name, amount]: stats) {
        std::cout << name << ": " << amount * 100.0 / carried_summ << "%" << std::endl;
    }

}

void TruckCreator::ObserveStorage() {
    while (isProducing)
    {
        auto tmp = storage_->GetFullnessPercent();
        if ((storage_->GetFullnessPercent() - 0.95) > 0) {
            if (trucs_.size() == 0) {
                trucs_.push_back(std::move(CreateTruck()));
            }

            for (auto &truc: trucs_) {
                truc->CarryFromStorage(storage_);
            }

            if ((storage_->GetFullnessPercent() - 0.95) > 0) {
                trucs_.push_back(std::move(CreateTruck()));
            }
        }
    }
}

/* ------------------------ Warehouse ------------------------ */

void Warehouse::AddToStorage(Product &&new_product) {
    std::lock_guard<std::mutex> locl(mtx_);

    current_amount_++;
    storage_[new_product.name].push_back(new_product);
    CalculateVeights();
}

void Warehouse::CalculateVeights() {
    for (const auto &[name, production]: storage_) {
        storage_weights_[name] = production.size() * 1.0 / current_amount_;
    }
}

char Warehouse::MostValuablePackage() const {
    auto package_iter = std::max_element(storage_weights_.begin(), 
            storage_weights_.end(), 
            [](const auto& lhs, const auto& rhs) { return lhs.second < rhs.second; });
    
    return package_iter->first;
}

void Warehouse::SendProductsToStore(Truck &truck) {
    std::lock_guard<std::mutex> locl(mtx_);

    char mvp_package = MostValuablePackage();
    auto &packages = storage_.at(mvp_package);

    packages.erase(packages.end() - truck.GetMaxProducts(), packages.end());
    truck.CarryPackage(mvp_package);
    current_amount_ -= truck.GetMaxProducts();
    CalculateVeights();
}

float Warehouse::GetFullnessPercent() const {
    return current_amount_ * 1.0 / max_amount_;
}

bool Warehouse::IsStorageFull() const {
    return max_amount_ < current_amount_;
}

void Warehouse::PrintStat() {
    while (isProducing)
    {
        std::cout << "Current products on storage:" << std::endl;
        for (const auto &[name, production]: storage_) {
            std::cout << name << ": " << production.size() << std::endl;
        }
        std::cout << "Storage amount at: " << current_amount_ << "/" 
                  << max_amount_ << ". " << current_amount_ * 100.0 / max_amount_ << "%" << std::endl;

        std::cout << std::endl << "Press enter to stop production.." << std::endl << std::endl;

        std::this_thread::sleep_until(std::chrono::system_clock::now() + std::chrono::milliseconds(100));
    }
}