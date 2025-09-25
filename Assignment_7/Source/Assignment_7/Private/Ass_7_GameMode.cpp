// Fill out your copyright notice in the Description page of Project Settings.


#include "Ass_7_GameMode.h"
#include "NBC_Pawn.h"
#include "Ass_7_PlayerController.h"

AAss_7_GameMode::AAss_7_GameMode()
{
	DefaultPawnClass = ANBC_Pawn::StaticClass();
	PlayerControllerClass = AAss_7_PlayerController::StaticClass();
}