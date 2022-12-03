// Fill out your copyright notice in the Description page of Project Settings.


#include "Peak_Frequency.h"

float UPeak_Frequency::PeakFreqData(const UEnum* SetFFT_Size,
	const TArray<float>& FFTInput,
    const float& SampleRate,
	TArray<float>& FFTOutput) {

    FFTOutput = FFTInput;

    //initilise variables
    float peakFreq = 0;
    float sum = 0;
    float maxFreq = 0;
    int maxBin = 0;

    //detect peak frequency
    for (int i = 0; i < FFTInput.Num() /2; i++) {
        sum += FFTInput[i];
        if (FFTInput[i] > maxFreq) {
            maxFreq = FFTInput[i];
            maxBin = i;
        }
    }

    //scale for samplerate 
    peakFreq = (float)maxBin / FFTInput.Num() * 48000;

    //return value
	return peakFreq;
}