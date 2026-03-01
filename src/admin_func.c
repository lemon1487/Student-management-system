#include "../include/admin_func.h"
#include "../include/teacher_func.h"
#include "../include/file_func.h"
#include "../include/common.h"

// 管理员主菜单
void admin_menu(StudentNode* stu_head, int stu_count, UserNode* user_head, int user_count,
    Appeal* appeal_list, int appeal_count, Todo* todo_list, int todo_count) {
    int choice;
    while (1) {
        printf("\n===== 管理员功能菜单 =====\n");
        printf("1. 管理用户账号\n");
        printf("2. 管理学生信息\n");
        printf("3. 处理申诉（成绩+密码找回）\n");
        printf("4. 查看/处理待办事项\n");
        printf("5. 学生成绩排序\n");
        printf("6. 导出所有账号密码\n");
        printf("7. 导入账号密码\n");
        printf("8. 修改管理员密码\n");
        printf("9. 登录教师端（修改学生信息）\n");
        printf("0. 退出登录\n");
        printf("请选择操作：");

        while (scanf("%d", &choice) != 1 || choice < 0 || choice > 9) {
            clear_buffer();
            printf("输入错误！请输入0-9：");
        }
        clear_buffer();

        switch (choice) {
        case 1: manage_user(&user_head, &user_count); break;
        case 2: {
            int sub_choice;
            while (1) {
                printf("\n===== 学生信息管理 =====\n");
                printf("1. 添加学生\n");
                printf("2. 删除学生\n");
                printf("3. 修改学生成绩\n");
                printf("4. 查询学生成绩\n");
                printf("5. 导出学生成绩\n");
                printf("0. 返回\n");
                printf("请选择：");
                scanf("%d", &sub_choice);
                clear_buffer();
                switch (sub_choice) {
                case 1: add_student(&stu_head, &stu_count); break;
                case 2: {
                    char id[MAX_ID_LEN] = { 0 };
                    printf("请输入要删除的学生学号：");
                    scanf("%11s", id);
                    clear_buffer();
                    del_student(&stu_head, &stu_count, id);
                    break;
                }
                case 3: {
                    char id[MAX_ID_LEN] = { 0 };
                    printf("请输入要修改的学生学号：");
                    scanf("%11s", id);
                    clear_buffer();
                    edit_student(stu_head, stu_count, id);
                    break;
                }
                case 4: query_student(stu_head, stu_count); break;
                case 5: export_student(stu_head, stu_count); break;
                case 0: goto ADMIN_STU_MENU_EXIT;
                default: printf("输入错误！\n");
                }
            }
        ADMIN_STU_MENU_EXIT:
            break;
        }

        case 3: manage_appeal(appeal_list, &appeal_count, &user_head, &user_count); break;
        case 4: manage_todo(todo_list, &todo_count); break;
        case 5: {
            if (stu_head == NULL || stu_count == 0) {
                printf("暂无学生数据！\n");
                break;
            }
            Student temp_stu[stu_count];
            StudentNode* p = stu_head;
            int i = 0;
            while (p != NULL && i < stu_count) {
                temp_stu[i] = p->data;
                p = p->next;
                i++;
            }
            int field, type;
            printf("选择排序字段(1-数学 2-语文 3-英语 4-总分)：");
            scanf("%d", &field);
            printf("选择排序方式(0-升序 1-降序)：");
            scanf("%d", &type);
            sort_students(temp_stu, stu_count, field, type);
            printf("排序后成绩：\n");
            printf("%-15s %-22s %-10s %-10s %-8s %-12s %-8s\n",
                "学号", "姓名", "班级", "数学", "语文", "英语", "平均分", "总分");
            printf("--------------------------------------------------------------------------------\n");
            for (i = 0; i < stu_count; i++) {
                printf("%-12s %-20s %-20s %-8.2f %-8.2f %-8.2f %-8.2f %-8.2f\n",
                    temp_stu[i].id, temp_stu[i].name, temp_stu[i].class,
                    temp_stu[i].math, temp_stu[i].chinese, temp_stu[i].english,
                    temp_stu[i].avg, temp_stu[i].total);
            }
            break;
        }
        case 6: export_users(user_head, user_count); break;
        case 7: import_users(&user_head, &user_count); break;
        case 8: admin_change_pwd(user_head, user_count); break;
        case 9: teacher_menu(stu_head, stu_count, user_head, user_count, "admin_temp"); break;
        case 0: printf("退出管理员登录...\n"); return;
        default: printf("输入错误！\n");
        }
    }
}

