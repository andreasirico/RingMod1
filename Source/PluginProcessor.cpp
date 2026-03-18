#include "PluginProcessor.h"
#include "PluginEditor.h"
#include "PluginParameters.h"

//==============================================================================

RingMod1AudioProcessor::RingMod1AudioProcessor()
    : AudioProcessor(BusesProperties()
        .withInput("Input", juce::AudioChannelSet::stereo(), true)
        .withInput("Sidechain", juce::AudioChannelSet::stereo(), true)
        .withOutput("Output", juce::AudioChannelSet::stereo(), true)),
    parameters(*this, nullptr, "RM", Parameters::createParameterLayout()),
    drywetter(Parameters::defaultDryWet),
    useInternalNoise(Parameters::defaultMode)
{
    Parameters::addListenerToAllParameters(parameters, this);
}

RingMod1AudioProcessor::~RingMod1AudioProcessor() {}

//==============================================================================
void RingMod1AudioProcessor::prepareToPlay(double sampleRate, int samplesPerBlock)
{
    ringMod.prepareToPlay(sampleRate, samplesPerBlock);
    drywetter.prepareToPlay(sampleRate, samplesPerBlock);
    sidechainInternalBuffer.setSize(2, samplesPerBlock);
    sidechainFilter.prepareToPlay(sampleRate, samplesPerBlock, sidechainInternalBuffer.getNumChannels());
}

void RingMod1AudioProcessor::releaseResources()
{
    drywetter.releaseResources();
}

bool RingMod1AudioProcessor::isBusesLayoutSupported(const BusesLayout& layouts) const
{
    if (layouts.getMainOutputChannelSet() != juce::AudioChannelSet::stereo())
        return false;
    if (layouts.getMainOutputChannelSet() != layouts.getMainInputChannelSet())
        return false;

    if (!layouts.inputBuses[1].isDisabled() &&
        layouts.inputBuses[1] != juce::AudioChannelSet::stereo())
        return false;

    return true;
}

void RingMod1AudioProcessor::processBlock(juce::AudioBuffer<float>& buffer, juce::MidiBuffer&)
{
    auto mainBuffer = getBusBuffer(buffer, true, 0);

    auto sideBuffer = getBusBuffer(buffer, true, 1);

    const int numSamples = buffer.getNumSamples();
    sidechainInternalBuffer.clear();

    if (useInternalNoise)
    {
        for (int ch = 0; ch < sidechainInternalBuffer.getNumChannels(); ++ch)
        {
            auto* writePtr = sidechainInternalBuffer.getWritePointer(ch);
            for (int i = 0; i < numSamples; ++i)
            {
                // rumore
                writePtr[i] = randomSource.nextFloat() * 0.5f - 0.25f;
            }
        }
    }
    else
    {
        if (sideBuffer.getNumChannels() > 0)
        {
            for (int ch = 0; ch < sideBuffer.getNumChannels(); ++ch)
            {
                if (ch < sidechainInternalBuffer.getNumChannels())
                {
                    // sc esterno
                    sidechainInternalBuffer.copyFrom(ch, 0, sideBuffer, ch, 0, numSamples);
                }
            }
        }
    }

    sidechainFilter.process(sidechainInternalBuffer);
    drywetter.copyDrySignal(mainBuffer);
    ringMod.process(mainBuffer, sidechainInternalBuffer);
    drywetter.mixDrySignal(mainBuffer);
}

//==============================================================================
bool RingMod1AudioProcessor::hasEditor() const {
    return true; 
}

juce::AudioProcessorEditor* RingMod1AudioProcessor::createEditor() { 
    return new RingMod1AudioProcessorEditor(*this); 
}

//==============================================================================
void RingMod1AudioProcessor::getStateInformation(juce::MemoryBlock& destData)
{
    auto state = parameters.copyState();
    std::unique_ptr<XmlElement> xml(state.createXml());
    copyXmlToBinary(*xml, destData);
}

void RingMod1AudioProcessor::setStateInformation(const void* data, int sizeInBytes)
{
    std::unique_ptr<XmlElement> xmlState(getXmlFromBinary(data, sizeInBytes));
    if (xmlState.get() != nullptr)
        if (xmlState->hasTagName(parameters.state.getType()))
            parameters.replaceState(ValueTree::fromXml(*xmlState));
}

void RingMod1AudioProcessor::parameterChanged(const juce::String& paramID, float newValue)
{
	// Sidechain Filter
    if (paramID == Parameters::nameFrequency)
        sidechainFilter.setFrequency(newValue);
    if (paramID == Parameters::nameQ)
        sidechainFilter.setResonance(newValue);

	// Dry/Wet
    if (paramID == Parameters::nameDryWet)
        drywetter.setDWRatio(newValue);

	// Mode (0 - esterno, 1 - interno)
    if (paramID == Parameters::nameMode)
    {
        useInternalNoise = (newValue > 0.5f);
    }
}

//=========================================================================
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter() { 
    return new RingMod1AudioProcessor(); 
}
