#!/usr/bin/env bash


# Check if the script is run as root
if [[ $EUID -eq 0 ]]; then
    echo "Uninstallation cannot be performed as root"
    exit 1
fi


bindir=/usr/local/bin
mandir=/usr/local/man
srcdir=/usr/local/src
confdir=$HOME/.config


# Check if rls is installed
if [ ! -f "$bindir/rls" ]; then
    echo "rls seems not to be installed"
    exit 0
fi


# Remove rls from bin directory
sudo rm $bindir/rls
if [ $? -eq 0 ]; then
    echo "Removed rls from $bindir"
else
    echo "Cannot remove rls from $bindir"
    exit 1
fi


# Remove rls directory
rm -rf $confdir/rls
if [ $? -eq 0 ]; then
    echo "Removed rls directory under $confdir"
else
    echo "Cannot remove rls directory under $confdir"
    exit 1
fi

# Remove rls man pages
rm -f $mandir/man1/rls.1 $mandir/man7/rls.7
if [ $? -eq 0 ]; then
    echo "Removed rls man pages under $mandir"
else
    echo "Cannot remove rls man pages under $mandir"
    exit 1
fi

# Remove rls source code
rm -rf $srcdir/rls
if [ $? -eq 0 ]; then
    echo "Removed rls source code under $srcdir"
else
    echo "Cannot remove rls source code under $srcdir"
    exit 1
fi

echo "rls has been uninstalled successfully"
exit 0

