#include "FPSGameMode.h"
#include "FPSHUD.h"
#include "FPSCharacter.h"
#include "UObject/ConstructorHelpers.h"
#include "Kismet/GameplayStatics.h"

AFPSGameMode::AFPSGameMode() {
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnClassFinder(TEXT("/Game/Blueprints/BP_Player"));
	DefaultPawnClass = PlayerPawnClassFinder.Class;

	HUDClass = AFPSHUD::StaticClass();
}

void AFPSGameMode::CompleteMission(APawn* InstigatorPawn, bool MissionSuccess) {
	if(!InstigatorPawn) return;
	if(!SpectatingViewpointClass) {
		UE_LOG(LogTemp, Error, TEXT("SpectatingViewpointClass is nullptr!. Can not change view spectating view target."));
		return;
	}

	UE_LOG(LogTemp, Log, TEXT("Mission Completed!"));
	InstigatorPawn->DisableInput(nullptr);

	TArray<AActor*> SpectatingActors;
	UGameplayStatics::GetAllActorsOfClass(this, SpectatingViewpointClass, SpectatingActors);
	if(SpectatingActors.Num() < 0) return;

	AActor* NewViewTarget = SpectatingActors[0];	
	APlayerController* PlayerController = Cast<APlayerController>(
		InstigatorPawn->GetController()
	);
	if(!PlayerController) return;

	PlayerController->SetViewTargetWithBlend(
		NewViewTarget,
		0.5f,
		EViewTargetBlendFunction::VTBlend_Cubic
	);

	OnMissionCompleted(InstigatorPawn, MissionSuccess);
}
