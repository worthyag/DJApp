/*
  ==============================================================================

    PlaylistComponent.cpp
    Created: 21 Feb 2022 12:05:45pm
    Author:  Worthy

  ==============================================================================
*/

#include <JuceHeader.h>
#include "PlaylistComponent.h"

//==============================================================================
PlaylistComponent::PlaylistComponent(AudioFormatManager &   _formatManager) : formatManager(_formatManager)
{
    //==============================================================================
    // Open the saved file containing the tracks and meta data.
    std::string line;

    // Create the file name.
    juce::File f = juce::File::getCurrentWorkingDirectory();
    std::string fname = f.getFullPathName().toStdString();
    std::stringstream appendString;
    appendString << fname << "playlist.txt";
    std::string filename = appendString.str();

    currentPlaylistO.open(filename);

    std::vector<std::string> metaData;

    // Pushes saved info into vector.
    if (currentPlaylistO.is_open()) {
        while(getline(currentPlaylistO, line))
        {
            savedFiles.push_back(line);
        }
      
        // Iterates through vector containing the saved info.
        for (int i = 0; i < savedFiles.size(); i++) {

            std::stringstream extractData(savedFiles[i]);
            std::string metaDatum;

            while(getline(extractData, metaDatum, '*'))
            {
                metaData.push_back(metaDatum);
            }

            if (metaData.size() != 4) // error
            {
                std::cout << "Not equal: " << savedFiles[i] << std::endl;
                break;
            }

            // Adds the info to corresponding vectors to display in table.
            trackPaths.push_back(metaData[0]);
            trackTitles.push_back(metaData[1]);
            duration.push_back(metaData[2]);
            fileType.push_back(metaData[3]);

            filteredTrackPaths = trackPaths;
            filteredTracks = trackTitles;
            filteredDuration = duration;

            metaData.clear();

            // To check the data.
            std::cout << i+1 << ". " << savedFiles[i] << std::endl;
        }
        // Updates the table.
        tableComponent.updateContent();
        
        currentPlaylistO.close();
    }
    else std::cout << "Unable to open file- read ";

    //==============================================================================
    tableComponent.getHeader().addColumn("no.", 1, 30);
    tableComponent.getHeader().addColumn("Track title", 2, 360);
    tableComponent.getHeader().addColumn("Duration", 3, 100);
    tableComponent.getHeader().addColumn("Type", 4, 60);
    tableComponent.getHeader().addColumn("", 5, 70);
    tableComponent.setModel(this);
    addAndMakeVisible(tableComponent);

    //==============================================================================
    addTrackButton.setImages(true, true, true, addTrackBtn,
                             1.0, juce::Colours::transparentWhite,
                             addTrackOverBtn, 1.0, juce::Colour(),
                             addTrackOverBtn, 1.0, juce::Colour());
    addAndMakeVisible(addTrackButton);
    addTrackButton.addListener(this);

    //==============================================================================
    addMultipleButton.setImages(true, true, true, addMultipleTracksBtn,
                                1.0, juce::Colours::transparentWhite,
                                addMultipleTracksOverBtn, 1.0, juce::Colour(),
                                addMultipleTracksOverBtn, 1.0, juce::Colour());
    addAndMakeVisible(addMultipleButton);
    addMultipleButton.addListener(this);

    //==============================================================================
    removeTrackButton.setImages(true, true, true, removeTrackBtn,
                                1.0, juce::Colours::transparentWhite,
                                removeTrackOverBtn, 1.0, juce::Colour(),
                                removeTrackOverBtn, 1.0, juce::Colour());
    addAndMakeVisible(removeTrackButton);
    removeTrackButton.addListener(this);

    //==============================================================================
    logo.setImages(true, true, true, otoDecksLogo, 1.0,
                   juce::Colours::transparentWhite, otoDecksOverLogo,
                   1.0, juce::Colour(), otoDecksOverLogo, 1.0, juce::Colour());
    addAndMakeVisible(logo);

    //==============================================================================
    search.setColour(Label::textColourId, juce::Colour(0xff1F1C1C));
    search.setColour(Label::backgroundColourId, juce::Colour(0xff83BBEE));
    search.setColour(Label::outlineColourId, juce::Colour(0xff62636D));
    search.setColour(Label::backgroundWhenEditingColourId, juce::Colour(0xffDFEAF9));
    search.setColour(Label::textWhenEditingColourId, juce::Colour(0xff1F1C1C));
    search.setText("Search here...", NotificationType::dontSendNotification);
    search.setEditable(true, false, false);
    addAndMakeVisible(search);
    search.addListener(this);
}

