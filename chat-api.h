#ifndef CHAT_API_H
#define CHAT_API_H

#include <chrono>
#include <cpr/cpr.h>
#include <filesystem> // C++17 filesystem library
#include <fstream>
#include <iostream>
#include <json.hpp>
#include <thread>

using json = nlohmann::json;

class ChatClient {
public:
  ChatClient(const std::string &base_url) : base_url(base_url) {
    std::cout << "ChatClient created with base URL: " << base_url << std::endl;
  }

  bool start_chat(const std::string &model, const json &prompt, const json &dialogue) {
    json start_payload = {{"model", model}, {"prompt", prompt}, {"dialogue", dialogue}};
    std::cout << "Starting chat with payload: " << start_payload.dump() << std::endl;

    cpr::Response response = post_request("start", start_payload);
    auto json_response = parse_json(response);
    if (json_response.is_discarded()) {
      std::cerr << "Failed to parse JSON response from start chat." << std::endl;
      return false;
    }

    print_status(response, "Start Response");
    if (!json_response.contains("id")) {
      std::cerr << "Received invalid 'id' in start response\n";
      return false;
    }

    context_id = std::to_string(json_response["id"].get<int>());
    std::cout << "Chat started with ID: " << context_id << std::endl;
    return true;
  }

  bool clear_context() {
    if (context_id.empty()) {
      std::cerr << "Context ID is empty, cannot clear context." << std::endl;
      return false;
    }

    json payload = {{"id", context_id}};
    std::cout << "Clearing context with payload: " << payload.dump() << std::endl;
    return perform_operation("clear-context", payload, "Clear Context");
  }

  bool change_prompt(const json &new_prompt) {
    if (context_id.empty()) {
      std::cerr << "Context ID is empty, cannot change prompt." << std::endl;
      return false;
    }

    json payload = {{"id", context_id}, {"prompt", new_prompt}};
    std::cout << "Changing prompt with payload: " << payload.dump() << std::endl;
    return perform_operation("change-prompt", payload, "Change Prompt");
  }

  bool change_model(const std::string &new_model) {
    if (context_id.empty()) {
      std::cerr << "Context ID is empty, cannot change model." << std::endl;
      return false;
    }

    json payload = {{"id", context_id}, {"model", new_model}};
    std::cout << "Changing model with payload: " << payload.dump() << std::endl;
    return perform_operation("change-model", payload, "Change Model");
  }

  std::string generate_response(const std::string &message) {
    if (context_id.empty()) {
      std::cerr << "Context ID is empty, cannot generate response." << std::endl;
      return "";
    }

    json payload = {{"id", std::stoi(context_id)}, {"msg", message}};
    std::cout << "Generate Payload: " << payload.dump() << std::endl;

    cpr::Response response = post_request("generate", payload);
    std::cout << "Generate Response: " << response.text << std::endl;

    if (response.status_code != 200) {
      std::cerr << "Failed to generate response. HTTP Status Code: " << response.status_code << std::endl;
      return "";
    }

    auto json_response = parse_json(response);
    if (json_response.is_discarded()) {
      std::cerr << "Failed to parse JSON response" << std::endl;
      return "";
    }

    if (json_response.contains("status") && json_response["status"] == "SUCCESSFUL" && json_response.contains("response")) {
      std::cout << "Generate: " << json_response["status"] << '\n';
      return json_response["response"].get<std::string>();
    } else {
      std::cerr << "Generate operation failed with response: " << response.text << std::endl;
      return "";
    }
  }

  bool delete_message(int message_index) {
    if (context_id.empty()) {
      std::cerr << "Context ID is empty, cannot delete message." << std::endl;
      return false;
    }

    json payload = {{"id", context_id}, {"message_index", message_index}};
    std::cout << "Deleting message with payload: " << payload.dump() << std::endl;
    return perform_operation("delete-message", payload, "Delete Message");
  }

  bool end_chat(const std::string &title = "") {
    if (context_id.empty()) {
      std::cerr << "Context ID is empty, cannot end chat." << std::endl;
      return false;
    }

    std::string url = "end/" + context_id;
    std::cout << "Ending chat with URL: " << url << std::endl;
    cpr::Response response = get_request(url);

    if (response.status_code == 200) {
      std::cout << "End Chat: SUCCESSFUL\n";
      std::string dir_path = "contexts/";
      std::string file_path = dir_path + (title.empty() ? context_id : title) + ".zip";

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
      std::cerr << "Failed to download file. HTTP Status Code: " << response.status_code << std::endl;
    }

    return true;
  }

