#ifndef ADMIN_FUNC_H
#define ADMIN_FUNC_H

#include "common.h"

// 管理员主菜单
void admin_menu(StudentNode* stu_head, int stu_count, UserNode* user_head, int user_count,
    Appeal* appeal_list, int appeal_count, Todo* todo_list, int todo_count);
// 管理员修改密码
void admin_change_pwd(UserNode* head, int user_count);
// 用户管理
void manage_user(UserNode** head, int* count);
// 申诉管理
void manage_appeal(Appeal* appeal_list, int* appeal_count, UserNode** user_head, int* user_count);
// 待办管理
void manage_todo(Todo* todo_list, int* todo_count);
// 学生信息管理
void manage_student(StudentNode** stu_head, int* stu_count);

#endif // ADMIN_FUNC_H