PlaylistComponent::~PlaylistComponent()
{
    // Creates file with playlist info.
    
    // Creates file name.
    juce::File f = juce::File::getCurrentWorkingDirectory();
    std::string fname = f.getFullPathName().toStdString();
    std::stringstream appendString;
    appendString << fname << "playlist.txt";
    std::string filename = appendString.str();

    currentPlaylistI.open(filename, std::ios::out);

    if (currentPlaylistI.is_open()) {
        for (int i = 0; i < trackTitles.size(); i++) {
            currentPlaylistI << trackPaths[i] << "*" << trackTitles[i] << "*" << duration[i] << "*" << fileType[i] << std::endl;
        }
        currentPlaylistI.close();
    }
    else
        std::cout << "Unable to open file- write " << filename;
}

//==============================================================================
void PlaylistComponent::paint (juce::Graphics& g)
{
    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));// clear the background
    g.fillAll(juce::Colour(0xffDFEAF9)); //pastel yellow
    g.setColour (juce::Colour(0xff62636D)); //pastel dark grey
    g.drawRect (getLocalBounds(), 3.2);   // draw an outline around the component
    tableComponent.setColour(ListBox::backgroundColourId, juce::Colour(0xff83BBEE)); //pastel blue
    tableComponent.setColour(ListBox::outlineColourId, juce::Colour(0xff62636D)); // pastel dark grey
    tableComponent.setOutlineThickness(3);
}

void PlaylistComponent::resized()
{
    juce::Grid grid;
    using Track = juce::Grid::TrackInfo;
    using Fr = juce::Grid::Fr;

    grid.setGap(2_px);

    grid.templateRows = {
                            Track (Fr (1)), Track (Fr (2)),
                            Track (Fr (2)), Track (Fr (2)),
                            Track (Fr (2)), Track (Fr (2)),
                            Track (Fr (1)), Track (Fr (1))
                        }; //8 rows
    
    grid.templateColumns = {
                                Track (Fr (6)), Track (Fr (1)),
                                Track (Fr (1)), Track (Fr (1))
                            };

    grid.items = {
                    GridItem(&tableComponent).withArea(2, 1, 9, 2),
                    GridItem(&addTrackButton).withArea(2, 2, 2, 3),
                    GridItem(&addMultipleButton).withArea(2, 3, 2, 4),
                    GridItem(&removeTrackButton).withArea(2, 4, 2, 5),
                    GridItem(&logo).withArea(5, 3, 8, 6),
                    GridItem(&search).withArea(1, 1, 1, 2)
                 };

    grid.performLayout (getLocalBounds());
}

//==============================================================================
int PlaylistComponent::getNumRows ()
{
    return filteredTracks.size(); //the numbers of rows is equal to the number of tracks
}

void PlaylistComponent::paintRowBackground (Graphics & g,
                                            int rowNumber,
                                            int width,
                                            int height,
                                            bool rowIsSelected)
{
    // just highlight selected rows
    if (rowIsSelected)
    {
        g.fillAll(juce::Colour(0xff80818A)); // pastel dark grey
        g.setColour(juce::Colour(0xffFFFFFF));
        g.setFont(21.0f);
    }
    else {
        g.fillAll(juce::Colour(0xff83BBEE)); //pastel blue
        g.setFont(20.0f);
        g.setColour(juce::Colour(0xffFFFFFF)); // pastel dark grey
    }
}

void PlaylistComponent::paintCell (Graphics & g,
                                  int rowNumber,
                                  int columnId,
                                  int width,
                                  int height,
                                  bool rowIsSelected)
{
    if (columnId == 1) {
        auto rowNum = rowNumber + 1;
        g.drawText (String(rowNum), // the important bit
                    2, 1,
                    width - 5, height,
                    Justification::centredLeft,
                true);
    }

    if (columnId == 2 && (rowNumber < getNumRows())) {
        g.drawText (filteredTracks[rowNumber], // the important bit
                    2, 0,
                    width - 5, height,
                    Justification::centredLeft,
                    true);
    }

    if (columnId == 3) {
        g.drawText (filteredDuration[rowNumber], // the important bit
                    2, 1,
                    width - 5, height,
                    Justification::centredLeft,
                    true);
    }

    if (columnId == 4) {
        g.drawText (fileType[rowNumber], // the important bit
                    2, 1,
                    width - 5, height,
                    Justification::centredLeft,
                    true);
    }
}

