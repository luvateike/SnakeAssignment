#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SCR_OutroScoreDisplay.generated.h"

class UTextBlock;

UCLASS()
class SNAKEASSIGNMENT_API USCR_OutroScoreDisplay : public UUserWidget
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable)
	void SetText(const FString& NewText);

protected:
	virtual void NativeConstruct() override;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* MainText;
};
