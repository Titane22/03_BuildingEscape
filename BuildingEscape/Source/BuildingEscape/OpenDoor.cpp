// Fill out your copyright notice in the Description page of Project Settings.

#include "BuildingEscape.h"
#include "OpenDoor.h"


// Sets default values for this component's properties
UOpenDoor::UOpenDoor()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	bWantsBeginPlay = true;
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UOpenDoor::BeginPlay()
{
	Super::BeginPlay();
	//Find owning Actor
	Owner = GetOwner();
	//Set Player's Pawn

	if (!PressurePlate)
	{
		UE_LOG(LogTemp, Error, TEXT("%s missing Pressure Plate!!"), *GetOwner()->GetName())
	}
}

void UOpenDoor::OpenDoor() {
	Owner->SetActorRotation(FRotator(0.f, OpenAngle, 0.f));
}

void UOpenDoor::CloseDoor() {
	Owner->SetActorRotation(FRotator(0.f, 0.f, 0.f));
}

// Called every frame
void UOpenDoor::TickComponent( float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction )
{
	Super::TickComponent( DeltaTime, TickType, ThisTickFunction );

	if (GetTotalMassOfActorsOnPlate() > 30.0f )// TODO make into a parameter
	{
		OpenDoor();
		LastOpenDoorTime = GetWorld()->GetTimeSeconds();
	}
	//Check if it's time to close the door
	if (GetWorld()->GetTimeSeconds() - LastOpenDoorTime > DoorCloseDelay)
	{
		CloseDoor();
	}
}

float UOpenDoor::GetTotalMassOfActorsOnPlate()
{
	float TotalMass = 0.0f;
	if (!PressurePlate) { return TotalMass; }
	// Find all the overlapping actors
	TArray<AActor*>OverlappingActors;
	PressurePlate->GetOverlappingActors(OUT OverlappingActors);

	// Iterate through them adding their masses
	for (auto& Actor : OverlappingActors)
	{
		TotalMass += Actor->FindComponentByClass<UPrimitiveComponent>()->GetMass();
	}
	return TotalMass;
}

