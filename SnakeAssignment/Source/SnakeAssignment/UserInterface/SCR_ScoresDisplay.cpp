#include "SCR_ScoresDisplay.h"
#include "Components/TextBlock.h"

void USCR_ScoresDisplay::NativeConstruct()
{
	Super::NativeConstruct();

	if (MainText)
	{
		MainText->SetText(FText::FromString("Initialized"));
	}
}

void USCR_ScoresDisplay::SetText(const FString& NewText)
{
	if (MainText)
	{
		MainText->SetText(FText::FromString(NewText));
	}
}
