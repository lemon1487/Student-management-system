#include "../include/common.h"

// 学生主菜单入口（核心功能导航）
void student_menu(Student* stu_list, int stu_count, User* user_list, int user_count,
    char* account, Appeal* appeal_list, int* appeal_count,
    Todo* todo_list, int* todo_count) {
    if (stu_list == NULL || user_list == NULL || account == NULL ||
        appeal_list == NULL || appeal_count == NULL || todo_list == NULL || todo_count == NULL) {
        printf("参数错误！\n");
        return;
    }
    int choice;
    while (1) {
        printf("\n===== 学生功能菜单 =====\n");
        printf("1. 成绩查询（个人）\n");
        printf("2. 查询本班成绩\n");
        printf("3. 成绩分析（排名）\n");
        printf("4. 提交成绩申诉\n");
        printf("5. 修改密码（需原密码）\n");
        printf("0. 返回上一级\n");
        printf("请选择操作：");

        while (scanf("%d", &choice) != 1 || choice < 0 || choice > 5) {
            clear_buffer();
            printf("输入错误！请输入0-5：");
        }
        clear_buffer();

        switch (choice) {
        case 1: view_my_score(stu_list, stu_count, account); break;
        case 2: view_class_score(stu_list, stu_count, account); break;
        case 3: score_analysis(stu_list, stu_count, account); break;
        case 4: {
            if (*appeal_count >= 50) {
                printf("申诉数量已达上限！\n");
                break;
            }
            Appeal new_appeal = { 0 };
            strncpy(new_appeal.stu_id, account, MAX_ID_LEN - 1); // 申诉学生学号
            printf("请输入成绩申诉内容：");
            fgets(new_appeal.content, MAX_APPEAL_LEN, stdin);
            new_appeal.content[strcspn(new_appeal.content, "\n")] = '\0'; // 移除换行符
            new_appeal.type = 1; // 1-成绩申诉
            new_appeal.is_processed = 0; // 未处理
            strcpy(new_appeal.feedback, ""); // 暂无反馈
            appeal_list[*appeal_count] = new_appeal;
            (*appeal_count)++;
            save_appeal(appeal_list, *appeal_count); // 保存申诉数据

            // 同步生成管理员待办事项
            if (*todo_count < 50) {
                char todo_content[MAX_TODO_LEN] = { 0 };
                snprintf(todo_content, MAX_TODO_LEN - 1, "[成绩申诉] 学号:%s 需处理", account);
                strncpy(todo_list[*todo_count].content, todo_content, MAX_TODO_LEN - 1);
                todo_list[*todo_count].is_done = 0; // 未完成
                (*todo_count)++;
                save_todo(todo_list, *todo_count); // 保存待办
            }
            printf("成绩申诉提交成功！等待管理员处理\n");
            break;
        }
        case 5: student_change_pwd(user_list, user_count, account); break;
        case 0: printf("返回上一级菜单...\n"); return;
        default: printf("输入错误！\n");
        }
    }
}

// 查看个人成绩（按学号匹配）
void view_my_score(Student* stu_list, int stu_count, char* account) {
    if (stu_list == NULL || account == NULL || stu_count == 0) {
        printf("暂无成绩信息/参数错误！\n");
        return;
    }
    for (int i = 0; i < stu_count; i++) {
        if (strcmp(stu_list[i].id, account) == 0) {
            printf("\n===== 个人成绩 =====\n");
            printf("学号：%s\n", stu_list[i].id);
            printf("姓名：%s\n", stu_list[i].name);
            printf("班级：%s\n", stu_list[i].class);
            printf("数学：%8.1f\n", stu_list[i].math);
            printf("语文：%8.1f\n", stu_list[i].chinese);
            printf("英语：%8.1f\n", stu_list[i].english);
            printf("平均分：%6.1f\n", stu_list[i].avg);
            printf("总分：%8.1f\n", stu_list[i].total);
            return;
        }
    }
    printf("未找到你的成绩信息！\n");
}

