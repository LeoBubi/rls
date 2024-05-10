# rls
## NAME
`rls` - Remote Login Shell

## SYNOPSIS
`rls destination [-l username] [-p port]`

## DESCRIPTION
A simple program to start a terminal session on a remote server. `destination` can be either the IP address or the DNS name of the remote server.

Options:

-l `username`    Log in as `username`. This overrides the default which the name of the user invoking `rls`.
-p `port`        Connect to the server on port `port`. This overrides the default which is **login**.

## FILES
~/.rls    rls configuration file

## AUTHOR
Written by Leonardo Brugnano <brugnano.2128518@studenti.uniroma1.it>.
