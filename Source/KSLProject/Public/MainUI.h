// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MainUI.generated.h"

class ASignPlayerController;
/**
 * 
 */
UCLASS()
class KSLPROJECT_API UMainUI : public UUserWidget
{
	GENERATED_BODY()
	
	
public:

	UPROPERTY()
	ASignPlayerController* pc;

	// 버튼
	
	
	// 시작 버튼
	UPROPERTY(meta = (BindWidget)) 
	class UTextBlock* txt_start;
	
	UPROPERTY(meta = (BindWidget)) 
	class UButton* btn_start; // 시작버튼
	
	// 숫자
	UPROPERTY(meta = (BindWidget)) 
	class UButton* btn_word1;

	// 의사소통표현
	UPROPERTY(meta = (BindWidget))
	class UButton* btn_word2;

	// 랜덤
	UPROPERTY(meta = (BindWidget))
	class UButton* btn_word3;
	
	virtual void NativeConstruct() override;
	virtual void NativeOnInitialized() override;

	// 버튼 클릭 함수
	UFUNCTION()
	void btn_click_word1();
	UFUNCTION()
	void btn_click_word2();
	UFUNCTION()
	void btn_click_word3();


	// 주제 선택 메뉴로 가기 위해서 hidden 처리
	UFUNCTION(BlueprintImplementableEvent)
	void GoSelectTopic();


	void ChangeToNextLevel();

	FTimerHandle TimerHandle;
};