// 待办管理
void manage_todo(Todo* todo_list, int* todo_count) {
    if (todo_list == NULL || *todo_count == 0) {
        printf("暂无待办事项！\n");
        return;
    }

    int choice, i;
    while (1) {
        printf("\n===== 待办管理 =====\n");
        printf("所有待办事项：\n");
        for (i = 0; i < *todo_count; i++) {
            printf("序号：%d | 状态：%s | 内容：%s\n",
                i + 1, todo_list[i].is_done ? "✅ 已完成" : "⏳ 未完成",
                todo_list[i].content);
        }

        printf("\n操作选项：\n");
        printf("1. 标记待办为已完成\n");
        printf("2. 删除待办事项\n");
        printf("0. 返回\n");
        printf("请选择操作：");

        while (scanf("%d", &choice) != 1 || choice < 0 || choice > 2) {
            clear_buffer();
            printf("输入错误！请输入0-2：");
        }
        clear_buffer();

        switch (choice) {
        case 1: {
            printf("请输入要标记完成的待办序号（0返回）：");
            while (scanf("%d", &choice) != 1 || choice < 0 || choice > *todo_count) {
                clear_buffer();
                printf("输入错误！请输入0-%d：", *todo_count);
            }
            clear_buffer();
            if (choice == 0) break;

            choice--;
            if (todo_list[choice].is_done) {
                printf("该待办已标记为完成！\n");
                break;
            }
            todo_list[choice].is_done = 1;
            save_todo(todo_list, *todo_count);
            printf("待办已标记为完成！\n");
            break;
        }
        case 2: {
            printf("请输入要删除的待办序号（0返回）：");
            while (scanf("%d", &choice) != 1 || choice < 0 || choice > *todo_count) {
                clear_buffer();
                printf("输入错误！请输入0-%d：", *todo_count);
            }
            clear_buffer();
            if (choice == 0) break;

            choice--;
            // 数组移位删除
            for (i = choice; i < *todo_count - 1; i++) {
                todo_list[i] = todo_list[i + 1];
            }
            (*todo_count)--;
            save_todo(todo_list, *todo_count);
            printf("待办已删除！\n");
            break;
        }
        case 0: return;
        default: printf("输入错误！\n");
        }
    }
}

