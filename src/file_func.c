#include "../include/file_func.h"

// 保存用户
void save_user(UserNode* head, int count) {
    system("md data 2>nul");
    FILE* fp = fopen("./data/users.dat", "w");
    if (!fp) {
        printf("保存用户数据失败！\n");
        return;
    }

    UserNode* p = head;
    while (p != NULL) {
        fprintf(fp, "%s %s %d\n", p->data.account, p->data.pwd, p->data.type);
        p = p->next;
    }

    fclose(fp);
}

// 加载用户数据
int load_user(UserNode** head, int* count) {
    // 第一步：彻底释放旧链表，避免残留数据
    UserNode* curr = *head;
    while (curr != NULL) {
        UserNode* tmp = curr;
        curr = curr->next;
        free(tmp);
    }
    *head = NULL;
    *count = 0;

    // 第二步：打开文件
    FILE* fp = fopen("./data/users.dat", "r");
    if (!fp) {
        printf("暂无用户数据，初始化默认管理员账号...\n");

        UserNode* new_node = (UserNode*)malloc(sizeof(UserNode));
        strcpy(new_node->data.account, "admin");
        strcpy(new_node->data.pwd, "123456");
        new_node->data.type = ADMIN;
        new_node->next = NULL;

        *head = new_node;
        *count = 1;

        save_user(*head, *count);
        return 1;
    }

    char account[21], pwd[21];
    int type;
    UserNode* tail = NULL;

    while (fscanf(fp, "%20s %20s %d", account, pwd, &type) == 3) {
        UserNode* new_node = (UserNode*)malloc(sizeof(UserNode));
        if (!new_node) {
            printf("内存分配失败！\n");
            fclose(fp);
            return *count;
        }

        strncpy(new_node->data.account, account, MAX_ACCOUNT_LEN);
        new_node->data.account[MAX_ACCOUNT_LEN - 1] = '\0';
        strncpy(new_node->data.pwd, pwd, MAX_PWD_LEN);
        new_node->data.pwd[MAX_PWD_LEN - 1] = '\0';
        new_node->data.type = type;
        new_node->next = NULL;

        if (*head == NULL) {
            *head = new_node;
            tail = new_node;
        }
        else {
            tail->next = new_node;
            tail = new_node;
        }
        (*count)++;
    }

    fclose(fp);
    return *count;
}

// 保存学生成绩数据
void save_student(StudentNode* head, int count) {
    system("md data 2>nul");
    FILE* fp = fopen("./data/students.dat", "w");
    if (!fp) {
        printf("保存学生数据失败！\n");
        return;
    }
    StudentNode* p = head;
    while (p != NULL) {
        // 用引号包裹带空格的字符串，或者用\t分隔
        fprintf(fp, "%s\t%s\t%s\t%.2f\t%.2f\t%.2f\t%.2f\t%.2f\n",
            p->data.id, p->data.name, p->data.class,
            p->data.math, p->data.chinese, p->data.english,
            p->data.avg, p->data.total);
        p = p->next;
    }
    fclose(fp);
}

// 加载学生成绩数据
int load_student(StudentNode** head, int* count) {
    StudentNode* curr = *head;
    while (curr != NULL) {
        StudentNode* tmp = curr;
        curr = curr->next;
        free(tmp);
    }
    *head = NULL;
    *count = 0;

    FILE* fp = fopen("./data/students.dat", "r");
    if (!fp) {
        printf("暂无学生数据！\n");
        return 0;
    }

    char id[13], name[21], cls[21];
    float math, chinese, english, avg, total;
    StudentNode* tail = NULL;

    // 用\t分隔符读取，支持带空格的名字
    while (fscanf(fp, "%12[^\t]\t%20[^\t]\t%20[^\t]\t%f\t%f\t%f\t%f\t%f\n",
        id, name, cls, &math, &chinese, &english, &avg, &total) == 8) {
        StudentNode* new_node = (StudentNode*)malloc(sizeof(StudentNode));
        if (!new_node) {
            printf("内存分配失败！\n");
            fclose(fp);
            return *count;
        }
        strncpy(new_node->data.id, id, MAX_ID_LEN);
        new_node->data.id[MAX_ID_LEN - 1] = '\0';
        strncpy(new_node->data.name, name, MAX_NAME_LEN);
        new_node->data.name[MAX_NAME_LEN - 1] = '\0';
        strncpy(new_node->data.class, cls, MAX_CLASS_LEN);
        new_node->data.class[MAX_CLASS_LEN - 1] = '\0';
        new_node->data.math = math;
        new_node->data.chinese = chinese;
        new_node->data.english = english;
        new_node->data.avg = avg;
        new_node->data.total = total;
        new_node->next = NULL;

        if (*head == NULL) {
            *head = new_node;
            tail = new_node;
        }
        else {
            tail->next = new_node;
            tail = new_node;
        }
        (*count)++;
    }

    fclose(fp);
    return *count;
}

