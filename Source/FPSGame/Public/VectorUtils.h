#pragma once

#include "CoreMinimal.h"

class VectorUtils 
{
public:
    static float DistanceBetween(FVector VectorA, FVector VectorB) { 
        return (VectorA - VectorB).Size();
    }
};