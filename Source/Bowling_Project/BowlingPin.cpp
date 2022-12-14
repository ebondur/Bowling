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
    isHit = false, isMoving = false;
}

// Called when the game starts or when spawned
void ABowlingPin::BeginPlay()
{
	Super::BeginPlay();

    CurrentLoc = GetActorLocation();
    NewLoc = GetActorLocation();
    CollisionLoc = FVector(0);
    PostCollisionLoc = FVector(0);
	
}

// Called every frame
void ABowlingPin::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
    DrawDebugLine(GetWorld(), CurrentLoc, NewLoc, FColor::Blue, false, -1, 0, 10);
    
    if (isMoving) {
        CurrentLoc = FMath::VInterpTo(CurrentLoc, NewLoc, DeltaTime, 0.5f);
        SetActorLocation(CurrentLoc);
        //GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::Printf(TEXT("PIN HIT")));
    }


    if (CurrentLoc.Equals(CollisionLoc, 5.0f)) {
        NewLoc = PostCollisionLoc;
        CollisionLoc = FVector(0.0f);
        BP->MovePin();
        MovePin();
    }
}

// Called after a bowling pin is hit
void ABowlingPin::MovePin()
{
    CurrentLoc = GetActorLocation();
    FVector NewLocNormal = NewLoc - CurrentLoc;
    NewLocNormal.Normalize();


    GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::Printf(TEXT("CurrentLoc: %s"), *CurrentLoc.ToString()));

    GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("NewLoc: %s"), *NewLoc.ToString()));

    FComponentQueryParams P = FComponentQueryParams();
    // P.AddIgnoredActor(this); //so we dont hit ourself

    // create tarray for hit results
    TArray<FHitResult> OutHits;
    isHit = GetWorld()->ComponentSweepMulti(OutHits, Mesh, CurrentLoc, CurrentLoc + FVector(1000, 0, 0), GetActorRotation(), P);

    if (isHit) {
        GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue, FString::Printf(TEXT("Mission Success! YAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAY")));
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

        // Gets direction the pin will move in
        FVector PinLocDirection = (OutHits[0].ImpactPoint - OutHits[0].Location);
        PinLocDirection.Normalize();


        float MaxAngle = 0.5 * PI;
        float AngleMultiplier = modf(acos(FVector::DotProduct(NewLocNormal, PinLocDirection) / (NewLocNormal.Size() * PinLocDirection.Size())), &MaxAngle);
        float PinLocMagnitude = (NewLoc - GetActorLocation()).Size() * AngleMultiplier;

        BP->NewLoc += PinLocDirection * PinLocMagnitude;
        PostCollisionLoc = NewLoc - (PinLocDirection * PinLocMagnitude);

        CollisionLoc = OutHits[0].Location;

        // DrawDebugLine(GetWorld(), OutHits[0].Location, OutHits[0].ImpactPoint, FColor::Red, false, 100, 0, 10);

    }
    isMoving = true;
}

