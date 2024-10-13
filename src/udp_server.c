#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/printk.h>
#include "include/udp_server.h"

/*DEBUG DEFINE*/
#define MODULE_NAME "ksocket"

struct kthread_t *kthread = NULL;

static void ksocket_start(void)
{
    int size, err;
    int bufsize = BUFFER_SIZE;  // Increased buffer size to 100 bytes
    unsigned char buf[BUFFER_SIZE + 1];

    kthread->running = 1;

    // Create UDP sockets for receiving and sending
    if (((err = sock_create(AF_INET, SOCK_DGRAM, IPPROTO_UDP, &kthread->sock)) < 0) ||
        ((err = sock_create(AF_INET, SOCK_DGRAM, IPPROTO_UDP, &kthread->sock_send)) < 0)) {
        printk(KERN_INFO MODULE_NAME ": Could not create a datagram socket, error = %d\n", -ENXIO);
        ksocket_stop();
    }

    memset(&kthread->addr, 0, sizeof(struct sockaddr));
    memset(&kthread->addr_send, 0, sizeof(struct sockaddr));
    kthread->addr.sin_family = AF_INET;
    kthread->addr_send.sin_family = AF_INET;

    kthread->addr.sin_addr.s_addr = htonl(INADDR_ANY);
    kthread->addr_send.sin_addr.s_addr = htonl(INADDR_SEND);

    kthread->addr.sin_port = htons(DEFAULT_PORT);
    kthread->addr_send.sin_port = htons(CONNECT_PORT);

    if (((err = kthread->sock->ops->bind(kthread->sock, (struct sockaddr *)&kthread->addr, sizeof(struct sockaddr))) < 0) ||
        (err = kthread->sock_send->ops->connect(kthread->sock_send, (struct sockaddr *)&kthread->addr_send, sizeof(struct sockaddr), 0) < 0)) {
        printk(KERN_INFO MODULE_NAME ": Could not bind or connect to socket, error = %d\n", -err);
        ksocket_stop();
    }

    printk(KERN_INFO MODULE_NAME ": listening on port %d\n", DEFAULT_PORT);

    // Main loop
    for (;;) {
        memset(&buf, 0, bufsize + 1);
        size = ksocket_receive(kthread->sock, &kthread->addr, buf, bufsize);

        if (signal_pending(current))
            break;

        if (size < 0) {
            printk(KERN_INFO MODULE_NAME ": error getting datagram, sock_recvmsg error = %d\n", size);
        } else {
            if (size >= BUFFER_SIZE)
            {
                printk(KERN_INFO MODULE_NAME ": received %d bytes, but buffer is too small\n", size);
                continue;
            }
            
            printk(KERN_INFO MODULE_NAME ": received %d bytes\n", size);
            printk(KERN_INFO MODULE_NAME ": received data: %s\n", buf);

            // Sending response "OK"
            memset(&buf, 0, bufsize + 1);  // Clear the buffer
            strcpy(buf, "OK");  // Set response to "OK"
            ksocket_send(kthread->sock_send, &kthread->addr_send, buf, strlen(buf));
            printk(KERN_INFO MODULE_NAME ": sent response: OK\n");
        }
    }

    ksocket_stop();
}

int ksocket_send(struct socket *sock, struct sockaddr_in *addr, unsigned char *buf, int len)
{
        struct msghdr msg = { 0 };
        struct kvec iov;
        int size = 0;

        if (sock->sk == NULL)
                return 0;

        iov.iov_base = buf;
        iov.iov_len = len;

        msg.msg_name = addr;
        msg.msg_namelen = sizeof(struct sockaddr_in);

        size = kernel_sendmsg(sock, &msg, &iov, 1, len);
        return size;
}

int ksocket_receive(struct socket *sock, struct sockaddr_in *addr, unsigned char *buf, int len)
{
        struct msghdr msg = { 0 };
        struct kvec iov;
        int size = 0;

        if (sock->sk == NULL)
                return 0;

        iov.iov_base = buf;
        iov.iov_len = len;

        msg.msg_name = addr;
        msg.msg_namelen = sizeof(struct sockaddr_in);

        size = kernel_recvmsg(sock, &msg, &iov, 1, len, msg.msg_flags);
        return size;
}

void ksocket_stop(void)
{
    if (kthread->sock) {
        sock_release(kthread->sock);  // Release the listening socket
        kthread->sock = NULL;          // Set to NULL for safety
    }
    
    if (kthread->sock_send) {
        sock_release(kthread->sock_send);  // Release the sending socket
        kthread->sock_send = NULL;          // Set to NULL for safety
    }

    kthread->running = 0;  // Mark the thread as not running
    printk(KERN_INFO MODULE_NAME ": sockets released and cleanup done.\n");
}

int ksocket_init(void)
{
        kthread = kmalloc(sizeof(struct kthread_t), GFP_KERNEL);
        memset(kthread, 0, sizeof(struct kthread_t));

        /* start kernel thread */
        kthread->thread = kthread_run((void *)ksocket_start, NULL, MODULE_NAME);
        if (IS_ERR(kthread->thread)) {
                printk(KERN_INFO MODULE_NAME ": unable to start kernel thread\n");
                kfree(kthread);
                kthread = NULL;
                return -ENOMEM;
        }

        return 0;
}

int ksocket_exit(void)
{
    if (kthread->thread) {
        kthread_stop(kthread->thread);  // Stop the kernel thread safely
        kthread->thread = NULL;          // Nullify the thread pointer
    }

    ksocket_stop();  // Call the stop function to cleanup resources

    kfree(kthread);  // Free allocated memory
    kthread = NULL;  // Nullify the kthread pointer

    printk(KERN_INFO MODULE_NAME ": module unloaded\n");
    return 0;
}