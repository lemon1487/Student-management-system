#ifndef COMMON_H
#define COMMON_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

// 常量定义
#define MAX_ID_LEN 12        // 学号长度
#define MAX_NAME_LEN 20      // 姓名长度
#define MAX_CLASS_LEN 20     // 班级长度
#define MAX_ACCOUNT_LEN 20   // 账号长度
#define MAX_PWD_LEN 20       // 密码长度
#define MAX_CONTENT_LEN 100  // 申诉/待办内容长度
#define MAX_USER 100         // 最大用户数
#define MAX_APPEAL 100       // 最大申诉数
#define MAX_TODO 100         // 最大待办数
#define SORT_ASC 0           // 升序
#define SORT_DESC 1          // 降序

// 用户类型枚举
typedef enum {
    ADMIN = 1,
    TEACHER = 2,
    STUDENT = 3
} UserType;

// 学生结构体
typedef struct {
    char id[MAX_ID_LEN];     // 学号
    char name[MAX_NAME_LEN]; // 姓名
    char class[MAX_CLASS_LEN];// 班级
    float math;              // 数学成绩
    float chinese;           // 语文成绩
    float english;           // 英语成绩
    float avg;               // 平均分
    float total;             // 总分
} Student;

// 学生链表节点
typedef struct StudentNode {
    Student data;
    struct StudentNode* next;
} StudentNode;

// 用户结构体
typedef struct {
    char account[MAX_ACCOUNT_LEN]; // 账号
    char pwd[MAX_PWD_LEN];         // 密码
    UserType type;                 // 用户类型
} User;

// 用户链表节点
typedef struct UserNode {
    User data;
    struct UserNode* next;
} UserNode;

// 申诉结构体
typedef struct {
    char stu_id[MAX_ID_LEN];       // 学生学号
    char content[MAX_CONTENT_LEN]; // 申诉内容
    int type;                      // 1-成绩申诉 2-密码找回 3-用户注册
    int is_processed;              // 0-未处理 1-已处理
    char feedback[MAX_CONTENT_LEN];// 处理反馈
} Appeal;

// 待办事项结构体
typedef struct {
    char content[MAX_CONTENT_LEN]; // 待办内容
    int is_done;                   // 0-未完成 1-已完成
} Todo;

// 公共工具函数声明
void clear_buffer();                // 清空输入缓冲区
int check_score(float score);       // 校验成绩范围（0-100）
void calc_student_score(Student* stu); // 计算总分/平均分
void sort_students(Student* stu_list, int count, int field, int type); // 成绩排序

// 功能函数声明
void register_user(UserNode** user_head, int* user_count, Appeal* appeal_list, int* appeal_count, Todo* todo_list, int* todo_count);//用户注册
void find_password(UserNode* user_head, int user_count, Appeal* appeal_list, int* appeal_count, Todo* todo_list, int* todo_count);//密码找回
int get_student_rank(StudentNode* stu_head, int stu_count, char* stu_id, int field);//成绩排名

#endif // COMMON_H