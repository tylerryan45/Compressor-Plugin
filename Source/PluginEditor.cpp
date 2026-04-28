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
    
    setSize (600, 500);
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
