#include "GuardIdleWalkingState.h"
#include "GuardCharacter.h"
#include "GuardStateEnum.h"
#include "FPSGameMode.h"
#include "DebugUtils.h"

UGuardIdleWalkingState::UGuardIdleWalkingState() {}

EGuardState UGuardIdleWalkingState::GetType() { return EGuardState::IdleWalking; }

FString UGuardIdleWalkingState::GetName() { return GetClass()->GetFName().ToString(); }

void UGuardIdleWalkingState::OnSeePawnEvent(
    AGuardCharacter* Character,
    APawn* SeePawn
) {
    Character->Pause();
    Character->CallCompleteMission(SeePawn, false);
    Character->SetState(EGuardState::Alerted);
}

void UGuardIdleWalkingState::OnHearNoiseEvent(
    AGuardCharacter* Character, 
    APawn* PawnInstigator, 
    const FVector& Location
) {
    Character->RotateTo(Location);
    Character->StartResetOrientation();
    Character->Pause();
    Character->SetState(EGuardState::Suspicious);
}

void UGuardIdleWalkingState::Tick(AGuardCharacter* Character, float DeltaTime) {
    Character->TurnPatrolDirection();
}
