// Fill out your copyright notice in the Description page of Project Settings.


#include "Public/MainUI.h"

#include "SignPlayerController.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"


void UMainUI::NativeConstruct()
{
	Super::NativeConstruct();
	// 버튼 클릭시 topic 담아두기
	if (btn_word1)                                                                                                                                           
	{                                                                                                                                                               
		btn_word1->OnClicked.AddDynamic(this, &UMainUI::btn_click_word1);
		btn_word2->OnClicked.AddDynamic(this, &UMainUI::btn_click_word2);
		btn_word3->OnClicked.AddDynamic(this, &UMainUI::btn_click_word3);     
	}  
}


void UMainUI::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	pc = Cast<ASignPlayerController>(GetWorld()->GetFirstPlayerController());
}



void UMainUI::btn_click_word1()
{
	// 숫자
	pc->topicId = 0;
	pc->SetGameState(GamePlayState::LessonIntro);
}

void UMainUI::btn_click_word2()
{
	// 기본 표현
	pc->topicId = 1;
	pc->SetGameState(GamePlayState::LessonIntro);
}

void UMainUI::btn_click_word3()
{
	// 랜덤
	pc->topicId = 2;
	pc->SetGameState(GamePlayState::LessonIntro);
}

