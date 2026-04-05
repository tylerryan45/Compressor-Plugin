/*
  ==============================================================================

    CompressorProcessor.h
    Created: 18 Mar 2026 9:57:53pm
    Author:  Tyler Ryan

  ==============================================================================
*/

#pragma once

class Compressor {
    public:
    
    void setThreshold(float threshold);
    void setRatio(float ratio);
    void setAttackTime(float attackTime);
    void setReleaseTime(float releaseTime);
    void setMakeUpgain(float makeUpGain);
    void prepareToPlay(float Fs, float threshold, float ratio, float attackTime, float releaseTime, float makeUpGain);
    void processBuffer(float* buffer, int c, int N);
    float processSample(float x, int c);
    float detectGainChange(float x, int c);
    
    
    private:
    
    float Fs = 44100;
    float threshold = -20.f; //dBFS
    float ratio = 20.f;
    float attackTime = 0.1; //seconds
    float alphaA = 0.f;
    float releaseTime = 0.1; //seconds
    float alphaR = 0.f;
    float makeUpgain = 0.f;
    float gainSmoothPrev[2] = {0};
    
};
