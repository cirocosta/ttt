#include "ttt/tls_connection.hh"

namespace ttt
{
namespace net
{

TLSConnection::TLSConnection(const std::string& hostname, uint16_t port,
                             ConnectionType type)
    : Connection(hostname, port, type)
{
  const SSL_METHOD* method;

  if (isPassive()) {
    method = TLSv1_2_server_method();
  } else {
    method = TLSv1_2_client_method();
  }

  if (!(m_ctx = SSL_CTX_new(method))) {
    ERR_print_errors_fp(stderr);
    exit(EXIT_FAILURE);
  }

  if (isPassive()) {
    _load_certificates();
  } else {
    m_ssl = SSL_new(m_ctx);
  }
}

TLSConnection::~TLSConnection()
{
  if (m_ssl)
    SSL_free(m_ssl);
  SSL_CTX_free(m_ctx);
}

TLSConnectionPtr TLSConnection::accept_tls()
{
  SSL* ssl = SSL_new(m_ctx);
  TLSConnectionPtr conn(static_cast<TLSConnection*>(accept().release()));

  SSL_set_fd(ssl, conn->getSocket());
  ASSERT(~SSL_accept(ssl), "");

  conn->m_ssl = ssl;

  return conn;
}

void TLSConnection::connect_tls()
{
  SSL_set_fd(m_ssl, getSocket());

  if (!~SSL_connect(m_ssl)) {
    ERR_print_errors_fp(stderr);
    exit(EXIT_FAILURE);
  }
}

void TLSConnection::_load_certificates()
{
  if (SSL_CTX_use_certificate_file(m_ctx, TLS_FNAME_CERTIFICATE,
                                   SSL_FILETYPE_PEM) <= 0) {
    ERR_print_errors_fp(stderr);
    exit(EXIT_FAILURE);
  }

  if (SSL_CTX_use_PrivateKey_file(m_ctx, TLS_FNAME_PKEY, SSL_FILETYPE_PEM) <=
      0) {
    ERR_print_errors_fp(stderr);
    exit(EXIT_FAILURE);
  }

  ASSERT(SSL_CTX_check_private_key(m_ctx),
         "Private key does not match the public certificate\n");
}
}
};
