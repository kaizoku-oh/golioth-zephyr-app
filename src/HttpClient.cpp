// Lib C
#include <string.h>
#include <assert.h>

// Zephyr includes
#include <zephyr/net/net_ip.h>
#include <zephyr/net/socket.h>
#include <zephyr/net/tls_credentials.h>
#include <zephyr/net/http/client.h>
#include <zephyr/logging/log.h>
LOG_MODULE_REGISTER(HttpClient);

// User C++ class headers
#include "HttpClient.h"

#define TLS_PEER_HOSTNAME "localhost"
#define CA_CERTIFICATE_TAG 1
#define HTTPS_PORT 4443

/* This is the same cert as what is found in net-tools/https-cert.pem file
 */
static const unsigned char ca_certificate[] = {
};

static void responseCallback(http_response *response,
                                 enum http_final_call finalData,
                                 void *userData);

HttpClient::HttpClient(char *server, uint16_t port) {
  assert(server);
  assert(port);

  // 1. Initialize attributes
  this->sock = 0;
  this->server = server;
  this->port = port;
  memset((void *)&this->socketAddress, 0x00, sizeof(this->socketAddress));
  memset((void *)&this->responseBuffer, 0x00, sizeof(this->responseBuffer));
}

HttpClient::~HttpClient() {
  // Destructor is automatically called when the object goes out of scope or is explicitly deleted
}

int HttpClient::get(const char *endpoint, std::function<void(HttpResponse *)> callback) {
  int ret = 0;
  struct http_request request = {0};

  assert(endpoint);
  assert(callback);

  // 0. Create socket
  memset((void *)&this->socketAddress, 0x00, sizeof(this->socketAddress));
  net_sin(&this->socketAddress)->sin_family = AF_INET;
  net_sin(&this->socketAddress)->sin_port = htons(port);
  inet_pton(AF_INET, server, &net_sin(&this->socketAddress)->sin_addr);

  if (IS_ENABLED(CONFIG_NET_SOCKETS_SOCKOPT_TLS)) {
    sec_tag_t sec_tag_list[] = {
      CA_CERTIFICATE_TAG,
    };

    this->sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TLS_1_2);
    if (this->sock >= 0) {
      ret = setsockopt(this->sock, SOL_TLS, TLS_SEC_TAG_LIST, sec_tag_list, sizeof(sec_tag_list));
      if (ret < 0) {
        LOG_ERR("Failed to set %s secure option (%d)", -errno);
        ret = -errno;
      }

      ret = setsockopt(this->sock, SOL_TLS, TLS_HOSTNAME, TLS_PEER_HOSTNAME, sizeof(TLS_PEER_HOSTNAME));
      if (ret < 0) {
        LOG_ERR("Failed to set TLS_HOSTNAME " "option (%d)", -errno);
        ret = -errno;
      }
    }
  } else {
    this->sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
  }

  if (this->sock < 0) {
    LOG_ERR("Failed to create HTTP socket (%d)\r\n", -errno);
  }

  this->callback = callback;

  if (IS_ENABLED(CONFIG_NET_SOCKETS_SOCKOPT_TLS)) {
    ret = tls_credential_add(CA_CERTIFICATE_TAG,
                             TLS_CREDENTIAL_CA_CERTIFICATE,
                             ca_certificate,
                             sizeof(ca_certificate));
    if (ret < 0) {
      LOG_ERR("Failed to register public certificate: %d", ret);
      return ret;
    }

    port = HTTPS_PORT;
  }

  // 1. Open TCP connection
  ret = connect(this->sock, &this->socketAddress, sizeof(this->socketAddress));
  if (ret < 0) {
    LOG_ERR("Cannot connect to remote (%d)", -errno);
    ret = -errno;
    return ret;
  }

  // 2. Send GET request
  request.method = HTTP_GET;
  request.url = endpoint;
  request.host = this->server;
  request.protocol = "HTTP/1.1";
  request.response = responseCallback;
  request.recv_buf = this->responseBuffer;
  request.recv_buf_len = sizeof(this->responseBuffer);
  ret = http_client_req(this->sock, &request, 5000, (void *)&this->callback);
  if (ret < 0) {
    LOG_ERR("Error sending GET request\r\n");
    ret = -errno;
    return ret;
  }

  // 3. Close TCP connection
  close(this->sock);

  return ret;
}

int HttpClient::post(const char *endpoint,
                     const char *data,
                     uint32_t length,
                     std::function<void(HttpResponse *)> callback) {
  int ret = 0;
  struct http_request request = {0};

  assert(endpoint);
  assert(data);
  assert(length);
  assert(callback);

  // 0. Create socket
  memset((void *)&this->socketAddress, 0x00, sizeof(this->socketAddress));
  net_sin(&this->socketAddress)->sin_family = AF_INET;
  net_sin(&this->socketAddress)->sin_port = htons(port);
  inet_pton(AF_INET, server, &net_sin(&this->socketAddress)->sin_addr);
  this->sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

  if (this->sock < 0) {
    LOG_ERR("Failed to create HTTP socket (%d)\r\n", -errno);
  }

  this->callback = callback;

  // 1. Open TCP connection
  ret = connect(this->sock, &this->socketAddress, sizeof(this->socketAddress));
  if (ret < 0) {
    LOG_ERR("Cannot connect to remote (%d)", -errno);
    ret = -errno;
    return ret;
  }

  // 2. Send POST request
  request.method = HTTP_POST;
  request.host = this->server;
  request.url = endpoint;
  request.protocol = "HTTP/1.1";
  request.response = responseCallback;
  request.payload = data;
  request.payload_len = length;
  request.recv_buf = this->responseBuffer;
  request.recv_buf_len = sizeof(this->responseBuffer);
  ret = http_client_req(this->sock, &request, 5000, (void *)this);
  if (ret < 0) {
    LOG_ERR("Error sending POST request\r\n");
    ret = -errno;
    return ret;
  }

  // 3. Close TCP connection
  close(this->sock);

  return ret;
}

static void responseCallback(http_response *response, enum http_final_call finalData, void *userData) {
  HttpClient *clientInstance  = static_cast<HttpClient *>(userData);
  HttpResponse httpResponse = {0};

  assert(userData);
  assert(response);
  assert(clientInstance);

  if (response->body_found) {
    httpResponse.header = response->recv_buf;
    httpResponse.headerLength = response->data_len - response->body_frag_len;
    httpResponse.body = response->body_frag_start;
    httpResponse.bodyLength = response->body_frag_len;
    httpResponse.totalSize = response->content_length;
    httpResponse.isComplete = (finalData == HTTP_DATA_FINAL);
  } else {
    httpResponse.header = response->recv_buf;
    httpResponse.headerLength = response->data_len;
  }

  if (clientInstance->callback) {
    clientInstance->callback(&httpResponse);
  }
}
