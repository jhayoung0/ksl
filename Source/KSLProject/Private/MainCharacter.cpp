// Fill out your copyright notice in the Description page of Project Settings.


#include "Public/MainCharacter.h"
#include "MotionRow.h"


// Sets default values
AMainCharacter::AMainCharacter()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void AMainCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AMainCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

// Called to bind functionality to input
void AMainCharacter::SetupPlayerInputComponent(
	UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}


// 
bool AMainCharacter::PlaySignMontageByKey(FString label, int32 Seqcount, float PlayRate, float StartTime)
{
	if (!MotionTable)
	{
		return false;
	}
	
	
	const FMotionRow* Row = MotionTable->FindRow<FMotionRow>(FName(label), TEXT("PlaySignMontageByKey"));
	if (!Row)
	{
		return false;
	}
	if (!Row->signMontage)
	{
		return false;
	}

	if (USkeletalMeshComponent* MeshComp = GetMesh())
	{
		if (UAnimInstance* AnimInst = MeshComp->GetAnimInstance())
		{
			const float Len = AnimInst->Montage_Play(Row->signMontage, PlayRate,
				EMontagePlayReturnType::MontageLength, StartTime, true);
			return (Len > 0.f);
		}
	}
	return false;
	
}


