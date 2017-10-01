// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CharacterBase.h"
#include "MyPlayer.generated.h"

/**
 * 
 */
UCLASS()
class GAME_API AMyPlayer : public ACharacterBase
{
	GENERATED_BODY()
	
public:
	// Sets default values for this character's properties
	AMyPlayer();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	void MoveForward(float Value);
	void MoveRight(float Value);
	void StartAttack();
	void StopAttack();

	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	FORCEINLINE class UCameraComponent* GetFlowCamera() const { return FlowCamera; }

public:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
		class USpringArmComponent* CameraBoom;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
		class UCameraComponent* FlowCamera;

private:
	UPROPERTY(EditDefaultsOnly, Category = Animation)
		UAnimMontage* AttackAnim;

	bool IsAttacking = false;
};
