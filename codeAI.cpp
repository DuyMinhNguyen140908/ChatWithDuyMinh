#include <iostream>
#include <cpprest/http_client.h>
#include <cpprest/filestream.h>

using namespace web::http;
using namespace web::http::client;

void getGPTResponse(const std::wstring& inputText) {
    utility::string_t apiKey = U("YOUR_API_KEY");  // Thay YOUR_API_KEY bằng API key của bạn
    utility::string_t apiUrl = U("https://api.openai.com/v1/engines/davinci/completions");

    web::http::client::http_client client(apiUrl);
    web::http::http_request request(methods::POST);
    request.headers().set_content_type(U("application/json"));
    request.headers().set_authorization(U("Bearer ") + apiKey);

    json::value requestData;
    requestData[U("prompt")] = json::value::string(inputText);

    request.set_body(requestData);

    client.request(request).then([](http_response response) {
        if (response.status_code() == status_codes::OK) {
            return response.extract_json();
        }
        return pplx::task_from_result(json::value());
    }).then([](json::value jsonResponse) {
        if (!jsonResponse.is_null()) {
            std::wcout << L"AI Response: " << jsonResponse[U("choices")][0][U("text")].as_string() << std::endl;
        } else {
            std::wcerr << L"Error in API response." << std::endl;
        }
    }).wait();
}

int main() {
    std::wcout << L"Enter your prompt: ";
    std::wstring inputText;
    std::getline(std::wcin, inputText);

    getGPTResponse(inputText);

    return 0;
}
