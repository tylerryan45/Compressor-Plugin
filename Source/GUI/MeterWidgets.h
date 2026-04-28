/*
  ==============================================================================

    MeterWidgets.h
    Created: 26 Apr 2026 4:54:11pm
    Author:  Tyler Ryan

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>
#include "../SharedImages.h"
#include "../PluginProcessor.h"


//class VUMeter : public juce::Component, private juce::Timer
//{
//public:
//    VUMeter(CompressorPluginAudioProcessor& p) :
//        audioProcessor      (p),
//        m_pSharedImages     (audioProcessor.getSharedImagesPtr()),
//        vuMeter             (m_pSharedImages->getVUMeter())
//    {
//        
//        startTimerHz(30);
//        
//    }
//    
//    void paint(juce::Graphics& g) override
//    {
//        // Intended size frame x frameHeight
//        int frameStartY = index * frameHeight;
//        
//        if(frameStartY > maxFramePosition)
//            frameStartY = maxFramePosition;
//            
//        if (frameStartY < 0)
//            frameStartY = 0;
//        
//        g.drawImage(vuMeter, 0, 0, getWidth(), getHeight(), 0, frameStartY, frameWidth, frameHeight);
//        
//    }
//    
//    void timerCallback() override
//    {
//        
//        updateMeter();
//        
//        repaint();
//    }
//    
//    void updateMeter()
//    {
//        float linLevel = audioProcessor.getOutputLevel(true);
//        float dbLevel  = juce::Decibels::gainToDecibels(linLevel, -100.f);
//        
//        index = floor(juce::jmap(dbLevel, -100.f, 0.f, 0.f, (float) (numFrames-1)));
//        
//        
//    }
//    
//private:
//    CompressorPluginAudioProcessor& audioProcessor;
//    SharedImages* m_pSharedImages;
//    
//    juce::Image& vuMeter;
//    
//    int frame = 0;
//    
//    int numFrames = 129;
//    
//    int frameHeight = 280;
//    
//    int frameWidth = 536;
//    
//    int index = 0;
//    
//    int maxFramePosition = (int) (frameHeight * numFrames);
//    
//};

