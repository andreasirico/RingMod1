#pragma once
#include <JuceHeader.h>
#include "RingModulator.h"
#include "DryWet.h"
#include "SidechainFilter.h"

class RingMod1AudioProcessor : public juce::AudioProcessor, public juce::AudioProcessorValueTreeState::Listener
{
public:
    //==============================================================================
    RingMod1AudioProcessor();
    ~RingMod1AudioProcessor() override;

    //==============================================================================
    void prepareToPlay(double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;

    #ifndef JucePlugin_PreferredChannelConfigurations
        bool isBusesLayoutSupported(const BusesLayout& layouts) const override;
    #endif

    void processBlock(juce::AudioBuffer<float>&, juce::MidiBuffer&) override;

    //==============================================================================
    juce::AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override;

    //==============================================================================
    const juce::String getName() const override { return JucePlugin_Name; };
    bool acceptsMidi() const override { return false; };
    bool producesMidi() const override { return false; };
    bool isMidiEffect() const override { return false; };
    double getTailLengthSeconds() const override { return 0.0; };
    int getNumPrograms() override { return 1; };
    int getCurrentProgram() override { return 0; };
    void setCurrentProgram(int) override {};
    const juce::String getProgramName(int index) override { return{}; };
    void changeProgramName(int index, const juce::String& newName) override {};

    //==============================================================================
    void getStateInformation(juce::MemoryBlock& destData) override;
    void setStateInformation(const void* data, int sizeInBytes) override;

    juce::AudioProcessorValueTreeState& getValueTreeState()
    {
        return parameters;
    }

private:

    void parameterChanged(const String& paramID, float newValue) override;

    AudioProcessorValueTreeState parameters;

    RingModulator ringMod;

    DryWet drywetter;

    SidechainFilter sidechainFilter;

    juce::AudioBuffer<float> sidechainInternalBuffer;

    std::atomic<bool> sidechainConnected{ false };

    // rumore
    juce::Random randomSource;
    bool useInternalNoise;

    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(RingMod1AudioProcessor)
};
