/*
  ==============================================================================

    PlaylistComponent.h
    Created: 21 Feb 2022 12:05:45pm
    Author:  Worthy

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include <algorithm>
#include "DeckGUI.h"
#include "DJAudioPlayer.h"
#include "WaveformDisplay.h"
#include <cmath>
#include <sstream>
#include <fstream>

using namespace juce;

//==============================================================================
class PlaylistComponent  : public juce::Component,
                           public juce::TableListBoxModel,
                           public Button::Listener,
                           public Label::Listener,
                           public TextEditor::Listener
{
  
public:
    //==============================================================================
    PlaylistComponent(AudioFormatManager &   _formatManager);
    ~PlaylistComponent() override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;
    
    //==============================================================================
    // Returns the number of items in the list.
    int getNumRows () override;
    // Draws the background behind one of the rows in the table.
    void paintRowBackground (Graphics &,
                            int rowNumber,
                            int width,
                            int height,
                            bool rowIsSelected) override;
    // Draws one of the cells.
    void paintCell (Graphics &,
                    int rowNumber,
                    int columnId,
                    int width,
                    int height,
                    bool rowIsSelected) override;
    // This callback is made when the user clicks on one of the cells in the table.
    virtual void cellClicked (int rowNumber, int columnId, const MouseEvent &) override;
  
    //==============================================================================
    // Creates or updates a custom component to go in a cell.
    Component* refreshComponentForCell (int rowNumber,
                                        int columnId,
                                        bool isRowSelected,
                                        Component *existingComponentToUpdate) override;
    // Called when the button is clicked
    void buttonClicked(Button* button) override;

    //==============================================================================
    // Sets the track titles to be displayed in the table.
    void setTracks( std::vector<File>);
    // Sets the track duration to be displayed in the table.
    void setDuration( std::vector<File>);
    // Sets the track type to be displayed in the table.
    void setFileType( std::vector<File> );

    //==============================================================================
    // Allows rows from your list to be dragged-and-dropped.
    virtual var getDragSourceDescription(const SparseSet< int > &currentlySelectedRows) override;
  
    //==============================================================================
    // Informs when the delete key is pressed.
    virtual void deleteKeyPressed(int lastRowSelected) override;
    
    //==============================================================================
    // Called when a Label's text has changed.
    virtual void labelTextChanged (Label *t) override;

    
private:
    //==============================================================================
    // For file creation
    std::ifstream currentPlaylistO;
    std::ofstream currentPlaylistI;
  
    //==============================================================================
    // Table component
    TableListBox tableComponent;
  
    //==============================================================================
    // To save file strings
    std::vector<std::string> savedFiles;
  
    //==============================================================================
    // Data for table
    std::vector<std::string> trackPaths;
    std::vector<std::string> trackTitles;
    std::vector<std::string> duration;
    std::vector<std::string> fileType;
    std::vector<juce::TextButton*> deleteButtons;
  
    //==============================================================================
    // Filtered data for table
    std::vector<std::string> filteredTrackPaths;
    std::vector<std::string> filteredTracks;
    std::vector<std::string> filteredDuration;
  
    //==============================================================================
    // Where users choosen songs gets saved to.
    std::vector<File> files;
  
    //==============================================================================
    // For searching and filtering.
    std::string trackString;
    std::string filterString;
    std::string trackPath;
    std::string filterPath;
    std::string durationString;
  
    //==============================================================================
    // Image buttons for playlist functions.
    ImageButton addTrackButton{ "Add Track" };
    Image addTrackBtn = ImageCache::getFromMemory(BinaryData::AddTrackBtn_png, BinaryData::AddTrackBtn_pngSize);
    Image addTrackOverBtn = ImageCache::getFromMemory(BinaryData::AddTrackOverBtn_png, BinaryData::AddTrackOverBtn_pngSize);
  
    ImageButton addMultipleButton{ "Add Multiple Tracks" };
    Image addMultipleTracksBtn = ImageCache::getFromMemory(BinaryData::AddMultipleTracksBtn_png, BinaryData::AddMultipleTracksBtn_pngSize);
    Image addMultipleTracksOverBtn = ImageCache::getFromMemory(BinaryData::AddMultipleTracksOverBtn_png, BinaryData::AddMultipleTracksOverBtn_pngSize);
  
    ImageButton removeTrackButton{"Remove Track"};
    Image removeTrackBtn = ImageCache::getFromMemory(BinaryData::RemoveTrackBtn_png, BinaryData::RemoveTrackBtn_pngSize);
    Image removeTrackOverBtn = ImageCache::getFromMemory(BinaryData::RemoveTrackOverBtn_png, BinaryData::RemoveTrackOverBtn_pngSize);
  
    //==============================================================================
    // For extracting the track length.
    AudioFormatManager& formatManager;
    
    //==============================================================================
    // Logo for app.
    ImageButton logo{ "Logo" };
    Image otoDecksLogo = ImageCache::getFromMemory(BinaryData::OtoDecksLogo_png, BinaryData::OtoDecksLogo_pngSize);
    Image otoDecksOverLogo = ImageCache::getFromMemory(BinaryData::OtoDecksOverLogo_png, BinaryData::OtoDecksOverLogo_pngSize);
    
    //==============================================================================
    // For searching and filtering.
    juce::Label search;
  
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (PlaylistComponent)
};

