// Fill out your copyright notice in the Description page of Project Settings.


#include "PickupManager.h"

// Sets default values
APickupManager::APickupManager()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

void APickupManager::Init(const TArray<FVector>& SpawnLocations, TSubclassOf<APickup> WeaponPickup, float FrequencyOfSpawn)
{
	PossibleSpawnLocations = SpawnLocations;
	WeaponPickupClass = WeaponPickup;
	FrequencyOfWeaponPickupSpawn = FrequencyOfSpawn;
}

void APickupManager::SpawnShieldPickup()
{
	
}

// Called when the game starts or when spawned
void APickupManager::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void APickupManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

