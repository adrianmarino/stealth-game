#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "GuardState.h"
#include "GuardIdleState.generated.h"

class AGuardCharacter;

UCLASS()
class FPSGAME_API UGuardIdleState : public UObject, public IGuardState
{
	GENERATED_BODY()

	UGuardIdleState();

	virtual EGuardState GetType() override;

	virtual EGuardState OnSeePawnEvent(AGuardCharacter* Character, APawn* Pawn) override;

	virtual EGuardState OnHearNoiseEvent(
		AGuardCharacter* Character,
		APawn* PawnInstigator,
		const FVector& Location
	) override;
};
