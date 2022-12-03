// Fill out your copyright notice in the Description page of Project Settings.


#include "FFTArrayMaker.h"

float UFFTArrayMaker::FFTArrayCreation(const UEnum* FFT_Size, 
	                                   TArray<float>& FFTFreqs) {

	//FFT frequencies, must be same as FFT size in unreal node
	int bands = 4096;
	

	float freqs;

	
	//Loop to output all frequencies. Currently linear.
	for (int i = 0; i < bands; i++) {
		
		FFTFreqs.Add(i * (48000 / bands));
			freqs = FFTFreqs[i];
	}

	

	return freqs;
}