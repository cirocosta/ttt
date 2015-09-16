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
  -   heartbeat
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

### Grammar

The grammar is made to be extremely simple to parse. All of the content is expected to be encoded in ASCII. The following grammar is written conforming to ABNF syntax rules (see https://tools.ietf.org/html/rfc5234).

`message`s are expected to be at max 512bytes long, ending with CRLF.

CRLF can be understood as a message separator.

```
message = cmd args crlf
cmd = (command / reply) ":"
args = *(arg ":")

command = "CMD_" 2*(ALPHA)
reply = "RPL_" 2*(ALPHA)
arg = 1*(%x01-39 / %x3B-FF)     ; any octet except NULL and ":"

crlf = "%x0D %x0A"              ; '\r\n'
```

### Commands

Commands are intended to be "named requests". Some of them are not intended to generate a reply. Replies are specified in [replies](#replies) section.


#### REG

```
client:
  CMD_REG:my_user_id:my_password:
server:
  RPL_OK:Registration Ok!:
```

> Request for registration


#### IN

> Login

```
client:
  CMD_IN:my_user_id:my_password:
server:
  RPL_OK:Welcome!:
  RPL_LIST_START:Here comes the list of users:
  RPL_USER:user_id:type_of_conn:status:
  ...
  RPL_LIST_END:
```

The client connection wants to log into the server with a given user.

#### OUT

> Logout
```
client:
  CMD_OUT:
server:
  RPL_OK:You've been logged out. Closing connection:
```

#### HB

> Heartbeat

// TODO perform clock sync here?

```
server:
  CMD_HB:
client:
  RPL_HB:
```

#### STATE

> Transmits the state. 

// TODO synchronize clocks?


### Replies

// TODO gather those replies specified above.

`RPL_`

### LICENSE

Mozilla Public License Version 2.0

