#ifndef STUDENT_FUNC_H
#define STUDENT_FUNC_H

#include "common.h"

// 学生功能函数声明
void student_menu(Student* stu_list, int stu_count, User* user_list, int user_count,
    char* account, Appeal* appeal_list, int* appeal_count,
    Todo* todo_list, int* todo_count);// 学生主菜单入口
void view_my_score(Student* stu_list, int stu_count, char* account); // 查看个人成绩信息
void view_class_score(Student* stu_list, int stu_count, char* account); // 查看本班成绩信息
void score_analysis(Student* stu_list, int stu_count, char* account); // 查看本班成绩排名分析
void student_change_pwd(User* user_list, int user_count, char* account); // 学生修改自身密码

#endif