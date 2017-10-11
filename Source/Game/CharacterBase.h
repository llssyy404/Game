// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "CharacterBase.generated.h"

UCLASS()
class GAME_API ACharacterBase : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ACharacterBase();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	float GetHP() const { return HP; }

	virtual float TakeDamage(float Damage, struct FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;

protected:
	virtual void OnHit(float DamageTaken, struct FDamageEvent const& DamageEvent, class APawn* PawnInstigator, class AActor* DamageCauser);
	virtual void Die(float KillingDamage, struct FDamageEvent const& DamageEvent, AController* Killer, class AActor* DamageCauser);
	virtual void NotifyActorBeginOverlap(AActor* OtherActor) override;

	void OnDieAnimationEnd();
	UFUNCTION(BlueprintCallable, Category = Properties)
	void StartAttack();
	void StopAttack();

protected:
	UPROPERTY(EditDefaultsOnly, Category = Animation)
		UAnimMontage* AttackAnim;

	UPROPERTY(EditDefaultsOnly, Category = Animation)
		UAnimMontage* DamageAnim;

	UPROPERTY(EditDefaultsOnly, Category = Animation)
		UAnimMontage* DeathAnim;

	bool IsAttacking = false;
	float Damage = 50.0f;

public:
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = Health)
		float HP = 100.f;
};
