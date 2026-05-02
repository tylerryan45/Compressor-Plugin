/*
  ==============================================================================

    CompressorProcessor.h
    Created: 18 Mar 2026 9:57:53pm
    Author:  Tyler Ryan

  ==============================================================================
*/

#pragma once

#include <string>
#include <vector>
#include "JuceHeader.h"

class Compressor {
    public:
    
    std::vector<float> getGainReduction();
    void updateAlphaA();
    void updateAlphaR();
    void prepareToPlay(float Fs, std::atomic<float>* threshold, std::atomic<float>* ratio, std::atomic<float>* attackTime, std::atomic<float>* releaseTime, std::atomic<float>* makeUpGain);
    void processBuffer(float* buffer, int c, int N);
    float processSample(float x, int c);
    float detectGainChange(float x, int c);
    
    
    private:
    
    float x_dB = 0.f;
    float gainSC = 0.f;
    float gainChange_dB = 0.f;
    float gainSmooth = 0.f;
    float lin_A = 0.f;
    float lin_MUG = 0.f;
    
    float Fs = 0.f;
    double smoothLength = 0.2; //seconds
    
    std::atomic<float>* threshold; //dBFS
    juce::LinearSmoothedValue<float> smoothedThreshold { 0.f };
    float thresh;
    
    std::atomic<float>* ratio;
    juce::LinearSmoothedValue<float> smoothedRatio { 0.f };
    float rat;
    
    std::atomic<float>* attackTime; //seconds
    juce::LinearSmoothedValue<float> smoothedAttackTime { 0.f };
    float attack;
    float alphaA = 0.f;
    
    std::atomic<float>* releaseTime; //seconds
    juce::LinearSmoothedValue<float> smoothedReleaseTime { 0.f };
    float release;
    float alphaR = 0.f;
    
    std::atomic<float>* makeUpGain;
    juce::LinearSmoothedValue<float> smoothedMakeUpGain { 0.f };
    float gain;
    
    std::vector<float> gainSmoothPrev = {0.f,0.f};
    
};
