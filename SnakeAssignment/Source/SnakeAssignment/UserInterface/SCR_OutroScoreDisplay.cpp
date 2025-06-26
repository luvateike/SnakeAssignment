#include "SCR_OutroScoreDisplay.h"
#include "Components/TextBlock.h"

void USCR_OutroScoreDisplay::NativeConstruct()
{
	Super::NativeConstruct();

	if (MainText)
	{
		MainText->SetText(FText::FromString("Initialized"));
	}
}

void USCR_OutroScoreDisplay::SetText(const FString& NewText)
{
	if (MainText)
	{
		MainText->SetText(FText::FromString(NewText));
	}
}
