#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"

class RingMod1AudioProcessorEditor : public juce::AudioProcessorEditor
{
public:
    RingMod1AudioProcessorEditor(RingMod1AudioProcessor&);
    ~RingMod1AudioProcessorEditor() override;

    void paint(juce::Graphics&) override;
    void resized() override;

private:
    RingMod1AudioProcessor& audioProcessor;

    juce::Slider dryWetSlider;
    juce::Label dryWetLabel;

    juce::Slider scFrequencySlider;
    juce::Label  scFrequencyLabel;
    juce::Slider scResonanceSlider;
    juce::Label  scResonanceLabel;

    juce::ComboBox modeSelector;
    juce::Label modeLabel;

    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> dryWetAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> scFrequencyAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> scResonanceAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::ComboBoxAttachment> modeAttachment;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(RingMod1AudioProcessorEditor)
};
