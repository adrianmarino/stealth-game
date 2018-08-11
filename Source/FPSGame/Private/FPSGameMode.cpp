#include "FPSGameMode.h"
#include "FPSHUD.h"
#include "FPSCharacter.h"
#include "UObject/ConstructorHelpers.h"
#include "Kismet/GameplayStatics.h"
#include "FPSGameState.h"
#include "FPSPlayerController.h"

AFPSGameMode::AFPSGameMode() {
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnClassFinder(TEXT("/Game/Blueprints/BP_Player"));
	DefaultPawnClass = PlayerPawnClassFinder.Class;

	HUDClass = AFPSHUD::StaticClass();
	GameStateClass = AFPSGameState::StaticClass();
	PlayerControllerClass = AFPSPlayerController::StaticClass();
}

void AFPSGameMode::CompleteMission(APawn* InstigatorPawn, bool MissionSuccess) {
	if(!InstigatorPawn) return;
	if(!SpectatingViewpointClass) {
		UE_LOG(LogTemp, Error, TEXT("SpectatingViewpointClass is nullptr!. Can not change view spectating view target."));
		return;
	}

	TArray<AActor*> SpectatingActors;
	UGameplayStatics::GetAllActorsOfClass(this, SpectatingViewpointClass, SpectatingActors);
	if(SpectatingActors.Num() < 0) return;

	AActor* NewViewTarget = SpectatingActors[0];	
	APlayerController* PlayerController = Cast<APlayerController>(
		InstigatorPawn->GetController()
	);
	if(!PlayerController) return;

	for(FConstPlayerControllerIterator It = GetWorld()->GetPlayerControllerIterator(); It; It++)
    {
        AFPSPlayerController* CurrentController = Cast<AFPSPlayerController>(It->Get());
        if(CurrentController) 
        {  
			CurrentController->SetViewTargetWithBlend(
				NewViewTarget,
				0.5f,
				EViewTargetBlendFunction::VTBlend_Cubic
			);
		}
	}

	AFPSGameState* GameState = GetGameState<AFPSGameState>();
	if(GameState) GameState->MulticastOnMissionComplete(InstigatorPawn, MissionSuccess);
}
