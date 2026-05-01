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
#include "WaveformViewer.h"
#include "MeterWidgets.h"

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
    
    int labelWidth = 50;
    int labelHeight = 30;
    
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
    
    juce::Slider zoomSlider;
    juce::Label zoomSliderLabel;
    
    int viewerChannels;
    juce::WaveformViewer waveViewer;
    
    VerticalGradientMeter inputL;
    VerticalGradientMeter inputR;
    juce::Label inputLabel;
    
    VerticalGradientMeter outputL;
    VerticalGradientMeter outputR;
    juce::Label outputLabel;
    
    GainReductionMeter gainReduction;
    
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MainComponent)
    
    
};
