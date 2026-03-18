#pragma once
#include <JuceHeader.h>

class RingModulator
{
public:
    void prepareToPlay(double sampleRate, int samplesPerBlock)
    {
        juce::ignoreUnused(sampleRate, samplesPerBlock);
    }

    void process(juce::AudioBuffer<float>& buffer, const juce::AudioBuffer<float>& sidechain)
    {
        if (sidechain.getNumChannels() == 0)
            return;

        for (int ch = 0; ch < buffer.getNumChannels(); ++ch)
        {
            auto* main = buffer.getWritePointer(ch);
            auto* side = sidechain.getReadPointer(juce::jmin(ch, sidechain.getNumChannels() - 1));
            juce::FloatVectorOperations::multiply(main, side, buffer.getNumSamples());
        }
    }
};
