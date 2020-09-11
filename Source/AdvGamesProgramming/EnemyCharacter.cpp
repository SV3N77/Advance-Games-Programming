// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyCharacter.h"
#include "EngineUtils.h"
#include "Engine/World.h"




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
	bHeardActor = false; // initialise the variable to false
	
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
	FTimerHandle DelayHandle;

	if (bHeardActor == true) // if the AI hears the player's whistle
	{
		if (Path.Num() == 0 && Manager != nullptr)
		{
			if ((GetActorLocation() - CurrentNode->GetActorLocation()).IsNearlyZero(200.0f))
			{
				Path = Manager->GeneratePath(CurrentNode, Manager->FindNearestNode(StimulusLocation)); // generate a path to the location of the sound
				//GetWorldTimerManager().SetTimer(DelayHandle, this, &AEnemyCharacter::AgentPatrol, 5.0f, false); // add a delay when the AI investigates the location of the sound, before returning to PATROL (not working)
			}
			
		}
	}
}

void AEnemyCharacter::SensePlayer(AActor* ActorSensed, FAIStimulus Stimulus)
{
	//Getting the Senses ID from enemy character
	HearingSenseID = UAISense::GetSenseID<UAISense_Hearing>();
	SightSenseID = UAISense::GetSenseID<UAISense_Sight>();
	//Runs if the perception component of the hearing sense ID is not pointing to null
		if (PerceptionComponent->GetSenseConfig(HearingSenseID) != nullptr)
		{
			//Supposed to get the perception info of the freshest trace of hearing sense ID 
			const FActorPerceptionInfo* HeardPerceptionInfo = PerceptionComponent->GetFreshestTrace(HearingSenseID);
			//Supposed to run when there is an active stimulus of heardperceptioninfo towards the target stimulus
			if (HeardPerceptionInfo != nullptr && PerceptionComponent->HasActiveStimulus(*HeardPerceptionInfo->Target, HearingSenseID))
			{
				//supoposed to get the location of stimulus
				StimulusLocation = HeardPerceptionInfo->GetStimulusLocation(HearingSenseID);
				bHeardActor = true;
				UE_LOG(LogTemp, Warning, TEXT("Player Detected"))
			}
		}
		else if (PerceptionComponent->GetSenseConfig(SightSenseID) != nullptr)
		{
			//supposed to get the perception info for sight
			const FActorPerceptionInfo* SightPerceptionInfo = PerceptionComponent->GetFreshestTrace(SightSenseID);
			//when there is an active stimulus of SightPerceptionInfo towards the target stimulus
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
//I tried to do this with the onperceptionupdated function but wasn't able to covert it from blueprints to c++
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

