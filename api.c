#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <curl/curl.h>

// Define a structure to hold response data
struct Memory {
    char *response;
    size_t size;
};

// Callback function to handle data received from the server
static size_t WriteCallback(void *contents, size_t size, size_t nmemb, void *userp) {
    size_t totalSize = size * nmemb;
    struct Memory *mem = (struct Memory *)userp;

    // Allocate memory for the response
    char *ptr = realloc(mem->response, mem->size + totalSize + 1);
    if (ptr == NULL) {
        printf("Not enough memory to allocate!\n");
        return 0;
    }

    mem->response = ptr;
    memcpy(&(mem->response[mem->size]), contents, totalSize);
    mem->size += totalSize;
    mem->response[mem->size] = '\0';

    return totalSize;
}

// Function to search for "plus_code" in the JSON response
void extractPlusCode(const char *json) {
    const char *key = "\"plus_code\":\"";
    char *start = strstr(json, key);

    if (start) {
        start += strlen(key);  // Move pointer to the start of the value
        char *end = strchr(start, '"');
        if (end) {
            char plus_code[100];
            strncpy(plus_code, start, end - start);
            plus_code[end - start] = '\0';
            printf("Plus code: %s\n", plus_code);
            return;
        }
    }
    printf("Plus code not found.\n");
}

int main() {
    char location[100];
    printf("Enter location: ");
    fgets(location, sizeof(location), stdin);

    // Remove newline character from input
    size_t len = strlen(location);
    if (len > 0 && location[len - 1] == '\n') {
        location[len - 1] = '\0';
    }

    // URL encoding for the location
    char encodedLocation[200];
    snprintf(encodedLocation, sizeof(encodedLocation), "http://py4e-data.dr-chuck.net/opengeo?q=%s", location);

    printf("Retrieving %s\n", encodedLocation);

    CURL *curl;
    CURLcode res;
    struct Memory chunk = {0};

    // Initialize CURL
    curl_global_init(CURL_GLOBAL_DEFAULT);
    curl = curl_easy_init();
    if (curl) {
        curl_easy_setopt(curl, CURLOPT_URL, encodedLocation);
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void *)&chunk);

        // Perform the request
        res = curl_easy_perform(curl);
        if (res != CURLE_OK) {
            fprintf(stderr, "CURL failed: %s\n", curl_easy_strerror(res));
        } else {
            printf("Retrieved %lu characters\n", (unsigned long)chunk.size);
            extractPlusCode(chunk.response);
        }

        // Cleanup
        curl_easy_cleanup(curl);
    }

    free(chunk.response);
    curl_global_cleanup();
    return 0;
}