void PlaylistComponent::cellClicked (int rowNumber, int columnId, const MouseEvent &)
{
    // worksheet challenge
    std::cout << "Track tile: " << rowNumber << ", Column ID: " << columnId << std::endl;
}

Component* PlaylistComponent::refreshComponentForCell (
                              int rowNumber,
                              int columnId,
                              bool isRowSelected,
                              Component *existingComponentToUpdate)
{
    if (columnId == 5)
    {
        if (existingComponentToUpdate == nullptr)
        {
            TextButton* btn = new TextButton("delete");
            existingComponentToUpdate = btn;
            btn->setColour(TextButton::buttonColourId, juce::Colour(0xffF26B6B)); //pastel red
            btn->setColour(ComboBox::outlineColourId, juce::Colour(0xff62636D)); //dark grey
            btn->addListener(this);
            String id{std::to_string(rowNumber)};
            btn->setComponentID(id);
            deleteButtons.push_back(btn);
        }
    }
    return existingComponentToUpdate;
}

//==============================================================================
void PlaylistComponent::buttonClicked(Button* button)
{
    //==============================================================================
    if (button == &addTrackButton)
    {
        DBG(" MainComponent::buttonClicked: addTrackButton");
        FileChooser chooser{"Select a file..."};

        if (chooser.browseForFileToOpen())
        {
            files.push_back(chooser.getResult());

            PlaylistComponent::setTracks(files);
            PlaylistComponent::setDuration(files);
            PlaylistComponent::setFileType(files);
            
            // to empty the vector, so that tracks don't get added back when they're deleted
            files.clear();
        }
    }

    //==============================================================================
    if (button == &addMultipleButton)
    {
        DBG(" MainComponent::buttonClicked: addMultipleButton");
        FileChooser chooser{"Select files..."};

        if (chooser.browseForMultipleFilesToOpen())
        {
            // saves tracks to vector
            for (int i = 0; i < chooser.getResults().size(); i++)
            {
                files.push_back(chooser.getResults()[i]);
            }

            PlaylistComponent::setTracks(files);   // to set the track titles
            PlaylistComponent::setDuration(files); // to set the track length
            PlaylistComponent::setFileType(files); // to set the track type
              
            // to empty the vector, so that tracks don't get added back when they're deleted
            files.clear();
        }
    }

    //==============================================================================
    if (button == &removeTrackButton)
    {
        DBG(" MainComponent::buttonClicked: removeTrackButton");
        if (getNumRows() >= 1)
        {
            std::cout << "The track " << trackTitles[trackTitles.size() - 1] << " was deleted." << std::endl;
              
            trackTitles.erase(trackTitles.end() - 1);
            filteredTracks.erase(filteredTracks.end() - 1);

            trackPaths.erase(trackPaths.end() - 1);
            filteredTrackPaths.erase(filteredTrackPaths.end() - 1);
        }
        tableComponent.updateContent();
    }

    //==============================================================================
    if ((button != &addTrackButton) && (button != &addMultipleButton) &&(button != &removeTrackButton))
    {
        std::string id = button->getComponentID().toStdString();
        int intID = std::stoi(id);
        DBG("PlaylistComponent::buttonClicked " << trackTitles[intID]);
        std::cout << "The track " << trackTitles[intID] << " was deleted." << std::endl;

        trackTitles.erase(trackTitles.begin() + intID);
        filteredTracks.erase(filteredTracks.begin() + intID);

        trackPaths.erase(trackPaths.begin() + intID);
        filteredTrackPaths.erase(filteredTrackPaths.begin() + intID);

        tableComponent.updateContent();
    }
}

//==============================================================================
void PlaylistComponent::setTracks(std::vector<File> trackFiles)
{
    for (int i = 0; i < trackFiles.size(); i++)
    {
        trackString = trackFiles[i].getFileNameWithoutExtension().toStdString();
        trackPath = trackFiles[i].getFullPathName().toStdString();

        /* Stops user from selected a track that they have already added*/
        if (std::find(std::begin(trackTitles), std::end(trackTitles), trackString) != std::end(trackTitles))
            continue;
        else {
            trackTitles.push_back(trackString);
            filteredTracks = trackTitles;
            trackPaths.push_back(trackPath);
            filteredTrackPaths = trackPaths;
        }
    }
    tableComponent.updateContent();

}

