#include <string.h>
#include <stdio.h>
#include <math.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/event_groups.h"
#include "freertos/xtensa_api.h"
#include "freertos/queue.h"
#include "freertos/portmacro.h"

#include "esp_system.h"
#include "esp_wifi.h"
#include "esp_event_loop.h"
#include "esp_log.h"
#include "esp_attr.h"
#include "esp_err.h"
#include "nvs_flash.h"
#include "driver/ledc.h"
#include "driver/pcnt.h"
#include "driver/gpio.h"
#include "driver/periph_ctrl.h"
#include "driver/uart.h"

#include "lwip/err.h"
#include "lwip/sockets.h"
#include "lwip/sys.h"
#include "lwip/netdb.h"
#include "lwip/dns.h"

#include "rom/uart.h"
#include "driver/i2c.h"
#include "soc/soc.h"
#include "soc/ledc_reg.h"
#include "soc/ledc_struct.h"
#include "soc/gpio_sig_map.h"
#include "soc/uart_struct.h"

#include "Arduino.h"
#include "esp32-hal.h"
#include "big_doll.h"
#define true 1
#define false 0

#define RXD1  18
#define TXD1  19
#define RXD2  21
#define TXD2  22
#define BUF_SIZE (1024)
doll d;
void smile(doll *r){
	r->l_mouth.angle=30;
	r->r_mouth.angle=30;
}
void cry(doll *r){
	r->l_mouth.angle=-30;
	r->r_mouth.angle=-30;
}
void normal_face(doll *r){
	r->l_mouth.angle=0;
	r->r_mouth.angle=0;
}
static void big_doll(){

	int l_bt,r_bt,l_x,l_y,r_x,r_y;
    uart_config_t uart_config = {
        .baud_rate = 38400,
        .data_bits = UART_DATA_8_BITS,
        .parity = UART_PARITY_DISABLE,
        .stop_bits = UART_STOP_BITS_1,
        .flow_ctrl = UART_HW_FLOWCTRL_DISABLE,
        .rx_flow_ctrl_thresh = 122,
    };
    uart_param_config(UART_NUM_1, &uart_config);
    uart_set_pin(UART_NUM_1,TXD1, RXD1, NULL, NULL);
    uart_driver_install(UART_NUM_1, BUF_SIZE * 2, 0, 0, NULL, 0);

	uart_param_config(UART_NUM_2, &uart_config);
	uart_set_pin(UART_NUM_2,TXD2, RXD2, NULL, NULL);
	uart_driver_install(UART_NUM_2, BUF_SIZE * 2, 0, 0, NULL, 0);

    uint8_t* left_str = (uint8_t*) malloc(BUF_SIZE);
    uint8_t* right_str = (uint8_t*) malloc(BUF_SIZE);

    memset(left_str,0,BUF_SIZE);
    memset(right_str,0,BUF_SIZE);
	int state=0,left_str_len=0,right_str_len=0;
    while(1) {
	switch(state){
		case 0:
			left_str_len = uart_read_bytes(UART_NUM_1, left_str, BUF_SIZE, 5/ portTICK_RATE_MS);
			if(left_str_len>0)
				state=1;
			break;
		case 1:
			right_str_len = uart_read_bytes(UART_NUM_2, right_str, BUF_SIZE, 5/ portTICK_RATE_MS);
			if(right_str_len>0)
				state=2;
			break;
		case 2:

			if( sscanf((char *)left_str,"%d,%d,%d\n",&l_bt,&l_x,&l_y) == 3 && sscanf((char *)right_str,"%d,%d,%d\n",&r_bt,&r_x,&r_y) == 3		){
				printf("left=%d,%d,%d right=%d,%d,%d\n",l_bt,l_x,l_y,r_bt,r_x,r_y);
				d.l_bow.y=((double)l_y-512)/512*11-10;
				d.r_bow.y=((double)l_y-512)/512*11-9;
				d.l_bow.angle=d.r_bow.angle=((double)l_x-512)/512*30;
				if( (l_bt>>3)%2){
					d.l_ear.angle=43;
				}
				else if(l_bt%2){
					d.l_ear.angle=-25;
				}

				if( (l_bt>>2)%2 ){
					d.c_mouth.angle=30;
				}
				else{
					d.c_mouth.angle =0;
				}
				double eye_x=((double)r_x-511)/1.2;
				double eye_y=((double)r_y-511)/1.2;
				double eye_r = sqrt(pow(eye_x,2) + pow(eye_y,2) );
				double eye_angle=degrees(atan2(eye_y+0.001,eye_x+0.001));
				d.l_eye.r  =d.r_eye.r= eye_r;
				d.l_eye.angle =d.r_eye.angle = eye_angle;

				if( (r_bt>>3)%2){
					d.r_ear.angle=43;
				}
				else if(r_bt%2 ){
					d.r_ear.angle=-25;
				}

				if( (r_bt>>2)%2){
					smile(&d);
				}
				else if( (r_bt>>1)%2){
					cry(&d);
				}
				else if( (l_bt>>1)%2){
					normal_face(&d);
				}
				doll_set(d);
			}
		    memset(left_str,0,BUF_SIZE);
		    memset(right_str,0,BUF_SIZE);
			state=0;
			break;
		}
    }
}

#define EXAMPLE_WIFI_SSID "27H7F"
#define EXAMPLE_WIFI_PASS "0926980187"
static EventGroupHandle_t wifi_event_group;
const int CONNECTED_BIT = BIT0;
static const char *TAG = "example";