  bool load_chat(const std::string &zip_path) {
    // Create a multipart request
    cpr::Multipart payload = {{"chat_file", cpr::File{zip_path}}};
    std::cout << "Loading chat with zip file: " << zip_path << std::endl;

    cpr::Response response = cpr::Post(cpr::Url{base_url + "load"}, payload);
    std::cout << "Load Chat Response: " << response.text << std::endl;

    auto json_response = parse_json(response);
    if (json_response.is_discarded()) {
      std::cerr << "Failed to parse JSON response from load chat." << std::endl;
      return false;
    }

    print_status(response, "Load Chat");
    if (!json_response.contains("id")) {
      std::cerr << "Received invalid 'id' in load response\n";
      return false;
    }

    context_id = std::to_string(json_response["id"].get<int>());
    std::cout << "Loaded chat with ID: " << context_id << std::endl;

    // Additional re-initialization steps
    initialize_chat_state();

    return true;
  }

  bool add_rag(const std::string &doc_file_path) {
    if (context_id.empty()) {
      std::cerr << "Context ID is empty, cannot add RAG." << std::endl;
      return false;
    }

    cpr::Multipart multipart{{"id", std::stoi(context_id)}, {"doc_file", cpr::File{doc_file_path}}};
    std::cout << "Adding RAG with doc file: " << doc_file_path << std::endl;

    cpr::Response response = cpr::Post(cpr::Url{base_url + "add_rag"}, multipart);
    auto json_response = parse_json(response);

    std::cout << "Add RAG Response: " << response.text << '\n';

    return print_status(response, "Add RAG");
  }

  std::string rag_generate(const std::string &message) {
    if (context_id.empty()) {
      std::cerr << "Context ID is empty, cannot generate RAG response." << std::endl;
      return "";
    }

    json payload = {{"id", std::stoi(context_id)}, {"msg", message}};
    std::cout << "RAG Generate Payload: " << payload.dump() << std::endl;

    cpr::Response response = post_request("rag_generate", payload);
    std::cout << "RAG Generate Response: " << response.text << std::endl;

    auto json_response = parse_json(response);
    if (json_response.is_discarded()) {
      std::cerr << "Failed to parse JSON response" << std::endl;
      return "";
    }

    if (json_response.contains("status") && json_response["status"] == "SUCCESSFUL" && json_response.contains("response")) {
      std::cout << "RAG Generate: " << json_response["status"] << '\n';
      return json_response["response"].get<std::string>();
    } else {
      std::cerr << "RAG Generate operation failed with response: " << response.text << std::endl;
      return "";
    }
  }

private:
  std::string base_url;
  std::string context_id;

  void initialize_chat_state() {
    // Re-initialize any necessary state variables or settings
    std::cout << "Re-initializing chat state for context ID: " << context_id << std::endl;
    // Add any necessary re-initialization code here
  }

  cpr::Response post_request(const std::string &endpoint, const json &payload) {
    return cpr::Post(cpr::Url{base_url + endpoint}, cpr::Body{payload.dump()}, cpr::Header{{"Content-Type", "application/json"}});
  }

  cpr::Response get_request(const std::string &endpoint) {
    return cpr::Get(cpr::Url{base_url + endpoint});
  }

  json parse_json(const cpr::Response &response) {
    try {
      return json::parse(response.text, nullptr, false);
    } catch (const json::parse_error &e) {
      std::cerr << "JSON parse error: " << e.what() << " in response: " << response.text << std::endl;
      return json();
    }
  }

  bool print_status(const cpr::Response &response, const std::string &operation) {
    auto response_json = parse_json(response);
    if (!response_json.is_discarded() && response_json.contains("status") && response_json["status"].is_string()) {
      std::cout << operation << ": " << response_json["status"] << '\n';
      return true;
    }
    std::cerr << "Error: 'status' field missing or not a string in response.\n";
    std::cerr << "Full response: " << response.text << "\n";
    return false;
  }

  bool perform_operation(const std::string &endpoint, const json &payload, const std::string &operation) {
    cpr::Response response = post_request(endpoint, payload);
    return print_status(response, operation);
  }
};

#endif // CHAT_API_H
