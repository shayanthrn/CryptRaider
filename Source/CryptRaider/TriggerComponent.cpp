// Fill out your copyright notice in the Description page of Project Settings.


#include "TriggerComponent.h"


// Sets default values for this component's properties
UTriggerComponent::UTriggerComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}

// Called when the game starts
void UTriggerComponent::BeginPlay()
{
	Super::BeginPlay();
	
}


// Called every frame
void UTriggerComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	TArray<AActor*> OverlapingActors;
	GetOverlappingActors(OverlapingActors);
	int32 shouldtrigger=0;
	for(FName tag:RequiredTags){
		for(AActor* Actor:OverlapingActors){
			if(Actor->ActorHasTag(tag) && !Actor->ActorHasTag("grabbed")){
				shouldtrigger++; //found actor with tag
			}
		}
	}
	// TODO setsim phys off for actors and attach to trigger
	if(Mover != nullptr){
		if(shouldtrigger==RequiredTags.Num()){ // should trigger!
			Mover->setMove(true);
			for(AActor* Actor:OverlapingActors){
				Actor->AttachToComponent(this,FAttachmentTransformRules::KeepWorldTransform);
				Cast<UPrimitiveComponent>(Actor->GetRootComponent())->SetSimulatePhysics(false);
			}
		}
		else{
			Mover->setMove(false);
		}
	}
}

// Called when the game starts
void UTriggerComponent::SetMover(UMoverComponent* NewMover)
{
	Mover = NewMover;	
}