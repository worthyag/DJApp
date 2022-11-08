/*
  ==============================================================================

    DJLookAndFeel.h
    Created: 3 Mar 2022 10:25:28am
    Author:  Worthy

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

//==============================================================================
class DJLookAndFeel  : public juce::Component,
                       public juce::LookAndFeel_V4
{
public:
    //==============================================================================
    DJLookAndFeel();
    ~DJLookAndFeel() override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;
    
    //==============================================================================
    void drawLinearSlider(Graphics &, int x, int y, int width, int height,
                          float sliderPos, float minSliderPos, float maxSliderPos,
                          const Slider::SliderStyle, juce::Slider& slider) override;
    Label* createSliderTextBox (Slider& slider) override;
  
    //==============================================================================
    float knobScale = 0.75;
    Graphics::ResamplingQuality quality = Graphics::highResamplingQuality;
    
    //==============================================================================
    juce::Image sliderKnobGrey = ImageCache::getFromMemory(BinaryData::KnobGrey_png, BinaryData::KnobGrey_pngSize);
    Image sliderKnobGrey2 = sliderKnobGrey.rescaled((int)sliderKnobGrey.getWidth() * (knobScale / 6.5), (int)sliderKnobGrey.getHeight() * (knobScale / 6.5), quality);
    
    Image sliderKnobRed = ImageCache::getFromMemory(BinaryData::KnobRed_png, BinaryData::KnobRed_pngSize);
    Image sliderKnobRed2 = sliderKnobRed.rescaled((int)sliderKnobRed.getWidth() * (knobScale / 6.5), (int)sliderKnobRed.getHeight() * (knobScale / 6.5), quality);
  
    Image sliderKnobOrange = ImageCache::getFromMemory(BinaryData::KnobOrange_png, BinaryData::KnobOrange_pngSize);
    Image sliderKnobOrange2 = sliderKnobOrange.rescaled((int)sliderKnobOrange.getWidth() * (knobScale / 6.5), (int)sliderKnobOrange.getHeight() * (knobScale / 6.5), quality);
  
    Image sliderKnobYellow = ImageCache::getFromMemory(BinaryData::KnobYellow_png, BinaryData::KnobYellow_pngSize);
    Image sliderKnobYellow2 = sliderKnobYellow.rescaled((int)sliderKnobYellow.getWidth() * (knobScale / 6.5), (int)sliderKnobYellow.getHeight() * (knobScale / 6.5), quality);
  
    Image sliderKnobGreen = ImageCache::getFromMemory(BinaryData::KnobGreen_png, BinaryData::KnobGreen_pngSize);
    Image sliderKnobGreen2 = sliderKnobGreen.rescaled((int)sliderKnobGreen.getWidth() * (knobScale / 6.5), (int)sliderKnobGreen.getHeight() * (knobScale / 6.5), quality);

private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (DJLookAndFeel)
};
