#include "../include/common.h"
#include "../include/admin_func.h"
#include "../include/teacher_func.h"
#include "../include/student_func.h"
#include "../include/file_func.h"

int main() {
    UserNode* user_head = NULL;
    StudentNode* stu_head = NULL;
    int user_count = 0, stu_count = 0, appeal_count = 0, todo_count = 0;
    Appeal appeal_list[MAX_APPEAL] = { 0 };
    Todo todo_list[MAX_TODO] = { 0 };

    load_user(&user_head, &user_count);
    load_student(&stu_head, &stu_count);
    appeal_count = load_appeal(appeal_list, MAX_APPEAL);
    todo_count = load_todo(todo_list, MAX_TODO);

    // 登录前主菜单
    int pre_choice;
    while (1) {
        printf("\n===== 成绩管理系统 =====\n");
        printf("1. 用户登录\n");
        printf("2. 用户注册\n");
        printf("3. 密码找回\n");
        printf("0. 退出系统\n");
        printf("请选择操作：");

        while (scanf("%d", &pre_choice) != 1 || pre_choice < 0 || pre_choice > 3) {
            clear_buffer();
            printf("输入错误！请输入0-3：");
        }
        clear_buffer();

        switch (pre_choice) {
        case 1: goto LOGIN;
        case 2: register_user(&user_head, &user_count, appeal_list, &appeal_count, todo_list, &todo_count); break;
        case 3: find_password(user_head, user_count, appeal_list, &appeal_count, todo_list, &todo_count); break;
        case 0:
            printf("感谢使用，再见！\n");
            UserNode* u_tmp = NULL;
            while (user_head != NULL) {
                u_tmp = user_head;
                user_head = user_head->next;
                free(u_tmp);
            }
            StudentNode* s_tmp = NULL;
            while (stu_head != NULL) {
                s_tmp = stu_head;
                stu_head = stu_head->next;
                free(s_tmp);
            }
            return 0;
        default: printf("输入错误！\n");
        }
    }

LOGIN:
    // 登录逻辑
    char account[MAX_ACCOUNT_LEN] = { 0 }, pwd[MAX_PWD_LEN] = { 0 };
    int login_ok = 0;
    UserNode* p = NULL;

    while (!login_ok) {
        printf("\n===== 用户登录 =====\n");
        printf("请输入账号：");
        scanf("%19s", account);
        printf("请输入密码：");
        scanf("%19s", pwd);
        clear_buffer();

        p = user_head;
        while (p != NULL) {
            if (strcmp(p->data.account, account) == 0 && strcmp(p->data.pwd, pwd) == 0) {
                login_ok = 1;
                break;
            }
            p = p->next;
        }

        if (!login_ok) {
            printf("账号或密码错误！请重新输入。\n");
        }
    }

    printf("登录成功！欢迎 %s 用户\n",
        p->data.type == ADMIN ? "管理员" : (p->data.type == TEACHER ? "教师" : "学生"));
    switch (p->data.type) {
    case ADMIN:
        admin_menu(stu_head, stu_count, user_head, user_count, appeal_list, appeal_count, todo_list, todo_count);
        break;
    case TEACHER:
        teacher_menu(stu_head, stu_count, user_head, user_count, account);
        break;
    case STUDENT:
        student_menu(stu_head, stu_count, user_head, user_count, account, appeal_list, &appeal_count, todo_list, &todo_count);
        break;
    }

    // 释放内存
    UserNode* u_tmp = NULL;
    while (user_head != NULL) {
        u_tmp = user_head;
        user_head = user_head->next;
        free(u_tmp);
    }
    StudentNode* s_tmp = NULL;
    while (stu_head != NULL) {
        s_tmp = stu_head;
        stu_head = stu_head->next;
        free(s_tmp);
    }

    printf("退出系统，感谢使用！\n");
    return 0;
}