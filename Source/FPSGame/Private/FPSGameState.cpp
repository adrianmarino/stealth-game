#include "FPSGameState.h"

void AFPSGameState::MulticastOnMissionComplete_Implementation(APawn* InstigatorPawn, bool MissionSuccess) {
    for(FConstPawnIterator It = GetWorld()->GetPawnIterator(); It; It++) {
        APawn* CurrentPawn = It->Get();
        if(CurrentPawn && CurrentPawn->IsLocallyControlled()) {
            CurrentPawn->DisableInput(nullptr);
        }
    }
}