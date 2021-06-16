/*
 * Goodix Tls driver for libfprint
 *
 * Copyright (C) 2021 Alexander Meiler <alex.meiler@protonmail.com>
 * Copyright (C) 2021 Matthieu CHARETTE <matthieu.charette@gmail.com>
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA
 */

#include <arpa/inet.h>
#include <errno.h>
#include <openssl/err.h>
#include <openssl/ssl.h>
#include <pthread.h>
#include <signal.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

#include "drivers_api.h"

#pragma once

FpiSsm *fpi_ssm;
pthread_t server;
int sock;
SSL_CTX *ctx;

static const char *psk_key =
    "ba1a86037c1d3c71c3af344955bd69a9a9861d9e911fa24985b677e8dbd72d43";

#define GOODIX_TLS_SERVER_PORT 4433

//#define TLS_PSK_WITH_AES_128_GCM_SHA256  ((const unsigned char *)"\x00\xa8")

SSL_CTX *TLS_server_create_ctx(void);

int TLS_server_create_socket(int port);

void TLS_server_config_ctx(void);

__attribute__((__noreturn__)) void *TLS_server_loop(void *arg);

void TLS_server_stop(void);

void *TLS_server_handshake_loop(void *arg);

void TLS_server_handshake_init(void);

void TLS_server_init(FpiSsm *ssm);
