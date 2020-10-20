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

#ifndef CONNECTEVENTDISPATCHER_H
#define CONNECTEVENTDISPATCHER_H

#ifndef DOXYGEN_SHOULD_SKIP_THIS

#endif /* DOXYGEN_SHOULD_SKIP_THIS */

#include "ConnectEventReceiver.h"
#include "EventDispatcher.h"

// IWYU pragma: no_forward_declare net::ConnectEventReceiver

namespace net {

    class ConnectEventDispatcher : public EventDispatcher<ConnectEventReceiver> {
    public:
        using EventDispatcher<ConnectEventReceiver>::EventDispatcher;
    };

} // namespace net

#endif // CONNECTEVENTDISPATCHER_H
