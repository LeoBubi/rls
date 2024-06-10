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


confdir=$HOME/.config


# Remove rls directory if it exists
if [ -d $confdir/rls ]; then
    echo "rls directory already exists, do you want to remove it? (Y/n)"
    read -n 1 ans
    if [ "$ans" != "y" -a "$ans" != "Y" ]; then
        echo "Configuration aborted"
        exit 0
    fi
    rm -rf $confdir/rls
    if [ $? -eq 0 ]; then
        echo "Removed rls directory under $confdir"
    else
        echo "Failed to remove rls directory"
        exit 1
    fi
fi


# Create rls directory
mkdir -p $confdir/rls
if [ $? -eq 0 ]; then
    echo "Created rls directory under $confdir"
else
    echo "Failed to create rls directory"
    exit 1
fi


# Create rls default configuration file
touch $confdir/rls/rls.conf
if [ $? -eq 0 ]; then
    echo "Created rls configuration file under $confdir/rls"
else
    echo "Failed to create rls configuration file"
    exit 1
fi

# Initialize rls configuration file
echo "USRNAME=$(whoami)" > $confdir/rls/rls.conf
echo "SRVPORT=513" >> $confdir/rls/rls.conf
echo "CONNTMO=10" >> $confdir/rls/rls.conf

if [ $? -eq 0 ]; then
    echo "Initialized rls configuration file"
else
    echo "Failed to initialize rls configuration file"
    exit 1
fi

return 0
