// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "PhysicsEngine/PhysicsHandleComponent.h"
#include "DefaultPawn_BP.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class BUILDINGESCAPE_API UDefaultPawn_BP : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UDefaultPawn_BP();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UPROPERTY(EditAnywhere)
	float Reach = 100.f;
		
	UPhysicsHandleComponent* PhysicsHandle = nullptr; 	

	UInputComponent* InputComponent = nullptr;

	void Grabber();
	void Release();
	void FindPhysicsHandle();
	void SetupInputComponent();
	FHitResult GetFirstPhysicsBodyInReach() const;
	FVector LineTraceEnd();
	};
