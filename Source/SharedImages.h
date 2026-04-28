/*
  ==============================================================================

    SharedImages.h
    Created: 26 Apr 2026 3:52:13pm
    Author:  Tyler Ryan

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>

class SharedImages {
    
public:
    SharedImages();
    
    void loadImages();
    
    juce::Image& getBackgroundImage() { return m_BackgroundImage; };
    
    juce::Image& getBigKnobImage()    { return m_BigKnob; };

private:
    
    juce::Image m_BackgroundImage;
    
    juce::Image m_BigKnob;
    
};
