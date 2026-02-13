#!/bin/bash
set -e # Stop if any command fails

# 1. Define paths
SFML_VERSION="3.0.2"
SFML_URL="https://github.com/SFML/SFML/releases/download/${SFML_VERSION}/SFML-3.0.2-Linux.GCC.tar.gz"
INSTALL_DIR="$HOME/SFML-3.0.2"

echo "--- 1. Installing System Dependencies ---"
# We need these so the SFML prebuilts can actually run and link
sudo apt update
sudo apt install -y build-essential cmake \
                     libx11-dev libxcursor-dev libxrandr-dev libxinerama-dev libxi-dev libgl1-mesa-dev libudev-dev \
                     libopengl-dev libflac-dev libvorbis-dev \
                     libopenal-dev libfreetype-dev

echo "--- 2. Downloading SFML Prebuilts ---"
if [ ! -d "$INSTALL_DIR" ]; then
    wget -O sfml.tar.gz $SFML_URL
    mkdir -p $INSTALL_DIR
    tar -xzf sfml.tar.gz -C $INSTALL_DIR --strip-components=1
    rm sfml.tar.gz
    echo "SFML installed to $INSTALL_DIR"
else
    echo "SFML already exists at $INSTALL_DIR, skipping download."
fi

echo "--- 3. Configuring and Building Project ---"
mkdir -p build && cd build

# We tell CMake exactly where we just put SFML
cmake .. -DCMAKE_BUILD_TYPE=Release \
         -DSFML_DIR="$INSTALL_DIR/lib/cmake/SFML"

make -j$(nproc)

echo "--- 4. Finalizing Assets ---"
# Create symlinks so the app finds assets relative to the binary
ln -sf ../wall_is_you/assets assets
ln -sf ../wall_is_you/dungeons dungeons

echo "----------------------------------------"
echo "Setup Complete! Run the game using:"
echo "./WallIsYouCpp"
echo "----------------------------------------"
