// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "HealthComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ADVGAMESPROGRAMMING_API UHealthComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UHealthComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Health")
	float MaxHealth;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Shield")
	int32 CurrentShield;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Shield")
	int32 MaxShield;

	UPROPERTY(BlueprintReadOnly)
	float CurrentHealth;

	UFUNCTION(BlueprintCallable)
	void OnTakeDamage(float Damage);

	UFUNCTION(BlueprintCallable)
	void OnDeath();

	float HealthPercentageRemaining();
};
