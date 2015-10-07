#ifndef TTT__CONSTANTS_HH
#define TTT__CONSTANTS_HH

#define TTT_MAX_BACKLOG 16
#define TTT_DEFAULT_PORT 1337
#define TTT_MAX_BUFSIZE 2048
#define TTT_MAX_MESSAGE_SIZE 512

namespace ttt
{

namespace net
{
const static char* TLS_FNAME_CERTIFICATE = "./certs/cert.pem";
const static char* TLS_FNAME_PKEY = "./certs/private.pem";

// TODO maybe we should remove TLS from here ...
typedef enum ConnectionType {
  UDP_ACTIVE,
  UDP_PASSIVE,
  TCP_ACTIVE,
  TCP_PASSIVE,
  TLS_ACTIVE,
  TLS_PASSIVE,
} ConnectionType;
}

namespace protocol
{

typedef enum COMMAND {
  CMD_REG = 1,
  CMD_IN,
  CMD_OUT,
  CMD_HB,
  CMD_UNDEFINED,
} COMMAND;
}

}; // ! ns ttt

#endif
