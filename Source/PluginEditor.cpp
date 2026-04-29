/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
CompressorPluginAudioProcessorEditor::CompressorPluginAudioProcessorEditor (CompressorPluginAudioProcessor& p)
    : AudioProcessorEditor (&p), audioProcessor (p), mainComponent(p)
{
    width = 10 * 70; //10 is the aspect ratio f the width of the BG image
    height = 7 * 70; //7 it the aspect ratio of the height of the BG image
    setSize (width, height);
    setResizable(true, true);
    setResizeLimits(width/2, height/2, width*2, height*2);
    getConstrainer()->setFixedAspectRatio(width/height);
    addAndMakeVisible(mainComponent);
    
}

CompressorPluginAudioProcessorEditor::~CompressorPluginAudioProcessorEditor()
{
}

//==============================================================================
void CompressorPluginAudioProcessorEditor::paint (juce::Graphics& g)
{
}

void CompressorPluginAudioProcessorEditor::resized()
{
    mainComponent.setBounds(0, 0, getWidth(), getHeight());
    
}
