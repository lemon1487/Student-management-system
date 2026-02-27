#include "../include/common.h"

// 检查成绩是否合法（0-100分）
int check_score(float score) {
    return (score >= 0 && score <= 100);
}

// 计算学生总分和平均分
void calc_student_score(Student* stu) {
    if (stu == NULL) return;
    stu->total = stu->math + stu->chinese + stu->english;
    stu->avg = stu->total / 3;
}

// 学生成绩排序（按指定字段升序/降序）
// field:1-数学 2-语文 3-英语 4-总分；type:0-升序 1-降序
void sort_students(Student* stu_list, int count, int field, int type) {
    if (stu_list == NULL || count <= 1) return;
    for (int i = 0; i < count - 1; i++) {
        for (int j = 0; j < count - i - 1; j++) {
            float val1, val2;
            switch (field) {
            case 1: val1 = stu_list[j].math; val2 = stu_list[j + 1].math; break;
            case 2: val1 = stu_list[j].chinese; val2 = stu_list[j + 1].chinese; break;
            case 3: val1 = stu_list[j].english; val2 = stu_list[j + 1].english; break;
            case 4: val1 = stu_list[j].total; val2 = stu_list[j + 1].total; break;
            default: return;
            }
            // 升序/降序判断
            if ((type == 0 && val1 > val2) || (type == 1 && val1 < val2)) {
                Student temp = stu_list[j];
                stu_list[j] = stu_list[j + 1];
                stu_list[j + 1] = temp;
            }
        }
    }
}

// 教师主菜单入口（核心功能导航）
void teacher_menu(Student* stu_list, int* stu_count, User* user_list, int user_count, char* account) {
    if (stu_list == NULL || stu_count == NULL || user_list == NULL || account == NULL) return;
    int choice;
    while (1) {
        printf("\n===== 教师功能菜单 =====\n");
        printf("1. 增删查改学生信息\n");
        printf("2. 查看班内成绩（翻页）\n");
        printf("3. 导出学生信息到文件\n");
        printf("4. 成绩分析（含排序）\n");
        printf("5. 修改密码（需原密码）\n");
        printf("0. 返回上一级\n");
        printf("请选择操作：");

        while (scanf("%d", &choice) != 1 || choice < 0 || choice > 5) {
            clear_buffer();
            printf("输入错误！请输入0-5：");
        }
        clear_buffer();

        switch (choice) {
        case 1: manage_student(stu_list, stu_count); break;
        case 2: view_class_score_page(stu_list, *stu_count); break;
        case 3: export_student(stu_list, *stu_count); break;
        case 4: teacher_score_analysis(stu_list, *stu_count); break;
        case 5: teacher_change_pwd(user_list, user_count, account); break;
        case 0: printf("返回上一级菜单...\n"); return;
        default: printf("输入错误！\n");
        }
    }
}

// 学生信息管理总入口（增删改查+排序）
void manage_student(Student* stu_list, int* stu_count) {
    if (stu_list == NULL || stu_count == NULL) return;
    int choice;
    while (1) {
        printf("\n===== 学生信息管理 =====\n");
        printf("1. 添加学生\n");
        printf("2. 删除学生\n");
        printf("3. 修改学生成绩\n");
        printf("4. 查询学生信息\n");
        printf("5. 成绩排序\n");
        printf("0. 返回上一级\n");
        printf("请选择：");

        while (scanf("%d", &choice) != 1 || choice < 0 || choice > 5) {
            clear_buffer();
            printf("输入错误！请输入0-5：");
        }
        clear_buffer();

        switch (choice) {
        case 1: add_student(stu_list, stu_count); break;
        case 2: del_student(stu_list, stu_count); break;
        case 3: edit_student(stu_list, *stu_count); break;
        case 4: query_student(stu_list, *stu_count); break;
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
            printf("排序完成！\n");
            query_student(stu_list, *stu_count);
            break;
        }
        case 0: return;
        default: printf("输入错误！\n");
        }
    }
}

