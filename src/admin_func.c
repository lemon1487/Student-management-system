#include "../include/common.h"

// 前置声明，解决隐式声明报错
void manage_user(User* user_list, int* user_count);
void manage_appeal(Appeal* appeal_list, int* appeal_count, User* user_list, int user_count);
void manage_todo(Todo* todo_list, int* todo_count);

// 管理员主菜单
void admin_menu(Student* stu_list, int* stu_count, User* user_list, int* user_count,
    Appeal* appeal_list, int* appeal_count, Todo* todo_list, int* todo_count) {
    if (stu_list == NULL || stu_count == NULL || user_list == NULL || user_count == NULL ||
        appeal_list == NULL || appeal_count == NULL || todo_list == NULL || todo_count == NULL) {
        printf("参数错误！\n");
        return;
    }

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
        case 1: manage_user(user_list, user_count); break;
        case 2: manage_student(stu_list, stu_count); break;
        case 3: manage_appeal(appeal_list, appeal_count, user_list, *user_count); break;
        case 4: manage_todo(todo_list, todo_count); break;
        case 5: { // 成绩排序
            if (*stu_count == 0) {
                printf("暂无学生数据！\n");
                break;
            }
            int field, type;
            printf("\n===== 成绩排序 =====\n");
            printf("请选择排序字段：\n");
            printf("1. 数学  2. 语文  3. 英语  4. 总分\n");
            printf("输入选择：");
            while (scanf("%d", &field) != 1 || field < 1 || field > 4) {
                clear_buffer();
                printf("输入错误！请输入1-4：");
            }
            clear_buffer();

            printf("请选择排序方式：\n");
            printf("0. 升序  1. 降序\n");
            printf("输入选择：");
            while (scanf("%d", &type) != 1 || type < 0 || type > 1) {
                clear_buffer();
                printf("输入错误！请输入0-1：");
            }
            clear_buffer();

            sort_students(stu_list, *stu_count, field, type);
            save_student(stu_list, *stu_count);
            printf("排序完成！已保存到文件\n");

            // 显示排序结果（前10条）
            printf("\n===== 排序后结果（前10条）=====\n");
            printf("%-12s %-20s %-20s %8s %8s %8s %8s %8s\n",
                "学号", "姓名", "班级", "数学", "语文", "英语", "平均分", "总分");
            printf("------------------------------------------------------------------------------------------------\n");
            int show_count = (*stu_count > 10) ? 10 : *stu_count;
            for (int i = 0; i < show_count; i++) {
                printf("%-12s %-20s %-20s %8.1f %8.1f %8.1f %8.1f %8.1f\n",
                    stu_list[i].id, stu_list[i].name, stu_list[i].class,
                    stu_list[i].math, stu_list[i].chinese, stu_list[i].english,
                    stu_list[i].avg, stu_list[i].total);
            }
            break;
        }
        case 6: export_users(user_list, *user_count); break;
        case 7: {
            int ret = import_users(user_list, user_count);
            if (ret > 0) printf("成功导入 %d 个账号！\n", ret);
            break;
        }
        case 8: admin_change_pwd(user_list, *user_count); break;
        case 9:
            printf("===== 管理员切换到教师功能端 =====\n");
            // 传入空账号不影响教师功能（教师菜单仅用账号做密码修改，管理员用不到）
            teacher_menu(stu_list, stu_count, user_list, *user_count, "admin_temp");
            break;
        case 0:
            printf("退出管理员登录...\n");
            return;
        default:
            printf("输入错误！\n");
        }
    }
}

