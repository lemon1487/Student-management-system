#include "../include/common.h"

// 保存用户数据到文件
void save_user(User* user_list, int count) {
    if (user_list == NULL || count <= 0) return;
    // 强制创建data目录
    system("mkdir -p data");
    FILE* fp = fopen("./data/users.dat", "w");
    if (!fp) {
        printf("保存用户数据失败！\n");
        return;
    }
    // 只存核心数据：账号 密码 类型
    for (int i = 0; i < count; i++) {
        fprintf(fp, "%s %s %d\n",
            user_list[i].account, user_list[i].pwd, user_list[i].type);
    }
	fflush(fp);//手动刷新缓存区，确保数据写入文件
    fclose(fp);
}

// 从文件加载用户数据
int load_user(User* user_list, int max_count) {
    if (user_list == NULL || max_count <= 0) return 0;
    FILE* fp = fopen("./data/users.dat", "r");
    if (!fp) return 0; // 文件不存在返回0，触发默认账号初始化

    int count = 0;
    char account[20], pwd[20];
    int type;
    // 逐行读取：账号 密码 类型
    while (count < max_count && fscanf(fp, "%19s %19s %d", account, pwd, &type) == 3) {
        strcpy(user_list[count].account, account);
        strcpy(user_list[count].pwd, pwd);
        user_list[count].type = type;
        count++;
    }
    fclose(fp);
    return count;
}


// 保存学生数据到文件（修复姓名/班级含空格问题，改用CSV格式）
void save_student(Student* stu_list, int count) {
    if (stu_list == NULL || count <= 0) return;
    // 确保data目录存在
    system("mkdir -p data");
    FILE* fp = fopen("./data/students.dat", "w");
    if (!fp) {
        printf("保存学生数据失败！\n");
        return;
    }
    // 表头：用逗号分隔字段，兼容含空格的姓名/班级
    fprintf(fp, "学号,姓名,班级,数学,语文,英语,平均分,总分\n");
    for (int i = 0; i < count; i++) {
        // 按CSV格式写入，逗号分隔，成绩保留1位小数
        fprintf(fp, "%s,%s,%s,%.1f,%.1f,%.1f,%.1f,%.1f\n",
            stu_list[i].id, stu_list[i].name, stu_list[i].class,
            stu_list[i].math, stu_list[i].chinese, stu_list[i].english,
            stu_list[i].avg, stu_list[i].total);
    }
    fclose(fp);
}

// 从文件加载学生数据（适配CSV格式，支持姓名/班级含空格）
int load_student(Student* stu_list, int max_count) {
    if (stu_list == NULL || max_count <= 0) return 0;
    FILE* fp = fopen("./data/students.dat", "r");
    if (!fp) return 0;

    char line[500] = { 0 };
    int count = 0;
    // 跳过表头行
    if (!fgets(line, 500, fp)) { fclose(fp); return 0; }

    while (count < max_count && fgets(line, 500, fp)) {
        Student temp = { 0 };
        float math = 0, chinese = 0, english = 0, avg = 0, total = 0;

        // 用逗号分隔解析字段，支持姓名/班级含空格
        // %[^,]：匹配直到逗号的所有字符（包含空格）
        int ret = sscanf(line, "%11[^,],%19[^,],%19[^,],%f,%f,%f,%f,%f",
            temp.id, temp.name, temp.class,
            &math, &chinese, &english, &avg, &total);

        // 解析成功（8个字段）才保存
        if (ret == 8) {
            // 校验成绩合法性（0-100分）
            temp.math = (math >= 0 && math <= 100) ? math : 0;
            temp.chinese = (chinese >= 0 && chinese <= 100) ? chinese : 0;
            temp.english = (english >= 0 && english <= 100) ? english : 0;
            temp.avg = avg;
            temp.total = total;
            stu_list[count] = temp;
            count++;
        }
    }
    fclose(fp);
    return count;
}

// 保存待办事项数据到文件
void save_todo(Todo* todo_list, int count) {
    if (todo_list == NULL || count <= 0) return;
    FILE* fp = fopen("./data/todos.dat", "w");
    if (!fp) {
        printf("保存待办数据失败！\n");
        return;
    }
    fprintf(fp, "%-60s %-8s\n", "待办内容", "状态");
    fprintf(fp, "----------------------------------------------------------------------\n");
    for (int i = 0; i < count; i++) {
        char status[10];
        strcpy(status, todo_list[i].is_done ? "已完成" : "未完成");
        fprintf(fp, "%-60s %-8s\n", todo_list[i].content, status);
    }
    fclose(fp);
}

// 从文件加载待办事项数据
int load_todo(Todo* todo_list, int max_count) {
    if (todo_list == NULL || max_count <= 0) return 0;
    FILE* fp = fopen("./data/todos.dat", "r");
    if (!fp) return 0;

    char line[300] = { 0 }, status[10] = { 0 };
    int count = 0;
    // 跳过表头和分隔线
    if (!fgets(line, 300, fp)) { fclose(fp); return 0; }
    if (!fgets(line, 300, fp)) { fclose(fp); return 0; }

    while (count < max_count) {
        Todo temp = { 0 };
        if (fscanf(fp, "%99[^\n] %9s", temp.content, status) != 2) break;
        temp.is_done = (strcmp(status, "已完成") == 0) ? 1 : 0;
        todo_list[count] = temp;
        count++;
        clear_buffer(); // 清空输入缓冲区
    }
    fclose(fp);
    return count;
}

