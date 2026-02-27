#ifndef FILE_FUNC_H
#define FILE_FUNC_H

#include "common.h"

// 文件操作函数声明
int load_user(User* user_list, int max_count);       // 从文件加载用户数据
void save_user(User* user_list, int count);          // 保存用户数据到文件
int load_student(Student* stu_list, int max_count);  // 从文件加载学生数据
void save_student(Student* stu_list, int count);     // 保存学生数据到文件
int load_todo(Todo* todo_list, int max_count);       // 从文件加载待办数据
void save_todo(Todo* todo_list, int count);          // 保存待办数据到文件
int load_appeal(Appeal* appeal_list, int max_count); // 从文件加载申诉数据
void save_appeal(Appeal* appeal_list, int count);    // 保存申诉数据到文件

#endif