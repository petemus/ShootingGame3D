// Copyright Epic Games, Inc. All Rights Reserved.

#include "ShootingGame3DGameMode.h"
#include "ShootingGame3DCharacter.h"
#include "UObject/ConstructorHelpers.h"

AShootingGame3DGameMode::AShootingGame3DGameMode()
	: Super()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnClassFinder(TEXT("/Game/FirstPerson/Blueprints/BP_FirstPersonCharacter"));
	DefaultPawnClass = PlayerPawnClassFinder.Class;

}
