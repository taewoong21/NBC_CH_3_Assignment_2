// Copyright Epic Games, Inc. All Rights Reserved.

#include "Assignment_7GameMode.h"
#include "Assignment_7Character.h"
#include "UObject/ConstructorHelpers.h"

AAssignment_7GameMode::AAssignment_7GameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPerson/Blueprints/BP_ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}
