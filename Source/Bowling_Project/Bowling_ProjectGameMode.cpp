// Copyright Epic Games, Inc. All Rights Reserved.


#include "Bowling_ProjectGameMode.h"
#include "BowlingBallBetter.h"
#include "Bowling_ProjectGameStateBase.h"

void ABowling_ProjectGameMode::PinsHitHandler();
{
	if (ABowling_ProjectGameStateBase* GS = GetGameState<ABowling_ProjectGameStateBase>())
	{
		GS->PinsHitHandler();
	}

}

