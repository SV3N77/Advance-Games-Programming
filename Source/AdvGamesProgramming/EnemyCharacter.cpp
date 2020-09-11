// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyCharacter.h"
#include "EngineUtils.h"
#include "Perception/AISense_Hearing.h"
#include "Perception/AISense_Sight.h"



// Sets default values
AEnemyCharacter::AEnemyCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	CurrentAgentState = AgentState::PATROL;
	
}

// Called when the game starts or when spawned
void AEnemyCharacter::BeginPlay()
{
	Super::BeginPlay();
	PerceptionComponent = FindComponentByClass<UAIPerceptionComponent>();
	if (!PerceptionComponent) { UE_LOG(LogTemp, Error, TEXT("NO PERCEPTION COMPONENT FOUND")) }
	PerceptionComponent->OnTargetPerceptionUpdated.AddDynamic(this,  &AEnemyCharacter::SensePlayer);

	HealthComponent = FindComponentByClass<UHealthComponent>();
	
	DetectedActor = nullptr;
	bCanSeeActor = false;
	bHeardActor = false;
	
}

void AEnemyCharacter::AgentPatrol()
{
	if (Path.Num() == 0 && Manager != nullptr)
	{
		Path = Manager->GeneratePath(CurrentNode, Manager->AllNodes[FMath::RandRange(0, Manager->AllNodes.Num() - 1)]);
	}
	
}

void AEnemyCharacter::AgentEngage()
{
	if (Path.Num() == 0 && Manager != nullptr)
	{
		if ((GetActorLocation() - CurrentNode->GetActorLocation()).IsNearlyZero(200.0f))
		{
			Path = Manager->GeneratePath(CurrentNode, Manager->FindNearestNode(DetectedActor->GetActorLocation()));
		}
	}
	if (bCanSeeActor)
	{
		DirectionToTarget = DetectedActor->GetActorLocation() - this->GetActorLocation();
		Fire(DirectionToTarget);
	}
}

void AEnemyCharacter::AgentEvade()
{
	if (Path.Num() == 0 && Manager != nullptr)
	{
		if ((GetActorLocation() - CurrentNode->GetActorLocation()).IsNearlyZero(200.0f))
		{
			Path = Manager->GeneratePath(CurrentNode, Manager->FindFurthestNode(DetectedActor->GetActorLocation()));
		}
	}
	if (bCanSeeActor)
	{
		DirectionToTarget = DetectedActor->GetActorLocation() - this->GetActorLocation();
		Fire(DirectionToTarget);
	}
}

void AEnemyCharacter::AgentInvestigate()
{
	if (bHeardActor == true)
	{
		if (Path.Num() == 0 && Manager != nullptr)
		{
			if ((GetActorLocation() - CurrentNode->GetActorLocation()).IsNearlyZero(200.0f))
			{
				Path = Manager->GeneratePath(CurrentNode, Manager->FindNearestNode(StimulusLocation));
			}
		}
	}
}

void AEnemyCharacter::SensePlayer(AActor* ActorSensed, FAIStimulus Stimulus)
{
	HearingSenseID = UAISense::GetSenseID<UAISense_Hearing>();
	SightSenseID = UAISense::GetSenseID<UAISense_Sight>();

	if (PerceptionComponent->GetSenseConfig(HearingSenseID) != nullptr)
	{
		const FActorPerceptionInfo* HeardPerceptionInfo = PerceptionComponent->GetFreshestTrace(HearingSenseID);
		if (HeardPerceptionInfo != nullptr && PerceptionComponent->HasActiveStimulus(*HeardPerceptionInfo->Target, HearingSenseID))
		{
			StimulusLocation = HeardPerceptionInfo->GetStimulusLocation(HearingSenseID);
			bHeardActor = true;
			UE_LOG(LogTemp, Warning, TEXT("Player Detected"))
		}
	}
	else if (PerceptionComponent->GetSenseConfig(SightSenseID) != nullptr)
	{
		const FActorPerceptionInfo* SightPerceptionInfo = PerceptionComponent->GetFreshestTrace(SightSenseID);
		if (SightPerceptionInfo != nullptr && PerceptionComponent->HasActiveStimulus(*SightPerceptionInfo->Target, SightSenseID))
		{
			bCanSeeActor = true;
			UE_LOG(LogTemp, Warning, TEXT("Player Detected"))
		}
	}
	else
	{
		bHeardActor = false;
		bCanSeeActor = false;
		UE_LOG(LogTemp, Warning, TEXT("Player Lost"))
	}

	/*{
		if (Stimulus.WasSuccessfullySensed())
		{
			DetectedActor = ActorSensed;
			bCanSeeActor = true;
			UE_LOG(LogTemp, Warning, TEXT("Player Detected"))
		}
		else
		{
			bCanSeeActor = false;
			UE_LOG(LogTemp, Warning, TEXT("Player Lost"))
		}
	}*/
}

