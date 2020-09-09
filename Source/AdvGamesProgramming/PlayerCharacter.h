// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Camera/CameraComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Perception/AISense_Hearing.h"
#include "Sound/SoundCue.h"
#include "PlayerCharacter.generated.h"

UCLASS()
class ADVGAMESPROGRAMMING_API APlayerCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	APlayerCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	void MoveForward(float Value);
	void Strafe(float Value);
	void LookUp(float Value);
	void Turn(float Value);

	void SprintStart();
	void SprintEnd();
	void Distract();

private:
	UPROPERTY(EditInstanceOnly)
	float LookSensitivity;

	UPROPERTY(EditInstanceOnly)
	float SprintMultiplier;
	
	USoundCue* DistractCue;
	
	UCameraComponent* Camera;
};
