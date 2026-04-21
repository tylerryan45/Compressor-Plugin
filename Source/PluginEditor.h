/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"

//==============================================================================
/**
*/
class CompressorPluginAudioProcessorEditor  : public juce::AudioProcessorEditor
{
public:
    CompressorPluginAudioProcessorEditor (CompressorPluginAudioProcessor&);
    ~CompressorPluginAudioProcessorEditor() override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;

private:
    
    juce::Slider thresholdSlider;
    juce::Label thresholdSliderLabel;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> thresholdSliderAttatchment;
    
    juce::Slider ratioSlider;
    juce::Label ratioSliderLabel;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> ratioSliderAttatchment;
    
    juce::Slider attackTimeSlider;
    juce::Label attackTimeSliderLabel;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> attackTimeSliderAttatchment;
    
    juce::Slider releaseTimeSlider;
    juce::Label releaseTimeSliderLabel;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> releaseTimeSliderAttatchment;
    
    juce::Slider gainSlider;
    juce::Label gainSliderLabel;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> gainSliderAttatchment;
    
    
    CompressorPluginAudioProcessor& audioProcessor;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (CompressorPluginAudioProcessorEditor)
};
