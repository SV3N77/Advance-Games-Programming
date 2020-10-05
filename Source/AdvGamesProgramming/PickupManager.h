// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Pickup.h"
#include "TimerManager.h"
#include "GameFramework/Actor.h"
#include "PickupManager.generated.h"

UCLASS()
class ADVGAMESPROGRAMMING_API APickupManager : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APickupManager();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void Init(const TArray<FVector>& SpawnLocations, TSubclassOf<APickup> WeaponPickup, float FrequencyOfSpawn);

private:

	TArray<FVector> PossibleSpawnLocations;
	TSubclassOf<APickup> WeaponPickupClass;
	float FrequencyOfWeaponPickupSpawn;
	FTimerHandle WeaponSpawnTimer;


	void SpawnShieldPickup();
};
