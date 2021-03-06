// Copyright 2014 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef EXTENSIONS_BROWSER_API_CAST_CHANNEL_CAST_TRANSPORT_H_
#define EXTENSIONS_BROWSER_API_CAST_CHANNEL_CAST_TRANSPORT_H_

#include <queue>
#include <string>

#include "base/memory/ref_counted.h"
#include "base/threading/thread_checker.h"
#include "extensions/browser/api/cast_channel/logger.h"
#include "extensions/common/api/cast_channel.h"
#include "extensions/common/api/cast_channel/logging.pb.h"
#include "net/base/completion_callback.h"

namespace net {
class DrainableIOBuffer;
class IPEndPoint;
class IOBuffer;
class DrainableIOBuffer;
class GrowableIOBuffer;
class Socket;
}  // namespace net

namespace extensions {
namespace core_api {
namespace cast_channel {
class CastMessage;
struct LastErrors;
class Logger;
class MessageFramer;

class CastTransport {
 public:
  virtual ~CastTransport() {}

  // Object to be informed of incoming messages and read errors.
  class Delegate {
   public:
    virtual ~Delegate() {}

    // An error occurred on the channel. |last_errors| contains the last errors
    // logged for the channel from the implementation.
    // The caller is responsible for closing |socket| if an error occurred.
    virtual void OnError(ChannelError error_state,
                         const LastErrors& last_errors) = 0;
    // A message was received on the channel.
    virtual void OnMessage(const CastMessage& message) = 0;
  };

  // Sends a CastMessage to |socket_|.
  // |message|: The message to send.
  // |callback|: Callback to be invoked when the write operation has finished.
  // Virtual for testing.
  virtual void SendMessage(const CastMessage& message,
                           const net::CompletionCallback& callback) = 0;

  // Initializes the reading state machine and starts reading from the
  // underlying socket.
  // Virtual for testing.
  virtual void StartReading() = 0;

  // Changes the delegate for processing read events. Pending reads remain
  // in-flight.
  virtual void SetReadDelegate(Delegate* delegate) = 0;
};

// Manager class for reading and writing messages to/from a socket.
// TODO(kmarshall): Handle heartbeat messages in this layer.
class CastTransportImpl : public CastTransport, public base::NonThreadSafe {
 public:
  // Adds a CastMessage read/write layer to a socket.
  // Message read events are propagated to the owner via |read_delegate|.
  // |socket|, |read_delegate| and |logger| must all out-live the
  // CastTransportImpl instance.
  // |vlog_prefix| sets the prefix used for all VLOGged output.
  CastTransportImpl(net::Socket* socket,
                    Delegate* read_delegate,
                    int channel_id,
                    const net::IPEndPoint& ip_endpoint_,
                    ChannelAuthType channel_auth_,
                    scoped_refptr<Logger> logger);

  virtual ~CastTransportImpl();

  // CastTransport interface.
  virtual void SendMessage(const CastMessage& message,
                           const net::CompletionCallback& callback) override;
  virtual void StartReading() override;
  virtual void SetReadDelegate(Delegate* delegate) override;

 private:
  // Internal write states.
  enum WriteState {
    WRITE_STATE_NONE,
    WRITE_STATE_WRITE,
    WRITE_STATE_WRITE_COMPLETE,
    WRITE_STATE_DO_CALLBACK,
    WRITE_STATE_ERROR,
  };

  // Internal read states.
  enum ReadState {
    READ_STATE_NONE,
    READ_STATE_READ,
    READ_STATE_READ_COMPLETE,
    READ_STATE_DO_CALLBACK,
    READ_STATE_ERROR,
  };

  // Holds a message to be written to the socket. |callback| is invoked when the
  // message is fully written or an error occurrs.
  struct WriteRequest {
    explicit WriteRequest(const std::string& namespace_,
                          const std::string& payload,
                          const net::CompletionCallback& callback);
    ~WriteRequest();

    // Namespace of the serialized message.
    std::string message_namespace;
    // Write completion callback, invoked when the operation has completed or
    // failed.
    net::CompletionCallback callback;
    // Buffer with outgoing data.
    scoped_refptr<net::DrainableIOBuffer> io_buffer;
  };

  static proto::ReadState ReadStateToProto(CastTransportImpl::ReadState state);
  static proto::WriteState WriteStateToProto(
      CastTransportImpl::WriteState state);
  static proto::ErrorState ErrorStateToProto(ChannelError state);

  void SetReadState(ReadState read_state);
  void SetWriteState(WriteState write_state);
  void SetErrorState(ChannelError error_state);

  // Terminates all in-flight write callbacks with error code ERR_FAILED.
  void FlushWriteQueue();

  // Main method that performs write flow state transitions.
  void OnWriteResult(int result);

  // Each of the below Do* method is executed in the corresponding
  // write state. For example when write state is WRITE_STATE_WRITE_COMPLETE
  // DowriteComplete is called, and so on.
  int DoWrite();
  int DoWriteComplete(int result);
  int DoWriteCallback();
  int DoWriteError(int result);

  // Main method that performs write flow state transitions.
  void OnReadResult(int result);

  // Each of the below Do* method is executed in the corresponding
  // write state. For example when read state is READ_STATE_READ_COMPLETE
  // DoReadComplete is called, and so on.
  int DoRead();
  int DoReadComplete(int result);
  int DoReadCallback();
  int DoReadError(int result);

  // Queue of pending writes. The message at the front of the queue is the one
  // being written.
  std::queue<WriteRequest> write_queue_;

  // Buffer used for read operations. Reused for every read.
  scoped_refptr<net::GrowableIOBuffer> read_buffer_;

  // Constructs and parses the wire representation of message frames.
  scoped_ptr<MessageFramer> framer_;

  // Last message received on the socket.
  scoped_ptr<CastMessage> current_message_;

  // Socket used for I/O operations.
  net::Socket* const socket_;

  // Methods for communicating message receipt and error status to client code.
  Delegate* read_delegate_;

  // Write flow state machine state.
  WriteState write_state_;

  // Read flow state machine state.
  ReadState read_state_;

  // The last error encountered by the channel.
  ChannelError error_state_;

  // Connection metadata for logging purposes.
  // Socket ID assigned by ApiResourceManager.
  int channel_id_;

  // IP address of the remote end.
  const net::IPEndPoint ip_endpoint_;

  // Authentication level for the connection.
  ChannelAuthType channel_auth_;

  // Accumulates details of events and errors, for debugging purposes.
  scoped_refptr<Logger> logger_;

  DISALLOW_COPY_AND_ASSIGN(CastTransportImpl);
};
}  // namespace cast_channel
}  // namespace core_api
}  // namespace extensions

#endif  // EXTENSIONS_BROWSER_API_CAST_CHANNEL_CAST_TRANSPORT_H_
