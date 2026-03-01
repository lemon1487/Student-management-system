#include "../include/student_func.h"
#include "../include/file_func.h"
#include "../include/common.h"


// 学生主菜单
void student_menu(StudentNode* stu_head, int stu_count, UserNode* user_head, int user_count, char* account,
    Appeal* appeal_list, int* appeal_count, Todo* todo_list, int* todo_count) {
    int choice;
    while (1) {
        printf("\n===== 学生功能菜单 =====\n");
        printf("1. 查看个人成绩\n");
        printf("2. 个人成绩分析\n");
        printf("3. 查看本班成绩\n"); 
        printf("4. 提交成绩/密码申诉\n");
        printf("5. 修改个人密码\n");
        printf("0. 退出登录\n");
        printf("请选择操作：");

        while (scanf("%d", &choice) != 1 || choice < 0 || choice > 5) {
            clear_buffer();
            printf("输入错误！请输入0-5：");
        }
        clear_buffer();

        switch (choice) {
        case 1: student_view_score(stu_head, stu_count, account); break;
        case 2: student_score_analysis(stu_head, stu_count, account); break;
        case 3: student_view_class_score(stu_head, stu_count, account); break;
        case 4: student_submit_appeal(appeal_list, appeal_count, todo_list, todo_count, account); break;
        case 5: student_change_pwd(user_head, user_count, account); break;
        case 0: printf("退出学生登录...\n"); return;
        default: printf("输入错误！\n");
        }
    }
}

// 学生端成绩分析
void student_score_analysis(StudentNode* stu_head, int stu_count, char* account) {
    if (stu_head == NULL || stu_count == 0) {
        printf("暂无学生数据！\n");
        return;
    }

    StudentNode* p = stu_head;
    while (p != NULL && strcmp(p->data.id, account) != 0) {
        p = p->next;
    }
    if (p == NULL) {
        printf("未找到你的成绩数据！\n");
        return;
    }

    int math_rank = get_student_rank(stu_head, stu_count, account, 1);
    int chinese_rank = get_student_rank(stu_head, stu_count, account, 2);
    int english_rank = get_student_rank(stu_head, stu_count, account, 3);
    int total_rank = get_student_rank(stu_head, stu_count, account, 4);

    printf("\n===== 个人成绩分析 =====\n");
    printf("学号：%s\n姓名：%s\n班级：%s\n",
        p->data.id, p->data.name, p->data.class);
    printf("\n成绩详情：\n");
    printf("科目\t成绩\t班级排名（共%d人）\n", stu_count);
    printf("数学\t%.2f\t第%d名\n", p->data.math, math_rank);
    printf("语文\t%.2f\t第%d名\n", p->data.chinese, chinese_rank);
    printf("英语\t%.2f\t第%d名\n", p->data.english, english_rank);
    printf("总分\t%.2f\t第%d名\n", p->data.total, total_rank);
}

