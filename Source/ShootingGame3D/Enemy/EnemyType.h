#pragma once

#include "CoreMinimal.h"
#include "EnemyType.generated.h"

UENUM(BlueprintType)
enum class EEnemyState : uint8
{
	None,
	Idle,
	Chasing,
	Attack,
	Die,
};



class EnemyType
{
public:
	
};
