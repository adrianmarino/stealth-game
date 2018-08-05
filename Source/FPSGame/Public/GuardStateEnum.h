#pragma once

#include "CoreMinimal.h"
#include "GuardStateEnum.generated.h"

UENUM(BlueprintType)
enum class EGuardState : uint8 { 
    IdleWalking,
    Suspicious, 
    Alerted
};