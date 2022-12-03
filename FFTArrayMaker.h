// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "FFTArrayMaker.generated.h"

/**
 * 
 */
UCLASS()
class BPLIBTEST_API UFFTArrayMaker : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

		//Setup 

		UFUNCTION(BlueprintCallable, meta = (ArrayParm = "FFTFreqs"), category = "UE Audio Feature Extraction")
		static float FFTArrayCreation(const UEnum* FFT_Size, 
			                   TArray<float>& FFTFreqs);
	
};
