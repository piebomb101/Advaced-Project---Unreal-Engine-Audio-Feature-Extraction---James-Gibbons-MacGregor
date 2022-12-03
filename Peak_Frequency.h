// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "Peak_Frequency.generated.h"

/**
 * 
 */
UCLASS()
class BPLIBTEST_API UPeak_Frequency : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

		//Setup 

		UFUNCTION(BlueprintCallable, meta = (ArrayParm = "FFTInput"), category = "UE Audio Feature Extraction")
		static float PeakFreqData(const UEnum* SetFFT_Size,
			const TArray<float>& FFTInput,
			const float& SampleRate,
			TArray<float>& FFTOutput
		);

	
};
