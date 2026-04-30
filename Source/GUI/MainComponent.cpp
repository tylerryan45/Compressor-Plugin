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
    waveViewer              (viewerChannels,p),
    inputL                  (audioProcessor.inputBuffer, 0, p.Fs),
    inputR                  (audioProcessor.inputBuffer, 1, p.Fs),
    outputL                 (audioProcessor.outputBuffer, 0, p.Fs),
    outputR                 (audioProcessor.outputBuffer, 1, p.Fs),
    gainReduction           (p)
{
    int labelWidth = 50;
    int labelHeight = 30;
    
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
    waveViewer.setRepaintRate(60);
    addAndMakeVisible(waveViewer);
    
    zoomSlider.setRange(0,1);
    zoomSlider.setValue(0.5);
    zoomSlider.setSliderStyle(juce::Slider::SliderStyle::LinearHorizontal);
    zoomSlider.setTextBoxStyle(juce::Slider::NoTextBox,true,0,0);
    zoomSlider.setColour(juce::Slider::ColourIds::trackColourId, findColour(juce::Slider::ColourIds::backgroundColourId));
    zoomSlider.onValueChange = [this] () {waveViewer.setBufferSize(50 + 200 * zoomSlider.getValue());};
    addAndMakeVisible(zoomSlider);
    
    addAndMakeVisible(inputL);
    addAndMakeVisible(inputR);
    addAndMakeVisible(outputL);
    addAndMakeVisible(outputR);
    
    gainReduction.setRange(0.f,30.f);
    gainReduction.setValue(0.f);
    gainReduction.setSliderStyle(juce::Slider::SliderStyle::LinearVertical);
    gainReduction.setTextBoxStyle(juce::Slider::NoTextBox, true, 0, 0);
    gainReduction.setColour(juce::Slider::ColourIds::backgroundColourId, juce::Colours::orange);
    gainReduction.setColour(juce::Slider::ColourIds::trackColourId, juce::Colours::black);
    gainReduction.setColour(juce::Slider::ColourIds::thumbColourId, juce::Colours::transparentBlack);
    addAndMakeVisible(gainReduction);
    
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
    float knobSizeWidth = 49 * getWidth()* 0.0041; // 49 is the width of the aspect ratio, and the 0.0041 is a scalar to fit the knobs into the plugin window
    float knobSizeHeight = 46 * getHeight() * 0.0041; // 46 is the height of the aspect ratio, and the 0.0041 is a scalar to fit the knobs into the plugin window
    
    thresholdSlider.setBounds((getWidth()*0),(getHeight()*1/5),knobSizeWidth,knobSizeHeight);
    ratioSlider.setBounds(thresholdSlider.getX()+thresholdSlider.getWidth(),thresholdSlider.getY(),thresholdSlider.getWidth(),thresholdSlider.getHeight());
    attackTimeSlider.setBounds(ratioSlider.getX()+ratioSlider.getWidth(),ratioSlider.getY(),ratioSlider.getWidth(),ratioSlider.getHeight());
    releaseTimeSlider.setBounds(attackTimeSlider.getX()+attackTimeSlider.getWidth(),attackTimeSlider.getY(),attackTimeSlider.getWidth(),attackTimeSlider.getHeight());
    gainSlider.setBounds(releaseTimeSlider.getX()+releaseTimeSlider.getWidth(),releaseTimeSlider.getY(),releaseTimeSlider.getWidth(),releaseTimeSlider.getHeight());

    waveViewer.setBounds((getWidth()*1/6),(getHeight()*2/5),(getWidth()*4/6),(getHeight()*1/2));
    zoomSlider.setBounds(waveViewer.getX(),waveViewer.getY() + waveViewer.getHeight(),waveViewer.getWidth(),getHeight()*0.05);
    
    int meterWidth = getWidth()/28;
    inputL.setBounds((getWidth()*1/12)-meterWidth,waveViewer.getY(),meterWidth,waveViewer.getHeight());
    inputR.setBounds((getWidth()*1/12),waveViewer.getY(),meterWidth,waveViewer.getHeight());
    
    outputL.setBounds((getWidth()*11/12)-meterWidth,waveViewer.getY(),meterWidth,waveViewer.getHeight());
    outputR.setBounds((getWidth()*11/12),waveViewer.getY(),meterWidth,waveViewer.getHeight());
    
    meterWidth = getWidth()/56;
    gainReduction.setBounds(waveViewer.getX()+waveViewer.getWidth(), waveViewer.getY(),meterWidth,waveViewer.getHeight());
}
