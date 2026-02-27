#include "../include/common.h"

// 清空输入缓冲区
void clear_buffer() {
    while (getchar() != '\n');
}