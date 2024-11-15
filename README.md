# rls

rls is a program for logging into and for executing commands on a remote machine running **rls-server**.

Client-server communication is **unencrypted**. Therefore, it is strongly recommended to make sure that such a communication is between two trusted hosts over a secure network.

### Installation

To install **rls**, please run the installation script **install.sh** without root privileges. If it is installed already but not configured for this user, run **config.sh** instead.

The provided Makefile has to be used only by the installation script, so refrain from running **make** manually.

To run **rls** in debugging mode (i.e. with extended verbosity) please add
```
-D __DEBUG
```
to the `CFLAGS` in the Makefile.

### Uninstallation

To uninstall **rls**, please run the uninstallation script **uninstall.sh** without root privileges.

### Manual

All the necessary documentation is provided in the **rls** manual pages.

`rls(1)`  provides usage information.

`rls(7)`  provides details about the internal structure and functioning of the client.

These manual pages are installed in `/usr/local/share/man`. Please make sure to have it in your **MANPATH**.
