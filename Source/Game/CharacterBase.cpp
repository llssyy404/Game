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

	if (HP <= 0.f)	// 체력이 0이하시 죽음
	{
		Die(ActualDamage, DamageEvent, EventInstigator, DamageCauser);
	}
	else				// 데미지입는 애니메이션 호출됨
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

	if (GetCapsuleComponent())	// 캡슐충돌체 존재시
	{
		GetCapsuleComponent()->BodyInstance.SetCollisionEnabled(ECollisionEnabled::NoCollision);	// 충돌체 비활성화
		GetCapsuleComponent()->BodyInstance.SetResponseToChannel(ECC_Pawn, ECR_Ignore);				// Pawn채널 무시
		GetCapsuleComponent()->BodyInstance.SetResponseToChannel(ECC_PhysicsBody, ECR_Ignore);		// PhysicsBody채널 무시
	}

	if (GetCharacterMovement())	// 캐릭터 무브먼트 존재시
	{
		GetCharacterMovement()->StopMovementImmediately();	// 즉시 이동중지(zeroes velocity, usually zeros acceleration for components with acceleration)
		GetCharacterMovement()->DisableMovement();			// 이동 불가능하게 설정됨
	}

	if (Controller != NULL)
	{
		Controller->UnPossess(); // Called when our Controller no longer possesses us.
	}

	float DeathAnimDuration = PlayAnimMontage(DeathAnim);	// 죽는 애니메이션 재생

															// 애니메이션 재생이 끝나면 OnDieAnimationEnd 호출됨
	FTimerHandle TimerHandle;
	GetWorldTimerManager().SetTimer(TimerHandle, this, &ACharacterBase::OnDieAnimationEnd, DeathAnimDuration, false);
}

void ACharacterBase::NotifyActorBeginOverlap(AActor* OtherActor)
{
	Super::NotifyActorBeginOverlap(OtherActor);

	// && 본인인지 체크 && 공격중에만 -> 데미지적용
	if (OtherActor->IsA(AActor::StaticClass()) && OtherActor != this && IsAttacking == true && OtherActor->ActorHasTag("Enemy") != ActorHasTag("Enemy"))
	{
		// 지정된 액터에 일반적인 피해를 줌
		UGameplayStatics::ApplyDamage(OtherActor, Damage, NULL, this, UDamageType::StaticClass());
	}
}

void ACharacterBase::OnDieAnimationEnd()
{
	StopAnimMontage(DeathAnim);
	//this->SetActorHiddenInGame(true);	// 게임안에서 액터를 숨김
	Destroy();
	//SetLifeSpan(0.1f);
}

// 공격애니메이션 재생
void ACharacterBase::StartAttack()
{
	if (IsAttacking)
		return;

	float AnimDuration = PlayAnimMontage(AttackAnim);
	IsAttacking = true;

	// 애니메이션이 끝나면 StopAttack 호출
	FTimerHandle TimerHandle_StopAttack;
	GetWorldTimerManager().SetTimer(TimerHandle_StopAttack, this, &ACharacterBase::StopAttack, AnimDuration, false);
}

// 공격애니메이션 정지
void ACharacterBase::StopAttack()
{
	StopAnimMontage(AttackAnim);
	IsAttacking = false;
}
