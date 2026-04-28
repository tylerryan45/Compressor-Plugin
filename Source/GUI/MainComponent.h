/*
  ==============================================================================

    MainComponent.h
    Created: 26 Apr 2026 4:52:45pm
    Author:  Tyler Ryan

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "../PluginProcessor.h"
#include "../SharedImages.h"
#include "SliderWidgets.h"

//==============================================================================
/*
*/
class MainComponent  : public juce::Component
{
public:
    MainComponent(CompressorPluginAudioProcessor&);
    ~MainComponent() override;

    void paint (juce::Graphics&) override;
    void resized() override;

private:
    CompressorPluginAudioProcessor&   audioProcessor;
    SharedImages*               m_pSharedImages;
    
    BigKnob thresholdSlider;
    juce::Label thresholdSliderLabel;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> thresholdSliderAttatchment;
    
    BigKnob ratioSlider;
    juce::Label ratioSliderLabel;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> ratioSliderAttatchment;
    
    BigKnob attackTimeSlider;
    juce::Label attackTimeSliderLabel;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> attackTimeSliderAttatchment;
    
    BigKnob releaseTimeSlider;
    juce::Label releaseTimeSliderLabel;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> releaseTimeSliderAttatchment;
    
    BigKnob gainSlider;
    juce::Label gainSliderLabel;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> gainSliderAttatchment;
    
    
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MainComponent)
    
    
};
