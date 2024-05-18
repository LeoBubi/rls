#!/usr/bin/env bash


# Check if the script is run as root
if [[ $EUID -eq 0 ]]; then
    echo "Uninstallation cannot be performed as root"
    exit 1
fi


# Check if rls is installed
if [ ! -d "$HOME/.config/rls" ] && [ ! -f /usr/local/bin/rls ]; then
    echo "rls seems not to be installed"
    exit 0
fi


# Remove rls from /usr/local/bin
sudo rm /usr/local/bin/rls
if [ $? -eq 0 ]; then
    echo "Removed rls from /usr/local/bin"
else
    echo "Cannot remove rls from /usr/local/bin"
    exit 1
fi


# Remove rls directory
rm -rf $HOME/.config/rls
if [ $? -eq 0 ]; then
    echo "Removed rls directory under $HOME/.config"
else
    echo "Cannot remove rls directory under $HOME/.config"
    exit 1
fi

echo "rls has been uninstalled successfully"
exit 0

