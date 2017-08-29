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
#include "interactive/interactive_common_cli.hpp"
#include "parser/parser.hpp"

namespace iroha_cli {
  namespace interactive {

    void printHelp(std::string command, std::vector<std::string> parameters) {
      std::cout << "Run " << command
                << " with following parameters: " << std::endl;
      std::for_each(parameters.begin(), parameters.end(),
                    [](auto el) { std::cout << "  " << el << std::endl; });
    };

    void printMenu(std::string message, std::vector<std::string> menu_points) {
      std::cout << message << std::endl;
      std::for_each(menu_points.begin(), menu_points.end(),
                    [](auto el) { std::cout << el << std::endl; });
    };

    std::string promtString(std::string message) {
      std::string line;
      std::cout << message << ": ";
      std::getline(std::cin, line);
      return line;
    }

    nonstd::optional<std::vector<std::string>> parseParams(
        std::string line, std::string command_name, ParamsMap params_map) {
      auto params_description = findInHandlerMap(command_name, params_map);
      if (not params_description.has_value()) {
        // Report no params where found for this command
        std::cout << "Command params not found" << std::endl;
        // Stop parsing, something is not implemented
        return nonstd::nullopt;
      }
      auto words = parser::split(line);
      if (words.size() == 1) {
        // Start interactive mode
        std::vector<std::string> params;
        std::for_each(params_description.value().begin(),
                      params_description.value().end(), [&params](auto param) {
                        params.push_back(promtString(param));
                      });
        return params;
      } else if (words.size() != params_description.value().size() + 1) {
        // Not enough parameters passed
        printHelp(command_name, params_description.value());
        return nonstd::nullopt;
      } else {
        // Remove command name
        words.erase(words.begin());
        return words;
      }
    }

    void add_menu_point(std::vector<std::string> &menu_points,
                        std::string description,
                        std::string command_short_name) {
      menu_points.push_back(std::to_string(menu_points.size() + 1) +
                            description + command_short_name);
    }
  }  // namespace interactive
}  // namespace iroha_cli