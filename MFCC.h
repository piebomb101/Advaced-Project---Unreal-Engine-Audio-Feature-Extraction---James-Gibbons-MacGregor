// THIS CODE IS TAKEN FROM THE MAXIMILLIAN AUDIO PROCESSING LIBRARY UNDER MIT LICENCE 
//AND HAS BEEN ADAPTED FOR UNREAL ENGINE BY JAMES GIBBONS-MACGREGOR

#pragma once

#include <math.h>
#include <typeinfo>
#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "MFCC.generated.h"


/**
 * 
 */
UCLASS()
class BPLIBTEST_API UMFCC : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

		UFUNCTION(BlueprintCallable, meta = (ArrayParm = "FFTInput"), category = "UE Audio Feature Extraction")
		static TArray<double> MFCCData(const UEnum* SetFFT_Size,
			const TArray<float>& FFTInput,
			const float& SampleRate,
			TArray<float>& FFTOutput
			//double* MFCCOUT
		);
};



using namespace std;

// implements this formula:
// mel = 2595 log10(Hz/700 + 1)
inline double hzToMel(double hz) {
	return 2595.0 * (log10(hz / 700.0 + 1.0));
}

// implements this formula
// Hz = 700 (10^(mel/2595) - 1)
inline double melToHz(double mel) {
	return 700.0 * (pow(10, mel / 2595.0) - 1.0);
}

template <class T>
class maxiMFCCAnalyser {
public:

	T* melBands;
	maxiMFCCAnalyser() :melFilters(NULL), dctMatrix(NULL), melBands(NULL) {
		string info = typeid(dctMatrix).name();
		string info2 = typeid(melBands).name();


		numFilters1 = 1;
		numCoeffs1 = 1;
	    minFreq1 = 1;
		 maxFreq1 = 1;
		 sampleRate1 = 1;
		  numBins1 = 1;

	};


	~maxiMFCCAnalyser() {
		if (melFilters) {
			//delete[] melBands;
			//delete[] dctMatrix;
			//delete[] melFilters;

		}
	}

	void setup(unsigned int numBins, unsigned int numFilters, unsigned int numCoeffs, double minFreq, double maxFreq, unsigned int sampleRate)
	{

		

		this->numFilters1 = numFilters;
		this->numCoeffs1 = numCoeffs;
		this->minFreq1 = minFreq;
		this->maxFreq1 = maxFreq;
		this->sampleRate1 = sampleRate;
		this->numBins1 = numBins;
		melFilters = NULL;
		melBands = (T*)malloc(sizeof(T) * numFilters);
		//melBands = new T[numFilters];

		//create new matrix
		dctMatrix = (T*)malloc(sizeof(T) * numCoeffs * numFilters);
		//dctMatrix = new T[numCoeffs * numFilters];
		calcMelFilterBank(sampleRate, numBins);

		for (unsigned int i = 0; i < (sizeof(melFilters) / sizeof(*melFilters)); i++)
		{
			if (isnan(melFilters[i])) {
				int a = 0;
			}
		}

		createDCTCoeffs();
	}
	void mfcc(float* powerSpectrum, T* mfccs) {
		melFilterAndLogSquare(powerSpectrum);


		for (unsigned int i = 0; i < numBins1; i++)
		{
			if (isnan(powerSpectrum[i])) {
				int a = 0;
			}
		}

		dct(mfccs);

		//delete[] dctMatrix;
		//delete[] melBands;
		//delete[] melFilters;

		free(dctMatrix);
		free(melBands);
	    free(melFilters);
	}

private:
	unsigned int numFilters1, numCoeffs1;
	double minFreq1, maxFreq1;
	unsigned int sampleRate1;
	T* melFilters;
	unsigned int numBins1;
	T* dctMatrix;



