// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Pickup.h"
#include "Powerup.generated.h"

UENUM(BlueprintType)
enum class PowerupType : uint8
{
	JUMPBOOST,
	SPEEDBOOST
};

/**
 * 
 */
UCLASS()
class ADVGAMESPROGRAMMING_API APowerup : public APickup
{
	GENERATED_BODY()
	
public:

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	UFUNCTION(BlueprintImplementableEvent)
	void OnPickup(AActor* ActorThatPickedUp) override;
	UFUNCTION(BlueprintCallable)
	void OnGenerate() override;

	UPROPERTY(Replicated, VisibleAnywhere, BlueprintReadOnly)
	PowerupType Powerup;

	UPROPERTY(Replicated, VisibleAnywhere, BlueprintReadOnly)
	float JumpBoostHeight;

	UPROPERTY(Replicated, VisibleAnywhere, BlueprintReadOnly)
	float SpeedBoostWalkSpeed;

};