// 保存申诉记录数据到文件
void save_appeal(Appeal* appeal_list, int count) {
    if (appeal_list == NULL || count <= 0) return;
    FILE* fp = fopen("./data/appeals.dat", "w");
    if (!fp) {
        printf("保存申诉数据失败！\n");
        return;
    }
    fprintf(fp, "%-12s %-40s %-8s %-8s %-40s\n",
        "申诉账号", "申诉内容", "类型", "状态", "反馈");
    fprintf(fp, "------------------------------------------------------------------------------------\n");
    for (int i = 0; i < count; i++) {
        char type_str[10];
        strncpy(type_str, appeal_list[i].type == 1 ? "成绩申诉" : "密码找回", 9);
        type_str[9] = '\0'; // 手动加终止符

        char status_str[10];
        strncpy(status_str, appeal_list[i].is_processed ? "已处理" : "未处理", 9);
        status_str[9] = '\0';

        fprintf(fp, "%-12s %-40s %-8s %-8s %-40s\n",
            appeal_list[i].stu_id, appeal_list[i].content, type_str, status_str, appeal_list[i].feedback);
    }
    fclose(fp);
}

// 从文件加载申诉记录数据
int load_appeal(Appeal* appeal_list, int max_count) {
    if (appeal_list == NULL || max_count <= 0) return 0;
    FILE* fp = fopen("./data/appeals.dat", "r");
    if (!fp) return 0;

    char line[500] = { 0 }, type_str[10] = { 0 }, status_str[10] = { 0 };
    int count = 0;
    // 跳过表头和分隔线
    if (!fgets(line, 500, fp)) { fclose(fp); return 0; }
    if (!fgets(line, 500, fp)) { fclose(fp); return 0; }

    while (count < max_count) {
        Appeal temp = { 0 };
        if (fscanf(fp, "%11s %39[^\n] %9s %9s %39[^\n]",
            temp.stu_id, temp.content, type_str, status_str, temp.feedback) != 5) {
            break;
        }
        // 转换类型和状态
        temp.type = strcmp(type_str, "成绩申诉") == 0 ? 1 : 2;
        temp.is_processed = strcmp(status_str, "已处理") == 0 ? 1 : 0;
        appeal_list[count] = temp;
        count++;
        clear_buffer(); // 清空输入缓冲区
    }
    fclose(fp);
    return count;
}

// 导出用户数据到CSV文件
void export_users(User* user_list, int count) {
    if (user_list == NULL || count <= 0) return;
    FILE* fp = fopen("./data/users_export.csv", "w");
    if (!fp) {
        printf("导出用户失败！\n");
        return;
    }
    fprintf(fp, "账号,密码,用户类型\n");
    for (int i = 0; i < count; i++) {
        char type_str[10] = { 0 };
        switch (user_list[i].type) {
        case 1: strcpy(type_str, "管理员"); break;
        case 2: strcpy(type_str, "教师"); break;
        case 3: strcpy(type_str, "学生"); break;
        }
        fprintf(fp, "%s,%s,%s\n", user_list[i].account, user_list[i].pwd, type_str);
    }
    fclose(fp);
    printf("用户已导出到 ./data/users_export.csv\n");
}

// 从CSV文件导入用户数据（去重）
int import_users(User* user_list, int* count) {
    if (user_list == NULL || count == NULL || *count >= 100) return 0;
    FILE* fp = fopen("./data/users_export.csv", "r");
    if (!fp) {
        printf("导入用户失败：文件不存在！\n");
        return 0;
    }

    char line[200] = { 0 };
    int import_count = 0;
    // 跳过表头
    if (!fgets(line, 200, fp)) { fclose(fp); return 0; }

    while (*count < 100) {
        User temp = { 0 };
        char type_str[10] = { 0 };
        if (fscanf(fp, "%19[^,],%19[^,],%9s", temp.account, temp.pwd, type_str) != 3) {
            break;
        }
        // 检查账号是否重复
        int exists = 0;
        for (int i = 0; i < *count; i++) {
            if (strcmp(user_list[i].account, temp.account) == 0) {
                exists = 1;
                break;
            }
        }
        if (exists) {
            clear_buffer();
            continue;
        }
        // 转换用户类型
        if (strcmp(type_str, "管理员") == 0) temp.type = 1;
        else if (strcmp(type_str, "教师") == 0) temp.type = 2;
        else if (strcmp(type_str, "学生") == 0) temp.type = 3;
        else continue;
        // 添加到用户列表
        user_list[*count] = temp;
        (*count)++;
        import_count++;
        clear_buffer();
    }
    fclose(fp);
    save_user(user_list, *count); // 同步保存到文件
    return import_count;
}