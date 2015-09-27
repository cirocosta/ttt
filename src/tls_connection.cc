#include "ttt/tls_connection.hh"

namespace ttt
{
namespace net
{

TLSConnection::TLSConnection(const std::string& hostname, uint16_t port,
                             ConnectionType type)
{
  m_connection = ConnectionPtr(new Connection(hostname, port, type));
  const SSL_METHOD* method;

  if (m_connection->isPassive()) {
    method = TLSv1_2_server_method();
  } else {
    method = TLSv1_2_client_method();
  }

  if (!(m_ctx = SSL_CTX_new(method))) {
    ERR_print_errors_fp(stderr);
    exit(EXIT_FAILURE);
  }

  if (m_connection->isPassive()) {
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
  ConnectionPtr tcp_conn = m_connection->accept();
  TLSConnectionPtr tls_conn(new TLSConnection(tcp_conn));

  SSL_set_fd(ssl, tls_conn->m_connection->getSocket());
  ASSERT(~SSL_accept(ssl), "");

  tls_conn->m_ssl = ssl;

  return tls_conn;
}

void TLSConnection::connect_tls()
{
  SSL_set_fd(m_ssl, m_connection->getSocket());

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

// TODO generate a string w/ sstream
void TLSConnection::display_certs()
{
  X509* cert;
  char* line;

  cert = SSL_get_peer_certificate(m_ssl);

  if (!cert) {
    fprintf(stderr, "No certificates.\n");
    return;
  }

  fprintf(stderr, "Server certificates:\n");
  line = X509_NAME_oneline(X509_get_subject_name(cert), 0, 0);
  fprintf(stderr, "\tSubject: %s\n", line);
  delete line;

  line = X509_NAME_oneline(X509_get_issuer_name(cert), 0, 0);
  fprintf(stderr, "Issuer: %s\n", line);

  delete line;
  X509_free(cert);
}
}
};
