#include "PluginProcessor.h"
#include "PluginEditor.h"
#include "PluginParameters.h"

//==============================================================================

RingMod1AudioProcessorEditor::RingMod1AudioProcessorEditor(RingMod1AudioProcessor& p)
    : AudioProcessorEditor(&p), audioProcessor(p)
{
	// Frequency Slider
    scFrequencySlider.setSliderStyle(juce::Slider::Rotary);
    scFrequencySlider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 60, 20);

    scFrequencySlider.setColour(juce::Slider::rotarySliderFillColourId, juce::Colour(0xfff5a623));
    scFrequencySlider.setColour(juce::Slider::rotarySliderOutlineColourId, juce::Colour(0xff2a2a2a));
    scFrequencySlider.setColour(juce::Slider::thumbColourId, juce::Colours::white);
    scFrequencySlider.setColour(juce::Slider::textBoxTextColourId, juce::Colours::white);
    scFrequencySlider.setColour(juce::Slider::textBoxBackgroundColourId, juce::Colour(0xff1e1e1e));

    scFrequencySlider.setTextValueSuffix(" Hz");
    addAndMakeVisible(scFrequencySlider);

    scFrequencyLabel.setText("Frequency", juce::dontSendNotification);
    scFrequencyLabel.setJustificationType(juce::Justification::centred);
    scFrequencyLabel.setColour(juce::Label::textColourId, juce::Colours::white);
    addAndMakeVisible(scFrequencyLabel);

    scFrequencyAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.getValueTreeState(), Parameters::nameFrequency, scFrequencySlider);

    // Resonance Q
    scResonanceSlider.setSliderStyle(juce::Slider::Rotary);
    scResonanceSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 60, 20);

    scResonanceSlider.setColour(juce::Slider::rotarySliderFillColourId, juce::Colour(0xfff5a623));
    scResonanceSlider.setColour(juce::Slider::rotarySliderOutlineColourId, juce::Colour(0xff2a2a2a));
    scResonanceSlider.setColour(juce::Slider::thumbColourId, juce::Colours::white);
    scResonanceSlider.setColour(juce::Slider::textBoxTextColourId, juce::Colours::white);
    scResonanceSlider.setColour(juce::Slider::textBoxBackgroundColourId, juce::Colour(0xff1e1e1e));

    addAndMakeVisible(scResonanceSlider);

    scResonanceLabel.setText("Q", juce::dontSendNotification);
    scResonanceLabel.setJustificationType(juce::Justification::centred);
    scResonanceLabel.setColour(juce::Label::textColourId, juce::Colours::white);
    addAndMakeVisible(scResonanceLabel);

    scResonanceAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.getValueTreeState(), Parameters::nameQ, scResonanceSlider);

    // Dry/Wet
    dryWetSlider.setSliderStyle(juce::Slider::Rotary);
    dryWetSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 60, 20);

    dryWetSlider.setColour(juce::Slider::rotarySliderFillColourId, juce::Colour(0xfff5a623));
    dryWetSlider.setColour(juce::Slider::rotarySliderOutlineColourId, juce::Colour(0xff2a2a2a));
    dryWetSlider.setColour(juce::Slider::thumbColourId, juce::Colours::white);
    dryWetSlider.setColour(juce::Slider::textBoxTextColourId, juce::Colours::white);
    dryWetSlider.setColour(juce::Slider::textBoxBackgroundColourId, juce::Colour(0xff1e1e1e));

    addAndMakeVisible(dryWetSlider);

    dryWetLabel.setText("Dry / Wet", juce::dontSendNotification);
    dryWetLabel.setJustificationType(juce::Justification::centred);
    dryWetLabel.setColour(juce::Label::textColourId, juce::Colours::white);
    addAndMakeVisible(dryWetLabel);

    dryWetAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.getValueTreeState(), Parameters::nameDryWet, dryWetSlider);

    // MODE
    modeSelector.addItem("External", 1);
    modeSelector.addItem("Internal Noise", 2);
    addAndMakeVisible(modeSelector);

    modeLabel.setText("Mod Source", juce::dontSendNotification);
    modeLabel.setJustificationType(juce::Justification::centred);
    addAndMakeVisible(modeLabel);

    modeAttachment = std::make_unique<juce::AudioProcessorValueTreeState::ComboBoxAttachment>(audioProcessor.getValueTreeState(), Parameters::nameMode, modeSelector);

    // finestra
    setSize(420, 300);
}

RingMod1AudioProcessorEditor::~RingMod1AudioProcessorEditor()
{
}

//==============================================================================

void RingMod1AudioProcessorEditor::paint(juce::Graphics& g)
{
    // Sfondo
    g.fillAll(juce::Colour(0xff1e1e1e));

    // Bordo
    auto panel = getLocalBounds().reduced(10);
    g.setColour(juce::Colour(0xff2a2a2a));
    g.fillRoundedRectangle(panel.toFloat(), 12.0f);

    // Titolo
    g.setColour(juce::Colours::white);
    g.setFont(juce::Font(18.0f, juce::Font::bold));
    auto titleArea = panel.removeFromTop(32);
    g.drawText("Ring Modulator", titleArea, juce::Justification::centred);
}

//==============================================================================

void RingMod1AudioProcessorEditor::resized()
{
    // dimensioni dei componenti
    const int knobSize = 100;
    const int spacing = 30;     // Spazio tra knob
    const int labelHeight = 20; // Altezza del testo sopra
    const int labelOffset = 5;  // Distanza tra label e knob
	    // Modalità
        const int comboWidth = 150;
        const int comboHeight = 30; 
        const int verticalGap = 40;

    // Area
    auto area = getLocalBounds();
    int centerY = area.getCentreY(); // Centro verticale

    // larghezza totale
    int totalWidth = (knobSize * 3) + (spacing * 2);

    // Partenza da sinistra per centrare
    int startX = (getWidth() - totalWidth) / 2;


    // Frequency
    int x1 = startX;
    scFrequencyLabel.setBounds(x1, centerY - (knobSize / 2) - labelHeight - labelOffset, knobSize, labelHeight);
    scFrequencySlider.setBounds(x1, centerY - (knobSize / 2), knobSize, knobSize);

    // Resonance Q
    int x2 = startX + knobSize + spacing;
    scResonanceLabel.setBounds(x2, centerY - (knobSize / 2) - labelHeight - labelOffset, knobSize, labelHeight);
    scResonanceSlider.setBounds(x2, centerY - (knobSize / 2), knobSize, knobSize);

    // Dry/Wet
    int x3 = startX + (knobSize + spacing) * 2;
    dryWetLabel.setBounds(x3, centerY - (knobSize / 2) - labelHeight - labelOffset, knobSize, labelHeight);
    dryWetSlider.setBounds(x3, centerY - (knobSize / 2), knobSize, knobSize);

	// Mode
    int x4 = (getWidth() - comboWidth) / 2;
    int y4 = centerY + (knobSize / 2) + verticalGap;
    modeLabel.setBounds(x4, y4 - labelHeight - labelOffset, comboWidth, labelHeight);
    modeSelector.setBounds(x4, y4, comboWidth, comboHeight);
}
