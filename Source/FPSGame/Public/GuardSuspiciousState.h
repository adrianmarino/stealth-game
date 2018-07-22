#pragma once

#include "CoreMinimal.h"
#include "GuardState.h"
#include "UObject/NoExportTypes.h"
#include "GuardSuspiciousState.generated.h"

UCLASS()
class FPSGAME_API UGuardSuspiciousState : public UObject, public IGuardState
{
	GENERATED_BODY()

	UGuardSuspiciousState();
	
	virtual EGuardState GetType() override;

	virtual EGuardState ResetOrientation(AGuardCharacter* Character) override;

	virtual EGuardState OnHearNoiseEvent(
		AGuardCharacter* Character,
		APawn* PawnInstigator,
		const FVector& Location
	) override;	
};