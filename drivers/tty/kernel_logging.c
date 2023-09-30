#include "logging.h"

static char* error_info[] = {
    "[ INFO ]",
    "[ WARNING ]",
    "[ ERROR ]",
    "[ FATAL ]"
};

// ----------|
// Utilities |
// ----------|

static void reverse_string(char* string, uint32_t length) {
    uint32_t start = 0;
    uint32_t end = length - 1;
    while (start < end) {
        char temp = string[start];
        string[start] = string[end];
        string[end] = temp;
        start++;
        end--;
    }
}

//TODO: move itoa to a library file

// Integer to all function from https://www.geeksforgeeks.org/implement-itoa/
static char* itoa(int integer, char* buffer_string, int base) {
    int i = 0;
    bool isNegative = false;

    /* Handle 0 explicitly, otherwise empty string is
     * printed for 0 */
    if (integer == 0) {
        buffer_string[i++] = '0';
        buffer_string[i] = '\0';
        return buffer_string;
    }

    // In standard itoa(), negative numbers are handled
    // only with base 10. Otherwise numbers are
    // considered unsigned.
    if (integer < 0 && base == 10) {
        isNegative = true;
        integer = -integer;
    }
 
    // Process individual digits
    while (integer != 0) {
        int rem = integer % base;
        buffer_string[i++] = (rem > 9) ? (rem - 10) + 'a' : rem + '0';
        integer = integer / base;
    }

    // If number is negative, append '-'
    if (isNegative)
        buffer_string[i++] = '-';

    buffer_string[i] = '\0'; // Append string terminator
 
    // Reverse the string
    reverse_string(buffer_string, i);

    return buffer_string;
}

// ------------------|
// Logging functions |
// ------------------|

// The classic printf function but reserved for the kernel and only prints to the tty textmode terminal for now.
void kprintf(char* fmt, ...) {
    va_list args;
    va_start(args, fmt);

    // This may be a little unreadable, however, its simple so its staying for the moment.
    while (*fmt != '\0') {
        if (*fmt == '%') {
            fmt++;
            char buffer[32];
            switch (*fmt) {
                case 'd':
                    itoa(va_arg(args, int), buffer, 10);
                    kprint(buffer);
                    break;
                case 'u':
                    itoa(va_arg(args, uint32_t), buffer, 10);
                    kprint(buffer);
                    break;
                case 'x':
                    itoa(va_arg(args, int), buffer, 16);
                    kprint(buffer);
                    break;
                case 's':
                    kprint(va_arg(args, char*));
                    break;
                case 'c':
                    kputc(va_arg(args, int));
                    break;
                case '%':
                    kputc('%');
                    break;
                case 'b':
                    itoa(va_arg(args, int), buffer, 2);
                    kprint(buffer);
                    break;
                case 'o':
                    itoa(va_arg(args, int), buffer, 8);
                    kprint(buffer);
                    break;
                default:
                    // print error
                    break;
            }
        } else {
            kputc(*fmt);
        }
        fmt++;
    }
}

// Logs an error, fatal, warn or info message to the screen
void log_attribute(error_type_t type, char* message) {
    kprintf("%s %s\n", error_info[type], message);
}
