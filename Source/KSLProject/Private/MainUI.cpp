// Fill out your copyright notice in the Description page of Project Settings.


#include "Public/MainUI.h"

#include "SignPlayerController.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"


void UMainUI::NativeConstruct()
{
	Super::NativeConstruct();
	// 버튼 클릭시 topic 담아두기
	if (ensure(btn_word1))                                                                                                                                           
	{                                                                                                                                                               
		btn_start->OnHovered.AddDynamic(this, &UMainUI::btn_hovered);
		btn_start->OnUnhovered.AddDynamic(this, &UMainUI::btn_unhovered);
		btn_start->OnClicked.AddDynamic(this, &UMainUI::btn_click_start);
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

void UMainUI::btn_click_start()
{
}


void UMainUI::btn_hovered()
{

	// 호버 될 경우 버튼 색 변경
	
	/*
	 * if (txt_start)
	{
		txt_start->SetText(FText("시작하기"));
	}

	*/
}

void UMainUI::btn_unhovered()
{
	// 언호버 될 경우 다시 원래로 돌리기
	
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

