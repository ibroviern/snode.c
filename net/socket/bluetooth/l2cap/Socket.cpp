/*
 * snode.c - a slim toolkit for network communication
 * Copyright (C) 2020 Volker Christian <me@vchrist.at>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published
 * by the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef DOXYGEN_SHOULD_SKIP_THIS

#include <bluetooth/bluetooth.h> // for BTPROTO_RFCOMM
#include <errno.h>               // for errno
#include <sys/socket.h>          // for socket, PF_BLUETOOTH, SOCK_STREAM

#endif /* DOXYGEN_SHOULD_SKIP_THIS */

#include "Socket.h"

namespace net::socket::bluetooth::l2cap {

    void Socket::open(const std::function<void(int errnum)>& onError, int flags) {
        errno = 0;

        int fd = ::socket(PF_BLUETOOTH, SOCK_SEQPACKET | flags, BTPROTO_L2CAP);

        if (fd >= 0) {
            open(fd);
            onError(0);
        } else {
            onError(errno);
        }
    }

} // namespace net::socket::bluetooth::l2cap