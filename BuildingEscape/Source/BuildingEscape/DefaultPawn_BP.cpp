// Fill out your copyright notice in the Description page of Project Settings.
#include "DefaultPawn_BP.h"
#include "DrawDebugHelpers.h"
#include "Engine/World.h"
#include "GameFramework/PlayerController.h"

// Sets default values for this component's properties
UDefaultPawn_BP::UDefaultPawn_BP()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}
// Called when the game starts
void UDefaultPawn_BP::BeginPlay()
{
	Super::BeginPlay();

	FindPhysicsHandle();
	SetupInputComponent();
}

void UDefaultPawn_BP:: SetupInputComponent()
{
	InputComponent = GetOwner()->FindComponentByClass<UInputComponent>();
	if (!InputComponent)
	{
		UE_LOG(LogTemp,Error,TEXT("%s does not have InputComponent Present"), *GetOwner()->GetName());
	}
	InputComponent->BindAction("Grab", IE_Pressed, this, &UDefaultPawn_BP::Grabber);
	InputComponent->BindAction("Grab",IE_Released,this, &UDefaultPawn_BP::Release);
}

void UDefaultPawn_BP:: FindPhysicsHandle()
{
	PhysicsHandle = GetOwner()->FindComponentByClass<UPhysicsHandleComponent>();

	if (!PhysicsHandle)
	{
		UE_LOG(LogTemp,Error,TEXT("%s does not have PhysicsHandle Present"), *GetOwner()->GetName());
	}
}

// Called every frame
void UDefaultPawn_BP::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	
	if (PhysicsHandle->GrabbedComponent)
	{
		PhysicsHandle->SetTargetLocation(LineTraceEnd());
	}	
}

FHitResult UDefaultPawn_BP::GetFirstPhysicsBodyInReach()const
{
	FVector PlayerViewLocation;
	FRotator PlayerViewRotation;

	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(PlayerViewLocation, PlayerViewRotation);

	FVector LineTraceEnd = PlayerViewRotation.Vector() * Reach + PlayerViewLocation;
	FHitResult Hit;

	FCollisionQueryParams TraceParam(FName(TEXT("")), false, GetOwner());

	GetWorld()->LineTraceSingleByObjectType(
		Hit, PlayerViewLocation, LineTraceEnd, FCollisionObjectQueryParams(ECollisionChannel::ECC_PhysicsBody), TraceParam);
	
	AActor* ActorThatWasHit = Hit.GetActor();

	if (ActorThatWasHit)
	{
		UE_LOG(LogTemp, Warning, TEXT("	%s has been hit"),*ActorThatWasHit->GetName());
	}

	return Hit;
}

void UDefaultPawn_BP::Grabber()
{
	FVector PlayerViewLocation;
	FRotator PlayerViewRotation;

	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(PlayerViewLocation, PlayerViewRotation);

	FVector LineTraceEnd =  PlayerViewLocation +PlayerViewRotation.Vector() * Reach;

	UE_LOG(LogTemp, Warning, TEXT("Grabber"));
	
	FHitResult HitResult =  GetFirstPhysicsBodyInReach();
	UPrimitiveComponent* ComponentToGrab = HitResult.GetComponent();
	
	if (HitResult.GetActor())
	{
		PhysicsHandle->GrabComponentAtLocationWithRotation(ComponentToGrab,NAME_None,LineTraceEnd,PlayerViewRotation);
	}
}

void UDefaultPawn_BP::Release()
{
	UE_LOG(LogTemp, Warning, TEXT("Release"));
	PhysicsHandle->ReleaseComponent();
}

FVector UDefaultPawn_BP::LineTraceEnd()
{
	FVector PlayerViewLocation;
	FRotator PlayerViewRotation;

	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(PlayerViewLocation, PlayerViewRotation);

	return PlayerViewLocation +PlayerViewRotation.Vector() * Reach;
}