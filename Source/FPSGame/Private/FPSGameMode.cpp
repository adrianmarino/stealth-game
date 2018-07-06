#include "FPSGameMode.h"
#include "FPSHUD.h"
#include "FPSCharacter.h"
#include "UObject/ConstructorHelpers.h"

AFPSGameMode::AFPSGameMode() {
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnClassFinder(TEXT("/Game/Blueprints/BP_Player"));
	DefaultPawnClass = PlayerPawnClassFinder.Class;

	HUDClass = AFPSHUD::StaticClass();
}

void AFPSGameMode::CompleteMission(APawn* InstigatorPawn) {
	if(!InstigatorPawn) return;

	UE_LOG(LogTemp, Log, TEXT("Mission Completed!"));
	InstigatorPawn->DisableInput(nullptr);
	OnMissionCompleted(InstigatorPawn);
}
