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

class TLSConnection
{
private:
  SSL_CTX* m_ctx;
  SSL* m_ssl;
  ConnectionPtr m_connection;

  void _init();
public:
  TLSConnection(const std::string& hostname, uint16_t port,
                ConnectionType type);
  TLSConnection(ConnectionPtr& conn); // acquires ownership
  ~TLSConnection();

  inline Connection* getConnection() const { return m_connection.get(); }

  inline ssize_t write(const std::string& content) const
  {
    return SSL_write(m_ssl, content.c_str(), content.size());
  }

  inline ssize_t read()
  {
    return SSL_read(m_ssl, (void*)m_connection->getBuffer(), TTT_MAX_BUFSIZE);
  }

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
