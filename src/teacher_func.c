#include "../include/teacher_func.h"
#include "../include/file_func.h"
#include "../include/common.h"

// 教师主菜单
void teacher_menu(StudentNode* stu_head, int stu_count, UserNode* user_head, int user_count, char* account) {
    int choice;
    while (1) {
        printf("\n===== 教师功能菜单 =====\n");
        printf("1. 管理学生信息\n");
        printf("2. 分页查看班级成绩\n");
        printf("3. 学生成绩分析\n");
        printf("4. 修改个人密码\n");
        printf("0. 退出登录\n");
        printf("请选择操作：");

        while (scanf("%d", &choice) != 1 || choice < 0 || choice > 4) {
            clear_buffer();
            printf("输入错误！请输入0-4：");
        }
        clear_buffer();

        switch (choice) {
        case 1: {
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
                case 0: goto TEACHER_STU_MENU_EXIT;
                default: printf("输入错误！\n");
                }
            }
        TEACHER_STU_MENU_EXIT:
            break;
        }
        case 2: view_class_score_page(stu_head, stu_count); break;
        case 3: teacher_score_analysis(stu_head, stu_count); break;
        case 4: teacher_change_pwd(user_head, user_count, account); break;
        case 0: printf("退出教师登录...\n"); return;
        default: printf("输入错误！\n");
        }
    }
}

