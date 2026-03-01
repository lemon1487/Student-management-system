#include "../include/common.h"
#include "../include/file_func.h"

// 清空输入缓冲区（解决scanf后回车残留问题）
void clear_buffer() {
    while (getchar() != '\n');
}

// 校验成绩是否在0-100之间
int check_score(float score) {
    return (score >= 0 && score <= 100);
}

// 计算学生总分和平均分
void calc_student_score(Student* stu) {
    if (stu == NULL) return;
    stu->total = stu->math + stu->chinese + stu->english;
    stu->avg = stu->total / 3;
}

// 成绩排序（field:1-数学 2-语文 3-英语 4-总分；type:0-升序 1-降序）
void sort_students(Student* stu_list, int count, int field, int type) {
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
            if ((type == SORT_ASC && val1 > val2) || (type == SORT_DESC && val1 < val2)) {
                Student temp = stu_list[j];
                stu_list[j] = stu_list[j + 1];
                stu_list[j + 1] = temp;
            }
        }
    }
}
//用户注册功能
void register_user(UserNode** user_head, int* user_count, Appeal* appeal_list, int* appeal_count, Todo* todo_list, int* todo_count) {
    if (*user_count >= MAX_USER) {
        printf("用户数量已达上限！\n");
        return;
    }
    if (*appeal_count >= MAX_APPEAL) {
        printf("申诉数量已达上限！\n");
        return;
    }
    if (*todo_count >= MAX_TODO) {
        printf("待办数量已达上限！\n");
        return;
    }

    char account[MAX_ACCOUNT_LEN] = { 0 }, pwd[MAX_PWD_LEN] = { 0 };
    int reg_type;
    printf("\n===== 用户注册 =====\n");
    printf("请输入注册账号（学号/工号）：");
    scanf("%19s", account);
    clear_buffer();

    // 检查账号是否已存在
    UserNode* p = *user_head;
    while (p != NULL) {
        if (strcmp(p->data.account, account) == 0) {
            printf("该账号已被注册！\n");
            return;
        }
        p = p->next;
    }

    printf("请输入密码：");
    scanf("%19s", pwd);
    clear_buffer();

    // 身份选择
    printf("请选择注册身份：\n");
    printf("1. 管理员\n");
    printf("2. 教师\n");
    printf("3. 学生\n");
    printf("请选择：");
    while (scanf("%d", &reg_type) != 1 || reg_type < 1 || reg_type > 3) {
        clear_buffer();
        printf("输入错误！请输入1-3：");
    }
    clear_buffer();

    char* type_str;
    switch (reg_type) {
    case 1: type_str = "管理员"; break;
    case 2: type_str = "教师"; break;
    case 3: type_str = "学生"; break;
    }

    Appeal new_appeal;
    strcpy(new_appeal.stu_id, account);
    sprintf(new_appeal.content, "注册新用户，身份：%s，初始密码：%s", type_str, pwd);
    new_appeal.type = 3; // 3-注册申诉
    new_appeal.is_processed = 0;
    memset(new_appeal.feedback, 0, MAX_CONTENT_LEN);
    appeal_list[*appeal_count] = new_appeal;
    (*appeal_count)++;
    save_appeal(appeal_list, *appeal_count);

    // 同步添加管理员待办
    Todo new_todo;
    sprintf(new_todo.content, "审核用户注册申请：%s（身份：%s）", account, type_str);
    new_todo.is_done = 0;
    todo_list[*todo_count] = new_todo;
    (*todo_count)++;
    save_todo(todo_list, *todo_count);

    printf("注册申请已提交（身份：%s），请等待管理员审核！\n", type_str);
}
//密码找回
void find_password(UserNode* user_head, int user_count, Appeal* appeal_list, int* appeal_count, Todo* todo_list, int* todo_count) {
    if (*appeal_count >= MAX_APPEAL) {
        printf("申诉数量已达上限！\n");
        return;
    }
    if (*todo_count >= MAX_TODO) {
        printf("待办数量已达上限！\n");
        return;
    }

    char account[MAX_ACCOUNT_LEN] = { 0 };
    printf("\n===== 密码找回 =====\n");
    printf("请输入原账号：");
    scanf("%19s", account);
    clear_buffer();

    // 检查账号是否存在
    UserNode* p = user_head;
    int found = 0;
    while (p != NULL) {
        if (strcmp(p->data.account, account) == 0) {
            found = 1;
            break;
        }
        p = p->next;
    }
    if (!found) {
        printf("该账号不存在！\n");
        return;
    }

    // 1. 提交密码找回申诉
    Appeal new_appeal;
    strcpy(new_appeal.stu_id, account);
    strcpy(new_appeal.content, "申请密码找回");
    new_appeal.type = 2;
    new_appeal.is_processed = 0;
    memset(new_appeal.feedback, 0, MAX_CONTENT_LEN);
    appeal_list[*appeal_count] = new_appeal;
    (*appeal_count)++;
    save_appeal(appeal_list, *appeal_count);

    // 2. 同步添加管理员待办
    Todo new_todo;
    sprintf(new_todo.content, "处理密码找回申请：%s", account);
    new_todo.is_done = 0;
    todo_list[*todo_count] = new_todo;
    (*todo_count)++;
    save_todo(todo_list, *todo_count);

    printf("密码找回申请已提交，管理员处理后密码将重置为123456！\n");
}
//计算学生排名
int get_student_rank(StudentNode* stu_head, int stu_count, char* stu_id, int field) {
    if (stu_head == NULL || stu_count == 0) return -1;

    Student temp_stu[stu_count];
    StudentNode* p = stu_head;
    int i = 0;
    while (p != NULL && i < stu_count) {
        temp_stu[i] = p->data;
        p = p->next;
        i++;
    }

    for (i = 0; i < stu_count - 1; i++) {
        for (int j = 0; j < stu_count - i - 1; j++) {
            float val1, val2;
            switch (field) {
            case 1: val1 = temp_stu[j].math; val2 = temp_stu[j + 1].math; break;
            case 2: val1 = temp_stu[j].chinese; val2 = temp_stu[j + 1].chinese; break;
            case 3: val1 = temp_stu[j].english; val2 = temp_stu[j + 1].english; break;
            case 4: val1 = temp_stu[j].total; val2 = temp_stu[j + 1].total; break;
            default: return -1;
            }
            if (val1 < val2) {
                Student temp = temp_stu[j];
                temp_stu[j] = temp_stu[j + 1];
                temp_stu[j + 1] = temp;
            }
        }
    }

    for (i = 0; i < stu_count; i++) {
        if (strcmp(temp_stu[i].id, stu_id) == 0) {
            return i + 1;
        }
    }
    return -1;
}