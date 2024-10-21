#ifndef PRODUCT_H
#define PRODUCT_H
#include <iostream>
#include <atomic>

inline std::atomic<bool> isProducing = true;

enum PackageType {
    CONSUMER, 
    PRODUCTION,
    TRANSPORT, 
    SPECIALITY,
    PRESERVATION
};

struct Product
{
    char name;
    unsigned int weight;
    PackageType type;
};

#endif