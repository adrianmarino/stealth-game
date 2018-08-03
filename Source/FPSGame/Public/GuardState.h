#pragma once

#include "CoreMinimal.h"
#include "GuardStateEnum.h"
#include "UObject/Interface.h"
#include "GuardState.generated.h"

class AGuardCharacter;

UINTERFACE(MinimalAPI) class UGuardState : public UInterface { GENERATED_BODY() };

class FPSGAME_API IGuardState {
	GENERATED_BODY()

public:

    virtual EGuardState GetType() = 0;

	virtual FString GetName() = 0;

	virtual void Tick(AGuardCharacter* Character, float DeltaTime);

	virtual void ResetOrientation(AGuardCharacter* Character);

	virtual void OnSeePawnEvent(AGuardCharacter* Character, APawn* Pawn);

	virtual void OnHearNoiseEvent(AGuardCharacter* Character, APawn* PawnInstigator, const FVector& Location);
};
