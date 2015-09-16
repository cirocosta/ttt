# ttt

> Tic Tac Toe over TCP and UDP

Over TCP all user info (and password) are encrypted w/ TLS.

## server

Centralizes all information about login, state of matches and players and the pontuation. If for some reason a player connection fails for some reason and the match was in its middle, if the player who disconnected gets back and the other player stays without playing with others then the match can be resumed.

It must:
-   support TCP and UDP connections
  -   if TCP: user info and pswd encrypted
-   exchange data with client regarding:
  -   login
  -   logout
  -   heatbeat
  -   state management
-   manage the state of connected players
  -   UDP players are only allowed to talk with other UDP players. The same for TCP. It's server responsibility to manage this.
-   keep a log file informing the events

## client

Conects to the server and from a list of connected players it can choose an adversary to play against.

-   connect through TCP or UDP with the server and other clients (for chatting purposes) - p2p
-   exchange text messages directly with other players w/out passing through the server
-   talk with the server:
  -   login
  -   heartbeat
  -   logout
  -   state management
-   Encrypt user info and pwd


## protocol

-   user registration
-   login
-   logout
-   heartbeat
-   direct message exchange
-   list of players w/ the same transport protocol (and if they're busy or not)
-   invite a player for a match
-   request end of match
-   inform where `X` or `O` was placed in the board
-   inform the Hall of Fame



### LICENSE

Mozilla Public License Version 2.0

