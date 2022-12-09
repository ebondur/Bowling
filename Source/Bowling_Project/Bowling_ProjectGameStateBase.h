#include "Bowling_ProjectGameStateBase.generated.h"

UCLASS()

class ABowling_ProjectGameStateBase : public AGameStateBase
{
	GENERATED_BODY()

public:
	ABowling_ProjectGameStateBase();

	uint16 PinsHit;

	PinsHitHandler();

	 
};