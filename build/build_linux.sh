# Get the script directory and source directory
SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
SOURCE_DIR="$(dirname "$SCRIPT_DIR")"

cd ..
mkdir -p /tmp/tmp_build
cd /tmp/tmp_build

# Search for Qt installation
QT_FOUND=0
QT_PREFIX_PATH=""

# First, check for specific Qt installation
if [ -d "/home/hors/Qt/5.15.2/gcc_64" ] && [ -f "/home/hors/Qt/5.15.2/gcc_64/bin/qmake" ]; then
    QT_PREFIX_PATH="/home/hors/Qt/5.15.2/gcc_64"
    QT_VERSION=$("/home/hors/Qt/5.15.2/gcc_64/bin/qmake" -query QT_VERSION 2>/dev/null || echo "5.15.2")
    QT_FOUND=1
    echo "Found Qt $QT_VERSION in custom installation: $QT_PREFIX_PATH"
# Check if qmake is available in PATH
elif command -v qmake >/dev/null 2>&1; then
    QT_PREFIX_PATH=$(qmake -query QT_INSTALL_PREFIX)
    QT_VERSION=$(qmake -query QT_VERSION)
    QT_FOUND=1
    echo "Found Qt $QT_VERSION in system PATH: $QT_PREFIX_PATH"
elif command -v qmake-qt5 >/dev/null 2>&1; then
    QT_PREFIX_PATH=$(qmake-qt5 -query QT_INSTALL_PREFIX)
    QT_VERSION=$(qmake-qt5 -query QT_VERSION)
    QT_FOUND=1
    echo "Found Qt5 $QT_VERSION in system PATH: $QT_PREFIX_PATH"
else
    # Search for Qt in common Linux installation paths
    for qt_path in "/usr" "/usr/local" "/opt/Qt5" "/opt/qt5" "/usr/lib/qt5" "/usr/lib/x86_64-linux-gnu/qt5" \
                   "/home/$USER/Qt/5.15.3/gcc_64" "/home/$USER/Qt/5.15.2/gcc_64" "/home/$USER/Qt/5.15.1/gcc_64" \
                   "/home/$USER/Qt/5.14.2/gcc_64" "/home/$USER/Qt/5.12.12/gcc_64"; do
        if [ -d "$qt_path" ] && ([ -f "$qt_path/bin/qmake" ] || [ -f "$qt_path/lib/cmake/Qt5/Qt5Config.cmake" ]); then
            QT_PREFIX_PATH="$qt_path"
            QT_FOUND=1
            if [ -f "$qt_path/bin/qmake" ]; then
                QT_VERSION=$("$qt_path/bin/qmake" -query QT_VERSION 2>/dev/null || echo "Unknown")
            else
                QT_VERSION="Unknown"
            fi
            echo "Found Qt $QT_VERSION in: $qt_path"
            break
        fi
    done
fi

if [ $QT_FOUND -eq 0 ]; then
    echo "Warning: Qt not found! Build may fail."
    echo "Please install Qt5 development packages or set CMAKE_PREFIX_PATH manually."
    cmake -DCMAKE_BUILD_TYPE=MinSizeRel "$SOURCE_DIR"
else
    echo "Using Qt installation: $QT_PREFIX_PATH"
    # Use the detected Qt installation
    cmake -DCMAKE_BUILD_TYPE=MinSizeRel -DCMAKE_PREFIX_PATH="$QT_PREFIX_PATH" "$SOURCE_DIR"
fi
make
cpack
cd ..
cp -Rf /tmp/tmp_build/packages .