static esp_err_t event_handler(void *ctx, system_event_t *event){
    switch(event->event_id) {
    case SYSTEM_EVENT_STA_START:
        esp_wifi_connect();
        break;
    case SYSTEM_EVENT_STA_GOT_IP:
        xEventGroupSetBits(wifi_event_group, CONNECTED_BIT);
        break;
    case SYSTEM_EVENT_STA_DISCONNECTED:
        /* This is a workaround as ESP32 WiFi libs don't currently
           auto-reassociate. */
        esp_wifi_connect();
        xEventGroupClearBits(wifi_event_group, CONNECTED_BIT);
        break;
    default:
        break;
    }
    return ESP_OK;
}
static void initialise_wifi(void){
    tcpip_adapter_init();
    wifi_event_group = xEventGroupCreate();
    ESP_ERROR_CHECK( esp_event_loop_init(event_handler, NULL) );
    wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();
    ESP_ERROR_CHECK( esp_wifi_init(&cfg) );
    ESP_ERROR_CHECK( esp_wifi_set_storage(WIFI_STORAGE_RAM) );
    wifi_config_t wifi_config = {
        .sta = {
            .ssid = EXAMPLE_WIFI_SSID,
            .password = EXAMPLE_WIFI_PASS,
        },
    };
    ESP_LOGI(TAG, "Setting WiFi configuration SSID %s...", wifi_config.sta.ssid);
    ESP_ERROR_CHECK( esp_wifi_set_mode(WIFI_MODE_STA) );
    ESP_ERROR_CHECK( esp_wifi_set_config(ESP_IF_WIFI_STA, &wifi_config) );
    ESP_ERROR_CHECK( esp_wifi_start() );
}
static void wifi_control(void *pvParameters)
{
    const struct addrinfo hints = {
        .ai_family = AF_INET,
        .ai_socktype = SOCK_STREAM,
    };
    struct addrinfo *res;
    struct in_addr *addr;
    int s, r;
    char recv_buf[64];
	char str[200];

        /* Wait for the callback to set the CONNECTED_BIT in the
           event group.
        */
	xEventGroupWaitBits(wifi_event_group, CONNECTED_BIT,
						false, true, portMAX_DELAY);
	ESP_LOGI(TAG, "Connected to AP");

	int err = getaddrinfo("192.168.0.12", "6000", &hints, &res);

	if(err != 0 || res == NULL) {
		ESP_LOGE(TAG, "DNS lookup failed err=%d res=%p", err, res);
		vTaskDelay(1000 / portTICK_PERIOD_MS);
		vTaskDelete(NULL);
	}

	/* Code to print the resolved IP.

	   Note: inet_ntoa is non-reentrant, look at ipaddr_ntoa_r for "real" code */
	addr = &((struct sockaddr_in *)res->ai_addr)->sin_addr;
	ESP_LOGI(TAG, "DNS lookup succeeded. IP=%s", inet_ntoa(*addr));

	s = socket(res->ai_family, res->ai_socktype, 0);
	if(s < 0) {
		ESP_LOGE(TAG, "... Failed to allocate socket.");
		freeaddrinfo(res);
		vTaskDelay(1000 / portTICK_PERIOD_MS);
		vTaskDelete(NULL);
	}
	ESP_LOGI(TAG, "... allocated socket\r\n");

	if(connect(s, res->ai_addr, res->ai_addrlen) != 0) {
		ESP_LOGE(TAG, "... socket connect failed errno=%d", errno);
		close(s);
		freeaddrinfo(res);
		vTaskDelay(4000 / portTICK_PERIOD_MS);
		vTaskDelete(NULL);
	}

	ESP_LOGI(TAG, "... connected");
	freeaddrinfo(res);

	if (write(s, "test", strlen("test")) < 0) {
		ESP_LOGE(TAG, "... socket send failed");
		close(s);
		vTaskDelay(4000 / portTICK_PERIOD_MS);
		vTaskDelete(NULL);
	}
	ESP_LOGI(TAG, "... socket send success");

	/* Read HTTP response */
	while(1){
		bzero(recv_buf, sizeof(recv_buf));
		r = read(s, recv_buf, sizeof(recv_buf)-1);
		if(r){
			sprintf(str," eye %f,%f,%f,%f\n ear %f,%f\n bow %f,%f,%f,%f\n mouth %f,%f,%f"
					,d.l_eye.r,d.l_eye.angle,d.r_eye.r,d.r_eye.angle,
					d.l_ear.angle,d.r_ear.angle,
					d.l_bow.angle,d.l_bow.y,d.r_bow.angle,d.r_bow.y,
					d.l_mouth.angle,d.c_mouth.angle,d.r_mouth.angle);
			write(s,str,strlen(str));
			memset(str,0,sizeof(str));
		}
		for(int i = 0; i < r; i++) {
			putchar(recv_buf[i]);
		}
		printf("%d\n",r);
		delay(100);
	}

}


void app_main(){
    ESP_ERROR_CHECK( nvs_flash_init() );
	d=doll_default_setting();
	doll_init(d);
	initialise_wifi();
    xTaskCreate(big_doll, "big_doll",2048, NULL, 10, NULL);
    xTaskCreate(wifi_control, "wifi_control",4096, NULL, 10, NULL);
}

