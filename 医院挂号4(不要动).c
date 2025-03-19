#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define MAX_DOCTORS 2
#define MAX_DEPARTMENTS 6

typedef struct {
    char name[50];
    int age;
    char title[30];      // 职称
    char specialty[50];  // 具体专业
    float fee;           // 挂号费
    int appointments[7][2]; 
    char reviews[3][100]; // 三条患者评价
} Doctor;

typedef struct {
    char name[50];
    Doctor doctors[MAX_DOCTORS];
    int doctorCount;
} Department;

Department departments[MAX_DEPARTMENTS];
int departmentCount = 0;

typedef struct {
    char patientName[50];
    char phoneNumber[20];
    char deptName[50];
    char docName[50];
    int dayOfWeek;
    int timeOfDay;
} AppointmentRecord;

AppointmentRecord appointmentRecords[100];
int recordCount = 0;

// 初始化数据
// 函数声明
void initDepartments();
void showMenu(int mode);
void showDepartmentsAndMakeAppointment();
void viewAppointments();
void makeAppointment(char patientName[], char phoneNumber[], int deptChoice, int docChoice, int dayOfWeek, int timeOfDay);
void deleteDoctor();
void addDoctor(); // 添加这个行来声明addDoctor函数
void modifyDoctor();

// 初始化数据
void initDepartments() {
    char *deptNames[MAX_DEPARTMENTS] = {
        "内科-消化科", "内科-心血管科", "内科-呼吸科",
        "外科-骨科", "外科-普外科", "外科-神经外科"
    };

    char *specialties[6][3] = {
        {"胃肠疾病", "肝胆疾病", "消化道肿瘤"},       // 消化科
        {"冠心病", "心律失常", "心力衰竭"},          // 心血管科
        {"肺炎", "哮喘", "慢性阻塞性肺疾病"},        // 呼吸科
        {"骨折", "关节置换", "脊柱外科"},            // 骨科
        {"阑尾炎", "疝气", "胃肠肿瘤"},              // 普外科
        {"脑肿瘤", "脑血管病", "神经创伤"}           // 神经外科
    };

    char *surnames[] = {"张", "王", "李", "赵", "陈", "刘"};
    char *titles[] = {"主任医师", "副主任医师", "主治医师"};
    float fees[] = {50.0, 30.0, 15.0};
    char *reviewPool[] = {
        "医术精湛，诊断准确",
        "耐心细致，解答清楚",
        "用药效果显著，推荐",
        "候诊时间稍长但值得",
        "对待患者非常亲切"
    };

    srand(time(NULL));
    
    for (int i = 0; i < MAX_DEPARTMENTS; i++) {
        strcpy(departments[i].name, deptNames[i]);
        
        for (int j = 0; j < MAX_DOCTORS; j++) {
            Doctor *doc = &departments[i].doctors[j];
            
            sprintf(doc->name, "%s%s", surnames[rand()%6], (j == 0) ? "主任" : "医生");
            strcpy(doc->specialty, specialties[i][rand()%3]);
            int titleLevel = (j == 0) ? 0 : (1 + rand()%2);
            strcpy(doc->title, titles[titleLevel]);
            doc->fee = fees[titleLevel];
            doc->age = 35 + titleLevel * 5 + rand()%5;
            
            // 初始化所有时间段为不可预约
            for(int d=0; d<7; d++){
                for(int t=0; t<2; t++){
                    doc->appointments[d][t] = 0;
                }
            }
            
            // 随机设置5个可预约时段
            int available = 0;
            while(available < 5) {
                int day = rand()%7;
                int time = rand()%2;
                if(doc->appointments[day][time] == 0){
                    doc->appointments[day][time] = 5;
                    available++;
                }
            }
            
            for(int k=0; k<3; k++){
                strcpy(doc->reviews[k], reviewPool[rand()%5]);
            }
        }
        departments[i].doctorCount = MAX_DOCTORS;
    }
    departmentCount = MAX_DEPARTMENTS;
}

