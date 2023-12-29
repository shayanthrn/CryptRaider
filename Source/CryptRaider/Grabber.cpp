// Fill out your copyright notice in the Description page of Project Settings.


#include "Grabber.h"
#include "DrawDebugHelpers.h"
#include "Engine/World.h"
#include "PhysicsEngine/PhysicsHandleComponent.h"


// Sets default values for this component's properties
UGrabber::UGrabber()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UGrabber::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UGrabber::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	UPhysicsHandleComponent* handler = GetOwner()->FindComponentByClass<UPhysicsHandleComponent>();
	if(handler != nullptr){
		if(handler->GetGrabbedComponent() != nullptr){
			handler->SetTargetLocationAndRotation(
						GetComponentLocation() + GetForwardVector() * HoldDistance,
						GetComponentRotation()
						);
					}
		}
}

//Grab Function
void UGrabber::Grab()
{
	FVector start = GetComponentLocation();
	FVector end = start + GetForwardVector() * MaxDistanceGrab;
	DrawDebugLine(GetWorld(), start, end, FColor::Red);
	FCollisionShape sphere = FCollisionShape::MakeSphere(grabRadius);
	FHitResult result;
	bool Hit = GetWorld()->SweepSingleByChannel(
		result,
		start, end,
		FQuat::Identity,
		ECC_GameTraceChannel2,
		sphere
	);
	if(Hit){
		
		UE_LOG(LogTemp, Display, TEXT("hit by: %s"), *result.GetActor()->GetName());
		UPhysicsHandleComponent* handler = GetOwner()->FindComponentByClass<UPhysicsHandleComponent>();
		if(handler != nullptr){
			result.GetComponent()->WakeAllRigidBodies();
			result.GetActor()->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
			result.GetComponent()->SetSimulatePhysics(true);
			handler->GrabComponentAtLocationWithRotation(
				result.GetComponent(),
				NAME_None,
				result.ImpactPoint,
				result.GetComponent()->GetComponentRotation()
			);
			result.GetActor()->Tags.Add("grabbed");
			
		}
	}
	
}

//Release Function
//TODO remove grab tag
void UGrabber::Release()
{
	UPhysicsHandleComponent* handler = GetOwner()->FindComponentByClass<UPhysicsHandleComponent>();
		if(handler != nullptr){
			handler->GetGrabbedComponent()->GetOwner()->Tags.Remove("grabbed");
			handler->ReleaseComponent();
		}
}