	void dct(T* mfccs) {
		for (unsigned int i = 0; i < numCoeffs1; i++) {
			mfccs[i] = 0.0;
		}
		for (unsigned int i = 0; i < numCoeffs1; i++) {
			for (unsigned int j = 0; j < numFilters1; j++) {
				int idx = i + (j * numCoeffs1);
				mfccs[i] += (dctMatrix[idx] * melBands[j]);
			}
		}
		for (unsigned int i = 0; i < numCoeffs1; i++) {
			mfccs[i] /= numCoeffs1;
		}
	}


	void melFilterAndLogSquare(float* powerSpectrum);
	void melFilterAndLogSq_Part2(float* powerSpectrum);


	void calcMelFilterBank(double sampleRate, int numBins) {

		double mel, dMel, maxMel, minMel, nyquist, binFreq, start, end, thisF, nextF, prevF;
		int numValidBins;

		// ignore bins over nyquist
		numValidBins = numBins;

		nyquist = sampleRate / 2;
		if (maxFreq1 > nyquist) {
			maxFreq1 = nyquist;
		}

		maxMel = hzToMel(maxFreq1);
		minMel = hzToMel(minFreq1);

		dMel = (maxMel - minMel) / (numFilters1 + 2 - 1);

		T* filtPos = (T*)malloc(sizeof(double) * (numFilters1 + 2));

		// first generate an array of start and end freqs for each triangle
		mel = minMel;
		for (unsigned int i = 0; i < numFilters1 + 2; i++) {
			// start of the triangle
			filtPos[i] = melToHz(mel);
			//		std::cout << "[" << i << "] MFCC: centre is at " <<filtPos[i]<<"hz "<<mel<<" mels" << endl;
			mel += dMel;
		}
		// now generate the coefficients for the mag spectrum
		melFilters = (T*)malloc(sizeof(T) * numFilters1 * numValidBins);
		//melFilters = new T[numFilters1 * numValidBins];

		for (unsigned int filter = 1; filter < numFilters1; filter++) {
			for (int bin = 0; bin < numValidBins; bin++) {
				// frequency this bin represents
				binFreq = (T)sampleRate / (T)numValidBins * (T)bin;
				thisF = filtPos[filter];
				nextF = filtPos[filter + 1];
				prevF = filtPos[filter - 1];
				int idx = filter + (bin * numFilters1);
				if (binFreq > nextF || binFreq < prevF) {
					// outside this filter
					melFilters[idx] = 0;
					//cout << "MFCCMYK: filter at " <<thisF << " bin at " <<binFreq <<" coeff " <<melFilters[filter][bin] << endl;
				}
				else {
					T height = 2.0 / (nextF - prevF);

					if (binFreq < thisF) {
						// up
						start = prevF;
						end = thisF;
						melFilters[idx] = (binFreq - start) * (height / (thisF - start));
					}
					else {
						// down
						start = thisF;
						end = nextF;
						melFilters[idx] = height + ((binFreq - thisF) * (-height / (nextF - thisF)));
					}
					//				cout << "MFCCMYK: filter at " <<thisF << " bin at " <<binFreq <<" coeff " <<melFilters[filter][bin] << endl;
					//cout << "MFCCMYK: filter at " <<thisF << " bin at " <<binFreq <<" coeff " <<melFilters[idx] << endl;
				}
			}
		}
	}
	void createDCTCoeffs() {
		T k = 3.14159265358979323846 / numFilters1;
		T w1 = 1.0 / (sqrt(numFilters1));
		T w2 = sqrt(2.0 / numFilters1);


		//generate dct matrix
		for (unsigned int i = 0; i < numCoeffs1; i++)
		{
			for (unsigned int j = 0; j < numFilters1; j++)
			{
				int idx = i + (j * numCoeffs1);
				if (i == 0)
					dctMatrix[idx] = w1 * cos(k * (i + 1) * (j + 0.5));
				else
					dctMatrix[idx] = w2 * cos(k * (i + 1) * (j + 0.5));
			}
		}


	}



};



typedef maxiMFCCAnalyser<double> maxiMFCC;
//typedef maxiMFCCAnalyser<float> maxiFloatMFCC;





	

