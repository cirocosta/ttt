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

  virtual void write(const std::string& content) const;
  /* virtual void read(); */
  void connect_tls();
  TLSConnectionPtr accept_tls();

  // FIXME refactor this
  void client_handling_example(int fd)
  {
    char buf[256] = { 0 };
    int sd = -1;
    // creates a new SSL structure needed to hold the data
    // for a TLS connection. It inherits the settings of the underlying
    // context (ctx).
    SSL* ssl = SSL_new(m_ctx);
    SSL_set_fd(ssl, fd);

    ASSERT(~SSL_accept(ssl), "");

    // do some read
    SSL_read(ssl, buf, sizeof(buf));
    // SSL_write ...

    sd = SSL_get_fd(ssl);
    SSL_free(ssl);
    close(sd);
  }

  // TODO document this. It must be initialize only once.
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
