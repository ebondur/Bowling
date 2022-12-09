// Fill out your copyright notice in the Description page of Project Settings.


#include "BowlingBallBetter.h"
#include "Camera/CameraComponent.h"
#include "Components/StaticMeshComponent.h"

// Sets default values
ABowlingBallBetter::ABowlingBallBetter()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
    
    AutoPossessPlayer = EAutoReceiveInput::Player0;
    
    UPROPERTY(EditAnywhere)
    float radius = 5.0f;
    
    // Create a dummy root component we can attach things to.
    OurVisibleComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));
    // Create a camera and a visible object
    UCameraComponent* OurCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("OurCamera"));
    OurVisibleComponent= CreateDefaultSubobject<UStaticMeshComponent>(TEXT("OurVisibleComponent"));
    // Attach our camera and visible object to our root component. Offset and rotate the camera.
    OurCamera->SetupAttachment(RootComponent);
    OurCamera->SetRelativeLocation(FVector(-250.0f, 0.0f, 250.0f));
    OurCamera->SetRelativeRotation(FRotator(-45.0f, 0.0f, 0.0f));
    OurVisibleComponent->SetupAttachment(RootComponent);
    
    

}

// Called when the game starts or when spawned
void ABowlingBallBetter::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ABowlingBallBetter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

    // Handle movement based on our "MoveX" and "MoveY" axes
    if (!CurrentVelocity.IsZero())
    {
        FVector NewLocation = GetActorLocation() + (CurrentVelocity * DeltaTime);
        SetActorLocation(NewLocation); //will move to FinterpV later
    }

}

// Called to bind functionality to input
void ABowlingBallBetter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
    
    // Respond every frame to the values of our two movement axes, "MoveX" and "MoveY".
   InputComponent->BindAxis("MoveX", this, &ABowlingBallBetter::Move_XAxis);
   InputComponent->BindAxis("MoveY", this, &ABowlingBallBetter::Move_YAxis);
}

void ABowlingBallBetter::Move_XAxis(float AxisValue)
{
    // Move at 100 units per second forward or backward
    CurrentVelocity.X = FMath::Clamp(AxisValue, -1.0f, 1.0f) * 100.0f;
}

void ABowlingBallBetter::Move_YAxis(float AxisValue)
{
    // Move at 100 units per second right or left
    CurrentVelocity.Y = FMath::Clamp(AxisValue, -1.0f, 1.0f) * 100.0f;
}