// 申诉管理
void manage_appeal(Appeal* appeal_list, int* appeal_count, UserNode** user_head, int* user_count) {
    if (appeal_list == NULL || *appeal_count == 0) {
        printf("暂无申诉记录！\n");
        return;
    }

    int choice, opt, i;
    while (1) {
        // 第一步：显示所有申诉
        printf("\n===== 申诉管理 =====\n");
        printf("所有申诉记录：\n");
        for (i = 0; i < *appeal_count; i++) {
            char* type_str;
            switch (appeal_list[i].type) {
            case 1: type_str = "成绩申诉"; break;
            case 2: type_str = "密码找回"; break;
            case 3: type_str = "用户注册"; break;
            default: type_str = "未知"; break;
            }
            printf("序号：%d | 学生学号：%s | 类型：%s | 状态：%s\n",
                i + 1, appeal_list[i].stu_id, type_str,
                appeal_list[i].is_processed == 0 ? "未处理" : "已处理");
            printf("内容：%s\n", appeal_list[i].content);
            if (appeal_list[i].is_processed) {
                printf("反馈：%s\n", appeal_list[i].feedback);
            }
            printf("------------------------\n");
        }

        // 第二步：选择操作类型
        printf("\n请选择操作：\n");
        printf("1. 处理申诉\n");
        printf("2. 删除申诉\n");
        printf("0. 返回\n");
        printf("请选择：");
        while (scanf("%d", &opt) != 1 || opt < 0 || opt > 2) {
            clear_buffer();
            printf("输入错误！请输入0-2：");
        }
        clear_buffer();

        if (opt == 0) return;

        // 第三步：选择要操作的申诉序号
        printf("请选择要操作的申诉序号（0返回）：");
        while (scanf("%d", &choice) != 1 || choice < 0 || choice > *appeal_count) {
            clear_buffer();
            printf("输入错误！请输入0-%d：", *appeal_count);
        }
        clear_buffer();
        if (choice == 0) continue;

        choice--; 

        // 第四步：执行操作
        if (opt == 1) {
            // ========== 处理申诉逻辑 ==========
            if (appeal_list[choice].is_processed) {
                printf("该申诉已处理！\n");
                continue;
            }

            printf("请输入处理反馈：");
            scanf("%99s", appeal_list[choice].feedback);
            clear_buffer();
            appeal_list[choice].is_processed = 1;

            // 处理密码找回申诉
            if (appeal_list[choice].type == 2) {
                UserNode* p = *user_head;
                while (p != NULL) {
                    if (strcmp(p->data.account, appeal_list[choice].stu_id) == 0 && p->data.type == STUDENT) {
                        strcpy(p->data.pwd, "123456");
                        save_user(*user_head, *user_count);
                        printf("已重置该学生密码为123456！\n");
                        break;
                    }
                    p = p->next;
                }
            }

            // 处理注册申诉
            if (appeal_list[choice].type == 3) {
                UserNode* new_node = (UserNode*)malloc(sizeof(UserNode));
                strcpy(new_node->data.account, appeal_list[choice].stu_id);

                char type_str[20] = { 0 };
                sscanf(appeal_list[choice].content, "注册新用户，身份：%[^，]，初始密码：%s", type_str, new_node->data.pwd);

                if (strcmp(type_str, "管理员") == 0) {
                    new_node->data.type = ADMIN;
                }
                else if (strcmp(type_str, "教师") == 0) {
                    new_node->data.type = TEACHER;
                }
                else {
                    new_node->data.type = STUDENT;
                }

                new_node->next = *user_head;
                *user_head = new_node;
                (*user_count)++;
                save_user(*user_head, *user_count);
                printf("已批准注册申请，新用户（身份：%s）已添加！\n", type_str);
            }

            save_appeal(appeal_list, *appeal_count);
            printf("申诉处理完成！\n");

        }
        else if (opt == 2) {
            // 删除申诉逻辑 
            char confirm[10] = { 0 };
            printf("确认要删除该申诉吗？(y/n)：");
            scanf("%9s", confirm);
            clear_buffer();
            if (strcmp(confirm, "y") != 0 && strcmp(confirm, "Y") != 0) {
                printf("已取消删除！\n");
                continue;
            }

            // 数组移位删除
            for (i = choice; i < *appeal_count - 1; i++) {
                appeal_list[i] = appeal_list[i + 1];
            }
            (*appeal_count)--;
            save_appeal(appeal_list, *appeal_count);
            printf("申诉已删除！\n");
        }
    }
}

//管理员修改密码
void admin_change_pwd(UserNode* head, int user_count) {
    if (head == NULL || user_count == 0) {
        printf("暂无管理员账号！\n");
        return;
    }

    char admin_account[MAX_ACCOUNT_LEN] = { 0 };
    printf("请输入要修改密码的管理员账号：");
    scanf("%19s", admin_account);
    clear_buffer();

    UserNode* p = head;
    int found = 0;
    while (p != NULL) {
        if (strcmp(p->data.account, admin_account) == 0 && p->data.type == ADMIN) {
            found = 1;
            break;
        }
        p = p->next;
    }
    if (!found) {
        printf("未找到该管理员账号！\n");
        return;
    }

    char old_pwd[MAX_PWD_LEN] = { 0 }, new_pwd[MAX_PWD_LEN] = { 0 }, confirm_pwd[MAX_PWD_LEN] = { 0 };
    printf("请输入原密码：");
    scanf("%19s", old_pwd);
    clear_buffer();
    if (strcmp(p->data.pwd, old_pwd) != 0) {
        printf("原密码错误！\n");
        return;
    }

    printf("请输入新密码：");
    scanf("%19s", new_pwd);
    clear_buffer();
    printf("请确认新密码：");
    scanf("%19s", confirm_pwd);
    clear_buffer();
    if (strcmp(new_pwd, confirm_pwd) != 0) {
        printf("两次密码不一致！\n");
        return;
    }

    strcpy(p->data.pwd, new_pwd);
    save_user(head, user_count);
    printf("管理员密码修改成功！\n");
}