// 添加学生（支持姓名/班级含空格，学号查重）
void add_student(Student* stu_list, int* stu_count) {
    if (stu_list == NULL || stu_count == NULL || *stu_count >= 1000) {
        printf("学生数量已达上限/参数错误！\n");
        return;
    }

    Student new_stu = { 0 };
    printf("请输入学号：");
    scanf("%11s", new_stu.id);
    clear_buffer(); // 清空缓冲区，避免影响后续fgets

    // 检查学号重复
    for (int i = 0; i < *stu_count; i++) {
        if (strcmp(stu_list[i].id, new_stu.id) == 0) {
            printf("该学号已存在！\n");
            return;
        }
    }

    // 读取姓名（支持空格）
    printf("请输入姓名（支持空格）：");
    fgets(new_stu.name, MAX_NAME_LEN, stdin);
    new_stu.name[strcspn(new_stu.name, "\n")] = '\0'; // 移除换行符

    // 读取班级（支持空格）
    printf("请输入班级（支持空格）：");
    fgets(new_stu.class, MAX_CLASS_LEN, stdin);
    new_stu.class[strcspn(new_stu.class, "\n")] = '\0'; // 移除换行符

    // 成绩输入（带合法性校验）
    printf("请输入数学成绩：");
    while (scanf("%f", &new_stu.math) != 1 || !check_score(new_stu.math)) {
        clear_buffer();
        printf("输入错误！请输入0-100的数字：");
    }
    printf("请输入语文成绩：");
    while (scanf("%f", &new_stu.chinese) != 1 || !check_score(new_stu.chinese)) {
        clear_buffer();
        printf("输入错误！请输入0-100的数字：");
    }
    printf("请输入英语成绩：");
    while (scanf("%f", &new_stu.english) != 1 || !check_score(new_stu.english)) {
        clear_buffer();
        printf("输入错误！请输入0-100的数字：");
    }
    clear_buffer();

    // 计算总分/平均分
    calc_student_score(&new_stu);

    // 添加到数组并保存
    stu_list[*stu_count] = new_stu;
    (*stu_count)++;
    save_student(stu_list, *stu_count);
    printf("学生信息添加成功！\n");
}

// 删除学生（按学号查找，删除后数据前移）
void del_student(Student* stu_list, int* stu_count) {
    if (stu_list == NULL || stu_count == NULL || *stu_count == 0) {
        printf("暂无学生数据/参数错误！\n");
        return;
    }

    char del_id[MAX_ID_LEN] = { 0 };
    printf("请输入要删除的学生学号：");
    scanf("%11s", del_id);
    clear_buffer();

    int del_idx = -1;
    for (int i = 0; i < *stu_count; i++) {
        if (strcmp(stu_list[i].id, del_id) == 0) {
            del_idx = i;
            break;
        }
    }
    if (del_idx == -1) {
        printf("学号不存在！\n");
        return;
    }

    // 移除学生（后续元素前移）
    for (int i = del_idx; i < *stu_count - 1; i++) {
        stu_list[i] = stu_list[i + 1];
    }
    (*stu_count)--;
    save_student(stu_list, *stu_count);
    printf("学生信息删除成功！\n");
}

// 修改学生成绩（按学号查找，修改后重新计算总分/平均分）
void edit_student(Student* stu_list, int stu_count) {
    if (stu_list == NULL || stu_count == 0) {
        printf("暂无学生数据/参数错误！\n");
        return;
    }

    char edit_id[MAX_ID_LEN] = { 0 };
    printf("请输入要修改的学生学号：");
    scanf("%11s", edit_id);
    clear_buffer();

    int edit_idx = -1;
    for (int i = 0; i < stu_count; i++) {
        if (strcmp(stu_list[i].id, edit_id) == 0) {
            edit_idx = i;
            break;
        }
    }
    if (edit_idx == -1) {
        printf("学号不存在！\n");
        return;
    }

    // 修改成绩（带合法性校验）
    printf("当前成绩：数学=%.1f 语文=%.1f 英语=%.1f\n",
        stu_list[edit_idx].math, stu_list[edit_idx].chinese, stu_list[edit_idx].english);
    printf("请输入新的数学成绩：");
    while (scanf("%f", &stu_list[edit_idx].math) != 1 || !check_score(stu_list[edit_idx].math)) {
        clear_buffer();
        printf("输入错误！请输入0-100的数字：");
    }
    printf("请输入新的语文成绩：");
    while (scanf("%f", &stu_list[edit_idx].chinese) != 1 || !check_score(stu_list[edit_idx].chinese)) {
        clear_buffer();
        printf("输入错误！请输入0-100的数字：");
    }
    printf("请输入新的英语成绩：");
    while (scanf("%f", &stu_list[edit_idx].english) != 1 || !check_score(stu_list[edit_idx].english)) {
        clear_buffer();
        printf("输入错误！请输入0-100的数字：");
    }
    clear_buffer();

    // 重新计算总分/平均分并保存
    calc_student_score(&stu_list[edit_idx]);
    save_student(stu_list, stu_count);
    printf("成绩修改成功！\n");
}

