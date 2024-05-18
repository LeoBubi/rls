#!/usr/bin/env bash


# Check if the script is run as root
if [[ $EUID -eq 0 ]]; then
    echo "Installation cannot be performed as root."
    exit 1
fi


# Check if rls is installed already
if [ -d $HOME/.config/rls ]; then
    echo "rls seems to be installed already"
    exit 0
fi


# Check if /usr/local/bin exists
if [ ! -d "/usr/local/bin" ]; then
    echo "Error: /usr/local/bin directory does not exist."
    exit 1
fi


# Compile rls
make
if [ $? -ne 0 ]; then
    echo "Failed to compile rls"
    exit 1
fi

# Copy rls to /usr/local/bin
sudo mv rls /usr/local/bin
if [ $? -eq 0 ]; then
    echo "Installed rls to /usr/local/bin"
else
    echo "Failed to install rls to /usr/local/bin"
    exit 1
fi

# Remove object files
make cleaninstall
if [ $? -eq 0 ]; then
    echo "Removed object files"
else
    echo "Failed to remove object files"
    exit 1
fi


# Create rls directory
if [ ! -d $HOME/.config ]; then
    mkdir -p $HOME/.config/rls
else
    mkdir $HOME/.config/rls
fi

if [ $? -eq 0 ]; then
    echo "Created rls directory under $HOME/.config"
else
    echo "Failed to create rls directory"
    exit 1
fi


# Create rls default configuration file
touch $HOME/.config/rls/rls.conf
if [ $? -eq 0 ]; then
    echo "Created rls configuration file under $HOME/.config/rls"
else
    echo "Failed to create rls configuration file"
    exit 1
fi

# Initialize rls configuration file
echo "USERNAME=$(whoami)" > $HOME/.config/rls/rls.conf
echo "PORT=513" >> $HOME/.config/rls/rls.conf

if [ $? -eq 0 ]; then
    echo "Initialized rls configuration file"
else
    echo "Failed to initialize rls configuration file"
    exit 1
fi

echo "rls has been installed successfully"
exit 0