//用户管理
void manage_user(UserNode** head, int* count) {
    if (*head == NULL || *count == 0) {
        printf("暂无用户数据！\n");
        return;
    }

    int choice;
    while (1) {
        printf("\n===== 用户管理 =====\n");
        printf("1. 查看所有用户\n");
        printf("2. 删除用户\n");
        printf("3. 修改用户类型\n");
        printf("0. 返回\n");
        printf("请选择：");
        scanf("%d", &choice);
        clear_buffer();

        switch (choice) {
        case 1: {
            UserNode* p = *head;
            printf("\n所有用户列表：\n");
            printf("%-20s %-20s %-10s\n", "账号", "密码", "类型");
            while (p != NULL) {
                char type_str[10] = { 0 };
                switch (p->data.type) {
                case ADMIN: strcpy(type_str, "管理员"); break;
                case TEACHER: strcpy(type_str, "教师"); break;
                case STUDENT: strcpy(type_str, "学生"); break;
                }
                printf("%-20s %-20s %-10s\n", p->data.account, p->data.pwd, type_str);
                p = p->next;
            }
            break;
        }
        case 2: {
            char del_account[MAX_ACCOUNT_LEN] = { 0 };
            printf("请输入要删除的账号：");
            scanf("%19s", del_account);
            clear_buffer();

            UserNode* p = *head, * pre = NULL;
            while (p != NULL && strcmp(p->data.account, del_account) != 0) {
                pre = p;
                p = p->next;
            }
            if (p == NULL) {
                printf("用户不存在！\n");
                break;
            }

            if (pre == NULL) *head = p->next;
            else pre->next = p->next;
            free(p);
            (*count)--;
            save_user(*head, *count);
            printf("用户删除成功！\n");
            break;
        }
        case 3: {
            char mod_account[MAX_ACCOUNT_LEN] = { 0 };
            int new_type;
            printf("请输入要修改类型的账号：");
            scanf("%19s", mod_account);
            clear_buffer();

            UserNode* p = *head;
            while (p != NULL && strcmp(p->data.account, mod_account) != 0) {
                p = p->next;
            }
            if (p == NULL) {
                printf("用户不存在！\n");
                break;
            }

            printf("请输入新类型(1-管理员 2-教师 3-学生)：");
            while (scanf("%d", &new_type) != 1 || new_type < 1 || new_type > 3) {
                clear_buffer();
                printf("输入错误！请输入1-3：");
            }
            clear_buffer();
            p->data.type = new_type;
            save_user(*head, *count);
            printf("用户类型修改成功！\n");
            break;
        }
        case 0: return;
        default: printf("输入错误！\n");
        }
    }
}
// 学生信息管理
void manage_student(StudentNode** stu_head, int* stu_count) {
    int sub_choice;
    while (1) {
        printf("\n===== 学生信息管理 =====\n");
        printf("1. 添加学生\n");
        printf("2. 删除学生\n");
        printf("3. 修改学生成绩\n");
        printf("4. 查询学生成绩\n");
        printf("5. 导出学生成绩\n");
        printf("0. 返回\n");
        printf("请选择：");
        scanf("%d", &sub_choice);
        clear_buffer();
        switch (sub_choice) {
        case 1: add_student(stu_head, stu_count); break;
        case 2: {
            char id[MAX_ID_LEN] = { 0 };
            printf("请输入要删除的学生学号：");
            scanf("%11s", id);
            clear_buffer();
            del_student(stu_head, stu_count, id);
            break;
        }
        case 3: {
            char id[MAX_ID_LEN] = { 0 };
            printf("请输入要修改的学生学号：");
            scanf("%11s", id);
            clear_buffer();
            edit_student(*stu_head, *stu_count, id);
            break;
        }
        case 4: query_student(*stu_head, *stu_count); break;
        case 5: export_student(*stu_head, *stu_count); break;
        case 0: return;
        default: printf("输入错误！\n");
        }
    }
}