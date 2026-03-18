#pragma once
#include <JuceHeader.h>

namespace Parameters
{
    static const juce::String nameDryWet = "DW";
    static const juce::String nameFrequency = "SC_FREQ";
    static const juce::String nameQ = "SC_Q";
    static const juce::String nameMode = "SC_MODE";

    static const float defaultDryWet = 0.5f;
    static const float defaultFrequency = 20000.0f;
    static const float defaultQ = 1.0f;
	static const int defaultMode = 1; // 0 esterno, 1 interno (rumore)

    static juce::AudioProcessorValueTreeState::ParameterLayout createParameterLayout()
    {
        std::vector<std::unique_ptr<juce::RangedAudioParameter>> params;

        int id = 1;
        params.push_back(std::make_unique<juce::AudioParameterFloat>(juce::ParameterID(nameDryWet, id++), "Dry/Wet", 0.0f, 1.0f, defaultDryWet));
        params.push_back(std::make_unique<juce::AudioParameterFloat>(juce::ParameterID(nameFrequency, id++), "Sidechain LowPass Frequency", juce::NormalisableRange<float>(20.0f, 20000.0f, 1.0f, 0.5f), defaultFrequency));
        params.push_back(std::make_unique<juce::AudioParameterFloat>(juce::ParameterID(nameQ, id++), "Sidechain Q Resonance", juce::NormalisableRange<float>(0.0f, 10.0f, 0.01f), defaultQ));
        params.push_back(std::make_unique<juce::AudioParameterChoice>(juce::ParameterID(nameMode, id++), "Modulation Source", juce::StringArray{ "External", "Internal Noise" }, defaultMode));

        return { params.begin(), params.end() };
    }

    static void addListenerToAllParameters(AudioProcessorValueTreeState& valueTreeState, AudioProcessorValueTreeState::Listener* listener)
    {
        std::unique_ptr<XmlElement> xml(valueTreeState.copyState().createXml());

        for (auto* element : xml->getChildWithTagNameIterator("PARAM"))
        {
            const String& id = element->getStringAttribute("id");
            valueTreeState.addParameterListener(id, listener);
        }
    }
}