// 显示科室列表并提供预约选项
void showDepartmentsAndMakeAppointment() {
    printf("\n=== 科室列表 ===\n");
    for(int i=0; i<departmentCount; i++){
        printf("%d. %s\n", i+1, departments[i].name);
    }
    
    int choice;
    printf("请选择科室查看专家（0返回）: ");
    scanf("%d", &choice);
    
    if(choice >0 && choice <= departmentCount){
        printf("\n=== %s专家列表 ===\n", departments[choice-1].name);
        for(int j=0; j<departments[choice-1].doctorCount; j++){
            Doctor doc = departments[choice-1].doctors[j];
            printf("%d. %s %s（挂号费：%.1f元）\n", 
                  j+1, doc.name, doc.title, doc.fee);
        }
        
        int docChoice;
        printf("选择医生查看详情或预约（0返回）: ");
        scanf("%d", &docChoice);
        
        if(docChoice >0 && docChoice <= departments[choice-1].doctorCount){
            Doctor doc = departments[choice-1].doctors[docChoice-1];
            printf("\n=== 医生详情 ===\n");
            printf("姓名：%s\n职称：%s\n专业：%s\n", doc.name, doc.title, doc.specialty);
            printf("挂号费：%.1f元\n", doc.fee);
            
            printf("\n可预约时间：\n");
            char *days[] = {"周一","周二","周三","周四","周五","周六","周日"};
            int hasAvailable = 0;
            for(int d=0; d<7; d++){
                for(int t=0; t<2; t++){
                    if(doc.appointments[d][t] >0){
                        printf("%s %s：剩余%d号\n", days[d], t==0 ? "上午" : "下午", doc.appointments[d][t]);
                        hasAvailable = 1;
                    }
                }
            }
            if(!hasAvailable) printf("本周暂无可用号源\n");
            
            printf("\n患者评价：\n");
            for(int k=0; k<3; k++){
                printf("· %s\n", doc.reviews[k]);
            }

            // 提供预约选项
            char patientName[50], phoneNumber[20];
            printf("请输入您的姓名: ");
            scanf("%s", patientName);
            printf("请输入您的电话号码: ");
            scanf("%s", phoneNumber);
            int dayOfWeek, timeOfDay;
            printf("是否现在预约？(y/n): ");
            char confirm;
            scanf(" %c", &confirm); // 注意前面有一个空格来吃掉换行符
            
            if(confirm == 'y' || confirm == 'Y'){
                printf("选择预约日期(星期几, 输入1-7): ");
                scanf("%d", &dayOfWeek);
                printf("选择时间段(上午输入0, 下午输入1): ");
                scanf("%d", &timeOfDay);

                makeAppointment(patientName, phoneNumber, choice, docChoice, dayOfWeek, timeOfDay);
            }
        }
    }
}

// 修改makeAppointment函数签名以接受更多参数
void makeAppointment(char patientName[], char phoneNumber[], int deptChoice, int docChoice, int dayOfWeek, int timeOfDay) {
    // 检查是否已有相同预约
    for (int i = 0; i < recordCount; i++) {
        if (strcmp(appointmentRecords[i].patientName, patientName) == 0 &&
            strcmp(appointmentRecords[i].docName, departments[deptChoice - 1].doctors[docChoice - 1].name) == 0 &&
            appointmentRecords[i].dayOfWeek == dayOfWeek &&
            appointmentRecords[i].timeOfDay == timeOfDay) {
            printf("您已经在相同的时间段预约了该医生，请选择其他时间或医生。\n");
            return;
        }
    }

    if (departments[deptChoice - 1].doctors[docChoice - 1].appointments[dayOfWeek - 1][timeOfDay] == 0) {
        printf("对不起，该时间段已满。\n");
    } else {
        departments[deptChoice - 1].doctors[docChoice - 1].appointments[dayOfWeek - 1][timeOfDay]--;
        strcpy(appointmentRecords[recordCount].patientName, patientName);
        strcpy(appointmentRecords[recordCount].phoneNumber, phoneNumber);
        strcpy(appointmentRecords[recordCount].deptName, departments[deptChoice - 1].name);
        strcpy(appointmentRecords[recordCount].docName, departments[deptChoice - 1].doctors[docChoice - 1].name);
        appointmentRecords[recordCount].dayOfWeek = dayOfWeek;
        appointmentRecords[recordCount].timeOfDay = timeOfDay;
        recordCount++;
        printf("预约成功!\n");
    }
}

void viewAppointments() {
    if (recordCount == 0) {
        printf("没有预约记录。\n");
    } else {
        for (int i = 0; i < recordCount; i++) {
            printf("预约人: %s, 联系方式: %s, 科室: %s, 医生: %s, 时间: 星期%d %s\n",
                   appointmentRecords[i].patientName,
                   appointmentRecords[i].phoneNumber,
                   appointmentRecords[i].deptName,
                   appointmentRecords[i].docName,
                   appointmentRecords[i].dayOfWeek,
                   appointmentRecords[i].timeOfDay == 0 ? "上午" : "下午");
        }
    }
}

