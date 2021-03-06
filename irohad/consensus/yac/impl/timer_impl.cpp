/**
 * Copyright Soramitsu Co., Ltd. 2017 All Rights Reserved.
 * http://soramitsu.co.jp
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *        http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "consensus/yac/impl/timer_impl.hpp"
#include <iostream>

namespace iroha {
  namespace consensus {
    namespace yac {

      TimerImpl::TimerImpl(std::shared_ptr<uvw::Loop> loop)
          : loop_(std::move(loop)),
            timer_(loop_->resource<uvw::TimerHandle>()) {
        timer_->on<uvw::TimerEvent>([this](const auto&, auto&) { handler_(); });
      }

      void TimerImpl::invokeAfterDelay(uint64_t millis,
                                       std::function<void()> handler) {
        deny();
        handler_ = std::move(handler);
        timer_->start(uvw::TimerHandle::Time(millis),
                      uvw::TimerHandle::Time(0));
      }

      void TimerImpl::deny() { timer_->stop(); }

      TimerImpl::~TimerImpl() { timer_->close(); }

    }  // namespace yac
  }    // namespace consensus
}  // namespace iroha
