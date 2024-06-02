#include <chat-api.h>

int main() {
  const std::string base_url = "http://IP-ADRESS:PORT/chat/";
  ChatClient client(base_url);

  // Test /chat/start
  json prompt = {
      {{"role", "system"}, {"content", "This is the default prompt"}}};
  json dialogue = {{{"role", "user"}, {"content", "hi"}},
                   {{"role", "assistant"}, {"content", "yo wassup"}}};

  if (!client.start_chat("meta-llama/Meta-Llama-3-8B-Instruct", prompt,
                         dialogue)) {
    return 1;
  }

  // Test /chat/clear-context
  if (!client.clear_context()) {
    return 1;
  }

  // Test /chat/change-prompt
  json new_prompt = {
      {{"role", "system"}, {"content", "This is the new prompt"}}};
  if (!client.change_prompt(new_prompt)) {
    return 1;
  }

  // Test /chat/change-model
  if (!client.change_model("meta-llama/Meta-Llama-3-70B-Instruct")) {
    return 1;
  }

  // Test /chat/generate
  try {
    std::string response = client.generate_response("Hello, how are you?");
    std::cout << "Generated response: " << response << '\n';
  } catch (const std::exception &e) {
    std::cerr << "Error during generate_response: " << e.what() << '\n';
    return 1;
  }

  // Test /chat/delete-message
  if (!client.delete_message(0)) {
    return 1;
  }

  // Test /chat/end
  try {
    if (client.end_chat()) {
      std::cout << "Chat history saved successfully.\n";
    }
  } catch (const std::exception &e) {
    std::cerr << "Error during end_chat: " << e.what() << '\n';
    return 1;
  }

  return 0;
}