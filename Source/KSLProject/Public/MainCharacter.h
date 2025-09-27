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

	// 몽타주 재생
	UFUNCTION(BlueprintCallable, Category="Sign")
	bool PlaySignMontageByKey(FName Rowkey);

	// 몽타주 재생
	UFUNCTION(BlueprintCallable, Category="Sign")
	bool PlaySignAnimByKey(FName Rowkey);
	
	

	// 정답, 오답 몽타주 재생
	UFUNCTION(BlueprintCallable, Category="Sign")
	void PlayResultMontage(bool IsCorrect);


	// 주제 완료 몽타주 실행
	UFUNCTION(BlueprintCallable, Category="Sign")
	void TopicComplete();
	
	// 몽타주
	UPROPERTY(EditAnywhere, Category = "Montage")
	UAnimMontage* TrueMontage;

	UPROPERTY(EditAnywhere, Category= "Montage")
	UAnimMontage* FalseMontage;

	UPROPERTY(EditAnywhere, Category= "Montage")
	UAnimMontage* TopicCompleteMontage;

	
};
