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

#include "apps/mqtt/server/SubscriberTree.h"

#ifndef DOXYGEN_SHOULD_SKIP_THIS

#include <utility>

#endif // DOXYGEN_SHOUÖD_SKIP_THIS

namespace apps::mqtt::server {

    void SubscriberTree::subscribe(const std::string& fullName, SocketContext* socketContext) {
        subscribe(fullName, fullName, socketContext);
    }

    void SubscriberTree::subscribe(const std::string& fullName, std::string remainingTopicName, SocketContext* socketContext) {
        if (remainingTopicName.empty()) {
            this->fullName = fullName;
            subscribers.push_back(socketContext);
        } else {
            std::string topicName = remainingTopicName.substr(0, remainingTopicName.find("/"));
            remainingTopicName.erase(0, topicName.size() + 1);

            if (subscriberTree.contains(topicName)) {
                subscriberTree.find(topicName)->second.subscribe(fullName, remainingTopicName, socketContext);
            } else {
                subscriberTree.insert({topicName, SubscriberTree()}).first->second.subscribe(fullName, remainingTopicName, socketContext);
            }
        }
    }

} // namespace apps::mqtt::server
