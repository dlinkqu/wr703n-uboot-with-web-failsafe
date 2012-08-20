/*
 *	Copyright 1994, 1995, 2000 Neil Russell.
 *	(See License)
 *	Copyright 2000, 2001 DENX Software Engineering, Wolfgang Denk, wd@denx.de
 */

#include <common.h>
#include <command.h>
#include <net.h>

#if defined(CONFIG_CMD_HTTPD)
#include <httpd.h>
#endif

#include "uip-0.9/uipopt.h"
#include "uip-0.9/uip.h"
#include "uip-0.9/uip_arp.h"


#if defined(CONFIG_CMD_HTTPD)

#define TIMEOUT		5

static int arptimer = 0;

void
HttpdHandler (void)
{
	int i;
	for(i = 0; i < UIP_CONNS; i++) {
		uip_periodic(i);
		if(uip_len > 0) {
			uip_arp_out();
			NetSendHttpd();
		}
	}
	if(++arptimer == 20) {
		uip_arp_timer();
		arptimer = 0;
	}
}

#if 0
static void
HttpdTimeout (void)
{
	puts ("T ");
	NetSetTimeout (TIMEOUT * 1000, HttpdTimeout);
}
#endif

void
HttpdStart (void)
{
	uip_init();
	httpd_init();
}

#if defined(CONFIG_CMD_HTTPD)
int do_http_upgrade(const unsigned char *data, const ulong size)
{
	char buf[256];

	if(getenv ("ram_addr") == NULL)
		return -1;
	if(getenv ("kernel_addr") == NULL)
		return -1;

	//here we upgrade the u-boot,we will keep it less than 64k ...
	if(size < 65536)
	{
		puts("u-boot upgrade\n");
		sprintf(buf, "era 0x9f000000 +0x%lx; cp.b ${ram_addr} 0x9f000000 0x%lx", 0x10000, 0x10000);
		printf("run cmd:%s\n", buf);
		return run_command(buf, 0);
	}

#if 0
	/* check the image */
	if(run_command("imi ${ram_addr}", 0) < 0) {
		return -1;
	}
#endif
	/* write the image to the flash */
	puts("http ugrade ...\n");
	sprintf(buf, "era ${kernel_addr} +0x%lx; cp.b ${ram_addr} ${kernel_addr} 0x%lx", size, size);
	printf("run cmd:%s\n", buf);
	return run_command(buf, 0);
}

extern void ar7240_all_led_on(void);
extern void ar7240_all_led_off(void);
int do_http_progress(const int state)
{
	/* toggle LED's here */
	switch(state) {
		case HTTP_PROGRESS_START:
		puts("http start\n");
		//on led
		ar7240_all_led_on();
		break;
		case HTTP_PROGRESS_TIMEOUT:
#if 0
		puts(".");
#endif
		break;
		case HTTP_PROGRESS_UPLOAD_READY:
		puts("http upload ready\n");
		//blink one time,we are try to erase the flash ...caution
		ar7240_all_led_off();
		udelay(1000 * 500);
		ar7240_all_led_on();
		break;
		case HTTP_PROGRESS_UGRADE_READY:
		puts("http ugrade ready\n");
		//blink three time,all well done.
		ar7240_all_led_off();
		udelay(1000 * 500);
		ar7240_all_led_on();
		udelay(1000 * 500);
		ar7240_all_led_off();
		udelay(1000 * 500);
		ar7240_all_led_on();
		udelay(1000 * 500);
		ar7240_all_led_off();
		udelay(1000 * 500);
		ar7240_all_led_on();
		break;
		case HTTP_PROGRESS_UGRADE_FAILED:
		puts("http ugrade failed\n");
		break;
	}
	return 0;
}

unsigned long do_http_tmp_address(void)
{
	char *s = getenv ("ram_addr");
	if (s) {
		ulong tmp = simple_strtoul (s, NULL, 16);
		return tmp;
	}
	return 0 /*0x80a00000*/;
}

#endif

#endif
