/*
Copyright Soramitsu Co., Ltd. 2016 All Rights Reserved.
http://soramitsu.co.jp

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

     http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
*/

#ifndef CORE_REPOSITORY_MERKLETRANSACTIONREPOSITORY_HPP_
#define CORE_REPOSITORY_MERKLETRANSACTIONREPOSITORY_HPP_

#include <string>
#include <memory>
#include "../../model/transactions/abstract_transaction.hpp"
#include "../../consensus/consensus_event.hpp"

namespace merkle_transaction_repository {

struct MerkleNode {
    std::string hash;
    std::string parent;
    std::tuple<std::string, std::string> children;

    bool isRoot() {
        return parent.empty();
    }

    bool isLeaf() {
        return std::get<0>(children).empty();
    }

    static std::string serialize(const MerkleNode node) {
        std::map<std::string, std::string> translateJSON; // key: ハッシュ, value: 変数の中身
        translateJSON.insert(std::make_pair("hash", node.hash));
        translateJSON.insert(std::make_pair("parent", node.parent));
        translateJSON.insert(std::make_pair("left_child", std::get<0>(node.children)));
        translateJSON.insert(std::make_pair("right_child", std::get<1>(node.children)));

        //TODO: create convertToJSON function

    }

    static  MerkleNode deserialize(std::string jsonStr) {
        //TODO:
    }
};

bool commit(const std::unique_ptr<consensus_event::ConsensusEvent> &event);

bool leafExists(std::string const hash);

std::string getLeaf(std::string const hash);

unsigned long long getLastLeafOrder();

std::unique_ptr<MerkleNode> calculateNewRoot(const std::unique_ptr<consensus_event::ConsensusEvent> &event);

};  // namespace merkle_transaction_repository

#endif  // CORE_REPOSITORY_MERKLETRANSACTIONREPOSITORY_HPP_
