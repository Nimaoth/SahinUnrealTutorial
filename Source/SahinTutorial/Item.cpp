// Fill out your copyright notice in the Description page of Project Settings.


#include "Item.h"

#include "Components/StaticMeshComponent.h"
#include "SahinTutorialCharacter.h"

// Sets default values
AItem::AItem()
{
    Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));

    Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
    Mesh->SetGenerateOverlapEvents(true);
    Mesh->AttachTo(Root);

    SetRootComponent(Root);


    bIsEnabled = true;
}

// Called when the game starts or when spawned
void AItem::BeginPlay()
{
    Super::BeginPlay();
    
    Mesh->OnComponentBeginOverlap.AddDynamic(this, &AItem::TriggerEnter);
}

void AItem::TriggerEnter(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) {
    if (ASahinTutorialCharacter* Actor = Cast<ASahinTutorialCharacter>(OtherActor)) {
        SetEnabled(false);
        Actor->ChangeHP(HealAmount);

        FVector SpawnLocation = GetActorLocation();
        GetWorld()->SpawnActor(*ConsumeEffect, &SpawnLocation);
    }
}

void AItem::SetEnabled(bool bEnabled) {
    this->bIsEnabled = bEnabled;
    SetActorHiddenInGame(!bEnabled);
    SetActorEnableCollision(bEnabled);
    SetActorTickEnabled(bEnabled);
}

bool AItem::IsEnabled() const {
    return bIsEnabled;
}
