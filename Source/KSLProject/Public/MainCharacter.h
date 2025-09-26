// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "MainCharacter.generated.h"

UCLASS()
class KSLPROJECT_API AMainCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AMainCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(
		class UInputComponent* PlayerInputComponent) override;
	
	// 모션 담겨있는 데이터 테이블
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Sign")
	UDataTable* MotionTable = nullptr;

	// sign_num + seq_Id로 RowName을 만들어 FindRow로 바로 재생
	UFUNCTION(BlueprintCallable, Category="Sign")
	bool PlaySignMontageByKey(FName SignNum, FName SeqId, int32 Seqcount, float PlayRate = 1.f, float StartTime = 0.f);

private:
	static FName MakeRowKey(FName SignNum, FName SeqId)
	{
		return FName(*(SignNum.ToString() + TEXT("_") + SeqId.ToString()));
	}
	


	
};
