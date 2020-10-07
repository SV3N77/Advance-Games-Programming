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

	UFUNCTION(BlueprintImplementableEvent)
	void OnPickup(AActor* ActorThatPickedUp) override;
	UFUNCTION(BlueprintCallable)
	void OnGenerate() override;

	/*UPROPERTY(VisibleAnywhere, BlueprintReadOnly)*/

};