// 查询所有学生信息（格式化展示）
void query_student(Student* stu_list, int stu_count) {
    if (stu_list == NULL || stu_count == 0) {
        printf("暂无学生数据/参数错误！\n");
        return;
    }

    printf("\n===== 学生信息列表 =====\n");
    printf("%-15s %-20s %-20s %12s %12s %10s %10s %10s\n",
        "学号", "姓名", "班级", "数学", "语文", "英语", "平均分", "总分");
    printf("--------------------------------------------------------------------------------------------------------\n");
    for (int i = 0; i < stu_count; i++) {
        printf("%-12s %-20s %-20s %8.1f %8.1f %8.1f %8.1f %8.1f\n",
            stu_list[i].id, stu_list[i].name, stu_list[i].class,
            stu_list[i].math, stu_list[i].chinese, stu_list[i].english,
            stu_list[i].avg, stu_list[i].total);
    }
}

// 翻页查看指定班级成绩（分页展示，支持上下页切换）
void view_class_score_page(Student* stu_list, int stu_count) {
    if (stu_list == NULL || stu_count == 0) {
        printf("暂无学生数据！\n");
        return;
    }

    // 获取班级列表（去重）
    char classes[100][MAX_CLASS_LEN] = { 0 };
    int class_count = 0;
    for (int i = 0; i < stu_count; i++) {
        int exists = 0;
        for (int j = 0; j < class_count; j++) {
            if (strcmp(classes[j], stu_list[i].class) == 0) {
                exists = 1;
                break;
            }
        }
        if (!exists) {
            strcpy(classes[class_count], stu_list[i].class);
            class_count++;
        }
    }

    // 选择班级
    printf("\n===== 班级列表 =====\n");
    for (int i = 0; i < class_count; i++) {
        printf("%d. %s\n", i + 1, classes[i]);
    }
    printf("请选择班级（输入序号）：");
    int class_choice;
    while (scanf("%d", &class_choice) != 1 || class_choice < 1 || class_choice > class_count) {
        clear_buffer();
        printf("输入错误！请输入1-%d：", class_count);
    }
    clear_buffer();
    char* target_class = classes[class_choice - 1];

    // 筛选本班学生
    Student class_stu[1000] = { 0 };
    int class_stu_count = 0;
    for (int i = 0; i < stu_count; i++) {
        if (strcmp(stu_list[i].class, target_class) == 0) {
            class_stu[class_stu_count] = stu_list[i];
            class_stu_count++;
        }
    }

    // 翻页显示
    int total_page = (class_stu_count + PAGE_SIZE - 1) / PAGE_SIZE;
    int curr_page = 1;
    while (1) {
        printf("\n===== %s 班级成绩（第%d页/共%d页）=====\n", target_class, curr_page, total_page);
        printf("%-15s %-17s %15s %8s %10s %11s %11s\n",
            "学号", "姓名", "数学", "语文", "英语", "平均分", "总分");
        printf("-------------------------------------------------------------------------------------------\n");

        int start = (curr_page - 1) * PAGE_SIZE;
        int end = start + PAGE_SIZE;
        if (end > class_stu_count) end = class_stu_count;

        for (int i = start; i < end; i++) {
            printf("%-12s %-20s %8.1f %8.1f %8.1f %8.1f %8.1f\n",
                class_stu[i].id, class_stu[i].name,
                class_stu[i].math, class_stu[i].chinese, class_stu[i].english,
                class_stu[i].avg, class_stu[i].total);
        }

        printf("\n操作：1-下一页 2-上一页 0-返回\n");
        int op;
        while (scanf("%d", &op) != 1 || op < 0 || op > 2) {
            clear_buffer();
            printf("输入错误！请输入0-2：");
        }
        clear_buffer();

        if (op == 0) break;
        else if (op == 1 && curr_page < total_page) curr_page++;
        else if (op == 2 && curr_page > 1) curr_page--;
    }
}

