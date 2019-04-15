/* UART asynchronous example, that uses separate RX and TX tasks

   This example code is in the Public Domain (or CC0 licensed, at your option.)

   Unless required by applicable law or agreed to in writing, this
   software is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
   CONDITIONS OF ANY KIND, either express or implied.
*/
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_system.h"
#include "esp_log.h"
#include "driver/uart.h"
#include "soc/uart_struct.h"
#include "string.h"
#include "packet_structure.h"

static const int RX_BUF_SIZE = 1024;

#define TXD_PIN (GPIO_NUM_4)
#define RXD_PIN (GPIO_NUM_5)
   
int sendData(const char* logName, const uint8_t* data,int len);
int send_packet (void)
{
    struct s1 packet;
    uint8_t frame_header_version [16] = {0x55aa , 0x00};
    uint8_t data_length [16] = {0x00, 0x05};
    uint8_t functional_length [16] = {0x00 , 0x01};
    packet.frame_header_version = frame_header_version;
    packet.command_word = 0x06;
    packet.data_length = data_length;
    packet.dpID = 0x6a;
    packet.type_of_data=0x01;
    packet.functional_length=functional_length;
    packet.functional_instruction=0x00;
    packet.checksum=0x0d;

    struct s1 packet2;
    packet.frame_header_version = frame_header_version;
    packet.command_word = 0x07;
    packet.data_length = data_length;
    packet.dpID = 0x6a;
    packet.type_of_data=0x01;
    packet.functional_length=functional_length;
    packet.functional_instruction=0x01;
    packet.checksum=0x0e;

    int arr_send[16]={0x55,0xaa,0x00,0x06,0x00,0x05,0x01,0x01,0x00,0x01,0x00,0x0d};
    int arr_receive[16]={0x55,0xaa,0x00,0x07,0x00,0x05,0x01,0x01,0x00,0x01,0x01,0x0e};
    sendData("hello", arr_send, 12);
    return 0;
}

void init() {
    const uart_config_t uart_config = {
        .baud_rate = 9600,
        .data_bits = UART_DATA_8_BITS,
        .parity = UART_PARITY_DISABLE,
        .stop_bits = UART_STOP_BITS_1,
        .flow_ctrl = UART_HW_FLOWCTRL_DISABLE
    };
    uart_param_config(UART_NUM_0, &uart_config);
    uart_set_pin(UART_NUM_0, TXD_PIN, RXD_PIN, UART_PIN_NO_CHANGE, UART_PIN_NO_CHANGE);
    // We won't use a buffer for sending data.
    uart_driver_install(UART_NUM_0, RX_BUF_SIZE * 2, 0, 0, NULL, 0);
}

int sendData(const char* logName, const uint8_t* data, int len)
{
    const int txBytes = uart_write_bytes(UART_NUM_0, (char*) data, len);
    ESP_LOGI(logName, "Wrote %d bytes", txBytes);
    return txBytes;
}

static void tx_task()
{
    static const char *TX_TASK_TAG = "TX_TASK";
    esp_log_level_set(TX_TASK_TAG, ESP_LOG_INFO);
    while (1) {
        send_packet();
        // sendData(TX_TASK_TAG, "arr_send);
        vTaskDelay(2000 / portTICK_PERIOD_MS);
    }
}

static void rx_task()
{
    static const char *RX_TASK_TAG = "RX_TASK";
    esp_log_level_set(RX_TASK_TAG, ESP_LOG_INFO);
    uint8_t* data = (uint8_t*) malloc(RX_BUF_SIZE+1);
    while (1) {
        const int rxBytes = uart_read_bytes(UART_NUM_0, data, RX_BUF_SIZE, 1000 / portTICK_RATE_MS);
        if (rxBytes > 0) {
            data[rxBytes] = 0;
            ESP_LOGI(RX_TASK_TAG, "Read %d bytes: '%s'", rxBytes, data);
            ESP_LOG_BUFFER_HEXDUMP(RX_TASK_TAG, data, rxBytes, ESP_LOG_INFO);
        }
    }
    free(data);
}

void app_main()
{
    init();
    xTaskCreate(rx_task, "uart_rx_task", 1024*2, NULL, configMAX_PRIORITIES, NULL);
    xTaskCreate(tx_task, "uart_tx_task", 1024*2, NULL, configMAX_PRIORITIES-1, NULL);
    send_packet();
}
