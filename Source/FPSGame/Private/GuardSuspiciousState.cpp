#include "GuardSuspiciousState.h"
#include "GuardIdleState.h"
#include "GuardCharacter.h"
#include "DebugUtils.h"
#include "GuardStateEnum.h"

UGuardSuspiciousState::UGuardSuspiciousState() {}

EGuardState UGuardSuspiciousState::GetType() { return EGuardState::Suspicious; }

EGuardState UGuardSuspiciousState::ResetOrientation(AGuardCharacter* Character) {
    Character->SetupOriginalOrientation();
    return EGuardState::Idle;
}

EGuardState UGuardSuspiciousState::OnHearNoiseEvent(
    AGuardCharacter* Character, 
    APawn* PawnInstigator, 
    const FVector& Location
) {
    DebugUtils::ShowSphereIn(Character->GetWorld(), Location, FColor::Blue);
    Character->RotateTo(Location);
    Character->StartResetOrientation();
    return EGuardState::Suspicious;
}
