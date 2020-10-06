// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Pickup.h"
#include "ShieldPickup.generated.h"

UENUM(BlueprintType)
enum class ShieldType : uint8
{
	SMALL,
	MEDIUM,
	LARGE
};

UCLASS()
class ADVGAMESPROGRAMMING_API AShieldPickup : public APickup
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintImplementableEvent)
		void OnPickup(AActor* ActorThatPickedUp) override;
	UFUNCTION(BlueprintCallable)
		void OnGenerate() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		ShieldType ShieldType;
	
};
