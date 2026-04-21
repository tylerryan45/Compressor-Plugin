/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
CompressorPluginAudioProcessorEditor::CompressorPluginAudioProcessorEditor (CompressorPluginAudioProcessor& p)
    : AudioProcessorEditor (&p), audioProcessor (p)
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
    releaseTimeSlider.setTextBoxStyle(juce::Slider::TextBoxAbove,true,50,25);
    releaseTimeSlider.onValueChange = [this] () {audioProcessor.comp.updateAlphaR(); };
    releaseTimeSliderLabel.setText(audioProcessor.aptvs.getParameter("RELEASE")->getName(7),juce::dontSendNotification);
    releaseTimeSliderLabel.setJustificationType(juce::Justification::centredTop);
    releaseTimeSliderLabel.attachToComponent(&releaseTimeSlider, false);
    addAndMakeVisible(releaseTimeSlider);
    releaseTimeSliderAttatchment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.aptvs,"RELEASE",releaseTimeSlider);
    
    
    gainSlider.setSliderStyle(juce::Slider::SliderStyle::RotaryHorizontalVerticalDrag);
    gainSlider.setTextBoxStyle(juce::Slider::TextBoxAbove,true,50,25);
    gainSliderLabel.setText(audioProcessor.aptvs.getParameter("GAIN")->getName(4),juce::dontSendNotification);
    gainSliderLabel.setJustificationType(juce::Justification::centredTop);
    gainSliderLabel.attachToComponent(&gainSlider, false);
    addAndMakeVisible(gainSlider);
    gainSliderAttatchment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.aptvs,"GAIN",gainSlider);
    
    setSize (600, 500);
}

CompressorPluginAudioProcessorEditor::~CompressorPluginAudioProcessorEditor()
{
}

//==============================================================================
void CompressorPluginAudioProcessorEditor::paint (juce::Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));

    //g.setColour (juce::Colours::white);
    //g.setFont (juce::FontOptions (15.0f));
    //g.drawFittedText ("Hello World!", getLocalBounds(), juce::Justification::centred, 1);
}

void CompressorPluginAudioProcessorEditor::resized()
{
    thresholdSlider.setBounds(50,50,100,150);
    ratioSlider.setBounds(250,50,100,150);
    attackTimeSlider.setBounds(450,50,100,150);
    releaseTimeSlider.setBounds(150,300,100,150);
    gainSlider.setBounds(350,300,100,150);
    
}
