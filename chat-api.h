#ifndef CHAT_API_H
#define CHAT_API_H

#include <chrono>
#include <cpr/cpr.h>
#include <iostream>
#include <json.hpp>
#include <thread>

using json = nlohmann::json;

class ChatClient {
public:
  ChatClient(const std::string &base_url) : base_url(base_url) {}

  bool start_chat(const std::string &model, const json &prompt,
                  const json &dialogue) {
    json start_payload = {
        {"model", model}, {"prompt", prompt}, {"dialogue", dialogue}};

    cpr::Response response = post_request("start", start_payload);
    auto json_response = parse_json(response);
    if (json_response.is_discarded()) {
      return false;
    }

    print_status(response, "Start Response");
    if (!json_response.contains("id")) {
      std::cerr << "Received invalid 'id' in start response\n";
      return false;
    }

    context_id = std::to_string(json_response["id"].get<int>());
    return true;
  }

  bool clear_context() {
    json payload = {{"id", context_id}};
    return perform_operation("clear-context", payload, "Clear Context");
  }

  bool change_prompt(const json &new_prompt) {
    json payload = {{"id", context_id}, {"prompt", new_prompt}};
    return perform_operation("change-prompt", payload, "Change Prompt");
  }

  bool change_model(const std::string &new_model) {
    json payload = {{"id", context_id}, {"model", new_model}};
    return perform_operation("change-model", payload, "Change Model");
  }

  std::string generate_response(const std::string &message) {
    json payload = {{"id", std::stoi(context_id)}, {"msg", message}};
    cpr::Response response = post_request("generate", payload);

    auto json_response = parse_json(response);
    if (json_response.is_discarded()) {
      throw std::runtime_error("Failed to parse JSON response");
    }

    if (json_response.contains("status") &&
        json_response["status"] == "SUCCESSFUL" &&
        json_response.contains("response")) {
      std::cout << "Generate: " << json_response["status"] << '\n';
      return json_response["response"].get<std::string>();
    } else {
      throw std::runtime_error("Generate operation failed with response: " +
                               response.text);
    }
  }

  bool delete_message(int message_index) {
    json payload = {{"id", context_id}, {"message_index", message_index}};
    return perform_operation("delete-message", payload, "Delete Message");
  }

#include <filesystem> // C++17 filesystem library
#include <fstream>

  // ChatClient class definition here...

  bool end_chat(const std::string &title = "") {
    std::string url = "end/" + context_id;
    cpr::Response response = get_request(url);

    if (response.status_code == 200) {
      std::cout << "End Chat: SUCCESSFUL\n";
      std::string dir_path = "contexts/";
      std::string file_path =
          dir_path + (title.empty() ? context_id : title) + ".zip";

      // Ensure the directory exists
      std::filesystem::create_directories(dir_path);

      // Open and write to the file
      std::ofstream file(file_path, std::ios::binary);
      if (file.is_open()) {
        // Write the response body to the file
        file.write(response.text.c_str(), response.text.size());
        file.close();
        std::cout << "File downloaded successfully!" << std::endl;
      } else {
        std::cerr << "Failed to open file for writing." << std::endl;
      }
    } else {
      std::cerr << "Failed to download file. HTTP Status Code: "
                << response.status_code << std::endl;
    }

    return 0;
  }

  bool load_chat(const std::string &zip_path) {
    json payload = {{"zip_path", zip_path}};
    cpr::Response response = post_request("load", payload);

    auto json_response = parse_json(response);
    if (json_response.is_discarded()) {
      return false;
    }

    print_status(response, "Load Chat");
    if (!json_response.contains("id")) {
      std::cerr << "Received invalid 'id' in load response\n";
      return false;
    }

    context_id = std::to_string(json_response["id"].get<int>());
    return true;
  }

  bool add_rag(const std::string &doc_file_path) {
    cpr::Multipart multipart{{"id", std::stoi(context_id)},
                             {"doc_file", cpr::File{doc_file_path}}};

    cpr::Response response =
        cpr::Post(cpr::Url{base_url + "add_rag"}, multipart);
    auto json_response = parse_json(response);

    std::cout << "Add RAG Response: " << response.text << '\n';

    return print_status(response, "Add RAG");
  }

  std::string rag_generate(const std::string &message) {
    json payload = {{"id", std::stoi(context_id)}, {"msg", message}};
    cpr::Response response = post_request("rag_generate", payload);

    auto json_response = parse_json(response);
    if (json_response.is_discarded()) {
      throw std::runtime_error("Failed to parse JSON response");
    }

    if (json_response.contains("status") &&
        json_response["status"] == "SUCCESSFUL" &&
        json_response.contains("response")) {
      std::cout << "RAG Generate: " << json_response["status"] << '\n';
      return json_response["response"].get<std::string>();
    } else {
      throw std::runtime_error("RAG Generate operation failed with response: " +
                               response.text);
    }
  }

private:
  std::string base_url;
  std::string context_id;
  cpr::Response post_request(const std::string &endpoint, const json &payload) {
    return cpr::Post(cpr::Url{base_url + endpoint}, cpr::Body{payload.dump()},
                     cpr::Header{{"Content-Type", "application/json"}});
  }

  cpr::Response get_request(const std::string &endpoint) {
    return cpr::Get(cpr::Url{base_url + endpoint});
  }

  json parse_json(const cpr::Response &response) {
    return json::parse(response.text, nullptr, false);
  }

  bool print_status(const cpr::Response &response,
                    const std::string &operation) {
    auto response_json = parse_json(response);
    if (!response_json.is_discarded() && response_json.contains("status") &&
        response_json["status"].is_string()) {
      std::cout << operation << ": " << response_json["status"] << '\n';
      return true;
    }
    std::cerr << "Error: 'status' field missing or not a string in response.\n";
    std::cerr << "Full response: " << response.text << "\n";
    return false;
  }

  bool perform_operation(const std::string &endpoint, const json &payload,
                         const std::string &operation) {
    cpr::Response response = post_request(endpoint, payload);
    return print_status(response, operation);
  }
};

#endif // CHAT_API_H
