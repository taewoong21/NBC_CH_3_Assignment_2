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

	// ĸ�� ������Ʈ ����
	CapsuleComponent = CreateDefaultSubobject<UCapsuleComponent>(TEXT("CapsuleComponent"));
	SetRootComponent(CapsuleComponent);
	CapsuleComponent->SetSimulatePhysics(false); // ���� �ùķ��̼� ��Ȱ��ȭ

	// �޽� ������Ʈ ����
	SkeletalMeshComponent = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("SkeletalMeshComponent"));
	SkeletalMeshComponent->SetupAttachment(RootComponent);
	SkeletalMeshComponent->SetSimulatePhysics(false); // ���� �ùķ��̼� ��Ȱ��ȭ

	// ������ �� ������Ʈ ����
	SpringArmComponent = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArmComponent"));
	SpringArmComponent->SetupAttachment(RootComponent);
	SpringArmComponent->TargetArmLength = 300.0f; // ĳ���Ϳ� ī�޶� ���� �⺻�� ����
	SpringArmComponent->bUsePawnControlRotation = false; // ��Ʈ�ѷ� ȸ���� ���� ������ �� ȸ�� ����

	// ī�޶� ������Ʈ ����
	CameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComponent"));
	CameraComponent->SetupAttachment(SpringArmComponent, USpringArmComponent::SocketName); // ������ ���� �� ��ġ -> (SocketName) �� ��ġ
	CameraComponent->bUsePawnControlRotation = false; // �̹� ������ �Ͽ��� ȸ���� �ѵ� ()
}

// Called when the game starts or when spawned
void ANBC_Pawn::BeginPlay()
{
	Super::BeginPlay();
	
	// Enhanced Input ����ý��ۿ� Mapping Context �߰�
	if (APlayerController* PlayerController = Cast<APlayerController>(GetController()))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			// MappingContext�� �Ҵ�Ǿ����� Ȯ��
			if (MappingContext)
			{
				Subsystem->AddMappingContext(MappingContext, 0);
			}
			else
			{
				// �Ҵ���� �ʾҴٸ� ���� �α�
				UE_LOG(LogForNBC, Error, TEXT("DefaultMappingContext is NOT assigned in the Blueprint!"));
			}
		}

		
	}
}

// Called every frame
void ANBC_Pawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// --- ȸ�� ó�� ---
	// FRotator(Pitch, Yaw, Roll) �����Դϴ�.
	FRotator DeltaRotation = FRotator(CurrentRotationInput.Pitch, CurrentRotationInput.Yaw, CurrentRotationInput.Roll) * 100.0f * DeltaTime;
	AddActorLocalRotation(DeltaRotation, true);

	// --- �̵� ó�� ---
	FVector DeltaMovement = FVector(CurrentMovementInput.X, CurrentMovementInput.Y, CurrentMovementInput.Z) * MoveSpeed * DeltaTime;
	AddActorLocalOffset(DeltaMovement, true);
}

// Called to bind functionality to input
void ANBC_Pawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	// PlayrInputComponent�� UEnhancedInputComponent�� ĳ����
	if (UEnhancedInputComponent* EnhancedInputComponent = 
		CastChecked<UEnhancedInputComponent>(PlayerInputComponent))
	{
		// MoveAction�� Move() �Լ��� ���ε�
		// MoveAction�� �Ҵ�Ǿ����� Ȯ��
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

		// LookAction�� Look() �Լ��� ���ε�
		// LookAction�� �Ҵ�Ǿ����� Ȯ��
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
			// �Ҵ���� �ʾҴٸ� ���� �α׸� ���ϴ�.
			UE_LOG(LogForNBC, Error, TEXT("LookAction is NOT assigned in the Blueprint!"));
		}

		// LevitationAction�� Look() �Լ��� ���ε�
		// LevitationAction�� �Ҵ�Ǿ����� Ȯ��
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
			// �Ҵ���� �ʾҴٸ� ���� �α׸� ���ϴ�.
			UE_LOG(LogForNBC, Error, TEXT("LevitationAction is NOT assigned in the Blueprint!"));
		}

		// RollAction�� Look() �Լ��� ���ε�
		// RollAction�� �Ҵ�Ǿ����� Ȯ��
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
			// �Ҵ���� �ʾҴٸ� ���� �α׸� ���ϴ�.
			UE_LOG(LogForNBC, Error, TEXT("RollAction is NOT assigned in the Blueprint!"));
		}
	}
}

void ANBC_Pawn::Move(const FInputActionValue& Value)
{
	// �Է°��� FVector2D�� ��������
	FVector2D MovementVector = Value.Get<FVector2D>();

	UE_LOG(LogForNBC, Warning, TEXT("Move Input: X=%f, Y=%f"), MovementVector.X, MovementVector.Y);

	if (Controller != nullptr)
	{
		// Pawn�� Forward / Right ���͸� Ȱ���Ͽ� �̵� ���� ����
		const FRotator Rotation = GetActorRotation();
		const FVector ForwardDirection = 
			FRotationMatrix(Rotation).GetUnitAxis(EAxis::X);
		const FVector RightDirection = 
			FRotationMatrix(Rotation).GetUnitAxis(EAxis::Y);

		// DeltaSeconds(DeltaTime)�� ����� ������ �������� �̵� ����
		FVector Offset = (ForwardDirection * MovementVector.Y + RightDirection * MovementVector.X) * MoveSpeed * GetWorld()->GetDeltaSeconds();

		// Pawn ���� ��ǥ�� �������� �̵�
		AddActorLocalOffset(Offset, true);
	}
}

void ANBC_Pawn::Look(const FInputActionValue& Value)
{
	// �Է°��� FVector2D�� ��������
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	UE_LOG(LogForNBC, Warning, TEXT("Look Input: X=%f, Y=%f"), LookAxisVector.X, LookAxisVector.Y);

	if (Controller != nullptr)
	{
		// Yaw(�¿�) ȸ���� Pawn ��ü�� ����
		AddActorLocalRotation(FRotator(0.0f, LookAxisVector.X, 0.0f));

		// Pitch(����) ȸ���� ������ �Ͽ��� �����Ͽ� Pawn�� �������� �ʰ� ��
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

