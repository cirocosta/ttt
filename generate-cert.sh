#!/bin/bash

if hash openssl 2>/dev/null; then
  openssl req -x509 -nodes -days 365 -newkey rsa:2048 -keyout certs/private.pem -out certs/cert.pem
else
  echo "Requires OpenSSL. Check README for install instructions."
  return 1
fi

