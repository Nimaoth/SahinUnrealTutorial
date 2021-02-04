// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Item.generated.h"


UCLASS()
class SAHINTUTORIAL_API AItem : public AActor
{
    GENERATED_BODY()
    

private:

    UPROPERTY(EditAnywhere)
    class USceneComponent* Root;

    UPROPERTY(EditAnywhere)
    class UStaticMeshComponent* Mesh;

    UPROPERTY(EditAnywhere, Category = "Item")
    int HealAmount;
    
    UPROPERTY(EditAnywhere)
    TSubclassOf<AActor> ConsumeEffect;

    bool bIsEnabled;

public:
    // Sets default values for this actor's properties
    AItem();

protected:
    // Called when the game starts or when spawned
    virtual void BeginPlay() override;

public:	
    UFUNCTION()
    void TriggerEnter(
        UPrimitiveComponent* OverlappedComponent,
        AActor* OtherActor,
        UPrimitiveComponent* OtherComp,
        int32 OtherBodyIndex,
        bool bFromSweep,
        const FHitResult& SweepResult);

    void SetEnabled(bool bEnabled);
    bool IsEnabled() const;
};
