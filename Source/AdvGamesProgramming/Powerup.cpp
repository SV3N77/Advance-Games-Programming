// Fill out your copyright notice in the Description page of Project Settings.


#include "Powerup.h"

void APowerup::OnGenerate()
{
	APickup::OnGenerate();
	UE_LOG(LogTemp, Warning, TEXT("GENERATING POWERUP"))
	int32 RandomValue = FMath::RandRange(1, 100);
	
	// Jump Boost = 50% chance
	// Speed Boost = 50% chance
	// Powerup includes a 50/50 chance between a 'Jump Boost Powerup' and a 'Speed Boost Powerup'.
	if (RandomValue <= 50)
	{
		Powerup = PowerupType::JUMPBOOST;
		JumpBoostHeight = 700.0f; // Increase jump height
	}
	else if (RandomValue > 50 && RandomValue <= 100)
	{
		Powerup = PowerupType::SPEEDBOOST;
		SpeedBoostWalkSpeed = 1400.0f; // Increase walk speed
	}
}