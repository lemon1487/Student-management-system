#ifndef ADMIN_FUNC_H
#define ADMIN_FUNC_H

#include "common.h"

// 管理员功能函数声明
void admin_menu(Student* stu_list, int* stu_count, User* user_list, int* user_count,
    Appeal* appeal_list, int* appeal_count, Todo* todo_list, int* todo_count);// 管理员主菜单入口
void register_user(User* user_list, int* user_count); // 主界面注册功能（新增用户账号）
void manage_user(User* user_list, int* user_count);    // 管理所有用户账号（增/删/查）
void manage_todo(Todo* todo_list, int* todo_count);    // 管理待办事项（查看/标记完成/删除）
void manage_appeal(Appeal* appeal_list, int* appeal_count, User* user_list, int user_count);// 处理学生申诉（成绩/密码找回）
void admin_change_pwd(User* user_list, int user_count); // 管理员修改自身密码

// 密码找回相关声明
void submit_find_pwd_appeal(Appeal* appeal_list, int* appeal_count, Todo* todo_list, int* todo_count, char* account);// 提交密码找回申诉

#endif