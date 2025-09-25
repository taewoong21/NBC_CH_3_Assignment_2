// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "Ass_7_PlayerController.generated.h"

// IMC & IA ���� ���� ����
//class UInputMappingContext; 
//class UInputAction; 

UCLASS()
class ASSIGNMENT_7_API AAss_7_PlayerController : public APlayerController
{
	GENERATED_BODY()
	
public:
	AAss_7_PlayerController();

	//// �����Ϳ��� ������ IMC
	//UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
	//UInputMappingContext* InputMappingContext;
	//// IA_Move�� ������ ����
	//UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
	//UInputAction* MoveAction;
	//// IA_Look�� ������ ����
	//UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
	//UInputAction* LookAction;

	virtual void BeginPlay() override;
};
