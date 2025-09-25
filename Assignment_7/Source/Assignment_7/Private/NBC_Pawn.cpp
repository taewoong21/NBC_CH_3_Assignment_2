// Fill out your copyright notice in the Description page of Project Settings.
#include "NBC_Pawn.h"
#include "Components/CapsuleComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"

DEFINE_LOG_CATEGORY(LogForNBC);

// Sets default values
ANBC_Pawn::ANBC_Pawn()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// 캡슐 컴포넌트 생성
	CapsuleComponent = CreateDefaultSubobject<UCapsuleComponent>(TEXT("CapsuleComponent"));
	SetRootComponent(CapsuleComponent);
	CapsuleComponent->SetSimulatePhysics(false); // 물리 시뮬레이션 비활성화

	// 메쉬 컴포넌트 생성
	SkeletalMeshComponent = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("SkeletalMeshComponent"));
	SkeletalMeshComponent->SetupAttachment(RootComponent);
	SkeletalMeshComponent->SetSimulatePhysics(false); // 물리 시뮬레이션 비활성화

	// 스프링 암 컴포넌트 생성
	SpringArmComponent = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArmComponent"));
	SpringArmComponent->SetupAttachment(RootComponent);
	SpringArmComponent->TargetArmLength = 300.0f; // 캐릭터와 카메라 사이 기본값 설정
	SpringArmComponent->bUsePawnControlRotation = false; // 컨트롤러 회전에 따라 스프링 암 회전 설정

	// 카메라 컴포넌트 생성
	CameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComponent"));
	CameraComponent->SetupAttachment(SpringArmComponent, USpringArmComponent::SocketName); // 스프링 암의 끝 위치 -> (SocketName) 에 배치
	CameraComponent->bUsePawnControlRotation = false; // 이미 스프링 암에서 회전을 켜둠 ()
}

// Called when the game starts or when spawned
void ANBC_Pawn::BeginPlay()
{
	Super::BeginPlay();
	
	// Enhanced Input 서브시스템에 Mapping Context 추가
	if (APlayerController* PlayerController = Cast<APlayerController>(GetController()))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			// MappingContext가 할당되었는지 확인
			if (MappingContext)
			{
				Subsystem->AddMappingContext(MappingContext, 0);
			}
			else
			{
				// 할당되지 않았다면 에러 로그
				UE_LOG(LogForNBC, Error, TEXT("DefaultMappingContext is NOT assigned in the Blueprint!"));
			}
		}

		
	}
}

// Called every frame
void ANBC_Pawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// --- 회전 처리 ---
	// FRotator(Pitch, Yaw, Roll) 순서입니다.
	FRotator DeltaRotation = FRotator(CurrentRotationInput.Pitch, CurrentRotationInput.Yaw, CurrentRotationInput.Roll) * 100.0f * DeltaTime;
	AddActorLocalRotation(DeltaRotation, true);

	// --- 이동 처리 ---
	FVector DeltaMovement = FVector(CurrentMovementInput.X, CurrentMovementInput.Y, CurrentMovementInput.Z) * MoveSpeed * DeltaTime;
	AddActorLocalOffset(DeltaMovement, true);
}

