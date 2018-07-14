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
	void RotateTo(FVector Location);

	void ShowSphereIn(FVector Location, FColor Color);

	FRotator OriginalRotator;

    FTimerHandle ResetRotationTimer;

protected:
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, Category = "AI")
	UPawnSensingComponent* PawnSensingComponent;

	UFUNCTION()
	void ResetOrientation();

public:
	UFUNCTION()
	void OnSeePawnEvent(APawn* Pawn);

	UFUNCTION()
	void OnHearNoiseEvent(APawn* PawnInstigator, const FVector& Location, float Volume);


	virtual void Tick(float DeltaTime) override;
};
