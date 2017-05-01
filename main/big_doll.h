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

#include "Arduino.h"
#include "esp32-hal.h"

#define eye_center 1500
#define servo_min 750
#define servo_max 2100
#define delay_time 20
#define timer_divider 80
#define timer_count 80000000/timer_divider*delay_time/1000
typedef struct eye_data{
		int channel_y;
		int channel_y_gpio;
		int channel_x;
		int channel_x_gpio;
		double x_add,y_add;
		double r;
		double  angle;
		int change_time_ms;
		int rev;
		int loop_time;
}eye_d;

typedef struct bow_data{
	int channel_y;
	int channel_y_gpio;
	int channel_angle;
	int channel_angle_gpio;
	double  angle,y,angle_add,y_add;
	int change_time_ms;
	int rev;
	int loop_time;
}bow_d;


typedef struct ear_data{
	int channel;
	int channel_gpio;
	double angle,duty_add;
	int change_time_ms;
	int rev;
	int loop_time;
}ear_d;

typedef struct  mouth_data{
	int channel;
	int channel_gpio;
	double angle;
	int change_time_ms;
	double duty_add;
	int loop_time;
	int side;
	int rev;

}mouth_d;

typedef struct doll_data{
	eye_d l_eye;
	eye_d r_eye;
	bow_d r_bow;
	bow_d l_bow;
	ear_d l_ear;
	ear_d r_ear;
	mouth_d l_mouth;
	mouth_d r_mouth;
	mouth_d c_mouth;

}doll;

doll doll_default_setting();
void doll_init(doll);
void doll_set(doll);
void eye_init(eye_d);
void mouth_init(mouth_d);
void ear_init(ear_d);
void bow_init(bow_d);

void eye_set(eye_d);
void mouth_set(mouth_d);//max 21.2 deg
void ear_set(ear_d); // max 71.4 deg
void bow_set(bow_d);


