// Fill out your copyright notice in the Description page of Project Settings.

#include "ConvertToAttackState.h"
#include "AIController.h"
#include "CharacterBase.h"

EBTNodeResult::Type UConvertToAttackState::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	auto ControlledPawn = OwnerComp.GetAIOwner()->GetPawn();
	ACharacterBase* Monster = dynamic_cast<ACharacterBase*>(ControlledPawn);
	if (!ensure(Monster)) { return EBTNodeResult::Failed; }
	
	Monster->StartAttack();

	return EBTNodeResult::Succeeded;
}


