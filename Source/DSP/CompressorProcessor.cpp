/*
  ==============================================================================

    CompressorProcessor.cpp
    Created: 18 Mar 2026 9:57:53pm
    Author:  Tyler Ryan

  ==============================================================================
*/

#include "CompressorProcessor.h"
#include "JuceHeader.h"

void Compressor::prepareToPlay(float Fs, std::atomic<float>* threshold, std::atomic<float>* ratio, std::atomic<float>* attackTime, std::atomic<float>* releaseTime, std::atomic<float>* makeUpGain){
    this->Fs = Fs;
    this->threshold = threshold;
    this->ratio = ratio;
    this->attackTime = attackTime;
    alphaA = exp(-log(9)/(Fs * attackTime->load()));
    this->releaseTime = releaseTime;
    alphaR = exp(-log(9)/(Fs * releaseTime->load()));
    this->makeUpGain = makeUpGain;
}

void Compressor::setThreshold(std::atomic<float>* threshold){
    this->threshold = threshold;
}

void Compressor::setRatio(std::atomic<float>* ratio){
    this->ratio = ratio;
}

void Compressor::setAttackTime(std::atomic<float>* attackTime){
    this->attackTime = attackTime;
    alphaA = exp(-log(9)/(Fs * attackTime->load()));
}

void Compressor::setReleaseTime(std::atomic<float>* releaseTime){
    this->releaseTime = releaseTime;
    alphaR = exp(-log(9)/(Fs * releaseTime->load()));
}

void Compressor::setMakeUpgain(std::atomic<float>* makeUpGain) {
    this->makeUpGain = makeUpGain;
}

void Compressor::processBuffer(float *buffer, int c, int N){
    for (int n = 0 ; n < N ; ++n) {
        buffer[n] = processSample(buffer[n], c);
    }
}

float Compressor::detectGainChange(float x, int c) {
    float thresh = threshold->load();
    float rat = ratio->load();
    
    // Turn the input into a uni-polar sample on the dB scale
    float x_dB;
    if (x != 0) {
        x_dB = 20*log10(abs(x));
    } else {
        // Set x_dB to arbitrarily small dB value to prevent it from becoming -inf
        x_dB = -120;
    }
    
    float gainSC;
    // Perform downward compression on appropriate sample
    if (x_dB > thresh) {
        gainSC = thresh + (x_dB - thresh)/rat;
    } else {
        gainSC = x_dB;
    }
    
    float gainChange_dB = gainSC - x_dB;
    
    // smooth the gain change
    float gainSmooth;
    if (gainChange_dB < gainSmoothPrev[c]) {
        // attack
        gainSmooth = -sqrt(((1-alphaA)*pow(gainChange_dB,2.f) + (alphaA*pow(gainSmoothPrev[c],2.f))));
    } else {
        // release
        gainSmooth = -sqrt(((1-alphaR)*pow(gainChange_dB,2.f) + (alphaR*pow(gainSmoothPrev[c],2.f))));
    }
    
    // Convert to linear amplitude scalar
    float lin_A = pow(10.f, gainSmooth/20);
    
    // Cache gainSmooth for next sample
    gainSmoothPrev[c] = gainSmooth;
    
    return lin_A;
}

float Compressor::processSample(float x, int c){
    
    // Find the linear amplitude scalar for the given sample
    float lin_A = detectGainChange(x,c);
    
    //convert make up gain to linear amplitude
    float lin_MUG = pow(10.f, makeUpGain->load()/20);
    
    // Apply the linear amplitude scalar and make up gain to the given sample
    return lin_A * lin_MUG * x;
}
