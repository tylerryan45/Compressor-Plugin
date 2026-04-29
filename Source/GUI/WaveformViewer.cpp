/*
  ==============================================================================

    WaveformViewer.cpp
    Created: 29 Apr 2026 11:27:46am
    Author:  Tyler Ryan

  ==============================================================================
*/

#include "WaveformViewer.h"
namespace juce
{

struct WaveformViewer::ChannelInfo
{
    void setFifoSize (int numBlocks)
    {
        fifoStorage.clear();
        fifoStorage.resize ((size_t) numBlocks);
        fifo.setTotalSize (numBlocks);
    }

    void setBufferSize (int numBlocks)
    {
        levels.clear();
        levels.resize ((size_t) numBlocks);
        nextSample = 0;
    }

    void clear()
    {
        for (auto& c : levels)
            c = {};

        counter = 0;
        value = {};
    }

    void pushSamples (int blockSize, Span<const float> samples)
    {
        for (const auto& sample : samples)
            pushSample (blockSize, sample);
    }

    void pushSample (int blockSize, float sample)
    {
        if (++counter < blockSize)
        {
            value = value.getUnionWith (sample);
            return;
        }

        fifo.write (1).forEach ([this] (auto index)
        {
            fifoStorage[(size_t) index] = value;
        });

        counter = 0;
        value = Range (sample, sample);
    }

    void popPending()
    {
        fifo.read (fifo.getNumReady()).forEach ([this] (auto index)
        {
            levels[nextSample] = fifoStorage[(size_t) index];
            nextSample = (nextSample + 1) % levels.size();
        });
    }

    Range<float> value;
    int counter = 0;

    std::vector<Range<float>> fifoStorage;
    AbstractFifo fifo { 1 };

    std::vector<Range<float>> levels;
    size_t nextSample = 0;
};

//==============================================================================
WaveformViewer::WaveformViewer (int initialNumChannels, CompressorPluginAudioProcessor& p)
    : numSamples (1024),
      inputSamplesPerBlock (256),
      backgroundColour (Colours::black),
      waveformColour (Colours::white),
      audioProcessor (p)
{
    setOpaque (true);
    setNumChannels (initialNumChannels);
    setRepaintRate (60);
}

WaveformViewer::~WaveformViewer() = default;

void WaveformViewer::setNumChannels (int numChannels)
{
    channels.clear();

    for (int i = 0; i < numChannels; ++i)
        channels.add (new ChannelInfo);

    for (auto* channel : channels)
        channel->setBufferSize (numSamples);

    updateChannelFifoSizes();
}

void WaveformViewer::setBufferSize (int newNumSamples)
{
    numSamples = newNumSamples;

    for (auto* c : channels)
        c->setBufferSize (newNumSamples);
}

void WaveformViewer::clear()
{
    for (auto* c : channels)
        c->clear();
}

void WaveformViewer::pushBuffer (const float* const* d, int numChannels, int num)
{
    numChannels = jmin (numChannels, channels.size());

    for (auto i = 0; i < numChannels; ++i)
        channels.getUnchecked (i)->pushSamples (inputSamplesPerBlock, { d[i], (size_t) num });
}

void WaveformViewer::pushBuffer (const AudioBuffer<float>& buffer)
{
    pushBuffer (buffer.getArrayOfReadPointers(),
                buffer.getNumChannels(),
                buffer.getNumSamples());
}

void WaveformViewer::pushBuffer (const AudioSourceChannelInfo& buffer)
{
    auto numChannels = jmin (buffer.buffer->getNumChannels(), channels.size());

    for (auto i = 0; i < numChannels; ++i)
    {
        channels.getUnchecked (i)->pushSamples (inputSamplesPerBlock,
                                                { buffer.buffer->getReadPointer (i, buffer.startSample), (size_t) buffer.numSamples });
    }
}

void WaveformViewer::pushSample (const float* d, int numChannels)
{
    numChannels = jmin (numChannels, channels.size());

    for (auto i = 0; i < numChannels; ++i)
        channels.getUnchecked (i)->pushSample (inputSamplesPerBlock, d[i]);
}

void WaveformViewer::setSamplesPerBlock (int newSamplesPerPixel) noexcept
{
    jassert (newSamplesPerPixel > 0);
    inputSamplesPerBlock = newSamplesPerPixel;
}

void WaveformViewer::setRepaintRate (int frequencyInHz)
{
    startTimerHz (frequencyInHz);
    updateChannelFifoSizes();
}

void WaveformViewer::timerCallback()
{
    pushBuffer(audioProcessor.outputBuffer);
    for (auto* channel : channels)
        channel->popPending();

    repaint();
}

void WaveformViewer::setColours (Colour bk, Colour fg) noexcept
{
    backgroundColour = bk;
    waveformColour = fg;
    repaint();
}

void WaveformViewer::paint (Graphics& g)
{
    g.fillAll (backgroundColour);

    auto r = getLocalBounds().toFloat();
    auto channelHeight = r.getHeight() / (float) channels.size();

    g.setColour (waveformColour);

    for (auto* c : channels)
    {
        paintChannel (g,
                      r.removeFromTop (channelHeight),
                      c->levels.data(),
                      (int) c->levels.size(),
                      (int) c->nextSample);
    }
}

void WaveformViewer::getChannelAsPath (Path& path, const Range<float>* levels,
                                                 int numLevels, int nextSample)
{
    path.preallocateSpace (4 * numLevels + 8);

    for (int i = 0; i < numLevels; ++i)
    {
        auto level = -(levels[(nextSample + i) % numLevels].getEnd());

        if (i == 0)
            path.startNewSubPath (0.0f, level);
        else
            path.lineTo ((float) i, level);
    }

    for (int i = numLevels; --i >= 0;)
        path.lineTo ((float) i, -(levels[(nextSample + i) % numLevels].getStart()));

    path.closeSubPath();
}

void WaveformViewer::paintChannel (Graphics& g, Rectangle<float> area,
                                             const Range<float>* levels, int numLevels, int nextSample)
{
    Path p;
    getChannelAsPath (p, levels, numLevels, nextSample);

    g.fillPath (p, AffineTransform::fromTargetPoints (0.0f, -1.0f,               area.getX(), area.getY(),
                                                      0.0f, 1.0f,                area.getX(), area.getBottom(),
                                                      (float) numLevels, -1.0f,  area.getRight(), area.getY()));
}

void WaveformViewer::updateChannelFifoSizes()
{
    // This is intended to make sure that the fifo for each channel is large enough to store
    // at least one frame's incoming blocks with some extra padding to avoid dropping too much info
    // if a frame is delayed.

    const auto maxSampleRate = 192'000;
    const auto maxBlocksPerSecond = inputSamplesPerBlock > 0
                                  ? ((maxSampleRate + inputSamplesPerBlock - 1) / inputSamplesPerBlock)
                                  : 1;
    const auto maxBlocksPerRepaint = (maxBlocksPerSecond * getTimerInterval() + 999) / 1000;
    const auto paddedBlocksPerRepaint = 10 + maxBlocksPerRepaint;

    for (auto* channel : channels)
        channel->setFifoSize (paddedBlocksPerRepaint);
}

} // namespace juce
