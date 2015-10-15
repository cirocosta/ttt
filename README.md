#ttt

> **Tic Tac Toe** over TCP (+TLS) and UDP.

## Table of Contents

<!-- START doctoc generated TOC please keep comment here to allow auto update -->
<!-- DON'T EDIT THIS SECTION, INSTEAD RE-RUN doctoc TO UPDATE -->

- [Runnning](#runnning)
- [Project Structure](#project-structure)
- [Server](#server)
- [Client](#client)
- [Structures](#structures)
- [Protocol](#protocol)
  - [Grammar](#grammar)
  - [Commands](#commands)
    - [REG](#reg)
    - [IN](#in)
    - [OUT](#out)
    - [HB](#hb)
    - [STATE](#state)
  - [Replies](#replies)
  - [LICENSE](#license)

<!-- END doctoc generated TOC please keep comment here to allow auto update -->

## Runnning

Make sure that you have OpenSSL installed. This is necessary for the TLS support (in Ubuntu-a-like sytems: `# apt-get install openssl libssl-dev`).

If you're willing to run a server, generate a certificate and a private key:

```sh
$ ./generate-cert.sh
```

This command will run `OpenSSL` and output the certificate and private key into `/certs`.

*(TODO: we should add a symmetric encryption step here so that the private key would be encrypted, increasing security)*

Now, build the project (out of tree):

```sh
$ mkdir build  $$ cd $_
$ cmake -Dtest=ON ..      # config for building w/ tests
#will build gtest as well
$ make -j4                # build w/ 4 cores
```

Then run the binaries:

```sh
$ ./ttt-server &      # run the server in the background
$ ./ttt-client        # initialize a client
```

## Project Structure

```
.
├── bin/       final executables 
├── certs/     certificate and private key generated
├── include/   header includes (ttt_INCLUDE_DIRS)
├── lib/       dependencies (gtest)
├── src/       obj sources  
└── tests/     tests
```

## Server

The server is responsible for centralizing all information regarding login, state of matches and players, as well as punctuation. If, for some reason, a player connection fails while the match was in its middle, and then the player who disconnected gets back and the other player stays online (not in `playing` state) then the match can be resumed (for such case we must implement some kind of 'cookie-a-like' system).

It must:
-   support TCP (+ TLS) and UDP connections
-   exchange data with client regarding:
  -   login
  -   logout
  -   heartbeat
  -   match state management
-   manage the state of connected players
  -   UDP players are only allowed to talk with other UDP players. The same for TCP. It's server responsibility to manage this.
-   keep a log file informing the events
-   keep a database for persisting user info

(ps: a client MUST NOT be capable of being logged at two clients at the same time).


## Client

Connects to the server, registers if it hadn't done before, selects a user to play against (from the list of available users), invites the user for a match, starts the match.

It must be capable of:

-   connecting through TCP or UDP with the server and other clients (for chatting purposes) - p2p
-   exchange text messages directly with other players w/out passing through the server
-   talk with the server for:
  -   login
  -   heartbeat
  -   logout
  -   state management
-   Encrypt user info and pwd


## Structures

```
Server
{
  ConnectionPtr udp; // connection handler
  ConnectionPtr tls; // connection handler
  std::map<int, ConnectionPtr> m_connections;

  std::vector<Room> rooms_list;
  std::vector<User> registered_users;
}

Room
{
  unsigned id;
  Board board;
  User userA; // X
  User userB; // O
}

Board 
{ 
  char game_data[9]; 
}

User
{
  unsigned id; // assigned on registration

  char* login;
  char* senha; // ps: shouldn't use plain text here
               // optimally we'd use bcrypt or something
               // like that

  unsigned score;   // (victory ==> 2 points, draw ==> 1 point)

  unsigned time_connected;

  Connection* current_connection; // also gives info about conn_type
  BOARD_MARK current_mark;        // O or X
  UserStatus status;              // (online|playing|offline)
}

Database 
{
  std::string fname;
  FILE* file;
}
```

## Protocol

-   user registration
-   login
-   logout
-   heartbeat
-   direct message exchange
-   list of players w/ the same transport protocol (and if they're busy or not)
-   match invite
-   request end of match(?)
-   inform where `X` or `O` was placed in the board
-   inform the Hall of Fame (in the server)

### Grammar

The grammar is made to be extremely simple to parse. All of the content is expected to be encoded in ASCII. The following grammar is written conforming to ABNF syntax rules (see [rfc 5234](https://tools.ietf.org/html/rfc5234)).

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


#### IN

> Performs a Login if the `usr` and `pwd` matches. If `usr` doesn't exist, creates a new user, associates it with the given `pwd` and logs in. Otherwise (wrong pwd), error.

```
client:
  CMD_IN:my_user_id:my_password:
server: (OK)
  RPL_WELCOME:id:score:
server: (ERROR)
  RPL_INPWD:Invalid Password:
  RPL_ALOGGED:Already Logged:
```

The client connection wants to log into the server with a given user.


#### OUT

> Logout
```
client:
  CMD_OUT:
server:
  RPL_BYE:You've been logged out. Closing connection:
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

`RPL_OK`

### LICENSE

Mozilla Public License Version 2.0
