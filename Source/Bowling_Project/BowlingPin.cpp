// Fill out your copyright notice in the Description page of Project Settings.


#include "BowlingPin.h"

// Sets default values
ABowlingPin::ABowlingPin()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
    
    // Create a dummy root component we can attach things to.
    RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));
    
    // Create a visible object
    Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMeshComponent"));
    Mesh->SetupAttachment(RootComponent);
    isHit = false;
}

// Called when the game starts or when spawned
void ABowlingPin::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ABowlingPin::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
    
    if (isHit) {
//        CurrentLoc = FMath::VInterpTo(CurrentLoc, NewLoc, DeltaTime, 0.1);
//        SetActorLocation(CurrentLoc);
        GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::Printf(TEXT("PIN HIT")));
    }
}

