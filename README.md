# Media Browser and Player CLI Application

## Overview

This application is a command-line interface (CLI) tool designed to browse, manage, and play media files on Linux. It supports both audio and video files and provides advanced features such as playlist management, metadata editing, and hardware control integration. The application can be run from any directory on the computer or from an external USB device.

## Features

### 1. Media File Browsing
- **Directory Navigation**: Browse media files (audio and video) in any directory on your system.
- **External Device Support**: Mount and parse media files from external USB devices.
- **Recursive Search**: View media files from the selected folder and all its sub-folders.
- **Pagination**: Display media files in pages, with up to 25 files per page.

### 2. Playlist Management
- **List Playlists**: View all available playlists.
- **View Playlist**: Display the contents of a selected playlist.
- **Create/Update/Delete Playlists**: Manage playlists by adding, updating, or removing entries.

### 3. Media File Metadata [Main Feature]
- **View Metadata**: Access detailed metadata for both audio and video files.
  - **Audio Metadata**: Track name, Album, Artist, Duration, Genre, Publisher, Publish Year, etc.
  - **Video Metadata**: Name, Size, Duration, Bitrate, Codec, etc.
- **Edit Metadata**: Modify existing metadata values or add new keys as needed.
- **TagLib Integration**: Utilize the Taglib library for handling media file tags and metadata.

### 4. Media Playback (SDL2 Integration)
- **Play Music**: Play audio files using the SDL2 library on a separate thread.
- **Playback Controls**:
  - **Play/Pause**: Start or pause the playback of the current track.
  - **Next/Previous**: Skip to the next or previous track.
  - **Automatic Track Transition**: Automatically move to the next song when the current one finishes.
- **Display Playback Info**: Show current time and total duration of the playing track.
- **Volume Control**: Adjust the device volume directly from the application.

### 5. Hardware Control Integration (S32K144 Board)
- **Volume Control by ADC**: Adjust the volume using the S32K144 board's Analog-to-Digital Converter (ADC).
- **Playback Control via Buttons**: Use physical buttons on the S32K144 board to control playback (play, pause, stop, next, previous).
- **Song Information Display**: Display the current song's information on the board's screen when playing.

## Requirements

### Software Dependencies
- **SDL2**: For media playback.
- **TagLib**: For reading and editing metadata.
- **g++**: GCC C++ compiler.
- **Make**: Build automation tool.

### Hardware Requirements
- **S32K144 Board**: For volume and playback control integration.

## Installation

1. **Clone the repository:**
    ```bash
    git clone <repository-url>
    cd <repository-directory>
    ```

2. **Install dependencies:**
    - Install `SDL2`:
      ```bash
      sudo apt-get install libsdl2-dev
      ```
    - Install `TagLib`:
      ```bash
      sudo apt-get install libtag1-dev
      ```

3. **Build the project:**
    ```bash
    make
    ```

4. **Run the application:**
    ```bash
    ./media_browser_player
    ```

## Usage

1. **Browse Media Files:**
   - Run the application and navigate through directories to find your media files.

2. **Manage Playlists:**
   - List, create, update, or delete playlists.

3. **View/Edit Metadata:**
   - Select a file to view or edit its metadata.

4. **Play Music:**
   - Use playback controls to play, pause, and skip tracks.

5. **Control with S32K144 Board:**
   - Adjust volume and control playback using the board's ADC and buttons.

## Contributing

1. Fork the repository.
2. Create a new branch (`git checkout -b feature-branch`).
3. Make your changes.
4. Commit your changes (`git commit -am 'Add new feature'`).
5. Push to the branch (`git push origin feature-branch`).
6. Create a new Pull Request.

## License

This project is licensed under the MIT License - see the [LICENSE](LICENSE) file for details.

## Acknowledgements

- [SDL2](https://www.libsdl.org/) for media playback.
- [TagLib](https://taglib.org/) for metadata management.
- Special thanks to the open-source community for tools and resources.
