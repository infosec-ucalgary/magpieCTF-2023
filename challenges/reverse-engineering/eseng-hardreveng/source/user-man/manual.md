# Satshell Client - User Guide

### Introductary Letter

Hello Valued Employee!

Recieving this user manual solidifies your position as an OMNIFLAGS INC.
Communication Servicer. Your core responsibility will be ensuring communication
between the central communication server and the extra-terrestrial dataserver.
This user manual should provide necessary information for you to manage this
connection and ensure the efficient flag distribution our company provides. Your
work is integral to the future of our company, and we expect great results from
your job term. Any questions should be asked to your employer directly via
verbal communication.

### Enforcing the connection

As a Communication Servicer, your responsibility is to establish a connection
between the central communication server and the extra-terrestrial dataserver.
Your supervisor should have provided the appropriate IP address and port you
will need to configure your client program as necessary.

The following steps will describe the process to update your client's connection
to the central communication server.

1. Log-in to your OMNIFLAGS employee account
2. If you are initializing your workspace for the first time, click on
   "Configure". Otherwise, navigate to "Options > Network > Connection to
   Central Server > Reconfigure"
3. Within the "Host" input field, enter the IP address as provided by your
   employer
4. Within the "Port" input field, enter the port value as provided by your
   employer
5. Ensure your connection is properly established, start the client's built-in
   debug tool. This will open the debug terminal along with the list of commands
   to use.
6. Run a test command by entering "POSN" into the debug terminal. This should
   return with an appropriate output.
7. Confirm your changes by clicking "Confirm"

It will be important to report any system downtime immediately. Failure to
establish a connection without notifying your supervisor is inefficient and will
result in termination.

Connections should only be made to the server that your supervisor instructs you
to connect to. Unauthorized connections are considered mutiny and will result in
termination.

Connections must be made within a specific order. Satellites are typically
labelled such that connections should be made in a numericly or
lexicographically increasing order. For example, satellites labelled `[1, 2, 3,
4, 5]` must be connected such that satellite 1 projects information to satellite
2, satellite 2 projects information to satellite 3, 3 to 4 and 4 to 5. It may be
assumed that satellite 1 and satellite 5 already communicates to the central
communication server and the extra-terrestrial dataserver respectively.
Establishing this communication enables a bidirectional datastream between the
two servers. This allows all OMNIFLAGS employees to browse all shared files.

When pinging the positions of satellites, 