// 注册用户（主界面调用）
void register_user(User* user_list, int* user_count) {
    if (user_list == NULL || user_count == NULL || *user_count >= 100) {
        printf("用户数量已达上限/参数错误！\n");
        return;
    }

    User new_user = { 0 };
    printf("\n===== 用户注册 =====\n");
    printf("请输入账号：");
    scanf("%19s", new_user.account);
    clear_buffer();

    // 检查账号重复
    int exists = 0;
    for (int i = 0; i < *user_count; i++) {
        if (strcmp(user_list[i].account, new_user.account) == 0) {
            exists = 1;
            break;
        }
    }
    if (exists) {
        printf("账号已存在！\n");
        return;
    }

    printf("请输入密码：");
    scanf("%19s", new_user.pwd);
    clear_buffer();

    printf("请选择用户类型：\n");
    printf("1. 管理员  2. 教师  3. 学生\n");
    while (scanf("%d", &new_user.type) != 1 || new_user.type < 1 || new_user.type > 3) {
        clear_buffer();
        printf("输入错误！请输入1-3：");
    }
    clear_buffer();

    // 添加到用户列表
    user_list[*user_count] = new_user;
    (*user_count)++;
    save_user(user_list, *user_count);
    printf("注册成功！\n");
}

// 管理用户账号
void manage_user(User* user_list, int* user_count) {
    if (user_list == NULL || user_count == NULL || *user_count >= 100) {
        printf("用户数量已达上限/参数错误！\n");
        return;
    }

    int choice;
    while (1) {
        printf("\n===== 用户账号管理 =====\n");
        printf("1. 添加账号\n");
        printf("2. 删除账号\n");
        printf("3. 查看所有账号\n");
        printf("0. 返回上一级\n");
        printf("请选择：");

        while (scanf("%d", &choice) != 1 || choice < 0 || choice > 3) {
            clear_buffer();
            printf("输入错误！请输入0-3：");
        }
        clear_buffer();

        switch (choice) {
        case 1: { // 添加账号
            User new_user = { 0 };
            printf("请输入账号：");
            scanf("%19s", new_user.account);
            clear_buffer();

            // 检查账号重复
            int exists = 0;
            for (int i = 0; i < *user_count; i++) {
                if (strcmp(user_list[i].account, new_user.account) == 0) {
                    exists = 1;
                    break;
                }
            }
            if (exists) {
                printf("账号已存在！\n");
                break;
            }

            printf("请输入密码：");
            scanf("%19s", new_user.pwd);
            clear_buffer();

            printf("请选择用户类型：\n");
            printf("1. 管理员  2. 教师  3. 学生\n");
            while (scanf("%d", &new_user.type) != 1 || new_user.type < 1 || new_user.type > 3) {
                clear_buffer();
                printf("输入错误！请输入1-3：");
            }
            clear_buffer();

            user_list[*user_count] = new_user;
            (*user_count)++;
            save_user(user_list, *user_count);
            printf("账号添加成功！\n");
            break;
        }
        case 2: { // 删除账号
            if (*user_count == 0) {
                printf("暂无账号可删！\n");
                break;
            }
            char del_account[MAX_ACCOUNT_LEN] = { 0 };
            printf("请输入要删除的账号：");
            scanf("%19s", del_account);
            clear_buffer();

            int del_idx = -1;
            for (int i = 0; i < *user_count; i++) {
                if (strcmp(user_list[i].account, del_account) == 0) {
                    del_idx = i;
                    break;
                }
            }
            if (del_idx == -1) {
                printf("账号不存在！\n");
                break;
            }

            // 移除账号（后续元素前移）
            for (int i = del_idx; i < *user_count - 1; i++) {
                user_list[i] = user_list[i + 1];
            }
            (*user_count)--;
            save_user(user_list, *user_count);
            printf("账号删除成功！\n");
            break;
        }
        case 3: { // 查看所有账号
            if (*user_count == 0) {
                printf("暂无账号！\n");
                break;
            }
            printf("\n===== 所有账号列表 =====\n");
            printf("%-20s %-20s %-10s\n", "账号", "密码", "类型");
            printf("--------------------------------------------\n");
            for (int i = 0; i < *user_count; i++) {
                char type_str[10] = { 0 };
                switch (user_list[i].type) {
                case 1: strcpy(type_str, "管理员"); break;
                case 2: strcpy(type_str, "教师"); break;
                case 3: strcpy(type_str, "学生"); break;
                }
                printf("%-20s %-20s %-10s\n", user_list[i].account, user_list[i].pwd, type_str);
            }
            break;
        }
        case 0: return;
        default: printf("输入错误！\n");
        }
    }
}

