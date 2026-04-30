/*
  ==============================================================================

    MeterWidgets.h
    Created: 26 Apr 2026 4:54:11pm
    Author:  Tyler Ryan

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>
#include "../SharedImages.h"
#include "../PluginProcessor.h"


class GainReductionMeter : public juce::Slider, private juce::Timer
{
public:
    GainReductionMeter(CompressorPluginAudioProcessor& p) :
    audioProcessor (p)
    {
        startTimerHz(30);
    }
    
    void timerCallback() override
    {
        std::vector<float> gainReduction = audioProcessor.getGainReduction();
        float value = -120;
        for (int i = 0; i < gainReduction.size() - 1; i++)
        {
            if (value < gainReduction[i])
            {
                value = gainReduction[i];
            }
        }
        setValue(getMaximum() - 20*log10(abs(value)));
    }
    
    
    
private:
    CompressorPluginAudioProcessor&   audioProcessor;
};


class VerticalGradientMeter : public juce::Component, private juce::Timer
{
public:
    VerticalGradientMeter(juce::AudioBuffer<float>& buffer, int channel, float Fs) :
        channel             (channel),
        Fs                  (Fs),
        buffer              (buffer)
    {
        rmsLevel.reset(Fs, 0.05);
        rmsLevel.setCurrentAndTargetValue(-100.0);
        startTimerHz(30);
    }
    
    void paint(juce::Graphics& g) override
    {
        auto bounds = getLocalBounds().toFloat().reduced(3.f);

        g.setColour(juce::Colours::black);
        g.fillRect(bounds);

        g.setGradientFill(gradient);
        int scaledY = 0;
        if (rmsLevel.getCurrentValue() >= -60.f) {
            scaledY = juce::jmap(rmsLevel.getCurrentValue(), -60.f, 6.f, 0.f, static_cast<float>(getHeight()));
        }
        g.fillRect(bounds.removeFromBottom(scaledY));
    }
    
    void resized() override {
        const auto bounds = getLocalBounds().toFloat();
        gradient = juce::ColourGradient{ juce::Colours::green, bounds.getBottomLeft(), juce::Colours::red, bounds.getTopLeft(), false };
        gradient.addColour(0.5, juce::Colours::yellow);
    }
    
    void timerCallback() override
    {
        updateMeter();
        repaint();
    }
    
    void updateMeter()
    {
        rmsLevel.skip(buffer.getNumSamples());
        float value = juce::Decibels::gainToDecibels(buffer.getRMSLevel(channel,0, buffer.getNumSamples()));
        if (value < rmsLevel.getCurrentValue())
        {
            rmsLevel.setTargetValue(value);
        } else {
            rmsLevel.setCurrentAndTargetValue(value);
        }
    }
private:
    int channel;
    float Fs;
    juce::AudioBuffer<float>& buffer;
    juce::LinearSmoothedValue<float> rmsLevel;
    juce::ColourGradient gradient{};
};

