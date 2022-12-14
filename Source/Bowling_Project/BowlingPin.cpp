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

    Capsule = CreateDefaultSubobject<UCapsuleComponent>(TEXT("CapsuleComponen"));
    Capsule->SetupAttachment(RootComponent);
    //Shape->MakeCapsule(0.5, 0.5);

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
    isMoving = true;
    CurrentLoc = GetActorLocation();
    FVector NewLocNormal = NewLoc - CurrentLoc;
    NewLocNormal.Normalize();


    //GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::Printf(TEXT("CurrentLoc: %s"), *CurrentLoc.ToString()));

    //GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("NewLoc: %s"), *NewLoc.ToString()));

    FCollisionQueryParams P = FComponentQueryParams();
    //FCollisionResponseParams R = FComponentQueryParams();
    P.AddIgnoredActor(this); //so we dont hit ourself  

    // create tarray for hit results
    TArray<FHitResult> OutHits;
    FCollisionShape S = FCollisionShape::MakeCapsule(50.0, 50.0);   

    isHit = GetWorld()->SweepMultiByChannel(OutHits, CurrentLoc, NewLoc, GetActorRotation().Quaternion(), ECC_WorldStatic, S, P);

    if (isHit) {
       
        for (auto& Hit : OutHits)
        {
            if (GEngine)
            {
                //GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue, FString::Printf(TEXT("Mission Success! YAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAY")));
                // screen log information on what was hit
               // GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::Printf(TEXT("PostCollision: %s"), *PostCollisionLoc.ToString()));
                // uncommnet to see more info on sweeped actor
                //GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("NewLoc: %s"), *NewLoc.ToString()));
            }
        }

        
       // NewLoc = OutHits[0].Location;
        
        //first pin we hit
        BP = NULL;
        int i = 0;
        while (BP == NULL) {
            BP = Cast<ABowlingPin>(OutHits[i].GetActor());
            i++;
        }
        i--;
        //BP->NewLoc = FVector(0, 0, 0);
        //BP->MovePin();
        
        // Gets direction the pin will move in
        FVector PinLocDirection = (OutHits[i].ImpactPoint - OutHits[i].Location);
        PinLocDirection.Normalize();


        float MaxAngle = 0.5 * PI;
        float Multiplier = acos(FVector::DotProduct(NewLocNormal, PinLocDirection));
        if (Multiplier > MaxAngle) {
            Multiplier = 2 * PI - Multiplier;
        }
        Multiplier /= MaxAngle;
        float PinLocMagnitude = (NewLoc - OutHits[i].Location).Size() * Multiplier;

        BP->NewLoc += PinLocDirection * PinLocMagnitude;
        PostCollisionLoc = NewLoc - (PinLocDirection * PinLocMagnitude);
        GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("NewLoc: %f"), (NewLoc - OutHits[i].Location).Size()));

        CollisionLoc = OutHits[i].Location;
        

        // DrawDebugLine(GetWorld(), OutHits[0].Location, OutHits[0].ImpactPoint, FColor::Red, false, 100, 0, 10);

    }
}

