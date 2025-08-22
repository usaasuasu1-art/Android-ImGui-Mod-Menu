#!/bin/bash

# Build script for ImGui Soft Keyboard Integration
# This script helps build the project with the new soft keyboard features

echo "Building ImGui Soft Keyboard Integration..."

# Check if we're in the right directory
if [ ! -f "app/build.gradle" ]; then
    echo "Error: Please run this script from the project root directory"
    exit 1
fi

# Clean previous builds
echo "Cleaning previous builds..."
./gradlew clean

# Build the project
echo "Building project..."
./gradlew assembleDebug

if [ $? -eq 0 ]; then
    echo "Build successful!"
    echo ""
    echo "The soft keyboard integration has been built successfully."
    echo ""
    echo "Features included:"
    echo "- Real Android soft keyboard support (not virtual keyboard)"
    echo "- Seamless ImGui integration"
    echo "- Automatic focus management"
    echo "- Full Unicode support"
    echo "- Multiple input types (text, number, email, password, phone)"
    echo "- Cursor position synchronization"
    echo "- Hint text support"
    echo ""
    echo "To test:"
    echo "1. Install the APK on your Android device"
    echo "2. Run the app"
    echo "3. Tap on any input field to see the soft keyboard"
    echo "4. Use the demo tabs to explore different input types"
    echo ""
    echo "Files created/modified:"
    echo "- app/src/main/java/com/reveny/imgui/mod/SoftKeyboardManager.java"
    echo "- app/src/main/java/com/reveny/imgui/mod/ImGuiKeyboardBridge.java"
    echo "- app/src/main/jni/Include/ImGuiSoftKeyboard.h"
    echo "- app/src/main/jni/Include/ImGuiSoftKeyboard.cpp"
    echo "- app/src/main/jni/Include/ImGuiSoftKeyboardJNI.cpp"
    echo "- app/src/main/jni/Include/ImGuiSoftKeyboardExample.h"
    echo "- app/src/main/jni/Include/ImGuiSoftKeyboardExample.cpp"
    echo "- app/src/main/jni/Main/Main.cpp (modified)"
    echo "- app/src/main/java/com/reveny/imgui/mod/MainActivity.java (modified)"
    echo "- app/src/main/jni/Build/Android.mk (modified)"
    echo ""
    echo "Documentation: SOFT_KEYBOARD_README.md"
else
    echo "Build failed! Please check the error messages above."
    exit 1
fi