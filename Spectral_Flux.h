// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "Spectral_Flux.generated.h"

/**
 * 
 */
UCLASS()
class BPLIBTEST_API USpectral_Flux : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

		UFUNCTION(BlueprintCallable, meta = (ArrayParm = "FFTInput"), category = "UE Audio Feature Extraction")
		static float SpectralFluxData(const UEnum* SetFFT_Size,
			                          const TArray<float>& FFTInput,
			                                TArray<float>& FFTOutput
		);
	
};
