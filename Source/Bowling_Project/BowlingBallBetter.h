// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "BowlingBallBetter.generated.h"

UCLASS()
class BOWLING_PROJECT_API ABowlingBallBetter : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	ABowlingBallBetter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
    
    UPROPERTY(EditAnywhere)
    USceneComponent* OurVisibleComponent;
    
    // Input functions
    void Move_XAxis(float AxisValue);
    void Move_YAxis(float AxisValue);
    
    // Input variables
    FVector CurrentVelocity;
};
