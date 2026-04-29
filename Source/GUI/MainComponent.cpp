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
    gainSlider              (m_pSharedImages),
    viewerChannels          (audioProcessor.getTotalNumInputChannels()),
    waveViewer              (viewerChannels)
{
    thresholdSlider.setSliderStyle(juce::Slider::SliderStyle::RotaryHorizontalVerticalDrag);
    thresholdSlider.setTextBoxStyle(juce::Slider::TextBoxBelow,false,labelWidth,labelHeight);
    thresholdSliderLabel.setText(audioProcessor.aptvs.getParameter("THRESHOLD")->getName(9),juce::dontSendNotification);
    thresholdSliderLabel.setJustificationType(juce::Justification::centredTop);
    thresholdSliderLabel.attachToComponent(&thresholdSlider, false);
    addAndMakeVisible(thresholdSlider);
    thresholdSliderAttatchment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.aptvs,"THRESHOLD",thresholdSlider);
    
    ratioSlider.setSliderStyle(juce::Slider::SliderStyle::RotaryHorizontalVerticalDrag);
    ratioSlider.setTextBoxStyle(juce::Slider::TextBoxBelow,true,labelWidth,labelHeight);
    ratioSliderLabel.setText(audioProcessor.aptvs.getParameter("RATIO")->getName(5),juce::dontSendNotification);
    ratioSliderLabel.setJustificationType(juce::Justification::centredTop);
    ratioSliderLabel.attachToComponent(&ratioSlider, false);
    addAndMakeVisible(ratioSlider);
    ratioSliderAttatchment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.aptvs,"RATIO",ratioSlider);
    
    attackTimeSlider.setSliderStyle(juce::Slider::SliderStyle::RotaryHorizontalVerticalDrag);
    attackTimeSlider.setTextBoxStyle(juce::Slider::TextBoxBelow,true,labelWidth,labelHeight);
    attackTimeSlider.onValueChange = [this] () { audioProcessor.comp.updateAlphaA(); };
    attackTimeSliderLabel.setText(audioProcessor.aptvs.getParameter("ATTACK")->getName(6),juce::dontSendNotification);
    attackTimeSliderLabel.setJustificationType(juce::Justification::centredTop);
    attackTimeSliderLabel.attachToComponent(&attackTimeSlider, false);
    addAndMakeVisible(attackTimeSlider);
    attackTimeSliderAttatchment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.aptvs,"ATTACK",attackTimeSlider);
    
    releaseTimeSlider.setSliderStyle(juce::Slider::SliderStyle::RotaryHorizontalVerticalDrag);
    releaseTimeSlider.setTextBoxStyle(juce::Slider::TextBoxBelow,true,labelWidth,labelHeight);
    releaseTimeSlider.onValueChange = [this] () {audioProcessor.comp.updateAlphaR(); };
    releaseTimeSliderLabel.setText(audioProcessor.aptvs.getParameter("RELEASE")->getName(7),juce::dontSendNotification);
    releaseTimeSliderLabel.setJustificationType(juce::Justification::centredTop);
    releaseTimeSliderLabel.attachToComponent(&releaseTimeSlider, false);
    addAndMakeVisible(releaseTimeSlider);
    releaseTimeSliderAttatchment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.aptvs,"RELEASE",releaseTimeSlider);
    
    gainSlider.setSliderStyle(juce::Slider::SliderStyle::RotaryHorizontalVerticalDrag);
    gainSlider.setTextBoxStyle(juce::Slider::TextBoxBelow,true,labelWidth,labelHeight);
    gainSliderLabel.setText(audioProcessor.aptvs.getParameter("GAIN")->getName(4),juce::dontSendNotification);
    gainSliderLabel.setJustificationType(juce::Justification::centredTop);
    gainSliderLabel.attachToComponent(&gainSlider, false);
    addAndMakeVisible(gainSlider);
    gainSliderAttatchment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.aptvs,"GAIN",gainSlider);
    
    waveViewer.setColours(juce::Colours::black, juce::Colours::white);
    addAndMakeVisible(waveViewer);
    
    zoomSlider.setRange(0,1);
    zoomSlider.setValue(0.5);
    zoomSlider.setSliderStyle(juce::Slider::SliderStyle::LinearHorizontal);
    zoomSlider.setTextBoxStyle(juce::Slider::NoTextBox,true,0,0);
    zoomSlider.setColour(juce::Slider::ColourIds::trackColourId, findColour(juce::Slider::ColourIds::backgroundColourId));
    addAndMakeVisible(zoomSlider);
    
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
    knobSizeWidth = 49 * getWidth()* 0.0041; // 49 is the width of the aspect ratio, and the 0.0041 is a scalar to fit the knobs into the plugin window
    knobSizeHeight = 46 * getHeight() * 0.0041; // 46 is the height of the aspect ratio, and the 0.0041 is a scalar to fit the knobs into the plugin window
    
    thresholdSlider.setBounds((getWidth()*0),(getHeight()*1/5),knobSizeWidth,knobSizeHeight);
    ratioSlider.setBounds(thresholdSlider.getX()+thresholdSlider.getWidth(),thresholdSlider.getY(),thresholdSlider.getWidth(),thresholdSlider.getHeight());
    attackTimeSlider.setBounds(ratioSlider.getX()+ratioSlider.getWidth(),ratioSlider.getY(),ratioSlider.getWidth(),ratioSlider.getHeight());
    releaseTimeSlider.setBounds(attackTimeSlider.getX()+attackTimeSlider.getWidth(),attackTimeSlider.getY(),attackTimeSlider.getWidth(),attackTimeSlider.getHeight());
    gainSlider.setBounds(releaseTimeSlider.getX()+releaseTimeSlider.getWidth(),releaseTimeSlider.getY(),releaseTimeSlider.getWidth(),releaseTimeSlider.getHeight());

    waveViewer.setBounds((getWidth()*1/6),(getHeight()*2/5),(getWidth()*4/6),(getHeight()*1/2));
    zoomSlider.setBounds(waveViewer.getX(),waveViewer.getY() + waveViewer.getHeight(),waveViewer.getWidth(),getHeight()*0.05);
    
    
}
