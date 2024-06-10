#!/usr/bin/env bash


# Check if the script is run as root
if [[ $EUID -eq 0 ]]; then
    echo "Installation cannot be performed as root"
    exit 1
fi

# Check if HOME environment variable is set
if [ -z "$HOME" ]; then
    echo "HOME environment variable is not set"
    exit 1
fi


bindir=/usr/local/bin
mandir=/usr/local/share/man
srcdir=/usr/local/src
confdir=$HOME/.config


# Check if rls is installed already
if [ -f "$bindir/rls" ]; then
    echo "rls seems to be installed already. To configure it, run reconfig.sh"
    exit 0
fi

# Check if make is installed
if [ ! -x "$(which make)" ]; then
    echo "make is not installed"
    exit 1
fi



bin=1
man=1
src=1
conf=1
missing=0


# Check if bin directory exists
if [ ! -d $bindir ]; then
    bin=0
    missing=1
fi

# Check if man directory exists
if [ ! -d $mandir ]; then
   man=0
   missing=1
fi

# Check if src directory exists
if [ ! -d $srcdir ]; then
    src=0
    missing=1
fi

#check if configuration directory exists
if [ ! -d $confdir ]; then
    conf=0
    missing=1
fi

if [ $missing -eq 1 ]; then
    echo "The following directories are going to be created:"
    if [ $bin -eq 0 ]; then
        echo "    $bindir"
    fi
    if [ $man -eq 0 ]; then
        echo "    $mandir"
    fi
    if [ $src -eq 0 ]; then
        echo "    $srcdir"
    fi
    if [ $conf -eq 0 ]; then
        echo "    $confdir"
    fi
    echo "Continue? (Y/n)"
    
    read -n 1 ans
    if [ "$ans" != "y" -a "$ans" != "Y" ]; then
        echo "Installation aborted"
        exit 0
    fi

    if [ $bin -eq 0 ]; then
        mkdir -p $bindir
        if [ $? -eq 0 ]; then
            echo "Created $bindir"
        else
            echo "Failed to create $bindir"
            exit 1
        fi
    fi
    if [ $man -eq 0 ]; then
        mkdir -p $mandir
        if [ $? -eq 0 ]; then
            echo "Created $mandir"
        else
            echo "Failed to create $mandir"
            exit 1
        fi
    fi
    if [ $src -eq 0 ]; then
        mkdir -p $srcdir
        if [ $? -eq 0 ]; then
            echo "Created $srcdir"
        else
            echo "Failed to create $srcdir"
            exit 1
        fi
    fi
    if [ $conf -eq 0 ]; then
        mkdir -p $confdir
        if [ $? -eq 0 ]; then
            echo "Created $confdir"
        else
            echo "Failed to create $confdir"
            exit 1
        fi
    fi
fi


# Compile rls
echo ""
echo "Compiling rls..."
make
if [ $? -ne 0 ]; then
    echo "Failed to compile rls"
    exit 1
fi
echo "...done"
echo ""

# Install rls in bin directory
sudo mv rls $bindir/rls
if [ $? -eq 0 ]; then
    echo "Installed rls to $bindir"
else
    echo "Failed to install rls to $bindir"
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


# Configure rls
./config.sh
if [ $? -ne 0 ]; then
    echo "Failed to configure rls"
    exit 1
fi

# Copy rls(1) to man directory
if [ ! -d "$mandir/man1" ]; then
    sudo mkdir $mandir/man1
    if [ $? -eq 0 ]; then
        echo "Created $mandir/man1 directory"
    else
        echo "Failed to create $mandir/man1 directory"
        exit 1
    fi
fi
sudo cp man/rls.1 $mandir/man1
if [ $? -eq 0 ]; then
    echo "Moved rls(1) to $mandir/man1"
else
    echo "Failed to move rls(1) to $mandir/man1"
    exit 1
fi

# Copy rls(7) to /usr/local/share/man/man7
if [ ! -d "$mandir/man7" ]; then
    sudo mkdir $mandir/man7
    if [ $? -eq 0 ]; then
        echo "Created $mandir/man7 directory"
    else
        echo "Failed to create $mandir/man7 directory"
        exit 1
    fi
fi
sudo cp man/rls.7 $mandir/man7
if [ $? -eq 0 ]; then
    echo "Moved rls(7) to $mandir/man7"
else
    echo "Failed to move rls(7) to $mandir/man7"
    exit 1
fi

# Copy source code to src directory
sudo mkdir $srcdir/rls
if [ $? -eq 0 ]; then
    echo "Created $srcdir/rls"
else
    echo "Failed to create $srcdir/rls"
    exit 1
fi
sudo cp -r include $srcdir/rls/include
sudo cp -r src $srcdir/rls/src
if [ $? -eq 0 ]; then
    echo "Copied source code to $srcdir/rls"
else
    echo "Failed to copy source code to $srcdir/rls"
    exit 1
fi


echo ""
echo "rls has been installed successfully"
echo ""
echo "    Default username: $(whoami)"
echo "    Default port: 513"
echo "    Default server communication delay limit: 10 seconds"
echo ""

exit 0

