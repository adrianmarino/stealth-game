#pragma once

#include "CoreMinimal.h"
#include "DrawDebugHelpers.h"

class DebugUtils {

public:
    static void ShowSphereIn(UWorld* World, FVector Location, FColor Color) {
        DrawDebugSphere(
            World,
            Location,
            32.0f,
            30,
            Color,
            false,
            10.0f
        );
    }
};