/*
void AEnemyCharacter::SensePlayer(TArray<AActor*>& ActorSensed ) 
{
	
	for (AActor* Actor : ActorSensed)
	{
		//PerceptionInfo = PerceptionComponent->GetActorInfo(Actor&);
		//PerceptionComponent->GetActorsPerception(Actor, PerceptionInfo);
	}
	
	
}*/

void AEnemyCharacter::MoveAlongPath()
{
	
	if (Path.Num() > 0 && Manager != nullptr)
	{
		//UE_LOG(LogTemp, Display, TEXT("Current Node: %s"), *CurrentNode->GetName())
		if ((GetActorLocation() - CurrentNode->GetActorLocation()).IsNearlyZero(200.0f))
		{
			UE_LOG(LogTemp, Display, TEXT("At Node %s"), *CurrentNode->GetName())
				CurrentNode = Path.Pop();
		}
		else
		{
			FVector WorldDirection = CurrentNode->GetActorLocation() - GetActorLocation();
			WorldDirection.Normalize();
			//UE_LOG(LogTemp, Display, TEXT("The World Direction(X:%f,Y:%f,Z:%f)"), WorldDirection.X, WorldDirection.Y, WorldDirection.Z)
			AddMovementInput(WorldDirection, 1.0f);

			//Get the AI to face in the direction of travel.
			FRotator FaceDirection = WorldDirection.ToOrientationRotator();
			FaceDirection.Roll = 0.f;
			FaceDirection.Pitch = 0.f;
			//FaceDirection.Yaw -= 90.0f;
			SetActorRotation(FaceDirection);
		}
	}
	
}

// Called every frame
void AEnemyCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	if (CurrentAgentState == AgentState::PATROL)
	{
		if (bCanSeeActor == true && HealthComponent->HealthPercentageRemaining() >= 0.4f)
		{
			CurrentAgentState = AgentState::ENGAGE;
			Path.Empty();
		}
		else if (bCanSeeActor == true && HealthComponent->HealthPercentageRemaining() < 0.4f)
		{
			Path.Empty();
			CurrentAgentState = AgentState::EVADE;
		}
		else if (bHeardActor == true)
		{
			Path.Empty();
			CurrentAgentState = AgentState::INVESTIGATE;
		}
		AgentPatrol();
	}
	else if (CurrentAgentState == AgentState::ENGAGE)
	{
		if (bCanSeeActor == false)
		{
			CurrentAgentState = AgentState::PATROL;
		}
		else if (bCanSeeActor == true && HealthComponent->HealthPercentageRemaining() < 0.4f)
		{
			Path.Empty();
			CurrentAgentState = AgentState::EVADE;
		}
		AgentEngage();
	}
	else if (CurrentAgentState == AgentState::EVADE)
	{
		if (bCanSeeActor == false)
		{
		CurrentAgentState = AgentState::PATROL;
		}
		else if (bCanSeeActor == true && HealthComponent->HealthPercentageRemaining() >= 0.4f)
		{
			CurrentAgentState = AgentState::ENGAGE;
			Path.Empty();
		}
		AgentEvade();

	}
	else if (CurrentAgentState == AgentState::INVESTIGATE)
	{
		if (bCanSeeActor == true && HealthComponent->HealthPercentageRemaining() >= 0.4f)
		{
			CurrentAgentState = AgentState::ENGAGE;
			Path.Empty();
		}
		else if (bCanSeeActor == true && HealthComponent->HealthPercentageRemaining() < 0.4f)
		{
			Path.Empty();
			CurrentAgentState = AgentState::EVADE;
		}
		AgentInvestigate();

	}
	MoveAlongPath();
	
	
	//Fire(FVector::ZeroVector);
}

// Called to bind functionality to input
void AEnemyCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

