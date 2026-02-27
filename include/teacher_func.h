#ifndef TEACHER_FUNC_H
#define TEACHER_FUNC_H

#include "common.h"

// 教师功能函数声明
void teacher_menu(Student* stu_list, int* stu_count, User* user_list, int user_count, char* account);// 教师主菜单入口
void manage_student(Student* stu_list, int* stu_count);    // 学生信息管理总入口（增删改查）
void add_student(Student* stu_list, int* stu_count);       // 添加新学生信息
void del_student(Student* stu_list, int* stu_count);       // 删除指定学生信息
void edit_student(Student* stu_list, int stu_count);       // 编辑学生成绩信息
void query_student(Student* stu_list, int stu_count);      // 查询学生信息（学号/姓名/班级）
void view_class_score_page(Student* stu_list, int stu_count);// 分页查看班级成绩
void export_student(Student* stu_list, int stu_count);     // 导出学生数据到文件
void teacher_score_analysis(Student* stu_list, int stu_count);// 学生成绩统计分析
void teacher_change_pwd(User* user_list, int user_count, char* account); // 教师修改自身密码

#endif