// 导出学生信息到CSV文件（覆盖式写入，解决重复定义问题）
void export_student(Student* stu_list, int stu_count) {
    if (stu_list == NULL || stu_count == 0) {
        printf("暂无学生数据！\n");
        return;
    }

    FILE* fp = fopen("./data/students_export.csv", "w");
    if (!fp) {
        printf("导出失败！\n");
        return;
    }

    // 写入CSV表头
    fprintf(fp, "学号,姓名,班级,数学,语文,英语,平均分,总分\n");
    // 写入学生数据
    for (int i = 0; i < stu_count; i++) {
        fprintf(fp, "%s,%s,%s,%.1f,%.1f,%.1f,%.1f,%.1f\n",
            stu_list[i].id, stu_list[i].name, stu_list[i].class,
            stu_list[i].math, stu_list[i].chinese, stu_list[i].english,
            stu_list[i].avg, stu_list[i].total);
    }
    fclose(fp);
    printf("学生信息已导出到 ./data/students_export.csv\n");
}

// 教师成绩分析（统计平均分/最高分/最低分，支持总分降序排名）
void teacher_score_analysis(Student* stu_list, int stu_count) {
    if (stu_list == NULL || stu_count == 0) {
        printf("暂无学生数据！\n");
        return;
    }

    // 统计各科平均分
    float math_sum = 0, chinese_sum = 0, english_sum = 0;
    for (int i = 0; i < stu_count; i++) {
        math_sum += stu_list[i].math;
        chinese_sum += stu_list[i].chinese;
        english_sum += stu_list[i].english;
    }

    printf("\n===== 成绩分析汇总 =====\n");
    printf("总人数：%d\n", stu_count);
    printf("数学平均分：%.1f\n", math_sum / stu_count);
    printf("语文平均分：%.1f\n", chinese_sum / stu_count);
    printf("英语平均分：%.1f\n", english_sum / stu_count);

    // 统计各科最高分/最低分
    float math_max = 0, math_min = 100;
    float chinese_max = 0, chinese_min = 100;
    float english_max = 0, english_min = 100;
    for (int i = 0; i < stu_count; i++) {
        if (stu_list[i].math > math_max) math_max = stu_list[i].math;
        if (stu_list[i].math < math_min) math_min = stu_list[i].math;
        if (stu_list[i].chinese > chinese_max) chinese_max = stu_list[i].chinese;
        if (stu_list[i].chinese < chinese_min) chinese_min = stu_list[i].chinese;
        if (stu_list[i].english > english_max) english_max = stu_list[i].english;
        if (stu_list[i].english < english_min) english_min = stu_list[i].english;
    }

    printf("\n各科目最高分/最低分：\n");
    printf("数学：%.1f / %.1f\n", math_max, math_min);
    printf("语文：%.1f / %.1f\n", chinese_max, chinese_min);
    printf("英语：%.1f / %.1f\n", english_max, english_min);

    // 可选：按总分降序展示学生排名
    printf("\n是否按总分降序显示所有学生？(1-是 0-否)：");
    int show_sort;
    while (scanf("%d", &show_sort) != 1 || show_sort < 0 || show_sort > 1) {
        clear_buffer();
        printf("输入错误！请输入0或1：");
    }
    clear_buffer();

    if (show_sort == 1) {
        // 临时复制数组，避免修改原数据
        Student temp_stu[1000];
        for (int i = 0; i < stu_count; i++) {
            temp_stu[i] = stu_list[i];
        }
        sort_students(temp_stu, stu_count, 4, 1); // 总分降序排序
        printf("\n===== 总分降序排名 =====\n");
        printf("%-15s %-20s %-22s %12s\n", "学号", "姓名", "班级", "总分");
        printf("-----------------------------------------------------------------------------\n");
        for (int i = 0; i < stu_count; i++) {
            printf("%-12s %-20s %-20s %8.1f\n",
                temp_stu[i].id, temp_stu[i].name, temp_stu[i].class, temp_stu[i].total);
        }
    }
}

// 教师修改密码（验证原密码，两次确认新密码，修改后保存）
void teacher_change_pwd(User* user_list, int user_count, char* account) {
    if (user_list == NULL || account == NULL || user_count == 0 || strcmp(account, "admin_temp") == 0) {
        printf("管理员无教师密码可修改！\n"); // 管理员切换后提示
        return;
    }

    // 查找当前教师账号
    int idx = -1;
    for (int i = 0; i < user_count; i++) {
        if (strcmp(user_list[i].account, account) == 0 && user_list[i].type == 2) {
            idx = i;
            break;
        }
    }
    if (idx == -1) {
        printf("教师账号不存在！\n");
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

    // 保存新密码
    strcpy(user_list[idx].pwd, new_pwd);
    save_user(user_list, user_count);
    printf("密码修改成功！\n");
}