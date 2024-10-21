#include "Factory.h"

void Factory::StartProduce() {
    while (isProducing)
    {
        if (!storage_->IsStorageFull()) {
            storage_->AddToStorage(
                { static_cast<char>(name_ + ('a' - 'A')),
                  static_cast<unsigned int>(timeout_.count()),
                  type_ });
            
            std::this_thread::sleep_until(std::chrono::system_clock::now() + timeout_);
        }
    }
}

std::unique_ptr<Factory> FactoryCreator::CreateFactory(Warehouse *storage) {
    static char name = 'A';
    float multiplyer = 1.0 + (name - 'A') * MULIPLYER_INCREASE;

    return std::make_unique<Factory>(storage, static_cast<char>(name++), static_cast<int>(product_per_hour_ * multiplyer));
}