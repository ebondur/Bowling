// Fill out your copyright notice in the Description page of Project Settings.


#include "BowlingBallBetter.h"

// Sets default values
ABowlingBallBetter::ABowlingBallBetter()
{
     // Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
    PrimaryActorTick.bCanEverTick = true;
    
    AutoPossessPlayer = EAutoReceiveInput::Player0;
        
    // Create a dummy root component we can attach things to.
    RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));
    
    // Create a visible object
    Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMeshComponent"));
    Mesh->SetupAttachment(RootComponent);
    
    SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("Spring Arm"));
    SpringArm->SetupAttachment(RootComponent);
    SpringArm->SetRelativeRotation(FRotator(0.0, -30.0, 0.0));
    
    // Attach our camera and visible object to our spring arm component.
    OurCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("OurCamera"));
    OurCamera->SetupAttachment(SpringArm);
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
        SetActorLocation(NewLocation);
    }

}

// Called to bind functionality to input
void ABowlingBallBetter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
    Super::SetupPlayerInputComponent(PlayerInputComponent);
    
    // Respond every frame to the values of our two movement axes, "Move_ZAxis"
   InputComponent->BindAxis("Move_YAxis", this, &ABowlingBallBetter::Move_YAxis);
}

void ABowlingBallBetter::Move_YAxis(float AxisValue)
{
    UE_LOG(LogTemp, Warning, TEXT("Calling move Y axis function"));
    
    // Move at 100 units per second forward or backward
    CurrentVelocity.Y = FMath::Clamp(AxisValue, -1.0f, 1.0f) * 100.0f;
}

