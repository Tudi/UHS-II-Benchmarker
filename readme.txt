Description
------------
Project goal is to be able to benchmark Read / Write speed of SD cards without any additional software layers. Project needs to have it's own optimized implementation of communication layers to remove added latency.

Milestones
----------
- read file of Link layer commands and packetize them to Physical layer
- read Physical layer packets and unpack them to Link layer commands
- initialize a lane for half duplex mode
- write something on a lane
- read something from a lane
- add higher level Link Layer to group commands for simplified expressions
- add very high level "language" to describe complex behavior : initialize, read, write, reinitialize

