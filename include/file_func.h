#ifndef FILE_FUNC_H
#define FILE_FUNC_H

#include "common.h"

// 保存用户数据到 ./data/users.dat
void save_user(UserNode* head, int count);
// 加载用户数据
int load_user(UserNode** head, int* count);
// 保存学生数据到 ./data/students.dat
void save_student(StudentNode* head, int count);
// 加载学生数据
int load_student(StudentNode** head, int* count);
// 保存待办事项到 ./data/todo.dat
void save_todo(Todo* todo_list, int count);
// 加载待办事项
int load_todo(Todo* todo_list, int max_count);
// 保存申诉数据到 ./data/appeal.dat
void save_appeal(Appeal* appeal_list, int count);
// 加载申诉数据
int load_appeal(Appeal* appeal_list, int max_count);
// 导出用户数据到 ./data/users_export.csv
void export_users(UserNode* head, int count);
// 导入用户数据
int import_users(UserNode** head, int* count);

#endif // FILE_FUNC_H