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
    
    Speed = 1000;
    Angle = 0;
    PinMass = 10;
    Mass = 10; //represents different weights/types of bowling balls, need to implement later
    isHit = false, isMoving = false;
}

// Called when the game starts or when spawned
void ABowlingBallBetter::BeginPlay()
{

    Super::BeginPlay();
    CurrentLoc = GetActorLocation();
    NewLoc = GetActorLocation();
    CollisionLoc = FVector(0);
    PostCollisionLoc = FVector(0);
}

// Called every frame
void ABowlingBallBetter::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
    
    DrawDebugLine(GetWorld(), GetActorLocation(), GetActorLocation() + FVector(Speed, Angle, 0), FColor::Blue, false, -1, 0, 10);

    // Handle side to side movement based on our "MoveY" axes
    if (!CurrentVelocity.IsZero())
    {
        SetActorLocation(GetActorLocation() + (CurrentVelocity * DeltaTime));
    }
    
    if (CurrentLoc.Equals(CollisionLoc, 5.0f)) {
        CollisionLoc = FVector(0.0f);
        BP->MovePin();
        RollBall();
        NewLoc = PostCollisionLoc;
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
    
    // Respond every frame to the values of our two movement axes, "Move_YAxis"
    InputComponent->BindAxis("Move_YAxis", this, &ABowlingBallBetter::Move_YAxis);

    InputComponent->BindAction("RollBall", IE_Pressed, this, &ABowlingBallBetter::RollBall);

    InputComponent->BindAction("AngleRight", IE_Pressed, this, &ABowlingBallBetter::AngleRight);
    InputComponent->BindAction("AngleLeft", IE_Pressed, this, &ABowlingBallBetter::AngleLeft);

    InputComponent->BindAction("SpeedUp", IE_Pressed, this, &ABowlingBallBetter::SpeedUp);
    InputComponent->BindAction("SpeedDown", IE_Pressed, this, &ABowlingBallBetter::SpeedDown);

    InputComponent->BindAction("MassUp", IE_Pressed, this, &ABowlingBallBetter::MassUp);
    InputComponent->BindAction("MassDown", IE_Pressed, this, &ABowlingBallBetter::MassDown);

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
    NewLoc = CurrentLoc + FVector(Speed, Angle, 0);
    FVector NewLocNormal = NewLoc - CurrentLoc;
    NewLocNormal.Normalize();
    
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
        BP = NULL;
        int i = 0;
        while (BP == NULL) {
            BP = Cast<ABowlingPin>(OutHits[i].GetActor());
            i++;
        }
        i--;

        // Gets direction the pin will move in
        FVector PinLocDirection = (OutHits[i].ImpactPoint - OutHits[i].Location);
        PinLocDirection.Normalize();

        // Calculates a modifier based on the angle of contact and the mass of the objects
        float MaxAngle = 0.5 * PI;
        float MassRatio = Mass / PinMass;
        float Multiplier = acos(FVector::DotProduct(NewLocNormal, PinLocDirection));
        if (Multiplier > MaxAngle) {
            Multiplier = 2 * PI - Multiplier;
        }
        Multiplier /= MaxAngle;
        if (MassRatio < 1.0) {
            Multiplier *= MassRatio;
        }
        float PinLocMagnitude = (NewLoc - OutHits[i].Location).Size() * Multiplier;

        BP->NewLoc += PinLocDirection * PinLocMagnitude;
        PostCollisionLoc = NewLoc - (PinLocDirection * PinLocMagnitude * (1 / MassRatio));

        CollisionLoc = OutHits[i].Location;
        
        DrawDebugLine(GetWorld(), OutHits[i].Location, OutHits[i].ImpactPoint, FColor::Red, false, 100, 0, 10);
        
    }
}

void ABowlingBallBetter::AngleRight() {
    Angle += 10.0;
}

void ABowlingBallBetter::AngleLeft() {
    Angle -= 10.0;
}

void ABowlingBallBetter::SpeedUp() {
    Speed += 100.0;
}

void ABowlingBallBetter::SpeedDown() {
    Speed -= 100.0;
}

void ABowlingBallBetter::MassUp() {
    Mass += 1.0;
}

void ABowlingBallBetter::MassDown() {
    Mass -= 1.0;
}