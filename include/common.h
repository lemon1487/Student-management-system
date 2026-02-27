#ifndef COMMON_H
#define COMMON_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

// 系统容量/长度/功能常量
#define MAX_USER 100        // 最大用户数
#define MAX_STUDENT 1000    // 最大学生数
#define MAX_APPEAL 50       // 最大申诉数
#define MAX_TODO 50         // 最大待办数
#define MAX_ACCOUNT_LEN 20  // 账号长度上限
#define MAX_PWD_LEN 20      // 密码长度上限
#define MAX_ID_LEN 12       // 学号/ID长度上限
#define MAX_NAME_LEN 20     // 姓名长度上限
#define MAX_CLASS_LEN 20    // 班级长度上限
#define MAX_APPEAL_LEN 100  // 申诉内容长度上限
#define MAX_TODO_LEN 100    // 待办内容长度上限
#define PAGE_SIZE 10        // 分页每页条数
#define ADMIN 1             // 管理员类型
#define TEACHER 2           // 教师类型
#define STUDENT 3           // 学生类型
#define SORT_ASC 0          // 升序排序
#define SORT_DESC 1         // 降序排序

// 用户登录信息结构体
typedef struct {
    char account[MAX_ACCOUNT_LEN]; // 账号
    char pwd[MAX_PWD_LEN];         // 密码
    int type;                      // 用户类型
} User;

// 学生信息与成绩结构体
typedef struct {
    char id[MAX_ID_LEN];    // 学号
    char name[MAX_NAME_LEN];// 姓名
    char class[MAX_CLASS_LEN];// 班级
    float math;             // 数学成绩
    float chinese;          // 语文成绩
    float english;          // 英语成绩
    float avg;              // 平均分
    float total;            // 总分
} Student;

// 申诉记录结构体
typedef struct {
    char stu_id[MAX_ID_LEN];// 申诉学生学号
    char content[MAX_APPEAL_LEN];// 申诉内容
    int type;               // 申诉类型(成绩/密码)
    int is_processed;       // 处理状态
    char feedback[MAX_APPEAL_LEN];// 处理反馈
} Appeal;

// 待办事项结构体
typedef struct {
    char content[MAX_TODO_LEN];// 待办内容
    int is_done;               // 完成状态
} Todo;

// 工具函数
void clear_buffer();// 清空输入缓冲区
int check_score(float score);// 校验成绩合法性
void calc_student_score(Student* stu);// 计算学生总分/平均分
void sort_students(Student* stu_list, int count, int field, int type);// 学生成绩排序

// 文件操作函数
void save_user(User* user_list, int count);// 保存用户数据
int load_user(User* user_list, int max_count);// 加载用户数据
void save_student(Student* stu_list, int count);// 保存学生数据
int load_student(Student* stu_list, int max_count);// 加载学生数据
void save_todo(Todo* todo_list, int count);// 保存待办数据
int load_todo(Todo* todo_list, int max_count);// 加载待办数据
void save_appeal(Appeal* appeal_list, int count);// 保存申诉数据
int load_appeal(Appeal* appeal_list, int max_count);// 加载申诉数据
void export_users(User* user_list, int count);// 导出用户数据
int import_users(User* user_list, int* count);// 导入用户数据

// 菜单入口函数
void admin_menu(Student* stu_list, int* stu_count, User* user_list, int* user_count,
    Appeal* appeal_list, int* appeal_count, Todo* todo_list, int* todo_count);// 管理员菜单
void teacher_menu(Student* stu_list, int* stu_count, User* user_list, int user_count, char* account);// 教师菜单
void student_menu(Student* stu_list, int stu_count, User* user_list, int user_count,
    char* account, Appeal* appeal_list, int* appeal_count,
    Todo* todo_list, int* todo_count);// 学生菜单

// 学生功能函数
void view_my_score(Student* stu_list, int stu_count, char* account);// 查看个人成绩
void view_class_score(Student* stu_list, int stu_count, char* account);// 查看本班成绩
void score_analysis(Student* stu_list, int stu_count, char* account);// 成绩分析
void student_change_pwd(User* user_list, int user_count, char* account);// 学生改密码

// 教师功能函数
void manage_student(Student* stu_list, int* stu_count);// 管理学生信息
void add_student(Student* stu_list, int* stu_count);// 添加学生
void del_student(Student* stu_list, int* stu_count);// 删除学生
void edit_student(Student* stu_list, int stu_count);// 编辑学生成绩
void query_student(Student* stu_list, int stu_count);// 查询学生
void view_class_score_page(Student* stu_list, int stu_count);// 分页查班级成绩
void export_student(Student* stu_list, int stu_count);// 导出学生数据
void teacher_score_analysis(Student* stu_list, int stu_count);// 成绩统计分析
void teacher_change_pwd(User* user_list, int user_count, char* account);// 教师改密码

// 管理员功能函数
void register_user(User* user_list, int* user_count);// 注册用户
void manage_user(User* user_list, int* user_count);// 管理用户
void manage_todo(Todo* todo_list, int* todo_count);// 管理待办
void manage_appeal(Appeal* appeal_list, int* appeal_count, User* user_list, int user_count);// 处理申诉
void admin_change_pwd(User* user_list, int user_count);// 管理员改密码
void submit_find_pwd_appeal(Appeal* appeal_list, int* appeal_count, Todo* todo_list, int* todo_count, char* account);// 提交找回密码申诉

#endif