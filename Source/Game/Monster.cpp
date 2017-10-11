// Fill out your copyright notice in the Description page of Project Settings.

#include "Monster.h"
#include "Components/SphereComponent.h"

AMonster::AMonster()
{
	//GetMesh()->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	WeaponCollision = CreateDefaultSubobject<USphereComponent>(TEXT("WeaponCollision"));
	WeaponCollision->InitSphereRadius(50.f);
	WeaponCollision->AttachTo(GetMesh(), "sword_Socket_Collision");

	Damage = 2.0f;
}

