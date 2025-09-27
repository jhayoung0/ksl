// Fill out your copyright notice in the Description page of Project Settings.


#include "Public/MainCharacter.h"
#include "CharacterAnimInstance.h"
#include "MotionRow.h"
#include "SignPlayerController.h"
#include "Animation/AnimInstance.h"


class ASignPlayerController;
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
void AMainCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}


// 몽타주를 rowname으로 찾아서 재생하자.
bool AMainCharacter::PlaySignMontageByKey(FName RowKey)
{
	if (!MotionTable)
	{
		return false;
	}

	const FMotionRow* Row = MotionTable->FindRow<FMotionRow>(RowKey, TEXT("PlaySignMontageByKey"));
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
		if (UCharacterAnimInstance* AnimInst =Cast<UCharacterAnimInstance>(MeshComp->GetAnimInstance()))
		{
			const float Len = AnimInst->Montage_Play(Row->signMontage);
			return (Len > 0.f);
		}
	}
	return false;
	
}

bool AMainCharacter::PlaySignAnimByKey(FName Rowkey)
{
	const FMotionRow* Row = MotionTable->FindRow<FMotionRow>(Rowkey, TEXT("PlaySignAnimByKey"));
	if (USkeletalMeshComponent* MeshComp = GetMesh())
	{
		// AnimBP를 거치지 않고 단일 애니메이션 모드로 전환
		MeshComp->SetAnimationMode(EAnimationMode::AnimationSingleNode);

		// 루프 재생
		MeshComp->PlayAnimation(Row->signAnim, true);
		return true;
	}
	return false;
}



void AMainCharacter::PlayResultMontage(bool bIsCorrect)
{
	if (UAnimInstance* AnimInst = GetMesh()->GetAnimInstance())
	{
		UAnimMontage* MontageToPlay = bIsCorrect ? TrueMontage : FalseMontage;
		if (MontageToPlay)
		{
			//AnimInst->OnMontageEnded.RemoveDynamic(this, &AMainCharacter::onResultMontageEnded);
			//AnimInst->OnMontageEnded.AddDynamic(this, &AMainCharacter::onResultMontageEnded);
			float Len = AnimInst->Montage_Play(MontageToPlay);
			if (Len > 0.f)
			{
			}
		}
	}
}


// 주제 완료 
void AMainCharacter::TopicComplete()
{
	// 완료 몽타주 실행
	if (USkeletalMeshComponent* MeshComp = GetMesh())
	{
		if (UCharacterAnimInstance* AnimInst =Cast<UCharacterAnimInstance>(MeshComp->GetAnimInstance()))
		{
			 AnimInst->Montage_Play(TopicCompleteMontage);
		}
	}
}






