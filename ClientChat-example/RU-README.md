## Описание

`ChatClient` - это классовая обертка на C++ для взаимодействия с серверными API-чата. Этот клиент позволяет управлять сессиями чата, изменять модель и контекст, генерировать ответы и сохранять историю чата в файлы.

## Использование

### Инициализация клиента

```cpp
const std::string base_url = "http://IP_ADRESS:PORT/chat/";
ChatClient client(base_url);
```

### Методы

#### start_chat

```cpp
bool start_chat(const std::string &model, const json &prompt, const json &dialogue);
```

Запускает новый чат с указанной моделью, подсказкой и диалогом.

- `model`: Название модели, используемой для чата.
- `prompt`: JSON объект, представляющий начальную подсказку.
- `dialogue`: JSON объект, представляющий начальный диалог.

Пример использования:

```cpp
json prompt = {
    {{"role", "system"}, {"content", "This is the default prompt"}}
};
json dialogue = {
    {{"role", "user"}, {"content", "hi"}},
    {{"role", "assistant"}, {"content", "yo wassup"}}
};

if (!client.start_chat("meta-llama/Meta-Llama-3-8B-Instruct", prompt, dialogue)) {
    return 1;
}
```

#### clear_context

```cpp
bool clear_context();
```

Очищает текущий контекст чата.

Пример использования:

```cpp
if (!client.clear_context()) {
    return 1;
}
```

#### change_prompt

```cpp
bool change_prompt(const json &new_prompt);
```

Изменяет текущую подсказку чата.

- `new_prompt`: Новый JSON объект, представляющий подсказку.

Пример использования:

```cpp
json new_prompt = {
    {{"role", "system"}, {"content", "This is the new prompt"}}
};
if (!client.change_prompt(new_prompt)) {
    return 1;
}
```

#### change_model

```cpp
bool change_model(const std::string &new_model);
```

Изменяет текущую модель чата.

- `new_model`: Название новой модели.

Пример использования:

```cpp
if (!client.change_model("meta-llama/Meta-Llama-3-70B-Instruct")) {
    return 1;
}
```

#### generate_response

```cpp
std::string generate_response(const std::string &message);
```

Генерирует ответ на заданное сообщение.

- `message`: Сообщение, на которое нужно сгенерировать ответ.

Пример использования:

```cpp
try {
    std::string response = client.generate_response("Hello, how are you?");
    std::cout << "Generated response: " << response << '\n';
} catch (const std::exception &e) {
    std::cerr << "Error during generate_response: " << e.what() << '\n';
    return 1;
}
```

#### delete_message

```cpp
bool delete_message(int message_index);
```

Удаляет сообщение по указанному индексу из текущего диалога.

- `message_index`: Индекс удаляемого сообщения.

Пример использования:

```cpp
if (!client.delete_message(0)) {
    return 1;
}
```

#### end_chat

```cpp
bool end_chat();
```

Завершает текущий чат и сохраняет историю в файл.

Пример использования:

```cpp
try {
    if (client.end_chat()) {
        std::cout << "Chat history saved successfully.\n";
    }
} catch (const std::exception &e) {
    std::cerr << "Error during end_chat: " << e.what() << '\n';
    return 1;
}
```

### Пример использования

Полный пример использования, включая инициализацию клиента и вызов всех методов:

```cpp
int main() {
    const std::string base_url = "http://IP_ADRESS:PORT/chat/";
    ChatClient client(base_url);

    // Test /chat/start
    json prompt = {
        {{"role", "system"}, {"content", "This is the default prompt"}}
    };
    json dialogue = {
        {{"role", "user"}, {"content", "hi"}},
        {{"role", "assistant"}, {"content", "yo wassup"}}
    };

    if (!client.start_chat("meta-llama/Meta-Llama-3-8B-Instruct", prompt, dialogue)) {
        return 1;
    }

    // Test /chat/clear-context
    if (!client.clear_context()) {
        return 1;
    }

    // Test /chat/change-prompt
    json new_prompt = {
        {{"role", "system"}, {"content", "This is the new prompt"}}
    };
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
```

### Программные требования

- **CPR**: Библиотека для HTTP-запросов.
- **nlohmann/json**: Библиотека для работы с JSON.
- **C++17**: Обязательная поддержка C++17, так как используется стандартная библиотека `filesystem`.

### Компиляция

Компилировать код с использованием флагов C++17:

```sh
g++ -std=c++17 -o chat_client main.cpp -lcpr -lssl -lcrypto
```

### Запуск

Исполняемый файл `chat_client` можно запустить следующей командой:

```sh
./chat_client
```

Этот документ предоставляет подробное руководство по использованию класса `ChatClient` для взаимодействия с API-чата, включая примеры кода и команды для компиляции и запуска приложения.