// 查看本班所有学生成绩（按当前学生班级筛选）
void view_class_score(Student* stu_list, int stu_count, char* account) {
    if (stu_list == NULL || account == NULL || stu_count == 0) {
        printf("暂无成绩信息/参数错误！\n");
        return;
    }
    char class_name[MAX_CLASS_LEN] = { 0 };
    // 获取当前学生的班级名称
    for (int i = 0; i < stu_count; i++) {
        if (strcmp(stu_list[i].id, account) == 0) {
            strncpy(class_name, stu_list[i].class, MAX_CLASS_LEN - 1);
            break;
        }
    }
    if (strlen(class_name) == 0) {
        printf("未找到你的班级信息！\n");
        return;
    }

    printf("\n===== %s 班级成绩 =====\n", class_name);
    printf("%-12s %-20s %-20s %8s %8s %8s %8s %8s\n",
        "学号", "姓名", "班级", "数学", "语文", "英语", "平均分", "总分");
    printf("------------------------------------------------------------------------------------------------\n");

    int has_data = 0;
    // 遍历展示本班所有学生成绩
    for (int i = 0; i < stu_count; i++) {
        if (strcmp(stu_list[i].class, class_name) == 0) {
            has_data = 1;
            printf("%-12s %-20s %-20s %8.1f %8.1f %8.1f %8.1f %8.1f\n",
                stu_list[i].id, stu_list[i].name, stu_list[i].class,
                stu_list[i].math, stu_list[i].chinese, stu_list[i].english,
                stu_list[i].avg, stu_list[i].total);
        }
    }
    if (!has_data) {
        printf("该班级暂无成绩数据！\n");
    }
}

// 成绩分析（展示本班总分排名+前10名）
void score_analysis(Student* stu_list, int stu_count, char* account) {
    if (stu_list == NULL || account == NULL || stu_count == 0) {
        printf("暂无成绩信息/参数错误！\n");
        return;
    }
    char class_name[MAX_CLASS_LEN] = { 0 };
    // 获取当前学生的班级名称
    for (int i = 0; i < stu_count; i++) {
        if (strcmp(stu_list[i].id, account) == 0) {
            strncpy(class_name, stu_list[i].class, MAX_CLASS_LEN - 1);
            break;
        }
    }
    if (strlen(class_name) == 0) {
        printf("未找到你的班级信息！\n");
        return;
    }

    // 复制本班学生数据（避免修改原数组）
    Student class_stu[1000] = { 0 };
    int class_count = 0;
    for (int i = 0; i < stu_count; i++) {
        if (strcmp(stu_list[i].class, class_name) == 0) {
            class_stu[class_count] = stu_list[i];
            class_count++;
        }
    }

    // 按总分降序排序
    sort_students(class_stu, class_count, 4, SORT_DESC);

    // 查找当前学生在本班的排名
    int rank = -1;
    for (int i = 0; i < class_count; i++) {
        if (strcmp(class_stu[i].id, account) == 0) {
            rank = i + 1;
            break;
        }
    }

    printf("\n===== %s 班级成绩排名（总分降序）=====\n", class_name);
    printf("你的排名：第 %d 名 / 共 %d 人\n", rank, class_count);
    printf("前10名成绩：\n");
    printf("%-12s %-20s %8s\n", "学号", "姓名", "总分");
    printf("----------------------------\n");
    int show_count = (class_count > 10) ? 10 : class_count; // 最多展示前10名
    for (int i = 0; i < show_count; i++) {
        printf("%-12s %-20s %8.1f\n",
            class_stu[i].id, class_stu[i].name, class_stu[i].total);
    }
}

// 学生修改密码（验证原密码，两次确认新密码，修改后保存）
void student_change_pwd(User* user_list, int user_count, char* account) {
    if (user_list == NULL || account == NULL || user_count == 0) {
        printf("参数错误！\n");
        return;
    }

    // 查找当前学生账号
    int idx = -1;
    for (int i = 0; i < user_count; i++) {
        if (strcmp(user_list[i].account, account) == 0 && user_list[i].type == STUDENT) {
            idx = i;
            break;
        }
    }
    if (idx == -1) {
        printf("账号不存在！\n");
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

    // 输入并确认新密码
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

    // 保存新密码并持久化
    strcpy(user_list[idx].pwd, new_pwd);
    save_user(user_list, user_count);
    printf("密码修改成功！\n");
}