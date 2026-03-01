#ifndef TEACHER_FUNC_H
#define TEACHER_FUNC_H

#include "common.h"

// 教师主菜单
void teacher_menu(StudentNode* stu_head, int stu_count, UserNode* user_head, int user_count, char* account);
// 教师修改密码
void teacher_change_pwd(UserNode* user_head, int user_count, char* account);
// 添加学生
void add_student(StudentNode** stu_head, int* stu_count);
// 删除学生
void del_student(StudentNode** stu_head, int* stu_count, char* id);
// 修改学生成绩
void edit_student(StudentNode* stu_head, int stu_count, char* id);
// 查询学生成绩
void query_student(StudentNode* stu_head, int stu_count);
// 分页查看班级成绩
void view_class_score_page(StudentNode* stu_head, int stu_count);
// 教师端成绩分析
void teacher_score_analysis(StudentNode* stu_head, int stu_count);
// 导出学生成绩
void export_student(StudentNode* stu_head, int stu_count);

#endif // TEACHER_FUNC_H