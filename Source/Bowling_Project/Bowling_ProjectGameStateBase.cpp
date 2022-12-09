#include "Bowling_ProjectGameStateBase.h"

ABowling_ProjectGameStateBase::Bowling_ProjectGameStateBase()
{
	PinsHit = 0;
}

void ABowling_ProjectGameStateBase::PinsHitHandler()
{
	++PinsHit;
	UE_LOG(LogTemp, Warning, Text("Server: PinsHit: %d"), PinsHit);
}