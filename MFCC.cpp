// THIS CODE IS TAKEN FROM THE MAXIMILLIAN AUDIO PROCESSING LIBRARY UNDER MIT LICENCE 
// AND HAS BEEN ADAPTED FOR UNREAL ENGINE BY JAMES GIBBONS-MACGREGOR


#include "MFCC.h"
#include <math.h>
#include <typeinfo>

TArray<double> UMFCC::MFCCData(const UEnum* SetFFT_Size,
	const TArray<float>& FFTInput,
	const float& SampleRate,
	TArray<float>& FFTOutput
	//double* MFCCOUT
	) {

	FFTOutput = FFTInput;

	int USampleRate1 = 48000;



	maxiMFCC mfcc;
	double* mfccs;
	int32 BinNum = 13;



	mfccs = new double[13]();

	for (int i = 0; i < BinNum; i++)
	{
		if (isnan(mfccs[i])) {
			int a = 0;
		}
	}
	



	mfcc.setup(4096, 42, 13, 20, 20000, USampleRate1);


	for (int i = 0; i < BinNum; i++)
	{
		if (isnan(mfccs[i])) {
			int a = 0;
		}
	}

	mfcc.mfcc(FFTOutput.GetData(), mfccs);


	for (int i = 0; i < BinNum; i++)
	{
		if (isnan(mfccs[i])) {
			int a = 0;
		}
	}

	//MFCCOUT = mfccs;

	//TArray<double> returnValue = new TArr
	TArray<double> returnValues;
	returnValues.Init(1.0, BinNum);

	TArray<double> lastVal;
	lastVal.Init(0.0, BinNum);


	for (int i = 0; i < BinNum; i++)
	{
		returnValues[i] = mfccs[i];

		if (isnan(mfccs[i])) {
			returnValues[i] = 0.0;
		}
		
	}
	

	//float randomArray[13];

	/*
	for (int i = 0; i < BinNum; i++)
	{
		if (isnan(mfccs[i])) {
			lastVal[i] = mfccs[i];
			randomArray[i] = rand() % 1;
		    returnValues[i] = lastVal[i] + (randomArray[i] / 100) ;
		}
	}
	*/
	

	//returnValue.

	delete[] mfccs;



	return returnValues;
}

template <>
void maxiMFCCAnalyser<double>::melFilterAndLogSquare(float* powerSpectrum) {

	melFilterAndLogSq_Part2(powerSpectrum);
}


template <>
void maxiMFCCAnalyser<float>::melFilterAndLogSquare(float* powerSpectrum) {

	melFilterAndLogSq_Part2(powerSpectrum);
}

template <class T>
void maxiMFCCAnalyser<T>::melFilterAndLogSq_Part2(float* powerSpectrum) {

	for (unsigned int filter = 0; filter < numFilters1; filter++) {
		melBands[filter] = 0.0;
		for (unsigned int bin = 0; bin < numBins1; bin++) {
					int idx = (numBins1 * filter) + bin;
			//int idx = filter + (bin * numFilters1);
			//int idx = filter + (bin * numFilters1);
			//			melBands[filter] += (melFilters[filter][bin] * powerSpectrum[bin]);
			melBands[filter] += (melFilters[idx] * powerSpectrum[bin]);
			if (isnan(melBands[filter])) {
				int a = 0;
			}

			if (isnan(melFilters[idx])) {
				int a = 0;
			}
		}


	}

	for (unsigned int filter = 0; filter < numFilters1; filter++) {
		// log the square
		melBands[filter] = melBands[filter] > 0.000001 ? log(melBands[filter] * melBands[filter]) : 0.0;

		if (isnan(melBands[filter])) {
			int a = 0;
		}
	}
}



