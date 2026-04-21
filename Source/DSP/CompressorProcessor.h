/*
  ==============================================================================

    CompressorProcessor.h
    Created: 18 Mar 2026 9:57:53pm
    Author:  Tyler Ryan

  ==============================================================================
*/

#pragma once

#include<string>

class Compressor {
    public:
    
    void setThreshold(std::atomic<float>* threshold);
    void setRatio(std::atomic<float>* ratio);
    void setAttackTime(std::atomic<float>* attackTime);
    void setReleaseTime(std::atomic<float>* releaseTime);
    void setMakeUpgain(std::atomic<float>* makeUpGain);
    void prepareToPlay(float Fs, std::atomic<float>* threshold, std::atomic<float>* ratio, std::atomic<float>* attackTime, std::atomic<float>* releaseTime, std::atomic<float>* makeUpGain);
    void processBuffer(float* buffer, int c, int N);
    float processSample(float x, int c);
    float detectGainChange(float x, int c);
    
    
    private:
    
    float Fs;
    std::atomic<float>* threshold; //dBFS
    std::atomic<float>* ratio;
    std::atomic<float>* attackTime; //seconds
    float alphaA = 0.f;
    std::atomic<float>* releaseTime; //seconds
    float alphaR = 0.f;
    std::atomic<float>* makeUpGain;
    float gainSmoothPrev[2] = {0};
    
};
