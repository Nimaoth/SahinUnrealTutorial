// Copyright Epic Games, Inc. All Rights Reserved.

#include "SahinTutorialGameMode.h"
#include "SahinTutorialCharacter.h"
#include "UObject/ConstructorHelpers.h"

#include "Kismet/GameplayStatics.h"
#include "Item.h"

ASahinTutorialGameMode::ASahinTutorialGameMode()
{
    PrimaryActorTick.bCanEverTick = true;

    // set default pawn class to our Blueprinted character
    static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPersonCPP/Blueprints/ThirdPersonCharacter"));
    if (PlayerPawnBPClass.Class != NULL)
    {
        DefaultPawnClass = PlayerPawnBPClass.Class;
    }
}

void ASahinTutorialGameMode::BeginPlay() {
    Super::BeginPlay();
    TArray<AActor*> ItemActors;
    UGameplayStatics::GetAllActorsOfClass(GetWorld(), AItem::StaticClass(), ItemActors);

    for (AActor* Actor : ItemActors) {
        Items.Add(Cast<AItem>(Actor));
    }

    UE_LOG(LogTemp, Warning, TEXT("%d items"), Items.Num());
}
void ASahinTutorialGameMode::Tick(float DeltaTime) {
    Super::Tick(DeltaTime);

    bool anyEnabled = false;
    for (AItem* Item : Items) {
        if (Item->IsEnabled()) {
            anyEnabled = true;
            break;
        }
    }
    //UE_LOG(LogTemp, Warning, TEXT("any enabled: %b"), anyEnabled);

    if (!anyEnabled) {
        for (AItem* Item : Items) {
            Item->SetEnabled(true);
        }
    }
}