// Called to bind functionality to input
void ANBC_Pawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	// PlayrInputComponent를 UEnhancedInputComponent로 캐스팅
	if (UEnhancedInputComponent* EnhancedInputComponent = 
		CastChecked<UEnhancedInputComponent>(PlayerInputComponent))
	{
		// MoveAction을 Move() 함수에 바인딩
		// MoveAction이 할당되었는지 확인
		if (MoveAction)
		{
			EnhancedInputComponent->BindAction(
				MoveAction, 
				ETriggerEvent::Triggered, 
				this, 
				&ANBC_Pawn::Move);

			UE_LOG(LogForNBC, Warning, TEXT("MoveAction is assigned in the Blueprint!"));
		}
		else
		{
			UE_LOG(LogForNBC, Error, TEXT("MoveAction is NOT assigned in the Blueprint!"));
		}

		// LookAction을 Look() 함수에 바인딩
		// LookAction이 할당되었는지 확인
		if (LookAction)
		{
			EnhancedInputComponent->BindAction(
				LookAction, 
				ETriggerEvent::Triggered, 
				this, 
				&ANBC_Pawn::Look);

			UE_LOG(LogForNBC, Warning, TEXT("LookAction is assigned in the Blueprint!"));
		}
		else
		{
			// 할당되지 않았다면 에러 로그를 띄웁니다.
			UE_LOG(LogForNBC, Error, TEXT("LookAction is NOT assigned in the Blueprint!"));
		}

		// LevitationAction을 Look() 함수에 바인딩
		// LevitationAction이 할당되었는지 확인
		if (LevitationAction)
		{
			EnhancedInputComponent->BindAction(
				LevitationAction,
				ETriggerEvent::Triggered,
				this,
				&ANBC_Pawn::Look);

			UE_LOG(LogForNBC, Warning, TEXT("LevitationAction is assigned in the Blueprint!"));
		}
		else
		{
			// 할당되지 않았다면 에러 로그를 띄웁니다.
			UE_LOG(LogForNBC, Error, TEXT("LevitationAction is NOT assigned in the Blueprint!"));
		}

		// RollAction을 Look() 함수에 바인딩
		// RollAction이 할당되었는지 확인
		if (LevitationAction)
		{
			EnhancedInputComponent->BindAction(
				RollAction,
				ETriggerEvent::Triggered,
				this,
				&ANBC_Pawn::Look);

			UE_LOG(LogForNBC, Warning, TEXT("RollAction is assigned in the Blueprint!"));
		}
		else
		{
			// 할당되지 않았다면 에러 로그를 띄웁니다.
			UE_LOG(LogForNBC, Error, TEXT("RollAction is NOT assigned in the Blueprint!"));
		}
	}
}

void ANBC_Pawn::Move(const FInputActionValue& Value)
{
	// 입력값을 FVector2D로 가져오기
	FVector2D MovementVector = Value.Get<FVector2D>();

	UE_LOG(LogForNBC, Warning, TEXT("Move Input: X=%f, Y=%f"), MovementVector.X, MovementVector.Y);

	if (Controller != nullptr)
	{
		// Pawn의 Forward / Right 벡터를 활용하여 이동 방향 결정
		const FRotator Rotation = GetActorRotation();
		const FVector ForwardDirection = 
			FRotationMatrix(Rotation).GetUnitAxis(EAxis::X);
		const FVector RightDirection = 
			FRotationMatrix(Rotation).GetUnitAxis(EAxis::Y);

		// DeltaSeconds(DeltaTime)을 사용해 프레임 독립적인 이동 구현
		FVector Offset = (ForwardDirection * MovementVector.Y + RightDirection * MovementVector.X) * MoveSpeed * GetWorld()->GetDeltaSeconds();

		// Pawn 로컬 좌표계 기준으로 이동
		AddActorLocalOffset(Offset, true);
	}
}

void ANBC_Pawn::Look(const FInputActionValue& Value)
{
	// 입력값을 FVector2D로 가져오기
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	UE_LOG(LogForNBC, Warning, TEXT("Look Input: X=%f, Y=%f"), LookAxisVector.X, LookAxisVector.Y);

	if (Controller != nullptr)
	{
		// Yaw(좌우) 회전은 Pawn 전체에 적용
		AddActorLocalRotation(FRotator(0.0f, LookAxisVector.X, 0.0f));

		// Pitch(상하) 회전은 스프링 암에만 적용하여 Pawn이 기울어지지 않게 함
		FRotator CurrentSpringArmRotation = SpringArmComponent->GetRelativeRotation();
		float NewPitch = 
			FMath::Clamp(CurrentSpringArmRotation.Pitch + (LookAxisVector.Y * -1.0f), -60.0f, 30.0f);
		SpringArmComponent->SetRelativeRotation(FRotator(NewPitch, 0.0f, 0.0f));
	}
}

void ANBC_Pawn::Levitation(const FInputActionValue& Value)
{

}

void ANBC_Pawn::Roll(const FInputActionValue& Value)
{

}