void deleteDoctor() {
    printf("\n=== 删除医生信息 ===\n");
    for (int i = 0; i < departmentCount; i++) {
        printf("%d. %s\n", i + 1, departments[i].name);
    }
    
    int deptChoice;
    printf("请选择要操作的科室(输入0返回): ");
    scanf("%d", &deptChoice);
    
    if(deptChoice > 0 && deptChoice <= departmentCount){
        Department *dept = &departments[deptChoice - 1];
        
        printf("\n=== %s专家列表 ===\n", dept->name);
        for(int j=0; j<dept->doctorCount; j++){
            printf("%d. %s\n", j+1, dept->doctors[j].name);
        }
        
        int docChoice;
        printf("选择要删除的医生编号: ");
        scanf("%d", &docChoice);
        
        if(docChoice > 0 && docChoice <= dept->doctorCount){
            // 将该医生的所有预约也一并删除
            for(int i=recordCount-1; i>=0; i--){
                if(strcmp(appointmentRecords[i].docName, dept->doctors[docChoice-1].name) == 0){
                    // 移动后面的记录向前覆盖
                    for(int j=i; j<recordCount-1; j++){
                        appointmentRecords[j] = appointmentRecords[j+1];
                    }
                    recordCount--;
                }
            }

            // 将最后一个医生的信息移动到被删除医生的位置
            dept->doctors[docChoice-1] = dept->doctors[dept->doctorCount-1];
            dept->doctorCount--;

            printf("医生信息删除成功。\n");
        } else {
            printf("无效的选择。\n");
        }
    }
}

