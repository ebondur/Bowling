// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/StaticMeshComponent.h"
#include "Components/CapsuleComponent.h"
#include "Engine/EngineTypes.h"
#include "BowlingPin.generated.h"

UCLASS()
class BOWLING_PROJECT_API ABowlingPin : public AActor
{
    GENERATED_BODY()
public:	
	// Sets default values for this actor's properties
	ABowlingPin();
    
    UPROPERTY(EditAnywhere)
    USceneComponent* OurVisibleComponent;
    
    UPROPERTY(EditAnywhere)
    UStaticMeshComponent* Mesh;

    UPROPERTY(EditAnywhere)
    UCapsuleComponent* Capsule;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

    void MovePin();
    
    bool isHit;
    bool isMoving;
    
    FVector CurrentLoc;
    FVector NewLoc;
    FVector CollisionLoc;
    FVector PostCollisionLoc;

    ABowlingPin* BP;

};
