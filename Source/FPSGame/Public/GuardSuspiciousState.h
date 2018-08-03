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

	virtual FString GetName() override;

	virtual void ResetOrientation(AGuardCharacter* Character) override;

	virtual void OnSeePawnEvent(AGuardCharacter* Character, APawn* SeePawn) override;
	
	virtual void OnHearNoiseEvent(
		AGuardCharacter* Character,
		APawn* PawnInstigator,
		const FVector& Location
	) override;	
};