void PlaylistComponent::setDuration (std::vector<File> trackDurations) {
    for (int i = 0; i < trackDurations.size(); i++)
    {
        URL fileURL = URL(trackDurations[i]);
        double lengthSeconds = 0;

        AudioFormatReader* rd = formatManager.createReaderFor(fileURL.createInputStream(false));

        if (rd != nullptr) // good file!
        {
            lengthSeconds = rd->lengthInSamples / rd->sampleRate;
            delete rd;

            /* To hold the corresponding int values and later
            format for time*/
            std::string minDuration;
            std::string secDuration;
            std::string hourDuration;

            /*
            Rounding adds a slight degree in accuracy (noticed 1 difference in
             20 songs)
            */
            int seconds = (int)(round(lengthSeconds * 100) / 100);
            int secRemain = seconds % 60;
            int minutes = (seconds - secRemain) / 60;
            int minRemain = minutes % 60;
            int hours = (minutes - minRemain) / 60;

            // To format the time
            if (hours < 10) {
                hourDuration = "0" + std::to_string(hours);
            }
            else {
                hourDuration = std::to_string(hours);
            }

            if (minRemain < 10) {
                minDuration = "0" + std::to_string(minRemain);
            }
            else {
                minDuration = std::to_string(minRemain);
            }

            if (secRemain < 10) {
                secDuration = "0" + std::to_string(secRemain);
            }
            else {
                secDuration = std::to_string(secRemain);
            }

            // Create typical time styling
            std::stringstream calculateTime;
            calculateTime << hourDuration << ":" << minDuration << ":" << secDuration;

            std::string trackDuration = calculateTime.str();

            // for testing
            std::cout << seconds << " seconds\n";
            std::cout << trackDuration << "\n";

            // Add to vector
            duration.push_back(trackDuration);
            filteredDuration = duration;
        }
    }
    tableComponent.updateContent();
}

void PlaylistComponent::setFileType(std::vector<File> trackTypes)
{
    for (int i = 0; i < trackTypes.size(); i++)
    {
        fileType.push_back(trackTypes[i].getFileExtension().toStdString());
    }
    tableComponent.updateContent();
}
  
//==============================================================================
var PlaylistComponent::getDragSourceDescription(const SparseSet< int > &selectedRows)
{
    StringArray rows;

    for (int i = 0; i < selectedRows.size(); ++i) {
        int rowNum = selectedRows[i]; //used to be selectedRows[0]

        String selected = String(filteredTrackPaths[rowNum]);
        DBG("getDragSourceDescription" << selected);

        rows.add(selected);
    }
    return rows.joinIntoString(", ");
}

//==============================================================================
void PlaylistComponent::deleteKeyPressed(int lastRowSelected)
{
    std::cout << "Row number " << lastRowSelected << " was deleted" << std::endl;

    trackTitles.erase(trackTitles.begin() + lastRowSelected);
    filteredTracks.erase(filteredTracks.begin() + lastRowSelected);

    trackPaths.erase(trackPaths.begin() + lastRowSelected);
    filteredTrackPaths.erase(filteredTrackPaths.begin() + lastRowSelected);

    tableComponent.updateContent();
}

//==============================================================================
void PlaylistComponent::labelTextChanged (Label *labelThatHasChanged)
{
    std::string userEntry = labelThatHasChanged->getText(true).toStdString();
    if (userEntry != "")
    {
        std::cout << "Text changed " << userEntry << std::endl;
        filteredTracks.clear();
        filteredTrackPaths.clear();
        filteredDuration.clear();

        for (int i = 0; i < trackTitles.size(); i++)
        {
            filterString = trackTitles[i];
            filterPath = trackPaths[i];
            durationString = duration[i];

            if(filterString.find(userEntry) != std::string::npos)
            {
                filteredTracks.push_back(filterString);
                filteredTrackPaths.push_back(filterPath);
                filteredDuration.push_back(durationString);
            }
            else
                continue;
        }
        tableComponent.updateContent();
  }
  
  if (userEntry == "")
  {
    std::cout << "Search is empty " << std::endl;
    filteredTracks = trackTitles;
    filteredTrackPaths = trackPaths;
    filteredDuration = duration;
    tableComponent.updateContent();
  }
}
