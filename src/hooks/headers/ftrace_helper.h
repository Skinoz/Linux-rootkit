#ifndef FTRACE_HELPER_H
#define FTRACE_HELPER_H

#include <linux/ftrace.h>
#include <linux/linkage.h>
#include <linux/slab.h>
#include <linux/uaccess.h>
#include <linux/version.h>

struct ftrace_hook {
    const char *name;
    void *function;
    void *original;

    unsigned long address;
    struct ftrace_ops ops;
};

#define HOOK(_name, _hook, _orig)   \
{                   \
    .name = (_name),        \
    .function = (_hook),        \
    .original = (_orig),        \
}

/* Function declarations */
int fh_install_hook(struct ftrace_hook *hook);
void fh_remove_hook(struct ftrace_hook *hook);
int fh_install_hooks(struct ftrace_hook *hooks, size_t count);
void fh_remove_hooks(struct ftrace_hook *hooks, size_t count);

void install_hook(void);
void exit_hook(void);

#endif /* FTRACE_HELPER_H */
