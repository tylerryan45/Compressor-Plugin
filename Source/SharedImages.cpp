/*
  ==============================================================================

    SharedImages.cpp
    Created: 26 Apr 2026 3:52:13pm
    Author:  Tyler Ryan

  ==============================================================================
*/

#include "SharedImages.h"

SharedImages::SharedImages(){
    loadImages();
}

void SharedImages::loadImages()
{
    
    m_BackgroundImage = juce::ImageCache::getFromMemory(BinaryData::bg_2_png, BinaryData::bg_2_pngSize);
    
    m_BigKnob         = juce::ImageCache::getFromMemory(BinaryData::Knob_big_png, BinaryData::Knob_big_pngSize);
    
    m_VuMeter         = juce::ImageCache::getFromMemory(BinaryData::vu_meter_png, BinaryData::vu_meter_pngSize);
    
}
