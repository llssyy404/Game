// Fill out your copyright notice in the Description page of Project Settings.

#include "CharacterBase.h"

#include "GameFramework/CharacterMovementComponent.h"
#include "Components/CapsuleComponent.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
ACharacterBase::ACharacterBase()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ACharacterBase::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ACharacterBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ACharacterBase::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

float ACharacterBase::TakeDamage(float Damage, struct FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	if (HP <= 0.f)
	{
		return 0.f;
	}

	const float ActualDamage = Super::TakeDamage(Damage, DamageEvent, EventInstigator, DamageCauser);
	if (ActualDamage > 0.f)
	{
		HP -= ActualDamage;
	}

	if (HP <= 0.f)	// ü���� 0���Ͻ� ����
	{
		Die(ActualDamage, DamageEvent, EventInstigator, DamageCauser);
	}
	else				// �������Դ� �ִϸ��̼� ȣ���
	{
		OnHit(ActualDamage, DamageEvent, EventInstigator ? EventInstigator->GetPawn() : NULL, DamageCauser);
	}

	return ActualDamage;
}

float ACharacterBase::GetHealthPercent() const
{
	return HP / (float)100.f;
}

void ACharacterBase::OnHit(float DamageTaken, struct FDamageEvent const& DamageEvent, class APawn* PawnInstigator, class AActor* DamageCauser)
{
	float DemagedAnimDuration = PlayAnimMontage(DamageAnim);

	if (DamageTaken > 0.f)
	{
		ApplyDamageMomentum(DamageTaken, DamageEvent, PawnInstigator, DamageCauser);
	}
}

void ACharacterBase::Die(float KillingDamage, struct FDamageEvent const& DamageEvent, AController* Killer, class AActor* DamageCauser)
{
	HP = FMath::Min(0.f, HP);

	GetWorldTimerManager().ClearAllTimersForObject(this);

	if (GetCapsuleComponent())	// ĸ���浹ü �����
	{
		GetCapsuleComponent()->BodyInstance.SetCollisionEnabled(ECollisionEnabled::NoCollision);	// �浹ü ��Ȱ��ȭ
		GetCapsuleComponent()->BodyInstance.SetResponseToChannel(ECC_Pawn, ECR_Ignore);				// Pawnä�� ����
		GetCapsuleComponent()->BodyInstance.SetResponseToChannel(ECC_PhysicsBody, ECR_Ignore);		// PhysicsBodyä�� ����
	}

	if (GetCharacterMovement())	// ĳ���� �����Ʈ �����
	{
		GetCharacterMovement()->StopMovementImmediately();	// ��� �̵�����(zeroes velocity, usually zeros acceleration for components with acceleration)
		GetCharacterMovement()->DisableMovement();			// �̵� �Ұ����ϰ� ������
	}

	if (Controller != NULL)
	{
		Controller->UnPossess(); // Called when our Controller no longer possesses us.
	}

	float DeathAnimDuration = PlayAnimMontage(DeathAnim);	// �״� �ִϸ��̼� ���

															// �ִϸ��̼� ����� ������ OnDieAnimationEnd ȣ���
	FTimerHandle TimerHandle;
	GetWorldTimerManager().SetTimer(TimerHandle, this, &ACharacterBase::OnDieAnimationEnd, DeathAnimDuration, false);
}

void ACharacterBase::NotifyActorBeginOverlap(AActor* OtherActor)
{
	Super::NotifyActorBeginOverlap(OtherActor);

	// && �������� üũ && �����߿��� -> ����������
	if (OtherActor->IsA(AActor::StaticClass()) && OtherActor != this && IsAttacking == true && OtherActor->ActorHasTag("Enemy") != ActorHasTag("Enemy"))
	{
		// ������ ���Ϳ� �Ϲ����� ���ظ� ��
		UGameplayStatics::ApplyDamage(OtherActor, Damage, NULL, this, UDamageType::StaticClass());
	}
}

void ACharacterBase::OnDieAnimationEnd()
{
	StopAnimMontage(DeathAnim);
	//this->SetActorHiddenInGame(true);	// ���Ӿȿ��� ���͸� ����
	Destroy();
	//SetLifeSpan(0.1f);
}

// ���ݾִϸ��̼� ���
void ACharacterBase::StartAttack()
{
	if (IsAttacking)
		return;

	float AnimDuration = PlayAnimMontage(AttackAnim);
	IsAttacking = true;

	// �ִϸ��̼��� ������ StopAttack ȣ��
	FTimerHandle TimerHandle_StopAttack;
	GetWorldTimerManager().SetTimer(TimerHandle_StopAttack, this, &ACharacterBase::StopAttack, AnimDuration, false);
}

// ���ݾִϸ��̼� ����
void ACharacterBase::StopAttack()
{
	StopAnimMontage(AttackAnim);
	IsAttacking = false;
}
