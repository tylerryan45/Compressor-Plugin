/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
CompressorPluginAudioProcessor::CompressorPluginAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
     : AudioProcessor (BusesProperties()
                     #if ! JucePlugin_IsMidiEffect
                      #if ! JucePlugin_IsSynth
                       .withInput  ("Input",  juce::AudioChannelSet::stereo(), true)
                      #endif
                       .withOutput ("Output", juce::AudioChannelSet::stereo(), true)
                     #endif
                       ), aptvs (*this,nullptr,"Parameters",createParameters())
#endif
{
}

CompressorPluginAudioProcessor::~CompressorPluginAudioProcessor()
{
}

//==============================================================================
const juce::String CompressorPluginAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool CompressorPluginAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool CompressorPluginAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool CompressorPluginAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double CompressorPluginAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int CompressorPluginAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int CompressorPluginAudioProcessor::getCurrentProgram()
{
    return 0;
}

void CompressorPluginAudioProcessor::setCurrentProgram (int index)
{
}

const juce::String CompressorPluginAudioProcessor::getProgramName (int index)
{
    return {};
}

void CompressorPluginAudioProcessor::changeProgramName (int index, const juce::String& newName)
{
}

//==============================================================================
void CompressorPluginAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    std::atomic<float>* threshold = aptvs.getRawParameterValue("THRESHOLD");
    std::atomic<float>* ratio = aptvs.getRawParameterValue("RATIO");
    std::atomic<float>* attackTime = aptvs.getRawParameterValue("ATTACK");
    std::atomic<float>* releaseTime = aptvs.getRawParameterValue("RELEASE");
    std::atomic<float>* makeUpGain = aptvs.getRawParameterValue("GAIN");
    
    inputBuffer.clear();
    outputBuffer.clear();
    inputBuffer.setSize(0,samplesPerBlock);
    outputBuffer.setSize(0,samplesPerBlock);
    
    Fs = sampleRate;
    
    comp.prepareToPlay(sampleRate, threshold, ratio, attackTime, releaseTime, makeUpGain);
}

void CompressorPluginAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool CompressorPluginAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
  #if JucePlugin_IsMidiEffect
    juce::ignoreUnused (layouts);
    return true;
  #else
    // This is the place where you check if the layout is supported.
    // In this template code we only support mono or stereo.
    // Some plugin hosts, such as certain GarageBand versions, will only
    // load plugins that support stereo bus layouts.
    if (layouts.getMainOutputChannelSet() != juce::AudioChannelSet::mono()
     && layouts.getMainOutputChannelSet() != juce::AudioChannelSet::stereo())
        return false;

    // This checks if the input layout matches the output layout
   #if ! JucePlugin_IsSynth
    if (layouts.getMainOutputChannelSet() != layouts.getMainInputChannelSet())
        return false;
   #endif

    return true;
  #endif
}
#endif

void CompressorPluginAudioProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    juce::ScopedNoDenormals noDenormals;
    auto totalNumInputChannels  = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();
    
    inputBuffer.makeCopyOf(buffer,true);

    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear (i, 0, buffer.getNumSamples());

    // This is the place where you'd normally do the guts of your plugin's
    // audio processing...
    // Make sure to reset the state if your inner loop is processing
    // the samples and the outer loop is handling the channels.
    // Alternatively, you can process the samples with the channels
    // interleaved by keeping the same state.
    for (int channel = 0; channel < totalNumInputChannels; ++channel)
    {
        auto* channelData = buffer.getWritePointer (channel);

        comp.processBuffer(channelData, channel, buffer.getNumSamples());
        
    }
    
    outputBuffer.makeCopyOf(buffer);
}

//==============================================================================
bool CompressorPluginAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor* CompressorPluginAudioProcessor::createEditor()
{
    return new CompressorPluginAudioProcessorEditor (*this);
}

//==============================================================================
void CompressorPluginAudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    auto state = aptvs.copyState();
    std::unique_ptr<juce::XmlElement> xml (state.createXml());
    copyXmlToBinary (*xml, destData);
}

void CompressorPluginAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    std::unique_ptr<juce::XmlElement> xmlState (getXmlFromBinary (data, sizeInBytes));
        if (xmlState.get() != nullptr)
            if (xmlState->hasTagName (aptvs.state.getType()))
                aptvs.replaceState (juce::ValueTree::fromXml (*xmlState));
}

SharedImages* CompressorPluginAudioProcessor::getSharedImagesPtr() {
    return m_pSharedImagesPtr;
}

std::vector<float> CompressorPluginAudioProcessor::getGainReduction()
{
    return comp.getGainReduction();
}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new CompressorPluginAudioProcessor();
}

juce::AudioProcessorValueTreeState::ParameterLayout CompressorPluginAudioProcessor::createParameters()
{
    std::vector<std::unique_ptr<juce::RangedAudioParameter>> params;
    
    params.push_back (std::make_unique<juce::AudioParameterFloat>(juce::ParameterID {"THRESHOLD",1},"Threshold",-60.0f,0.0f,0.0f));
    params.push_back (std::make_unique<juce::AudioParameterFloat>(juce::ParameterID{"RATIO",1},"Ratio",1.0f,20.0f,1.0f));
    params.push_back (std::make_unique<juce::AudioParameterFloat>(juce::ParameterID{"ATTACK",1},"Attack",0.0f,3.0f,0.0f));
    params.push_back (std::make_unique<juce::AudioParameterFloat>(juce::ParameterID{"RELEASE",1},"Release",0.0f,10.0f,0.0f));
    params.push_back (std::make_unique<juce::AudioParameterFloat>(juce::ParameterID{"GAIN",1},"Gain",0.0f,30.0f,0.0f));
    
    return { params.begin(), params.end() };
}