// 管理待办事项
void manage_todo(Todo* todo_list, int* todo_count) {
    if (todo_list == NULL || todo_count == NULL) {
        printf("参数错误！\n");
        return;
    }

    if (*todo_count == 0) {
        printf("暂无待办事项！\n");
        return;
    }

    printf("\n===== 待办事项列表 =====\n");
    for (int i = 0; i < *todo_count; i++) {
        char status[10];
        strcpy(status, todo_list[i].is_done ? "已完成" : "未完成");
        printf("%d. %s [%s]\n", i + 1, todo_list[i].content, status);
    }

    printf("\n请选择操作：\n");
    printf("1. 标记为已完成  2. 删除待办  0. 返回\n");
    int choice;
    while (scanf("%d", &choice) != 1 || choice < 0 || choice > 2) {
        clear_buffer();
        printf("输入错误！请输入0-2：");
    }
    clear_buffer();

    if (choice == 0) return;

    int idx;
    printf("请输入待办序号：");
    while (scanf("%d", &idx) != 1 || idx < 1 || idx > *todo_count) {
        clear_buffer();
        printf("输入错误！请输入1-%d：", *todo_count);
    }
    clear_buffer();
    idx--;

    if (choice == 1) {
        todo_list[idx].is_done = 1;
        printf("待办已标记为已完成！\n");
    }
    else if (choice == 2) {
        // 删除待办
        for (int i = idx; i < *todo_count - 1; i++) {
            todo_list[i] = todo_list[i + 1];
        }
        (*todo_count)--;
        printf("待办已删除！\n");
    }
    save_todo(todo_list, *todo_count);
}

// 处理申诉（成绩+密码找回）
void manage_appeal(Appeal* appeal_list, int* appeal_count, User* user_list, int user_count) {
    if (appeal_list == NULL || appeal_count == NULL || user_list == NULL) {
        printf("参数错误！\n");
        return;
    }

    int unprocessed = 0;
    for (int i = 0; i < *appeal_count; i++) {
        if (!appeal_list[i].is_processed) unprocessed++;
    }

    if (unprocessed == 0) {
        printf("暂无未处理申诉！\n");
        return;
    }

    printf("\n===== 未处理申诉列表 =====\n");
    int show_idx = 0;
    int appeal_idx_arr[50] = { 0 }; // 存储未处理申诉的索引
    for (int i = 0; i < *appeal_count; i++) {
        if (!appeal_list[i].is_processed) {
            show_idx++;
            appeal_idx_arr[show_idx - 1] = i;
            char type_str[20];
            strcpy(type_str, appeal_list[i].type == 1 ? "成绩申诉" : "密码找回");
            printf("%d. 账号：%s | 类型：%s | 内容：%s\n",
                show_idx, appeal_list[i].stu_id, type_str, appeal_list[i].content);
        }
    }

    printf("\n请选择要处理的申诉序号（0返回）：");
    int choice;
    while (scanf("%d", &choice) != 1 || choice < 0 || choice > show_idx) {
        clear_buffer();
        printf("输入错误！请输入0-%d：", show_idx);
    }
    clear_buffer();

    if (choice == 0) return;

    int idx = appeal_idx_arr[choice - 1];
    Appeal* curr_appeal = &appeal_list[idx];

    if (curr_appeal->type == 1) { // 成绩申诉
        printf("\n===== 处理成绩申诉 =====\n");
        printf("申诉账号：%s\n", curr_appeal->stu_id);
        printf("申诉内容：%s\n", curr_appeal->content);
        printf("请输入处理反馈：");
        fgets(curr_appeal->feedback, MAX_APPEAL_LEN, stdin);
        curr_appeal->feedback[strcspn(curr_appeal->feedback, "\n")] = '\0';
        curr_appeal->is_processed = 1;
        printf("成绩申诉处理完成！\n");
    }
    else if (curr_appeal->type == 2) { // 密码找回
        printf("\n===== 处理密码找回 =====\n");
        printf("申诉账号：%s\n", curr_appeal->stu_id);
        // 查找用户
        int user_idx = -1;
        for (int i = 0; i < user_count; i++) {
            if (strcmp(user_list[i].account, curr_appeal->stu_id) == 0) {
                user_idx = i;
                break;
            }
        }
        if (user_idx == -1) {
            printf("账号不存在！\n");
            return;
        }
        // 重置密码
        char new_pwd[MAX_PWD_LEN] = { 0 };
        printf("请输入新密码：");
        scanf("%19s", new_pwd);
        clear_buffer();
        strcpy(user_list[user_idx].pwd, new_pwd);
        // 填写反馈
        snprintf(curr_appeal->feedback, MAX_APPEAL_LEN, "密码已重置为：%s", new_pwd);
        curr_appeal->is_processed = 1;
        save_user(user_list, user_count);
        printf("密码重置成功！\n");
    }

    save_appeal(appeal_list, *appeal_count);
}

