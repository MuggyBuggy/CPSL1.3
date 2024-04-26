#include "mbed.h"
#include "WiFiInterface.h"

// WiFi credentials
#define WIFI_SSID "Bobcom"
#define WIFI_PASSWORD "robertbuggy"
#define MAX_RETRY_ATTEMPTS 5
#define RETRY_DELAY_MS 5000000 // 5 seconds delay between retry attempts

WiFiInterface *wifi; // Pointer to WiFi interface

int main() {
    // Instantiate the WiFi interface
    wifi = WiFiInterface::get_default_instance();
    if (!wifi) {
        printf("WiFi not available!\n");
        return -1;
    }

    // Retry connecting to WiFi
    printf("Connecting to WiFi...\n");
    int retry_count = 0;
    int ret = -1;
    while (retry_count < MAX_RETRY_ATTEMPTS && ret != 0) {
        ret = wifi->connect(WIFI_SSID, WIFI_PASSWORD);
        if (ret != 0) {
            printf("Connection attempt %d failed. Retrying in %d seconds...\n", retry_count + 1, RETRY_DELAY_MS / 1000);
            wait_us(RETRY_DELAY_MS);
        }
        retry_count++;
    }

    if (ret != 0) {
        printf("Failed to connect to WiFi after %d attempts.\n", MAX_RETRY_ATTEMPTS);
        return -1;
    }

    // Get IP address and print network information
    SocketAddress address;
    wifi->get_ip_address(&address);
    printf("Success! Connected to WiFi. IP address: %s\n", address.get_ip_address());

    // Report successful connection
    printf("WiFi connection successful!\n");

    // Disconnect from WiFi
    wifi->disconnect();
    printf("Disconnected from WiFi\n");

    return 0;
}
