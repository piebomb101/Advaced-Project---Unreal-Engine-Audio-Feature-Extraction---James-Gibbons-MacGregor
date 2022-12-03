


#include "Spectral_Flux.h"

float USpectral_Flux::SpectralFluxData(const UEnum* SetFFT_Size,
	                                   const TArray<float>& FFTInput,
	                                         TArray<float>& FFTOutput) {

	FFTOutput = FFTInput;

	   //initilise values to prevent memory leak
	   float LastFFTFrame[4096] = {};
	   float SpectralDifference[4096] = {};

	
	   double averageSpectralDifference = 0;


	   //subtract previous FFT frame from current FFT frame
	for (int i = 0; i < FFTInput.Num(); i++) {


		SpectralDifference[i] = FFTInput[i] - LastFFTFrame[i];
		LastFFTFrame[i] = FFTInput[i];
		averageSpectralDifference += SpectralDifference[i];

	}


	float r;
	
	//scale for FFT size
	r = averageSpectralDifference / FFTInput.Num();

	return r;

}
