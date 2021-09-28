// Copyright Epic Games, Inc. All Rights Reserved.

#include "HoloforiaCplusGameMode.h"
#include "HoloforiaCplusCharacter.h"
#include "UObject/ConstructorHelpers.h"

AHoloforiaCplusGameMode::AHoloforiaCplusGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPersonCPP/Blueprints/ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}
