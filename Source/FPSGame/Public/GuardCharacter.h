#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "GuardCharacter.generated.h"

class UPawnSensingComponent;

UENUM(BlueprintType)
enum class EGuardState : uint8 { Idel, Suspicious, Alerted };

UCLASS()
class FPSGAME_API AGuardCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	AGuardCharacter();

private:
	FRotator OriginalRotator;

    FTimerHandle ResetRotationTimer;

	EGuardState State;

	void SetState(EGuardState NextState);

	void RotateTo(FVector Location);

	void ShowSphereIn(FVector Location, FColor Color);

	void StartResetOrientation();

protected:
	UPROPERTY(VisibleAnywhere, Category = "AI")
	UPawnSensingComponent* PawnSensingComponent;

	virtual void BeginPlay() override;

	UFUNCTION()
	void ResetOrientation();

	UFUNCTION(BlueprintImplementableEvent, Category = "AI")
	void OnStateChanged(EGuardState State);

public:
	UFUNCTION()
	void OnSeePawnEvent(APawn* Pawn);

	UFUNCTION()
	void OnHearNoiseEvent(APawn* PawnInstigator, const FVector& Location, float Volume);
};
