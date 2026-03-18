#pragma once
#include <juce_dsp/juce_dsp.h>

class SidechainFilter
{
public:
    SidechainFilter() = default;

    void prepareToPlay(double sampleRate, int samplesPerBlock, int numChannels)
    {
        juce::dsp::ProcessSpec filterSpec;
        filterSpec.sampleRate = sampleRate;
        filterSpec.maximumBlockSize = samplesPerBlock;
        filterSpec.numChannels = numChannels;

        filter.prepare(filterSpec);
        filter.setType(juce::dsp::StateVariableTPTFilterType::lowpass);

        update();
    }

    void process(juce::AudioBuffer<float>& buffer)
    {
        juce::dsp::AudioBlock<float> block(buffer);
        juce::dsp::ProcessContextReplacing<float> context(block);
        filter.process(context);
    }

    void setFrequency(float newFrequency)
    {
        frequency = newFrequency;
        update();
    }

    void setResonance(float newQ)
    {
        resonance = newQ;
        update();
    }

private:
    void update()
    {
        filter.setCutoffFrequency(frequency);
        filter.setResonance(resonance);
    }

    juce::dsp::StateVariableTPTFilter<float> filter;

    float frequency = 20000.0f;
    float resonance = 1.0f;
};