// 保存待办事项
void save_todo(Todo* todo_list, int count) {
    system("md data 2>nul");
    FILE* fp = fopen("./data/todo.dat", "w");
    if (!fp) {
        printf("保存待办数据失败！\n");
        return;
    }
    for (int i = 0; i < count; i++) {
        fprintf(fp, "%s %d\n", todo_list[i].content, todo_list[i].is_done);
    }
    fclose(fp);
}

// 加载待办事项
int load_todo(Todo* todo_list, int max_count) {
    FILE* fp = fopen("./data/todo.dat", "r");
    if (!fp) {
        return 0;
    }
    int count = 0;
    char content[100];
    int is_done;
    while (fscanf(fp, "%99s %d", content, &is_done) == 2 && count < max_count) {
        strcpy(todo_list[count].content, content);
        todo_list[count].is_done = is_done;
        count++;
    }
    fclose(fp);
    return count;
}

// 保存申诉数据
void save_appeal(Appeal* appeal_list, int count) {
    system("md data 2>nul");
    FILE* fp = fopen("./data/appeal.dat", "w");
    if (!fp) {
        printf("保存申诉数据失败！\n");
        return;
    }
    for (int i = 0; i < count; i++) {
        fprintf(fp, "%s %s %d %d %s\n",
            appeal_list[i].stu_id, appeal_list[i].content,
            appeal_list[i].type, appeal_list[i].is_processed,
            appeal_list[i].feedback);
    }
    fclose(fp);
}

// 加载申诉数据
int load_appeal(Appeal* appeal_list, int max_count) {
    FILE* fp = fopen("./data/appeal.dat", "r");
    if (!fp) {
        return 0;
    }
    int count = 0;
    char stu_id[12], content[100], feedback[100];
    int type, is_processed;
    while (fscanf(fp, "%11s %99s %d %d %99s", stu_id, content, &type, &is_processed, feedback) == 5 && count < max_count) {
        strcpy(appeal_list[count].stu_id, stu_id);
        strcpy(appeal_list[count].content, content);
        appeal_list[count].type = type;
        appeal_list[count].is_processed = is_processed;
        strcpy(appeal_list[count].feedback, feedback);
        count++;
    }
    fclose(fp);
    return count;
}

// 导出用户数据为CSV
void export_users(UserNode* head, int count) {
    system("md data 2>nul");
    FILE* fp = fopen("./data/users_export.csv", "w");
    if (!fp) {
        printf("导出用户失败！\n");
        return;
    }
    fprintf(fp, "账号,密码,类型\n");
    UserNode* p = head;
    while (p != NULL) {
        char type_str[10] = { 0 };
        switch (p->data.type) {
        case ADMIN: strcpy(type_str, "管理员"); break;
        case TEACHER: strcpy(type_str, "教师"); break;
        case STUDENT: strcpy(type_str, "学生"); break;
        }
        fprintf(fp, "%s,%s,%s\n", p->data.account, p->data.pwd, type_str);
        p = p->next;
    }
    fclose(fp);
    printf("用户数据已导出到 ./data/users_export.csv\n");
}

// 从CSV导入用户数据
int import_users(UserNode** head, int* count) {
    FILE* fp = fopen("./data/users_export.csv", "r");
    if (!fp) {
        printf("导入文件不存在！\n");
        return 0;
    }

    char line[100], account[20], pwd[20], type_str[10];
    fgets(line, 100, fp); // 跳过表头
    while (fscanf(fp, "%[^,],%[^,],%s", account, pwd, type_str) == 3) {
        int type = 0;
        if (strcmp(type_str, "管理员") == 0) type = ADMIN;
        else if (strcmp(type_str, "教师") == 0) type = TEACHER;
        else if (strcmp(type_str, "学生") == 0) type = STUDENT;
        else continue;

        UserNode* new_node = (UserNode*)malloc(sizeof(UserNode));
        strcpy(new_node->data.account, account);
        strcpy(new_node->data.pwd, pwd);
        new_node->data.type = type;
        new_node->next = *head;
        *head = new_node;
        (*count)++;
    }
    fclose(fp);
    save_user(*head, *count); // 保存导入的数据
    printf("用户导入成功！\n");
    return 1;
}