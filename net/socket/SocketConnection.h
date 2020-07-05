#ifndef SOCKETCONNECTIONBASE_H
#define SOCKETCONNECTIONBASE_H

#ifndef DOXYGEN_SHOULD_SKIP_THIS

#endif /* DOXYGEN_SHOULD_SKIP_THIS */

#include "SocketConnectionBase.h"
#include "SocketServer.h"


template <typename Reader, typename Writer>
class SocketConnection
    : public SocketConnectionBase
    , public Reader
    , public Writer {
public:
    SocketConnection(int csFd, SocketServer<SocketConnection>* serverSocket,
                     const std::function<void(SocketConnection* cs, const char* junk, ssize_t n)>& onRead,
                     const std::function<void(SocketConnection* cs, int errnum)>& onReadError,
                     const std::function<void(SocketConnection* cs, int errnum)>& onWriteError)
        : Reader(
              [&](const char* junk, ssize_t n) -> void {
                  onRead(this, junk, n);
              },
              [&](int errnum) -> void {
                  onReadError(this, errnum);
              })
        , Writer([&](int errnum) -> void {
            onWriteError(this, errnum);
        })
        , serverSocket(serverSocket) {
        this->attachFd(csFd);
    }

    void enqueue(const char* buffer, int size) override {
        this->Writer::enqueue(buffer, size);
    }

    void end() override {
        serverSocket->end(this);
    }

    InetAddress& getRemoteAddress() {
        return remoteAddress;
    }

    void setRemoteAddress(const InetAddress& remoteAddress) {
        this->remoteAddress = remoteAddress;
    }

private:
    void unmanaged() override {
        serverSocket->disconnect(this);
    }

#ifndef DOXYGEN_SHOULD_SKIP_THIS

    SocketServer<SocketConnection>* serverSocket;

#endif /* DOXYGEN_SHOULD_SKIP_THIS */

    InetAddress remoteAddress{};

public:
    using ReaderType = Reader;
    using WriterType = Writer;
};

#endif // SOCKETCONNECTIONBASE_H