// 管理员修改密码
void admin_change_pwd(User* user_list, int user_count) {
    if (user_list == NULL || user_count == 0) {
        printf("参数错误！\n");
        return;
    }

    char account[MAX_ACCOUNT_LEN] = { 0 };
    printf("请输入管理员账号：");
    scanf("%19s", account);
    clear_buffer();

    // 查找管理员
    int idx = -1;
    for (int i = 0; i < user_count; i++) {
        if (strcmp(user_list[i].account, account) == 0 && user_list[i].type == 1) {
            idx = i;
            break;
        }
    }
    if (idx == -1) {
        printf("管理员账号不存在！\n");
        return;
    }

    // 验证原密码
    char old_pwd[MAX_PWD_LEN] = { 0 };
    printf("请输入原密码：");
    scanf("%19s", old_pwd);
    clear_buffer();
    if (strcmp(user_list[idx].pwd, old_pwd) != 0) {
        printf("原密码错误！\n");
        return;
    }

    // 输入新密码
    char new_pwd[MAX_PWD_LEN] = { 0 }, confirm_pwd[MAX_PWD_LEN] = { 0 };
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

    strcpy(user_list[idx].pwd, new_pwd);
    save_user(user_list, user_count);
    printf("密码修改成功！\n");
}

// 提交密码找回申诉
void submit_find_pwd_appeal(Appeal* appeal_list, int* appeal_count, Todo* todo_list, int* todo_count, char* account) {
    if (appeal_list == NULL || appeal_count == NULL || todo_list == NULL || todo_count == NULL || account == NULL) {
        printf("参数错误！\n");
        return;
    }

    if (*appeal_count >= 50) {
        printf("申诉数量已达上限！\n");
        return;
    }

    Appeal new_appeal = { 0 };
    strcpy(new_appeal.stu_id, account);
    strcpy(new_appeal.content, "申请找回密码，请管理员重置");
    new_appeal.type = 2; // 密码找回类型
    new_appeal.is_processed = 0;
    appeal_list[*appeal_count] = new_appeal;
    (*appeal_count)++;
    save_appeal(appeal_list, *appeal_count);

    // 添加待办
    if (*todo_count < 50) {
        char todo_content[MAX_TODO_LEN] = { 0 };
        snprintf(todo_content, MAX_TODO_LEN, "[密码找回] 账号:%s 需处理", account);
        strcpy(todo_list[*todo_count].content, todo_content);
        todo_list[*todo_count].is_done = 0;
        (*todo_count)++;
        save_todo(todo_list, *todo_count);
    }

    printf("密码找回申诉提交成功！等待管理员处理\n");
}
