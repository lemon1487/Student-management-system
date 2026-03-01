#ifndef STUDENT_FUNC_H
#define STUDENT_FUNC_H

#include "common.h"
//学生菜单展示
void student_menu(StudentNode* stu_head, int stu_count, UserNode* user_head, int user_count, char* account,
    Appeal* appeal_list, int* appeal_count, Todo* todo_list, int* todo_count);
//个人成绩
void student_view_score(StudentNode* stu_head, int stu_count, char* account);
//学生成绩分析
void student_score_analysis(StudentNode* stu_head, int stu_count, char* account);
//成绩申诉
void student_submit_appeal(Appeal* appeal_list, int* appeal_count, Todo* todo_list, int* todo_count, char* account);
//密码修改
void student_change_pwd(UserNode* user_head, int user_count, char* account);
//查看本班成绩
void student_view_class_score(StudentNode* stu_head, int stu_count, char* account);

#endif // STUDENT_FUNC_H