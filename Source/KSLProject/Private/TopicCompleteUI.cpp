// Fill out your copyright notice in the Description page of Project Settings.


#include "TopicCompleteUI.h"
#include "SignPlayerController.h"
#include "Components/Button.h"
#include "Components/Image.h"
#include "Kismet/GameplayStatics.h"

void UTopicCompleteUI::NativeConstruct()
{
	Super::NativeConstruct();

	if (btn_othertopic)                                                                                                                                           
	{                                                                                                                                                               
		btn_othertopic->OnClicked.AddDynamic(this, &UTopicCompleteUI::clickOthertopic);
		btn_exit->OnClicked.AddDynamic(this, &UTopicCompleteUI::clickExit);
	}
	
}

void UTopicCompleteUI::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	pc = Cast<ASignPlayerController>(GetWorld()->GetFirstPlayerController());

}

void UTopicCompleteUI::clickOthertopic()
{
	pc->SetGameState(GamePlayState::TopicSelect);
}

void UTopicCompleteUI::clickExit()
{
	UKismetSystemLibrary::QuitGame(GetWorld(), pc, EQuitPreference::Quit, true);
}


void UTopicCompleteUI::ShowbackgroundImg()
{
	img_background->SetVisibility(ESlateVisibility::Visible);
}
