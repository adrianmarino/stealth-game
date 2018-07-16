#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "GuardCharacter.generated.h"

class UPawnSensingComponent;

UCLASS()
class FPSGAME_API AGuardCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	AGuardCharacter();

private:
	FRotator OriginalRotator;

    FTimerHandle ResetRotationTimer;

	void RotateTo(FVector Location);

	void ShowSphereIn(FVector Location, FColor Color);

	void StartResetOrientation();

protected:
	UPROPERTY(VisibleAnywhere, Category = "AI")
	UPawnSensingComponent* PawnSensingComponent;

	virtual void BeginPlay() override;

	UFUNCTION()
	void ResetOrientation();

public:
	UFUNCTION()
	void OnSeePawnEvent(APawn* Pawn);

	UFUNCTION()
	void OnHearNoiseEvent(APawn* PawnInstigator, const FVector& Location, float Volume);
};
