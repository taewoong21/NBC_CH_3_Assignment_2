// Fill out your copyright notice in the Description page of Project Settings.


#include "Ass_7_PlayerController.h"
#include "EnhancedInputSubsystems.h" 
// Enhanced Input System�� Local Player Subsystem�� ����ϱ� ���� ����

AAss_7_PlayerController::AAss_7_PlayerController()
//:	InputMappingContext(nullptr),
//	MoveAction(nullptr),
//	LookAction(nullptr)
{
}

void AAss_7_PlayerController::BeginPlay()
{
	Super::BeginPlay();

	// ���� PlayerController�� ����� Local Player ��ü�� ������    
	//if (ULocalPlayer* LocalPlayer = GetLocalPlayer())
	//{
	//	// Local Player���� EnhancedInputLocalPlayerSubsystem�� ȹ��
	//	if (UEnhancedInputLocalPlayerSubsystem* Subsystem = 
	//		LocalPlayer->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>())
	//	{
	//		if (InputMappingContext)
	//		{
	//			// Subsystem�� ���� �츮�� �Ҵ��� IMC�� Ȱ��ȭ
	//			// �켱����(Priority)�� 0�� ���� ���� �켱����
	//			Subsystem->AddMappingContext(InputMappingContext, 0);
	//		}
	//	}
	//}
}