//教师修改密码
void teacher_change_pwd(UserNode* user_head, int user_count, char* account) {
    if (user_head == NULL || user_count == 0) {
        printf("暂无用户数据！\n");
        return;
    }

    UserNode* p = user_head;
    while (p != NULL && strcmp(p->data.account, account) != 0) {
        p = p->next;
    }
    if (p == NULL) {
        printf("教师账号不存在！\n");
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
    printf("密码修改成功！\n");
}

// 添加学生
void add_student(StudentNode** stu_head, int* stu_count) {
    Student temp;
    printf("请输入学生学号：");
    scanf("%11s", temp.id);
    clear_buffer();

    // 检查学号重复
    StudentNode* p = *stu_head;
    while (p != NULL) {
        if (strcmp(p->data.id, temp.id) == 0) {
            printf("该学号已存在！\n");
            return;
        }
        p = p->next;
    }

    // 用fgets读取带空格的名字
    printf("请输入学生姓名：");
    fgets(temp.name, MAX_NAME_LEN, stdin);
    size_t len = strlen(temp.name);
    if (len > 0 && temp.name[len - 1] == '\n') {
        temp.name[len - 1] = '\0';
    }

    printf("请输入学生班级：");
    fgets(temp.class, MAX_CLASS_LEN, stdin);
    len = strlen(temp.class);
    if (len > 0 && temp.class[len - 1] == '\n') {
        temp.class[len - 1] = '\0';
    }

    // 输入并校验成绩
    printf("请输入数学成绩：");
    while (scanf("%f", &temp.math) != 1 || !check_score(temp.math)) {
        clear_buffer();
        printf("成绩错误！请输入0-100的数值：");
    }
    clear_buffer();

    printf("请输入语文成绩：");
    while (scanf("%f", &temp.chinese) != 1 || !check_score(temp.chinese)) {
        clear_buffer();
        printf("成绩错误！请输入0-100的数值：");
    }
    clear_buffer();

    printf("请输入英语成绩：");
    while (scanf("%f", &temp.english) != 1 || !check_score(temp.english)) {
        clear_buffer();
        printf("成绩错误！请输入0-100的数值：");
    }
    clear_buffer();

    calc_student_score(&temp);

	// 插入学生信息到链表头部
    StudentNode* new_node = (StudentNode*)malloc(sizeof(StudentNode));
    new_node->data = temp;
    new_node->next = *stu_head;
    *stu_head = new_node;
    (*stu_count)++;

    save_student(*stu_head, *stu_count);
    printf("学生添加成功！\n");
}

//删除学生
void del_student(StudentNode** stu_head, int* stu_count, char* id) {
    if (*stu_head == NULL || *stu_count == 0) {
        printf("暂无学生数据！\n");
        return;
    }

    StudentNode* p = *stu_head, * pre = NULL;
    while (p != NULL && strcmp(p->data.id, id) != 0) {
        pre = p;
        p = p->next;
    }
    if (p == NULL) {
        printf("该学生不存在！\n");
        return;
    }

    // 删除节点
    if (pre == NULL) {
        *stu_head = p->next;
    }
    else {
        pre->next = p->next;
    }
    free(p);
    (*stu_count)--;

    save_student(*stu_head, *stu_count);
    printf("学生删除成功！\n");
}

// 修改学生成绩
void edit_student(StudentNode* stu_head, int stu_count, char* id) {
    if (stu_head == NULL || stu_count == 0) {
        printf("暂无学生数据！\n");
        return;
    }

    StudentNode* p = stu_head;
    while (p != NULL && strcmp(p->data.id, id) != 0) {
        p = p->next;
    }
    if (p == NULL) {
        printf("该学生不存在！\n");
        return;
    }

    printf("当前成绩：数学%.2f 语文%.2f 英语%.2f\n",
        p->data.math, p->data.chinese, p->data.english);
    printf("请输入新的数学成绩：");
    while (scanf("%f", &p->data.math) != 1 || !check_score(p->data.math)) {
        clear_buffer();
        printf("成绩错误！请输入0-100的数值：");
    }
    clear_buffer();

    printf("请输入新的语文成绩：");
    while (scanf("%f", &p->data.chinese) != 1 || !check_score(p->data.chinese)) {
        clear_buffer();
        printf("成绩错误！请输入0-100的数值：");
    }
    clear_buffer();

    printf("请输入新的英语成绩：");
    while (scanf("%f", &p->data.english) != 1 || !check_score(p->data.english)) {
        clear_buffer();
        printf("成绩错误！请输入0-100的数值：");
    }
    clear_buffer();

    calc_student_score(&p->data);
    save_student(stu_head, stu_count);
    printf("学生成绩修改成功！\n");
}

// 查询学生成绩
void query_student(StudentNode* stu_head, int stu_count) {
    if (stu_head == NULL || stu_count == 0) {
        printf("暂无学生数据！\n");
        return;
    }

    int query_choice, field, sort_type;
    printf("\n===== 学生成绩查询 =====\n");
    printf("1. 按学号查询\n");
    printf("2. 按姓名查询\n");
    printf("3. 按班级查询\n");
    printf("4. 按单科/总分排序查询\n");
    printf("请选择查询方式：");
    scanf("%d", &query_choice);
    clear_buffer();

    switch (query_choice) {
    case 1: {
        char key[MAX_ID_LEN] = { 0 };
        printf("请输入学生学号：");
        scanf("%11s", key);
        clear_buffer();
        StudentNode* p = stu_head;
        while (p != NULL) {
            if (strcmp(p->data.id, key) == 0) {
                printf("\n查询结果：\n");
                printf("学号：%s\n姓名：%s\n班级：%s\n",
                    p->data.id, p->data.name, p->data.class);
                printf("数学：%.2f 语文：%.2f 英语：%.2f\n",
                    p->data.math, p->data.chinese, p->data.english);
                printf("平均分：%.2f 总分：%.2f\n", p->data.avg, p->data.total);
                return;
            }
            p = p->next;
        }
        printf("未找到该学号的学生！\n");
        break;
    }
    case 2: {
        char key[MAX_NAME_LEN] = { 0 };
        printf("请输入学生姓名：");
        fgets(key, MAX_NAME_LEN, stdin);
        size_t len = strlen(key);
        if (len > 0 && key[len - 1] == '\n') key[len - 1] = '\0';
        StudentNode* p = stu_head;
        int found = 0;
        while (p != NULL) {
            if (strcmp(p->data.name, key) == 0) {
                if (!found) {
                    printf("\n查询结果：\n");
                    found = 1;
                }
                printf("学号：%s\n姓名：%s\n班级：%s\n",
                    p->data.id, p->data.name, p->data.class);
                printf("数学：%.2f 语文：%.2f 英语：%.2f\n",
                    p->data.math, p->data.chinese, p->data.english);
                printf("平均分：%.2f 总分：%.2f\n", p->data.avg, p->data.total);
                printf("------------------------\n");
            }
            p = p->next;
        }
        if (!found) printf("未找到该姓名的学生！\n");
        break;
    }
    case 3: {
        char key[MAX_CLASS_LEN] = { 0 };
        printf("请输入班级名称：");
        fgets(key, MAX_CLASS_LEN, stdin);
        size_t len = strlen(key);
        if (len > 0 && key[len - 1] == '\n') key[len - 1] = '\0';
        StudentNode* p = stu_head;
        int found = 0;
        while (p != NULL) {
            if (strcmp(p->data.class, key) == 0) {
                if (!found) {
                    printf("\n%s 班级成绩：\n", key);
                    printf("%-15s %-22s %-10s %-10s %-8s %-12s %-8s\n",
                        "学号", "姓名", "数学", "语文", "英语", "平均分", "总分");
					printf("--------------------------------------------------------------------------------\n");
                    found = 1;
                }
                printf("%-12s %-20s %-8.2f %-8.2f %-8.2f %-8.2f %-8.2f\n",
                    p->data.id, p->data.name,
                    p->data.math, p->data.chinese, p->data.english,
                    p->data.avg, p->data.total);
            }
            p = p->next;
        }
        if (!found) printf("未找到该班级的学生！\n");
        break;
    }
    case 4: {
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

        // 链表转数组
        Student temp_stu[stu_count];
        StudentNode* p = stu_head;
        int i = 0;
        while (p != NULL && i < stu_count) {
            temp_stu[i] = p->data;
            p = p->next;
            i++;
        }

        // 排序
        sort_students(temp_stu, stu_count, field, sort_type);

        // 打印结果
        char* field_name;
        switch (field) {
        case 1: field_name = "数学"; break;
        case 2: field_name = "语文"; break;
        case 3: field_name = "英语"; break;
        case 4: field_name = "总分"; break;
        }
        printf("\n按%s%s排序结果：\n", field_name, sort_type == 0 ? "升序" : "降序");
        printf("%-15s %-22s %-10s %-10s %-8s %-12s %-8s\n",
            "学号", "姓名", "数学", "语文", "英语", "平均分", "总分");
		printf("--------------------------------------------------------------------------------\n");
        for (i = 0; i < stu_count; i++) {
            printf("%-12s %-20s %-8.2f %-8.2f %-8.2f %-8.2f %-8.2f\n",
                temp_stu[i].id, temp_stu[i].name,
                temp_stu[i].math, temp_stu[i].chinese, temp_stu[i].english,
                temp_stu[i].avg, temp_stu[i].total);
        }
        break;
    }
    default: printf("输入错误！\n");
    }
}

// 分页查看班级成绩 
void view_class_score_page(StudentNode* stu_head, int stu_count) {
    if (stu_head == NULL || stu_count == 0) {
        printf("暂无学生数据！\n");
        return;
    }

    char class_name[MAX_CLASS_LEN] = { 0 };
    printf("请输入要查看的班级名称：");
    fgets(class_name, MAX_CLASS_LEN, stdin);
    size_t len = strlen(class_name);
    if (len > 0 && class_name[len - 1] == '\n') class_name[len - 1] = '\0';

    // 第一步：筛选该班级的学生到临时数组
    Student temp_stu[stu_count];
    int class_count = 0;
    StudentNode* p = stu_head;
    while (p != NULL) {
        if (strcmp(p->data.class, class_name) == 0) {
            temp_stu[class_count++] = p->data;
        }
        p = p->next;
    }

    if (class_count == 0) {
        printf("该班级暂无学生数据！\n");
        return;
    }

    // 第二步：询问是否排序
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

    // 第三步：分页显示
    int page_size = 5;
    int total_page = (class_count + page_size - 1) / page_size;
    int current_page = 1;

    while (1) {
        printf("\n===== %s 班级成绩 - 第%d页/共%d页 =====\n", class_name, current_page, total_page);
        printf("%-15s %-22s %-10s %-10s %-8s %-12s %-8s\n",
            "学号", "姓名", "数学", "语文", "英语", "平均分", "总分");
        printf("-----------------------------------------------------------------------\n");

        // 打印当前页数据
        int start = (current_page - 1) * page_size;
        int end = start + page_size;
        if (end > class_count) end = class_count;
        for (int i = start; i < end; i++) {
            printf("%-12s %-20s %-8.2f %-8.2f %-8.2f %-8.2f %-8.2f\n",
                temp_stu[i].id, temp_stu[i].name,
                temp_stu[i].math, temp_stu[i].chinese, temp_stu[i].english,
                temp_stu[i].avg, temp_stu[i].total);
        }

        // 分页操作
        printf("\n操作：1-上一页 2-下一页 0-退出\n");
        int opt;
        printf("请选择：");
        scanf("%d", &opt);
        clear_buffer();
        switch (opt) {
        case 1:
            if (current_page > 1) current_page--;
            else printf("已是第一页！\n");
            break;
        case 2:
            if (current_page < total_page) current_page++;
            else printf("已是最后一页！\n");
            break;
        case 0: return;
        default: printf("输入错误！\n");
        }
    }
}

// 教师端成绩分析 
void teacher_score_analysis(StudentNode* stu_head, int stu_count) {
    if (stu_head == NULL || stu_count == 0) {
        printf("暂无学生数据！\n");
        return;
    }

    float math_sum = 0, chinese_sum = 0, english_sum = 0;
    float math_max = 0, chinese_max = 0, english_max = 0;
    float math_min = 100, chinese_min = 100, english_min = 100;

    StudentNode* p = stu_head;
    while (p != NULL) {
        math_sum += p->data.math;
        chinese_sum += p->data.chinese;
        english_sum += p->data.english;

        if (p->data.math > math_max) math_max = p->data.math;
        if (p->data.chinese > chinese_max) chinese_max = p->data.chinese;
        if (p->data.english > english_max) english_max = p->data.english;

        if (p->data.math < math_min) math_min = p->data.math;
        if (p->data.chinese < chinese_min) chinese_min = p->data.chinese;
        if (p->data.english < english_min) english_min = p->data.english;
        p = p->next;
    }

    printf("\n===== 成绩分析报告 =====\n");
    printf("参与统计学生数：%d\n", stu_count);
    printf("科目\t平均分\t最高分\t最低分\n");
    printf("数学\t%.2f\t%.2f\t%.2f\n", math_sum / stu_count, math_max, math_min);
    printf("语文\t%.2f\t%.2f\t%.2f\n", chinese_sum / stu_count, chinese_max, chinese_min);
    printf("英语\t%.2f\t%.2f\t%.2f\n", english_sum / stu_count, english_max, english_min);
}

// 导出学生成绩
void export_student(StudentNode* stu_head, int stu_count) {
    if (stu_head == NULL || stu_count == 0) {
        printf("暂无学生数据！\n");
        return;
    }

    system("md data 2>nul");
    FILE* fp = fopen("./data/students_export.csv", "w");
    if (!fp) {
        printf("导出失败！\n");
        return;
    }

    fprintf(fp, "学号,姓名,班级,数学,语文,英语,平均分,总分\n");
    StudentNode* p = stu_head;
    while (p != NULL) {
        fprintf(fp, "%s,%s,%s,%.2f,%.2f,%.2f,%.2f,%.2f\n",
            p->data.id, p->data.name, p->data.class,
            p->data.math, p->data.chinese, p->data.english,
            p->data.avg, p->data.total);
        p = p->next;
    }

    fclose(fp);
    printf("学生成绩已导出到 ./data/students_export.csv\n");
}