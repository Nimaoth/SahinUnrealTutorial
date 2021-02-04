// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "SahinTutorialGameMode.generated.h"

UCLASS(minimalapi)
class ASahinTutorialGameMode : public AGameModeBase
{
    GENERATED_BODY()


private:
    TArray<class AItem*> Items;

public:
    ASahinTutorialGameMode();

    virtual void BeginPlay() override;
    virtual void Tick(float DeltaTime) override;
};



