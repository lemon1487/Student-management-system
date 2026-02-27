#include "../include/common.h"
#include "../include/admin_func.h"

// 系统主入口（初始化+核心菜单+登录鉴权）
int main() {
    // 自动创建data目录（跨平台兼容，避免文件操作失败）
#ifdef _WIN32
    system("md data 2>nul"); // Windows，2>nul忽略重复创建的错误提示
#else
    system("mkdir -p data"); // Linux/macOS，-p确保多级目录创建
#endif

    // 初始化核心数据数组（替换为common.h的MAX_*宏）
    User user_list[MAX_USER] = { 0 };         // 用户列表（管理员/教师/学生）
    Student stu_list[MAX_STUDENT] = { 0 };    // 学生成绩列表
    Appeal appeal_list[MAX_APPEAL] = { 0 };   // 申诉列表（密码找回/成绩申诉）
    Todo todo_list[MAX_TODO] = { 0 };         // 管理员待办列表

    // 从文件加载数据（替换为common.h的MAX_*宏）
    int user_count = load_user(user_list, MAX_USER);
    int stu_count = load_student(stu_list, MAX_STUDENT);
    int appeal_count = load_appeal(appeal_list, MAX_APPEAL);
    int todo_count = load_todo(todo_list, MAX_TODO);

    // 强制初始化默认账号（确保首次运行有基础登录账号）
    if (user_count == 0) {
        // 管理员：admin/123456（替换为common.h的ADMIN/TEACHER/STUDENT宏）
        strcpy(user_list[0].account, "admin");
        strcpy(user_list[0].pwd, "123456");
        user_list[0].type = ADMIN;
        // 教师：teacher/123456
        strcpy(user_list[1].account, "teacher");
        strcpy(user_list[1].pwd, "123456");
        user_list[1].type = TEACHER;
        // 学生：student/123456
        strcpy(user_list[2].account, "student");
        strcpy(user_list[2].pwd, "123456");
        user_list[2].type = STUDENT;

        user_count = 3;
        save_user(user_list, user_count); // 保存默认账号到文件
        printf("已创建默认账号：\n");
        printf("管理员：admin/123456\n");
        printf("教师：teacher/123456\n");
        printf("学生：student/123456\n");
    }

    int choice;
    while (1) { // 系统主循环（退出时break）
        printf("\n========== 学生成绩管理系统 ==========\n");
        printf("1. 管理员登录\n");
        printf("2. 教师登录\n");
        printf("3. 学生登录\n");
        printf("4. 用户注册\n");
        printf("5. 密码找回申请\n");
        printf("0. 退出系统\n");
        printf("=====================================\n");
        printf("请输入您的选择：");

        // 输入校验：确保输入是数字
        if (scanf("%d", &choice) != 1) {
            clear_buffer(); // 清空输入缓冲区
            printf("输入错误！请输入数字！\n");
            continue;
        }
        clear_buffer();

        // 退出系统：保存所有数据后退出
        if (choice == 0) {
            save_user(user_list, user_count);
            save_student(stu_list, stu_count);
            save_appeal(appeal_list, appeal_count);
            save_todo(todo_list, todo_count);
            printf("感谢使用，再见！\n");
            break;
        }

        // 注册功能
        if (choice == 4) {
            register_user(user_list, &user_count);
            continue;
        }

        // 密码找回申请
        if (choice == 5) {
            char account[MAX_ACCOUNT_LEN] = { 0 }; // 使用common.h的MAX_ACCOUNT_LEN宏
            printf("请输入需要找回密码的账号：");
            scanf("%19s", account); // 19是MAX_ACCOUNT_LEN-1，避免越界（MAX_ACCOUNT_LEN=20）
            clear_buffer();
            // 检查账号是否存在
            int exists = 0;
            for (int i = 0; i < user_count; i++) {
                if (strcmp(user_list[i].account, account) == 0) {
                    exists = 1;
                    break;
                }
            }
            if (!exists) {
                printf("账号不存在！\n");
                continue;
            }
            // 提交密码找回申诉并生成待办
            submit_find_pwd_appeal(appeal_list, &appeal_count, todo_list, &todo_count, account);
            continue;
        }

        // 登录逻辑：账号密码验证
        char account[MAX_ACCOUNT_LEN] = { 0 }; // 使用common.h的MAX_ACCOUNT_LEN宏
        char pwd[MAX_PWD_LEN] = { 0 };         // 使用common.h的MAX_PWD_LEN宏
        printf("请输入账号：");
        scanf("%19s", account); // MAX_ACCOUNT_LEN-1=19，避免缓冲区溢出
        printf("请输入密码：");
        scanf("%19s", pwd);     // MAX_PWD_LEN-1=19，避免缓冲区溢出
        clear_buffer();

        int login_success = 0;
        int user_idx = -1;
        // 遍历用户列表验证账号密码
        for (int i = 0; i < user_count; i++) {
            if (strcmp(user_list[i].account, account) == 0 &&
                strcmp(user_list[i].pwd, pwd) == 0) {
                login_success = 1;
                user_idx = i;
                break;
            }
        }

        if (!login_success) {
            printf("账号或密码错误！\n");
            continue;
        }

        // 登录成功，根据用户类型进入对应菜单（替换为common.h的ADMIN/TEACHER/STUDENT宏）
        printf("登录成功！\n");
        int user_type = user_list[user_idx].type;
        if (user_type == ADMIN) {
            // 管理员菜单（最高权限）
            admin_menu(stu_list, &stu_count, user_list, &user_count,
                appeal_list, &appeal_count, todo_list, &todo_count);
        }
        else if (user_type == TEACHER) {
            // 教师菜单（学生成绩管理）
            teacher_menu(stu_list, &stu_count, user_list, user_count, account);
        }
        else if (user_type == STUDENT) {
            // 学生菜单（成绩查询/申诉）
            student_menu(stu_list, stu_count, user_list, user_count,
                account, appeal_list, &appeal_count, todo_list, &todo_count);
        }
        else {
            // 兜底逻辑：异常类型默认赋予学生权限，避免系统崩溃
            printf("警告：账号类型异常（值：%d），已临时设为学生权限！\n", user_type);
            student_menu(stu_list, stu_count, user_list, user_count,
                account, appeal_list, &appeal_count, todo_list, &todo_count);
        }
    }

    return 0;
}