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
    
    // Attach our camera and visible object to our spring arm component.
    OurCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("OurCamera"));
    OurCamera->SetupAttachment(SpringArm);
    
    BallWeight = 10; //represents different weights/types of bowling balls, need to implement later
    isHit = false, isMoving = false;
}

// Called when the game starts or when spawned
void ABowlingBallBetter::BeginPlay()
{
    Super::BeginPlay();
    CurrentLoc = GetActorLocation(), NewLoc = GetActorLocation();
    CollisionLoc = FVector(0);
}

// Called every frame
void ABowlingBallBetter::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
    
    DrawDebugLine(GetWorld(), GetActorLocation(), GetActorLocation() + FVector(1000,0,0), FColor::Blue, false, -1, 0, 10);

    // Handle side to side movement based on our "MoveY" axes
    if (!CurrentVelocity.IsZero())
    {
        SetActorLocation(GetActorLocation() + (CurrentVelocity * DeltaTime));
    }
    
    if (CurrentLoc.Equals(CollisionLoc, 5.0f)) {
        NewLoc = CollisionLoc;
        CollisionLoc = FVector(0.0f);
        isMoving = false;
        BP->isHit = true;
    }
    
    if (CurrentLoc != NewLoc && isMoving) { //we have rolled the ball
        CurrentLoc = FMath::VInterpTo(CurrentLoc, NewLoc, DeltaTime, 0.5f);
        SetActorLocation(CurrentLoc);
    }
}

// Called to bind functionality to input
void ABowlingBallBetter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
    Super::SetupPlayerInputComponent(PlayerInputComponent);
    
    // Respond every frame to the values of our two movement axes, "Move_ZAxis"
    InputComponent->BindAxis("Move_YAxis", this, &ABowlingBallBetter::Move_YAxis);
    InputComponent->BindAction("RollBall", IE_Pressed, this, &ABowlingBallBetter::RollBall);
}

void ABowlingBallBetter::Move_YAxis(float AxisValue)
{
    UE_LOG(LogTemp, Warning, TEXT("Calling move Y axis function"));
    
    // Move at 100 units per second forward or backward
    CurrentVelocity.Y = FMath::Clamp(AxisValue, -1.0f, 1.0f) * 100.0f;
}

void ABowlingBallBetter::RollBall() {
    isMoving = true;
    CurrentLoc = GetActorLocation();
    NewLoc = GetActorLocation() + GetActorRightVector() + FVector(1000, 0, 0); //just for testing
    
    FComponentQueryParams P = FComponentQueryParams();
    P.AddIgnoredActor(this); //so we dont hit ourself
    
    // create tarray for hit results
    TArray<FHitResult> OutHits;
    isHit = GetWorld()->ComponentSweepMulti(OutHits, Mesh, CurrentLoc, NewLoc, GetActorRotation(), P);
    
    if (isHit) {
        for (auto& Hit : OutHits)
        {
            if (GEngine)
            {
                // screen log information on what was hit
                GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::Printf(TEXT("Hit Result: %s"), *Hit.GetActor()->GetName()));
                // uncommnet to see more info on sweeped actor
                 GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("All Hit Information: %s"), *Hit.ToString()));
            }
        }
        //first pin we hit
        BP = Cast<ABowlingPin>(OutHits[0].GetActor());
        BP->NewLoc += OutHits[0].Normal * 100;

        CollisionLoc = OutHits[0].Location;
        
        DrawDebugLine(GetWorld(), OutHits[0].Location, (OutHits[0].Normal * 100), FColor::Red, false, 100, 0, 10);
        
    }
}
