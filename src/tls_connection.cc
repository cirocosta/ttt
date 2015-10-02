#include "ttt/tls_connection.hh"

namespace ttt
{
namespace net
{

TLSConnection::TLSConnection(const std::string& hostname, uint16_t port,
                             ConnectionType type)
    : m_ctx(nullptr), m_ssl(nullptr)
{
  m_connection = ConnectionPtr(new Connection(hostname, port, type));
  _init();
}

TLSConnection::TLSConnection(ConnectionPtr& conn)
{
  conn->setType(conn->getType() == TCP_ACTIVE ? TLS_ACTIVE : TLS_PASSIVE);
  m_connection = std::move(conn);
  _init();
}

void TLSConnection::_init()
{
  const SSL_METHOD* method;

  if (m_connection->isPassive())
    method = TLSv1_2_server_method();
  else
    method = TLSv1_2_client_method();

  if (!(m_ctx = SSL_CTX_new(method))) {
    ERR_print_errors_fp(stderr);
    throw std::runtime_error("Couldn't create SSL ctx");
  }

  if (m_connection->isPassive())
    _load_certificates();
  else
    m_ssl = SSL_new(m_ctx);
}

TLSConnection::~TLSConnection()
{
  if (m_ssl)
    SSL_free(m_ssl);

  if (m_ctx)
    SSL_CTX_free(m_ctx);
}

TLSConnectionPtr TLSConnection::accept()
{
  int err;
  ConnectionPtr tcp_conn = m_connection->accept();
  TLSConnectionPtr tls_conn(new TLSConnection(tcp_conn));
  SSL* ssl = SSL_new(m_ctx);

  ASSERT(SSL_set_fd(ssl, tls_conn->m_connection->getSocket()),
         "Error in SSL_set_fd");

  if ((err = SSL_accept(ssl)) < 1) {
    LOGERR("An error raised during SSL_accept");

    // FIXME understand openssl errors properly
    switch ((err = SSL_get_error(ssl, err))) {
      default:
        LOGERR("Error %d", err);
    }

    ERR_print_errors_fp(stderr);
    exit(EXIT_FAILURE);
  }

  tls_conn->m_ssl = ssl;

  return tls_conn;
}

ssize_t TLSConnection::write(const std::string& content) const
{
  return SSL_write(m_ssl, content.c_str(), content.size());
}

ssize_t TLSConnection::read()
{
  memset(m_connection->getBuffer(), '\0', TTT_MAX_BUFSIZE);
  return SSL_read(m_ssl, (void*)m_connection->getBuffer(), TTT_MAX_BUFSIZE);
}

void TLSConnection::connect()
{
  m_connection->connect();
  SSL_set_fd(m_ssl, m_connection->getSocket());

  if (SSL_connect(m_ssl) < 1) {
    ERR_print_errors_fp(stderr);
    LOGERR("Error connecting w/ TLS");
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
//      debugging only
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
