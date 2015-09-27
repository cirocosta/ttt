#ifndef TTT__TLS_CONNECTION_H
#define TTT__TLS_CONNECTION_H

#include "ttt/connection.hh"
#include <openssl/ssl.h>
#include <openssl/err.h>

namespace ttt
{

namespace net
{

class TLSConnection;
typedef std::unique_ptr<TLSConnection> TLSConnectionPtr;

class TLSConnection : public Connection
{
private:
  SSL_CTX* m_ctx;
  SSL* m_ssl;

  TLSConnection(); // the TLS connection that is prepared with
                   // SSL_new (see client_handling_example).
public:
  TLSConnection(const std::string& hostname, uint16_t port,
                ConnectionType type);
  ~TLSConnection();

  virtual void write(const std::string& content) const {}
  virtual void read() const {}
  void connect_tls();
  TLSConnectionPtr accept_tls();

  static void initialize_TLS()
  {
    SSL_library_init();
    OpenSSL_add_all_algorithms();
    SSL_load_error_strings();
  }

  void display_certs();

private:
  void _load_certificates();
};
}
};

#endif
