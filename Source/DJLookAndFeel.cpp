/*
  ==============================================================================

    DJLookAndFeel.cpp
    Created: 3 Mar 2022 10:25:28am
    Author:  Worthy

  ==============================================================================
*/

#include <JuceHeader.h>
#include "DJLookAndFeel.h"

//==============================================================================
DJLookAndFeel::DJLookAndFeel() {}

DJLookAndFeel::~DJLookAndFeel() {}

void DJLookAndFeel::paint (juce::Graphics& g) {}

void DJLookAndFeel::resized() {}

//==============================================================================
void DJLookAndFeel::drawLinearSlider(Graphics& g, int x, int y, int width, int height,
                                     float sliderPos,
                                     float minSliderPos,
                                     float maxSliderPos,
                                     const Slider::SliderStyle style, Slider& slider)
{
    //==============================================================================
    if (slider.isBar())
    {
        g.setColour (slider.findColour (Slider::trackColourId));
        g.fillRect (slider.isHorizontal() ? Rectangle<float> (static_cast<float> (x), (float) y + 0.5f, sliderPos - (float) x, (float) height - 1.0f)
                                        : Rectangle<float> ((float) x + 0.5f, sliderPos, (float) width - 1.0f, (float) y + ((float) height - sliderPos)));
    }
    else
    {
        auto isTwoVal   = (style == Slider::SliderStyle::TwoValueVertical   || style == Slider::SliderStyle::TwoValueHorizontal);
        auto isThreeVal = (style == Slider::SliderStyle::ThreeValueVertical || style == Slider::SliderStyle::ThreeValueHorizontal);

        auto trackWidth = jmin (6.0f, slider.isHorizontal() ? (float) height * 0.25f : (float) width * 0.25f);

        Point<float> startPoint (slider.isHorizontal() ? (float) x : (float) x + (float) width * 0.5f,
                               slider.isHorizontal() ? (float) y + (float) height * 0.5f : (float) (height + y));

        Point<float> endPoint (slider.isHorizontal() ? (float) (width + x) : startPoint.x,
                             slider.isHorizontal() ? startPoint.y : (float) y);

        Path backgroundTrack;
        backgroundTrack.startNewSubPath (startPoint);
        backgroundTrack.lineTo (endPoint);
        g.setColour (slider.findColour (Slider::backgroundColourId));
        g.strokePath (backgroundTrack, { trackWidth, PathStrokeType::curved, PathStrokeType::rounded });

        Path valueTrack;
        Point<float> minPoint, maxPoint, thumbPoint;

        //==============================================================================
        if (isTwoVal || isThreeVal)
        {
            minPoint = { slider.isHorizontal() ? minSliderPos : (float) width * 0.5f,
                       slider.isHorizontal() ? (float) height * 0.5f : minSliderPos };

            if (isThreeVal)
              thumbPoint = { slider.isHorizontal() ? sliderPos : (float) width * 0.5f,
                             slider.isHorizontal() ? (float) height * 0.5f : sliderPos };

            maxPoint = { slider.isHorizontal() ? maxSliderPos : (float) width * 0.5f,
                       slider.isHorizontal() ? (float) height * 0.5f : maxSliderPos };
        }
        else
        {
            auto kx = slider.isHorizontal() ? sliderPos : ((float) x + (float) width * 0.5f);
            auto ky = slider.isHorizontal() ? ((float) y + (float) height * 0.5f) : sliderPos;

            minPoint = startPoint;
            maxPoint = { kx, ky };
        }

        valueTrack.startNewSubPath (minPoint);
        valueTrack.lineTo (isThreeVal ? thumbPoint : maxPoint);
        g.setColour (slider.findColour (Slider::trackColourId));
        g.strokePath (valueTrack, { trackWidth, PathStrokeType::curved, PathStrokeType::rounded });

        //==============================================================================
        if (! isTwoVal)
        {
            g.setColour (slider.findColour (Slider::thumbColourId));
            slider.setColour(juce::Slider::backgroundColourId, juce::Colour(0xffF8FBFF));
            
            // Checks whether the slider is vertical
            if (slider.isVertical()){
                if (slider.getValue() < 1.0) {
                    if (slider.getValue() >= 0.8) {
                      g.drawImage(sliderKnobRed2, (int)maxPoint.x - (sliderKnobRed2.getWidth()/2),
                                  (int)maxPoint.y - (sliderKnobRed2.getHeight()/2),
                                  sliderKnobRed2.getWidth(), sliderKnobRed2.getHeight(),
                                  0, 0, sliderKnobRed2.getWidth(),
                                  sliderKnobRed2.getHeight()); //pastel red
                      slider.setColour(juce::Slider::trackColourId , juce::Colour(0xffF26B6B)); //pastel red
                    }
                    else if (slider.getValue() >= 0.6) {
                      g.drawImage(sliderKnobOrange2, (int)maxPoint.x - (sliderKnobOrange2.getWidth()/2),
                                  (int)maxPoint.y - (sliderKnobOrange2.getHeight()/2),
                                  sliderKnobOrange2.getWidth(), sliderKnobOrange2.getHeight(),
                                  0, 0, sliderKnobOrange2.getWidth(),
                                  sliderKnobOrange2.getHeight()); //pastel orange
                      slider.setColour(juce::Slider::trackColourId , juce::Colour(0xffF5A78E)); //pastel orange
                    }
                    else if (slider.getValue() >= 0.4) {
                      g.drawImage(sliderKnobYellow2, (int)maxPoint.x - (sliderKnobYellow2.getWidth()/2),
                                  (int)maxPoint.y - (sliderKnobYellow2.getHeight()/2),
                                  sliderKnobYellow2.getWidth(), sliderKnobYellow2.getHeight(),
                                  0, 0, sliderKnobYellow2.getWidth(),
                                  sliderKnobYellow2.getHeight()); //pastel yellow
                      slider.setColour(juce::Slider::trackColourId , juce::Colour(0xffF5EDA4)); //pastel yellow
                    }
                    else if (slider.getValue() < 0.4 && slider.getValue() > 0.1) {
                      g.drawImage(sliderKnobGreen2, (int)maxPoint.x - (sliderKnobGreen2.getWidth()/2),
                                  (int)maxPoint.y - (sliderKnobGreen2.getHeight()/2),
                                  sliderKnobGreen2.getWidth(), sliderKnobGreen2.getHeight(),
                                  0, 0, sliderKnobGreen2.getWidth(),
                                  sliderKnobGreen2.getHeight()); //pastel green
                      slider.setColour(juce::Slider::trackColourId , juce::Colour(0xff9ED8C3)); //pastel green
                    }
                    else if (slider.getValue() <= 0.1 || slider.getValue() == 0.0) {
                      g.drawImage(sliderKnobGrey2, (int)maxPoint.x - (sliderKnobGrey2.getWidth()/2),
                                  (int)maxPoint.y - (sliderKnobGrey2.getHeight()/2),
                                  sliderKnobGrey2.getWidth(), sliderKnobGrey2.getHeight(),
                                  0, 0, sliderKnobGrey2.getWidth(),
                                  sliderKnobGrey2.getHeight()); // grey
                      slider.setColour(juce::Slider::trackColourId , juce::Colour(0xffC4C4C4)); // grey
                    }
            }
            else if (slider.getValue() > 1.0) {
                if (slider.getValue() >= 80.0) {
                  g.drawImage(sliderKnobRed2, (int)maxPoint.x - (sliderKnobRed2.getWidth()/2),
                              (int)maxPoint.y - (sliderKnobRed2.getHeight()/2), sliderKnobRed2.getWidth(),
                              sliderKnobRed2.getHeight(), 0, 0, sliderKnobRed2.getWidth(),
                              sliderKnobRed2.getHeight()); //pastel red
                  slider.setColour(juce::Slider::trackColourId , juce::Colour(0xffF26B6B)); //pastel red
                }
                else if (slider.getValue() >= 60.0) {
                  g.drawImage(sliderKnobOrange2, (int)maxPoint.x - (sliderKnobOrange2.getWidth()/2),
                              (int)maxPoint.y - (sliderKnobOrange2.getHeight()/2),
                              sliderKnobOrange2.getWidth(), sliderKnobOrange2.getHeight(),
                              0, 0, sliderKnobOrange2.getWidth(),
                              sliderKnobOrange2.getHeight()); //pastel orange
                  slider.setColour(juce::Slider::trackColourId , juce::Colour(0xffF5A78E)); //pastel orange
                }
                else if (slider.getValue() >= 40.0) {
                  g.drawImage(sliderKnobYellow2, (int)maxPoint.x - (sliderKnobYellow2.getWidth()/2),
                              (int)maxPoint.y - (sliderKnobYellow2.getHeight()/2),
                              sliderKnobYellow2.getWidth(), sliderKnobYellow2.getHeight(),
                              0, 0, sliderKnobYellow2.getWidth(),
                              sliderKnobYellow2.getHeight()); //pastel yellow
                  slider.setColour(juce::Slider::trackColourId , juce::Colour(0xffF5EDA4)); //pastel yellow
                }
                else if (slider.getValue() < 40.0 && slider.getValue() > 10.0) {
                  g.drawImage(sliderKnobGreen2, (int)maxPoint.x - (sliderKnobGreen2.getWidth()/2),
                              (int)maxPoint.y - (sliderKnobGreen2.getHeight()/2),
                              sliderKnobGreen2.getWidth(), sliderKnobGreen2.getHeight(),
                              0, 0, sliderKnobGreen2.getWidth(),
                              sliderKnobGreen2.getHeight()); //pastel green
                  slider.setColour(juce::Slider::trackColourId , juce::Colour(0xff9ED8C3)); //pastel green
                }
                else if (slider.getValue() <= 10.0 || slider.getValue() == 1.01) {
                  g.drawImage(sliderKnobGrey2, (int)maxPoint.x - (sliderKnobGrey2.getWidth()/2),
                              (int)maxPoint.y - (sliderKnobGrey2.getHeight()/2),
                              sliderKnobGrey2.getWidth(), sliderKnobGrey2.getHeight(),
                              0, 0, sliderKnobGrey2.getWidth(),
                              sliderKnobGrey2.getHeight()); // grey
                  slider.setColour(juce::Slider::trackColourId , juce::Colour(0xffC4C4C4)); // grey
                }
            }
          }
        }

        //==============================================================================
        if (isTwoVal || isThreeVal)
        {
            auto sr = jmin (trackWidth, (slider.isHorizontal() ? (float) height : (float) width) * 0.4f);
            auto pointerColour = slider.findColour (Slider::thumbColourId);

            if (slider.isHorizontal())
            {
                drawPointer (g, minSliderPos - sr,
                             jmax (0.0f, (float) y + (float) height * 0.5f - trackWidth * 2.0f),
                             trackWidth * 2.0f, pointerColour, 2);

                drawPointer (g, maxSliderPos - trackWidth,
                             jmin ((float) (y + height) - trackWidth * 2.0f, (float) y + (float) height * 0.5f),
                             trackWidth * 2.0f, pointerColour, 4);
            }
            else
            {
                drawPointer (g, jmax (0.0f, (float) x + (float) width * 0.5f - trackWidth * 2.0f),
                             minSliderPos - trackWidth,
                             trackWidth * 2.0f, pointerColour, 1);

                drawPointer (g, jmin ((float) (x + width) - trackWidth * 2.0f, (float) x + (float) width * 0.5f), maxSliderPos - sr,
                             trackWidth * 2.0f, pointerColour, 3);
            }
        }
    }
}

//==============================================================================
Label* DJLookAndFeel::createSliderTextBox (Slider& slider)
{
    auto* l = LookAndFeel_V2::createSliderTextBox (slider);
    slider.setTextBoxStyle(juce::Slider::TextEntryBoxPosition::TextBoxBelow, false, 40, 15);

    if (getCurrentColourScheme() == LookAndFeel_V4::getGreyColourScheme() &&
        (slider.getSliderStyle() == Slider::LinearBar ||
         slider.getSliderStyle() == Slider::LinearBarVertical))
    {
        l->setColour (Label::textColourId, juce::Colour(0xff62636D).withAlpha (0.7f)); //pastel gray
        l->setColour(Label::backgroundColourId , juce::Colour(0xffF8FBFF)); //pastel white-blue
    }
    return l;
}
