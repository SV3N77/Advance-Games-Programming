// Fill out your copyright notice in the Description page of Project Settings.


#include "ShieldPickup.h"

#include "Net/UnrealNetwork.h"

void AShieldPickup::OnGenerate()
{
	APickup::OnGenerate();
	UE_LOG(LogTemp, Warning, TEXT("GENERATING Shield PICKUP"))
	int32 RandValue = FMath::RandRange(1, 100);
	//Small shields more common so 60% chance green material
	//Medium shields has 30% chance blue material
	//Large shields have 10% chance gold material
	if (RandValue <= 60)
	{
		ShieldType = ShieldType::SMALL;
		CurrentShield = 25;
	}
	else if (RandValue > 60 && RandValue <= 90)
	{
		ShieldType = ShieldType::MEDIUM;
		CurrentShield = 50;
	}
	else if (RandValue > 90 && RandValue <= 100)
	{
		ShieldType = ShieldType::LARGE;
		CurrentShield = 100;
	}
	
}

void AShieldPickup::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(AShieldPickup, ShieldType);
	DOREPLIFETIME(AShieldPickup, CurrentShield);
}
