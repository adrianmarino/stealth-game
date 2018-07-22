#pragma once

#include "CoreMinimal.h"
#include "GuardStateEnum.h"
#include "GameFramework/Character.h"
#include "GuardCharacter.generated.h"

class UPawnSensingComponent;
class IGuardState;

UCLASS()
class FPSGAME_API AGuardCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	AGuardCharacter();

private:
	FRotator OriginalRotator;

    FTimerHandle ResetRotationTimer;

	IGuardState* GuardState;

protected:
	UPROPERTY(VisibleAnywhere, Category = "AI")
	UPawnSensingComponent* PawnSensingComponent;

	virtual void BeginPlay() override;

	UFUNCTION()
	void ResetOrientation();

	UFUNCTION()
	void OnSeePawnEvent(APawn* Pawn);

	UFUNCTION()
	void OnHearNoiseEvent(
		APawn* PawnInstigator,
		const FVector& Location,
		float Volume
	);

public:
	UFUNCTION(BlueprintImplementableEvent, Category = "AI")
	void OnStateChanged(EGuardState State);

	void CallCompleteMission(APawn* Pawn, bool Success);

	void SetState(EGuardState NextState);

	void RotateTo(FVector Location);

	void ShowSphereIn(FVector Location, FColor Color);

	void SetupOriginalOrientation();

	void StartResetOrientation();
};