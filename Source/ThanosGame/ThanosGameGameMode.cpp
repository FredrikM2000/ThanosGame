// Copyright Epic Games, Inc. All Rights Reserved.

#include "ThanosGameGameMode.h"
#include "ThanosGameCharacter.h"
#include "UObject/ConstructorHelpers.h"

AThanosGameGameMode::AThanosGameGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPersonCPP/Blueprints/ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}