void showMenu(int mode) {
    int choice;
    do {
        printf("\n1. 查看科室并预约\n2. 查看预约记录\n");
        if(mode == 1) {
            printf("3. 修改医生信息\n4. 删除医生信息\n5. 添加医生信息\n");
        }
        printf("0. 返回上级菜单\n请选择: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                showDepartmentsAndMakeAppointment();
                break;
            case 2:
                viewAppointments();
                break;
            case 3:
                if(mode == 1) {
                    modifyDoctor();
                } else {
                    printf("无效的选择\n");
                }
                break;
            case 4:
                if(mode == 1) {
                    deleteDoctor();
                } else {
                    printf("无效的选择\n");
                }
                break;
            case 5:
                if(mode == 1) {
                    addDoctor();
                } else {
                    printf("无效的选择\n");
                }
                break;
            case 0:
                break;
            default:
                printf("无效的选择\n");
        }
    } while (choice != 0);
}
void modifyDoctor() {
    printf("\n=== 修改医生信息 ===\n");
    
    // 选择科室
    printf("请选择需要修改的医生所属科室:\n");
    for (int i = 0; i < departmentCount; i++) {
        printf("%d. %s\n", i+1, departments[i].name);
    }
    int deptChoice;
    printf("输入科室编号（0返回）: ");
    scanf("%d", &deptChoice);
    if (deptChoice < 1 || deptChoice > departmentCount) return;

    Department *dept = &departments[deptChoice-1];
    
    // 选择医生
    printf("\n=== %s专家列表 ===\n", dept->name);
    for (int j = 0; j < dept->doctorCount; j++) {
        printf("%d. %s\n", j+1, dept->doctors[j].name);
    }
    int docChoice;
    printf("输入医生编号（0返回）: ");
    scanf("%d", &docChoice);
    if (docChoice < 1 || docChoice > dept->doctorCount) return;

    Doctor *doc = &dept->doctors[docChoice-1];
    
    int option;
    do {
        printf("\n请选择要修改的信息项:\n");
        printf("1.姓名 2.科室 3.职称 4.专业 5.挂号费 6.评价 0.返回\n选择: ");
        scanf("%d", &option);

        switch(option) {
            case 1: {
                printf("当前姓名: %s\n新姓名: ", doc->name);
                scanf("%s", doc->name);
                printf("姓名修改成功\n");
                break;
            }
            case 2: {
                printf("请选择新科室:\n");
                for (int i = 0; i < departmentCount; i++) {
                    printf("%d. %s\n", i+1, departments[i].name);
                }
                int newDeptChoice;
                printf("输入新科室编号（0取消）: ");
                scanf("%d", &newDeptChoice);
                
                if (newDeptChoice > 0 && newDeptChoice <= departmentCount) {
                    Department *newDept = &departments[newDeptChoice-1];
                    if (newDept->doctorCount >= MAX_DOCTORS) {
                        printf("目标科室已满\n");
                        break;
                    }
                    
                    // 迁移医生到新科室
                    newDept->doctors[newDept->doctorCount++] = *doc;
                    
                    // 从原科室删除
                    for (int j = docChoice-1; j < dept->doctorCount-1; j++) {
                        dept->doctors[j] = dept->doctors[j+1];
                    }
                    dept->doctorCount--;
                    
                    // 更新预约记录
                    for (int i = 0; i < recordCount; i++) {
                        if (strcmp(appointmentRecords[i].docName, doc->name) == 0) {
                            strcpy(appointmentRecords[i].deptName, newDept->name);
                        }
                    }
                    printf("科室修改成功\n");
                    return; // 科室结构已变更，需要退出函数
                }
                break;
            }
            case 3: {
                printf("当前职称: %s\n新职称: ", doc->title);
                scanf("%s", doc->title);
                printf("职称修改成功\n");
                break;
            }
            case 4: {
                printf("当前专业: %s\n新专业: ", doc->specialty);
                scanf("%s", doc->specialty);
                printf("专业修改成功\n");
                break;
            }
            case 5: {
                printf("当前挂号费: %.1f\n新挂号费: ", doc->fee);
                scanf("%f", &doc->fee);
                printf("挂号费修改成功\n");
                break;
            }
            case 6: {
                printf("当前评价:\n");
                for (int i = 0; i < 3; i++) {
                    printf("%d. %s\n", i+1, doc->reviews[i]);
                }
                int reviewChoice;
                printf("选择要修改的评价（1-3, 0取消）: ");
                scanf("%d", &reviewChoice);
                
                if (reviewChoice >= 1 && reviewChoice <= 3) {
                    printf("输入新评价: ");
                    getchar(); // 清除输入缓冲区
                    fgets(doc->reviews[reviewChoice-1], 100, stdin);
                    doc->reviews[reviewChoice-1][strcspn(doc->reviews[reviewChoice-1], "\n")] = 0; // 去除换行符
                    printf("评价修改成功\n");
                }
                break;
            }
            case 0:
                break;
            default:
                printf("无效的选择\n");
        }
    } while (option != 0);
}
void addDoctor() {
    printf("\n=== 添加医生信息 ===\n");
    for (int i = 0; i < departmentCount; i++) {
        printf("%d. %s\n", i + 1, departments[i].name);
    }
    
    int deptChoice;
    printf("请选择要添加医生的科室(输入0返回): ");
    scanf("%d", &deptChoice);

    if(deptChoice > 0 && deptChoice <= departmentCount){
        Department *dept = &departments[deptChoice - 1];
        
        if(dept->doctorCount >= MAX_DOCTORS) {
            printf("该科室已达最大医生数量，无法添加。\n");
            return;
        }

        Doctor newDoc;
        printf("请输入医生姓名: ");
        scanf("%s", newDoc.name);
        printf("请输入医生职称(主任医师/副主任医师/主治医师): ");
        scanf("%s", newDoc.title);
        printf("请输入医生专业领域: ");
        scanf("%s", newDoc.specialty);
        printf("请输入挂号费: ");
        scanf("%f", &newDoc.fee);
        newDoc.age = 35 + ((strcmp(newDoc.title, "主任医师") == 0) ? 0 : (strcmp(newDoc.title, "副主任医师") == 0) ? 5 : 10) + rand()%5;

        // 初始化所有时间段为不可预约
        for(int d=0; d<7; d++){
            for(int t=0; t<2; t++){
                newDoc.appointments[d][t] = 0;
            }
        }

        // 随机设置5个可预约时段
        int available = 0;
        while(available < 5) {
            int day = rand()%7;
            int time = rand()%2;
            if(newDoc.appointments[day][time] == 0){
                newDoc.appointments[day][time] = 5;
                available++;
            }
        }

        strcpy(newDoc.reviews[0], "暂无评价");
        strcpy(newDoc.reviews[1], "");
        strcpy(newDoc.reviews[2], "");

        dept->doctors[dept->doctorCount++] = newDoc;
        printf("医生信息添加成功。\n");
    } else if(deptChoice != 0) {
        printf("无效的选择。\n");
    }
}
int main() {
    int mode;
    int running = 1;

    initDepartments(); // 初始化数据

    while (running) {
        printf("\n选择模式 (0: 患者模式, 1: 开发者模式, 其他数字退出): ");
        int result = scanf("%d", &mode);
        if (result != 1) {
            // 输入非数字，清空输入缓冲区并退出
            while (getchar() != '\n');
            printf("退出程序。\n");
            break;
        }

        if (mode == 0 || mode == 1) {
            showMenu(mode); 
        } else {
            printf("退出程序。\n");
            running = 0;
        }
    }

    return 0;
}
