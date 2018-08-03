#include "GuardSuspiciousState.h"
#include "GuardIdleWalkingState.h"
#include "GuardCharacter.h"
#include "DebugUtils.h"
#include "GuardStateEnum.h"

UGuardSuspiciousState::UGuardSuspiciousState() {}

EGuardState UGuardSuspiciousState::GetType() { return EGuardState::Suspicious; }

FString UGuardSuspiciousState::GetName() { return GetClass()->GetFName().ToString(); }

void UGuardSuspiciousState::ResetOrientation(AGuardCharacter* Character) {
    Character->SetupOriginalOrientation();
    Character->Play();
    Character->SetState(EGuardState::IdleWalking);
}

void UGuardSuspiciousState::OnHearNoiseEvent(
    AGuardCharacter* Character, 
    APawn* PawnInstigator, 
    const FVector& Location
) {
    Character->RotateTo(Location);
    Character->StartResetOrientation();
}