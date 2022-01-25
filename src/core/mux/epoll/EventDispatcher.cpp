/*
 * snode.c - a slim toolkit for network communication
 * Copyright (C) 2020, 2021, 2022 Volker Christian <me@vchrist.at>
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

#include "core/mux/epoll/EventDispatcher.h"

#include "core/mux/epoll/DescriptorEventDispatcher.h"

#ifndef DOXYGEN_SHOULD_SKIP_THIS

#include "utils/Timeval.h" // for Timeval

#endif /* DOXYGEN_SHOULD_SKIP_THIS */

core::EventDispatcher& EventDispatcher() {
    static core::epoll::EventDispatcher eventDispatcher;

    return eventDispatcher;
}

namespace core::epoll {

    EventDispatcher::EventDispatcher()
        : core::EventDispatcher(new core::epoll::DescriptorEventDispatcher(epfds[RD], EPOLLIN),
                                new core::epoll::DescriptorEventDispatcher(epfds[WR], EPOLLOUT),
                                new core::epoll::DescriptorEventDispatcher(epfds[EX], EPOLLPRI)) {
        epfd = core::system::epoll_create1(EPOLL_CLOEXEC);

        epoll_event event;
        event.events = EPOLLIN;

        event.data.ptr = descriptorEventDispatcher[0];
        core::system::epoll_ctl(epfd, EPOLL_CTL_ADD, epfds[RD], &event);

        event.data.ptr = descriptorEventDispatcher[1];
        core::system::epoll_ctl(epfd, EPOLL_CTL_ADD, epfds[WR], &event);

        event.data.ptr = descriptorEventDispatcher[2];
        core::system::epoll_ctl(epfd, EPOLL_CTL_ADD, epfds[EX], &event);
    }

    int EventDispatcher::multiplex(utils::Timeval& tickTimeout) {
        return core::system::epoll_wait(epfd, ePollEvents, 3, tickTimeout.ms());
    }

    void EventDispatcher::dispatchActiveEvents(int count, const utils::Timeval& currentTime) {
        for (int i = 0; i < count; i++) {
            if ((ePollEvents[i].events & EPOLLIN) != 0) {
                static_cast<core::DescriptorEventDispatcher*>(ePollEvents[i].data.ptr)->dispatchActiveEvents(currentTime);
            }
        }

        for (core::DescriptorEventDispatcher* const eventDispatcher : descriptorEventDispatcher) {
            eventDispatcher->dispatchImmediateEvents(currentTime);
        }
    }

} // namespace core::epoll