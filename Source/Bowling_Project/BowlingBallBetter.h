// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/StaticMeshComponent.h"
#include "DrawDebugHelpers.h"
#include "Math/UnrealMathUtility.h"
#include "BowlingPin.h"

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
    
    UPROPERTY(EditAnywhere)
    UStaticMeshComponent* Mesh;
    
    UPROPERTY(EditAnywhere)
    UCameraComponent* OurCamera;
    
    UPROPERTY(EditAnywhere)
    USpringArmComponent* SpringArm;
    
    // Input functions
    void Move_YAxis(float AxisValue);
    void RollBall();
    void ChangeAngle();
    void ChangeSpeed();
    void ChangeMass();
    
    FVector CurrentVelocity;
    FVector CurrentLoc;
    FVector NewLoc;
    FVector CollisionLoc; 
    FVector PostCollisionLoc;
    FVector ThrowDistance;
    
    ABowlingPin* BP;
    
    int BallWeight;
    bool isHit, isMoving; 
};
