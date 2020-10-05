// Fill out your copyright notice in the Description page of Project Settings.


#include "ShieldPickup.h"

void AShieldPickup::OnGenerate()
{
	APickup::OnGenerate();
	
	int32 RandValue = FMath::RandRange(1, 100);
	//Small shields more common so 60% chance
	//Medium shields has 30% chance
	//Large shields have 10% chance
	if (RandValue <= 60)
	{
		ShieldType = ShieldType::SMALL;

	}
	else if (RandValue > 60 && RandValue <= 90)
	{
		ShieldType = ShieldType::MEDIUM;
	}
	else if (RandValue > 90 && RandValue <= 100)
	{
		ShieldType = ShieldType::LARGE;
	}
	
}

