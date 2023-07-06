// Fill out your copyright notice in the Description page of Project Settings.


#include "OD_PatrolCharacter.h"

// Sets default values
AOD_PatrolCharacter::AOD_PatrolCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AOD_PatrolCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AOD_PatrolCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AOD_PatrolCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

