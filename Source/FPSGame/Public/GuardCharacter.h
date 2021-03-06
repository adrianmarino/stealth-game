#pragma once
// ----------------------------------------------------------------------------
// Headers
// ----------------------------------------------------------------------------
#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "functional"
#include "GuardStateEnum.h"
#include "GuardCharacter.generated.h"

class UPawnSensingComponent;

UCLASS()
class FPSGAME_API AGuardCharacter : public ACharacter
{
	GENERATED_BODY()

// ----------------------------------------------------------------------------
// Attributes
// ----------------------------------------------------------------------------
private:
	FRotator OriginalRotator;

    FTimerHandle ResetRotationTimer;

	UPROPERTY(ReplicatedUsing=OnRep_GuardState)
	EGuardState CurrentState;
  
protected:
	UFUNCTION()
	void OnRep_GuardState();

	UPROPERTY(VisibleAnywhere, Category = "AI")
	UPawnSensingComponent* PawnSensingComponent;

	UPROPERTY(EditInstanceOnly, Category = "AI")
	bool enablePatrol;

	UPROPERTY(EditInstanceOnly, Category = "AI", meta = (EditCondition= "enablePatrol"))
	AActor* FirstPatrolPoint;

	UPROPERTY(EditInstanceOnly, Category = "AI", meta = (EditCondition= "enablePatrol"))
	AActor* SecondPatrolPoint;

	AActor* NextPatrolPoint;

// ----------------------------------------------------------------------------
// Constructor
// ----------------------------------------------------------------------------
public:
	AGuardCharacter();

// ----------------------------------------------------------------------------
// Methods
// ----------------------------------------------------------------------------
private:
	void MoveToNextPatrolPoint();

	float DistanceToPoint(AActor* Point);

protected:
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

	virtual void Tick(float DeltaTime) override;

	void Play();

	void Pause();

	void TurnPatrolDirection();
	
	void RotateTo(FVector Location);

	void SetupOriginalOrientation();

	void StartResetOrientation();

	void SetState(EGuardState NextState);
};