//学生查看个人成绩 
void student_view_score(StudentNode* stu_head, int stu_count, char* account) {
    if (stu_head == NULL || stu_count == 0) {
        printf("暂无学生成绩数据！\n");
        return;
    }

    StudentNode* p = stu_head;
    while (p != NULL && strcmp(p->data.id, account) != 0) {
        p = p->next;
    }
    if (p == NULL) {
        printf("未找到你的成绩数据！\n");
        return;
    }

    printf("\n===== 个人成绩 =====\n");
    printf("学号：%s\n姓名：%s\n班级：%s\n",
        p->data.id, p->data.name, p->data.class);
    printf("数学：%.2f 语文：%.2f 英语：%.2f\n",
        p->data.math, p->data.chinese, p->data.english);
    printf("平均分：%.2f 总分：%.2f\n", p->data.avg, p->data.total);
}
// 学生查看本班成绩
void student_view_class_score(StudentNode* stu_head, int stu_count, char* account) {
    if (stu_head == NULL || stu_count == 0) {
        printf("暂无学生数据！\n");
        return;
    }

    // 第一步：先找到自己的班级
    char my_class[MAX_CLASS_LEN] = { 0 };
    StudentNode* p = stu_head;
    while (p != NULL) {
        if (strcmp(p->data.id, account) == 0) {
            strncpy(my_class, p->data.class, MAX_CLASS_LEN);
            my_class[MAX_CLASS_LEN - 1] = '\0';
            break;
        }
        p = p->next;
    }

    if (strlen(my_class) == 0) {
        printf("未找到你的班级信息！\n");
        return;
    }

    // 第二步：筛选本班学生到临时数组
    Student temp_stu[stu_count];
    int class_count = 0;
    p = stu_head;
    while (p != NULL) {
        if (strcmp(p->data.class, my_class) == 0) {
            temp_stu[class_count++] = p->data;
        }
        p = p->next;
    }

    if (class_count == 0) {
        printf("本班暂无学生数据！\n");
        return;
    }

    // 第三步：询问是否排序
    int sort_choice, field, sort_type;
    printf("\n是否需要排序？(1-是 0-否，直接显示)：");
    while (scanf("%d", &sort_choice) != 1 || sort_choice < 0 || sort_choice > 1) {
        clear_buffer();
        printf("输入错误！请输入0或1：");
    }
    clear_buffer();

    if (sort_choice == 1) {
        printf("选择排序字段(1-数学 2-语文 3-英语 4-总分)：");
        while (scanf("%d", &field) != 1 || field < 1 || field > 4) {
            clear_buffer();
            printf("输入错误！请输入1-4：");
        }
        clear_buffer();

        printf("选择排序方式(0-升序 1-降序)：");
        while (scanf("%d", &sort_type) != 1 || sort_type < 0 || sort_type > 1) {
            clear_buffer();
            printf("输入错误！请输入0或1：");
        }
        clear_buffer();

        // 调用排序函数
        sort_students(temp_stu, class_count, field, sort_type);

        char* field_name;
        switch (field) {
        case 1: field_name = "数学"; break;
        case 2: field_name = "语文"; break;
        case 3: field_name = "英语"; break;
        case 4: field_name = "总分"; break;
        }
        printf("\n已按%s%s排序！\n", field_name, sort_type == 0 ? "升序" : "降序");
    }

    // 第四步：显示本班成绩
    printf("\n===== %s 班级成绩 =====\n", my_class);
    printf("%-15s %-22s %-10s %-10s %-8s %-12s %-8s\n",
        "学号", "姓名", "数学", "语文", "英语", "平均分", "总分");
    printf("--------------------------------------------------------------------------\n");

    for (int i = 0; i < class_count; i++) {
        printf("%-12s %-20s %-8.2f %-8.2f %-8.2f %-8.2f %-8.2f\n",
            temp_stu[i].id, temp_stu[i].name,
            temp_stu[i].math, temp_stu[i].chinese, temp_stu[i].english,
            temp_stu[i].avg, temp_stu[i].total);
    }
}

//学生提交申诉
void student_submit_appeal(Appeal* appeal_list, int* appeal_count, Todo* todo_list, int* todo_count, char* account) {
    if (*appeal_count >= MAX_APPEAL) {
        printf("申诉数量已达上限！\n");
        return;
    }
    if (*todo_count >= MAX_TODO) {
        printf("待办数量已达上限！\n");
        return;
    }

    Appeal new_appeal;
    strcpy(new_appeal.stu_id, account);
    new_appeal.is_processed = 0;
    memset(new_appeal.feedback, 0, MAX_CONTENT_LEN);

    printf("\n===== 提交申诉 =====\n");
    printf("选择申诉类型：1-成绩申诉 2-密码找回\n");
    while (scanf("%d", &new_appeal.type) != 1 || new_appeal.type < 1 || new_appeal.type > 2) {
        clear_buffer();
        printf("输入错误！请输入1或2：");
    }
    clear_buffer();

    printf("请输入申诉内容：");
    scanf("%99s", new_appeal.content);
    clear_buffer();

    // 1. 添加申诉
    appeal_list[*appeal_count] = new_appeal;
    (*appeal_count)++;
    save_appeal(appeal_list, *appeal_count);

    // 2. 同步添加管理员待办
    Todo new_todo;
    char* type_str = new_appeal.type == 1 ? "成绩申诉" : "密码找回";
    sprintf(new_todo.content, "处理%s：%s", type_str, account);
    new_todo.is_done = 0;
    todo_list[*todo_count] = new_todo;
    (*todo_count)++;
    save_todo(todo_list, *todo_count);

    printf("申诉提交成功！请等待管理员处理。\n");
}

//  学生修改密码
void student_change_pwd(UserNode* user_head, int user_count, char* account) {
    if (user_head == NULL || user_count == 0) {
        printf("暂无用户数据！\n");
        return;
    }

    UserNode* p = user_head;
    while (p != NULL && strcmp(p->data.account, account) != 0) {
        p = p->next;
    }
    if (p == NULL) {
        printf("学生账号不存在！\n");
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
    save_user(user_head, user_count);
    printf("密码修改成功！请重新登录。\n");
}

