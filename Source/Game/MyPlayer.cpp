// Fill out your copyright notice in the Description page of Project Settings.

#include "MyPlayer.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"

AMyPlayer::AMyPlayer()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 540.f, 0.0f);

	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->AttachTo(RootComponent);	// Root�� CameraBoom �߰�
	CameraBoom->TargetArmLength = 500.f;	// ĳ���Ϳ��� �Ÿ� ����
	CameraBoom->bUsePawnControlRotation = true;

	FlowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FlowCamera->AttachTo(CameraBoom, USpringArmComponent::SocketName);	// CameraBoom�� Camera �߰�
	FlowCamera->bUsePawnControlRotation = true;
}

// Called every frame
void AMyPlayer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AMyPlayer::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis("MoveForward", this, &AMyPlayer::MoveForward);
	PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);
	PlayerInputComponent->BindAction("BaseAttack", IE_Pressed, this, &AMyPlayer::StartAttack);
}

void AMyPlayer::MoveForward(float Value)
{
	if ((Controller != NULL) && (Value != 0.0f))
	{
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		AddMovementInput(Direction, Value);
	}
}

// ���ݾִϸ��̼� ���
void AMyPlayer::StartAttack()
{
	if (IsAttacking)
		return;

	float AnimDuration = PlayAnimMontage(AttackAnim);
	IsAttacking = true;

	// �ִϸ��̼��� ������ StopAttack ȣ��
	FTimerHandle TimerHandle_StopAttack;
	GetWorldTimerManager().SetTimer(TimerHandle_StopAttack, this, &AMyPlayer::StopAttack, AnimDuration, false);
}

// ���ݾִϸ��̼� ����
void AMyPlayer::StopAttack()
{
	StopAnimMontage(AttackAnim);
	IsAttacking = false;
}