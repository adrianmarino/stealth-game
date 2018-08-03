#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "GuardState.h"
#include "GuardIdleWalkingState.generated.h"

class AGuardCharacter;

UCLASS()
class FPSGAME_API UGuardIdleWalkingState : public UObject, public IGuardState
{
	GENERATED_BODY()

	UGuardIdleWalkingState();

	virtual EGuardState GetType() override;

	virtual FString GetName() override;

	virtual void Tick(AGuardCharacter* Character, float DeltaTime) override;

	virtual void OnSeePawnEvent(AGuardCharacter* Character, APawn* Pawn) override;

	virtual void OnHearNoiseEvent(
		AGuardCharacter* Character,
		APawn* PawnInstigator,
		const FVector& Location
	) override;
};
