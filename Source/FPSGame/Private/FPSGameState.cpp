#include "FPSGameState.h"
#include "FPSPlayerController.h"

void AFPSGameState::MulticastOnMissionComplete_Implementation(APawn* InstigatorPawn, bool MissionSuccess)
{
    for(FConstPlayerControllerIterator It = GetWorld()->GetPlayerControllerIterator(); It; It++)
    {
        AFPSPlayerController* CurrentController = Cast<AFPSPlayerController>(It->Get());
        if(CurrentController && CurrentController->IsLocalController()) 
        {            
            CurrentController->OnMissionCompleted(InstigatorPawn, MissionSuccess);

            if(MissionSuccess) {
                UE_LOG(LogTemp, Warning, TEXT("Mission Completed!"));
            } else {
                UE_LOG(LogTemp, Warning, TEXT("Mission Fail!"));
            }
         
            APawn* Pawn = CurrentController->GetPawn();
            if(Pawn) {
                Pawn->DisableInput(CurrentController);
            }
        }
    }
}