/*
  ==============================================================================

    MainComponent.cpp
    Created: 26 Apr 2026 4:52:45pm
    Author:  Tyler Ryan

  ==============================================================================
*/

#include <JuceHeader.h>
#include "MainComponent.h"

//==============================================================================
MainComponent::MainComponent(CompressorPluginAudioProcessor& p) :
    audioProcessor          (p),
    m_pSharedImages         (audioProcessor.getSharedImagesPtr()),
    thresholdSlider         (m_pSharedImages),
    ratioSlider             (m_pSharedImages),
    attackTimeSlider        (m_pSharedImages),
    releaseTimeSlider       (m_pSharedImages),
    gainSlider              (m_pSharedImages)
{
  
    thresholdSlider.setSliderStyle(juce::Slider::SliderStyle::RotaryHorizontalVerticalDrag);
    thresholdSlider.setTextBoxStyle(juce::Slider::TextBoxBelow,false,50,25);
    thresholdSliderLabel.setText(audioProcessor.aptvs.getParameter("THRESHOLD")->getName(9),juce::dontSendNotification);
    thresholdSliderLabel.setJustificationType(juce::Justification::centredTop);
    thresholdSliderLabel.attachToComponent(&thresholdSlider, false);
    addAndMakeVisible(thresholdSlider);
    thresholdSliderAttatchment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.aptvs,"THRESHOLD",thresholdSlider);
    
    ratioSlider.setSliderStyle(juce::Slider::SliderStyle::RotaryHorizontalVerticalDrag);
    ratioSlider.setTextBoxStyle(juce::Slider::TextBoxBelow,true,50,25);
    ratioSliderLabel.setText(audioProcessor.aptvs.getParameter("RATIO")->getName(5),juce::dontSendNotification);
    ratioSliderLabel.setJustificationType(juce::Justification::centredTop);
    ratioSliderLabel.attachToComponent(&ratioSlider, false);
    addAndMakeVisible(ratioSlider);
    ratioSliderAttatchment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.aptvs,"RATIO",ratioSlider);
    
    attackTimeSlider.setSliderStyle(juce::Slider::SliderStyle::RotaryHorizontalVerticalDrag);
    attackTimeSlider.setTextBoxStyle(juce::Slider::TextBoxBelow,true,50,25);
    attackTimeSlider.onValueChange = [this] () { audioProcessor.comp.updateAlphaA(); };
    attackTimeSliderLabel.setText(audioProcessor.aptvs.getParameter("ATTACK")->getName(6),juce::dontSendNotification);
    attackTimeSliderLabel.setJustificationType(juce::Justification::centredTop);
    attackTimeSliderLabel.attachToComponent(&attackTimeSlider, false);
    addAndMakeVisible(attackTimeSlider);
    attackTimeSliderAttatchment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.aptvs,"ATTACK",attackTimeSlider);
    
    releaseTimeSlider.setSliderStyle(juce::Slider::SliderStyle::RotaryHorizontalVerticalDrag);
    releaseTimeSlider.setTextBoxStyle(juce::Slider::TextBoxBelow,true,50,25);
    releaseTimeSlider.onValueChange = [this] () {audioProcessor.comp.updateAlphaR(); };
    releaseTimeSliderLabel.setText(audioProcessor.aptvs.getParameter("RELEASE")->getName(7),juce::dontSendNotification);
    releaseTimeSliderLabel.setJustificationType(juce::Justification::centredTop);
    releaseTimeSliderLabel.attachToComponent(&releaseTimeSlider, false);
    addAndMakeVisible(releaseTimeSlider);
    releaseTimeSliderAttatchment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.aptvs,"RELEASE",releaseTimeSlider);
    
    gainSlider.setSliderStyle(juce::Slider::SliderStyle::RotaryHorizontalVerticalDrag);
    gainSlider.setTextBoxStyle(juce::Slider::TextBoxBelow,true,50,25);
    gainSliderLabel.setText(audioProcessor.aptvs.getParameter("GAIN")->getName(4),juce::dontSendNotification);
    gainSliderLabel.setJustificationType(juce::Justification::centredTop);
    gainSliderLabel.attachToComponent(&gainSlider, false);
    addAndMakeVisible(gainSlider);
    gainSliderAttatchment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.aptvs,"GAIN",gainSlider);
    
}

MainComponent::~MainComponent()
{
}

void MainComponent::paint (juce::Graphics& g)
{
    int width  = getWidth();
    int height = getHeight();
    
    g.drawImage(m_pSharedImages->getBackgroundImage(), 0, 0, width, height, 0, 0, 2000, 1400);
    
}

void MainComponent::resized()
{
    
    thresholdSlider.setBounds(50,100,100,150);
    ratioSlider.setBounds(250,100,100,150);
    attackTimeSlider.setBounds(450,100,100,150);
    releaseTimeSlider.setBounds(150,325,100,150);
    gainSlider.setBounds(350,325,100,150);
    
}
