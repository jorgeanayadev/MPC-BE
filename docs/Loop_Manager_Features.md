# MPC-BE Loop Manager and Export Features

This document describes the new loop management and export features added to MPC-BE.

## Features Overview

### 1. Multiple Loop Support
- Create and manage multiple named loops within a media file
- Each loop has a name, start time, end time, and enabled/disabled state
- Loops are displayed in a dedicated Loop Manager panel
- Double-click any loop to jump to its start position
- Automatic looping: when playback reaches the end of an active loop, it jumps back to the start

### 2. Enhanced Milliseconds Display
- **Already Available**: Right-click on time display → "Show milliseconds"
- Shows precise HH:MM:SS.mmm format for accurate loop boundary setting
- Automatically enabled when Subresync bar is visible
- Essential for frame-accurate loop creation

### 3. Loop Manager Panel
- **Access**: View menu → Loop Manager
- **Docking**: The panel can be docked like other MPC-BE panels (playlist, capture, etc.)

#### Interface Elements:
- **List View**: Shows all loops with columns for Name, Start, End, and Duration
- **Add Button**: Opens the Loop Edit dialog to create a new loop
- **Remove Button**: Deletes the selected loop
- **Edit Button**: Modifies the selected loop
- **Export Button**: Exports the selected loop as a separate video file

#### Keyboard Shortcuts:
- **Delete Key**: Remove selected loop
- **Enter Key**: Jump to selected loop
- **Shift+L**: Quick add a 10-second loop starting from current position

### 4. Loop Edit Dialog
- **Name Field**: Enter a descriptive name for the loop
- **Start Time**: Enter start time in HH:MM:SS.mmm format
- **End Time**: Enter end time in HH:MM:SS.mmm format
- **Current Position Buttons**: Set start/end times to current playback position
- **Validation**: Checks for valid time ranges and warns about overlapping loops

### 5. Visual Indicators
- **Seek Bar Markers**: Green markers show loop boundaries on the seek bar
- **AB Repeat Markers**: Red markers (existing feature) show A-B repeat points
- **Compatible Display**: Loop markers work alongside existing chapter and AB repeat markers

### 6. FFmpeg Export
- Export any loop as a separate video file
- Supports multiple formats: MP4, AVI, MKV
- Uses stream copy for fast, lossless extraction
- Enhanced error handling and validation
- Progress indication and detailed error messages
- Automatic file extension detection
- File size validation to ensure successful export

## Usage Workflow

### Creating Loops

#### Method 1 - Quick Loop (Recommended for fast workflow):
1. Navigate to desired start position
2. Press **Shift+L** to create a 10-second loop
3. The Loop Manager panel will open automatically
4. Edit the loop if needed for precise timing

#### Method 2 - Precise Loop:
1. Open **View → Loop Manager**
2. Click **"Add"** button
3. Set start position (use "Current Position" button or manual entry)
4. Set end position (use "Current Position" button or manual entry)
5. Enter descriptive name
6. Click **OK**

#### Method 3 - Menu Access:
- **Play → Repeat → Quick Add Loop**

### Managing Loops
- **Navigate**: Double-click any loop in the list to jump to its start
- **Edit**: Select loop and click "Edit" or double-click
- **Remove**: Select loop and click "Remove" or press Delete
- **Export**: Select loop and click "Export"

### Playback with Loops
- When a loop is active and playback reaches the end time, it automatically jumps to the start
- Multiple loops can coexist - the first matching loop will be used
- Loops work alongside the existing A-B repeat functionality
- No performance impact on normal playback

### Exporting Loops
1. Select the loop you want to export
2. Click **"Export"** button
3. Choose output format and location
4. Wait for FFmpeg processing to complete
5. The exported file will contain only the loop segment

## Technical Features

### Error Handling & Validation
- **Time Validation**: Ensures end time is after start time
- **Overlap Detection**: Warns when loops overlap (optional)
- **File Validation**: Checks input file existence and accessibility
- **Export Validation**: Verifies FFmpeg availability and successful export
- **Duration Warnings**: Alerts for very short loops (< 1 second)

### Performance Optimizations
- **Efficient Drawing**: Loop markers integrated into existing seek bar rendering
- **Memory Management**: Loops stored efficiently in memory
- **Timer Integration**: Loop checking uses existing playback timer
- **Stream Copy Export**: Fast export without re-encoding

### File Format Support
- **Input**: All formats supported by MPC-BE
- **Output**: Any format supported by FFmpeg
- **Default Extensions**: Automatically detects appropriate output format
- **Quality Preservation**: Stream copy maintains original quality

## Menu Locations

- **View → Loop Manager**: Open/close the Loop Manager panel
- **Play → Repeat → Quick Add Loop**: Create a quick 10-second loop
- **Right-click on time display → Show milliseconds**: Toggle millisecond precision

## System Requirements

### For Loop Management:
- No additional requirements
- Works with all MPC-BE supported formats
- Minimal memory and CPU overhead

### For Export Functionality:
- **FFmpeg**: Must be configured in MPC-BE settings
- **Disk Space**: Sufficient space for exported files
- **Processing Time**: Varies by file size and system performance

## Tips for Best Results

### Precision Loop Creation:
1. Enable millisecond display for precise timing
2. Use frame-by-frame navigation (Ctrl+Right/Left arrows)
3. Set loop boundaries at scene changes or natural breaks
4. Use descriptive names for easy identification

### Export Optimization:
- Use stream copy (default) for fastest export
- Choose appropriate output format for your needs
- Ensure sufficient disk space before large exports
- Close unnecessary applications during export

### Workflow Efficiency:
- Use Shift+L for quick loops during initial review
- Refine loop boundaries with the Edit dialog
- Organize loops with descriptive names
- Export commonly used segments for quick access

## Troubleshooting

### Common Issues:
- **Export fails**: Check FFmpeg configuration in settings
- **Loops not visible**: Ensure Loop Manager panel is open
- **Timing inaccurate**: Enable millisecond display
- **Performance issues**: Close unused panels and loops

### FFmpeg Configuration:
1. Download FFmpeg from official website
2. Extract to a permanent location
3. Configure path in MPC-BE settings
4. Test with a small export first

This implementation provides a comprehensive loop management and export system that integrates seamlessly with MPC-BE's existing functionality while adding powerful new capabilities for video analysis, editing workflows, and content extraction.