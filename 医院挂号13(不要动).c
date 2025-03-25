	#include <stdio.h>
	#include <stdlib.h>
	#include <string.h>
	#include <time.h>
	#include <ctype.h>
	
	
	#define MAX_DOCTORS 10      // 每个科室最多10位医生
	#define MAX_DEPARTMENTS 20  // 最大科室数20
// 修改为（根据实际需求调整）
#define MAX_DISEASES 1000  // 总疾病容量
#define MAX_DEPT_DISEASES 50  // 单个科室最大疾病数
	// 疾病-科室对应结构
	typedef struct {
	    char disease[50];
	    char department[50];
	} DiseaseMap;
	
	
// 疾病-科室对应结构
DiseaseMap diseaseDatabase[MAX_DISEASES] = {
    // 内科-消化科（10种）
    {"胃肠炎", "内科-消化科"},
    {"胃溃疡", "内科-消化科"},
    {"胃炎", "内科-消化科"},
    {"肝硬化", "内科-消化科"},
    {"消化道出血", "内科-消化科"},
    {"肠易激综合征", "内科-消化科"},

    // 内科-心血管科（10种）
    {"冠心病", "内科-心血管科"},
    {"心绞痛", "内科-心血管科"},
    {"心肌梗死", "内科-心血管科"},
    {"心律失常", "内科-心血管科"},
    {"高血压", "内科-心血管科"},
    {"动脉硬化", "内科-心血管科"},

    // 内科-呼吸科（10种）
    {"肺炎", "内科-呼吸科"},
    {"哮喘", "内科-呼吸科"},
    {"慢性阻塞性肺疾病", "内科-呼吸科"},
    {"肺结核", "内科-呼吸科"},
    {"肺气肿", "内科-呼吸科"},
    {"胸膜炎", "内科-呼吸科"},

    // 外科-骨科（10种）
    {"骨折", "外科-骨科"},
    {"骨质疏松", "外科-骨科"},
    {"腰椎间盘突出", "外科-骨科"},
    {"关节炎", "外科-骨科"},
    {"腱鞘炎", "外科-骨科"},
    {"颈椎病", "外科-骨科"},

    // 外科-普外科（10种）
    {"阑尾炎", "外科-普外科"},
    {"疝气", "外科-普外科"},
    {"胆囊结石", "外科-普外科"},
    {"肠梗阻", "外科-普外科"},
    {"胰腺癌", "外科-普外科"},
    {"甲状腺结节", "外科-普外科"},

    // 外科-神经外科（10种）
    {"脑肿瘤", "外科-神经外科"},
    {"脑出血", "外科-神经外科"},
    {"脑外伤", "外科-神经外科"},
    {"脑积水", "外科-神经外科"},
    {"帕金森病", "外科-神经外科"},
    {"三叉神经痛", "外科-神经外科"},

};

int diseaseCount = 36;  // 当前疾病数量（6个科室×10种疾病）


	
	// 医生结构
	typedef struct {
	    char name[50];
	    int age;
	    char title[30];      // 职称
	    char specialty[50]; // 专业方向
	    float fee;           // 挂号费
	    int appointments[7][2]; 
	    char reviews[3][100]; // 患者评价
	} Doctor;
	
	// 科室结构
	typedef struct {
	    char name[50];
	    Doctor doctors[MAX_DOCTORS];
	    int doctorCount;
	} Department;
	
	Department departments[MAX_DEPARTMENTS];
	int departmentCount = 0;
	
	// 预约记录结构
	typedef struct {
	    char patientName[50];
	    char phoneNumber[20];
	    char deptName[50];
	    char docName[50];
	    int dayOfWeek;
	    int timeOfDay;
	} AppointmentRecord;  // 补全结构体闭合
	
	AppointmentRecord appointmentRecords[100];
	int recordCount = 0;
	// 函数声明
	void initDepartments();
	void showMenu(int mode);
	void showDepartmentsAndMakeAppointment();
	void viewAppointments();
	void makeAppointment(char*,char*,int,int,int,int);
	void deleteDoctor();
	void addDoctor();
	void modifyDoctor();
	void searchDoctor();
	void searchByDisease(int);
	void diseaseManagement();
	void addDepartment();  // 新增函数声明
	void normalizeDepartment(const char *input, char *output);  // 新增声明
	void cancelAppointment(int index);

	
	void addDepartment() {
	    if (departmentCount >= MAX_DEPARTMENTS) {
	        printf("科室数量已达上限（%d）\n", MAX_DEPARTMENTS);
	        return;
	    }
	    
	    char name[50];
	    printf("请输入新科室名称: ");
	    scanf("%s", name);
	    
	    // 检查重复
	    for (int i = 0; i < departmentCount; i++) {
	        if (strcmp(departments[i].name, name) == 0) {
	            printf("该科室已存在\n");
	            return;
	        }
	    }
	    
	    strcpy(departments[departmentCount].name, name);
	    departments[departmentCount].doctorCount = 0;
	    departmentCount++;
	    printf("科室添加成功！\n");
	}

// 新增函数：科室名称标准化处理
void normalizeDepartment(const char *input, char *output) {
    char buffer[100];
    int j = 0;
    
    // 第一步：移除所有非中文字符和空格
    for (int i = 0; input[i] != '\0'; i++) {
        if (isalpha((unsigned char)input[i]) || 
            isdigit((unsigned char)input[i]) || 
            input[i] == ' ' || 
            input[i] == '-') {
            buffer[j++] = tolower(input[i]);
        }
    }
    buffer[j] = '\0';

    // 第二步：统一分隔符为单连字符
    j = 0;
    char prev = '\0';
    for (int i = 0; buffer[i] != '\0'; i++) {
        if (buffer[i] == ' ' || buffer[i] == '_' || buffer[i] == ',') {
            if (prev != '-') {
                output[j++] = '-';
                prev = '-';
            }
        } else {
            output[j++] = buffer[i];
            prev = buffer[i];
        }
    }
    output[j] = '\0';

    // 第三步：智能补全科室前缀
    const char *prefix = "内科";
    if (strstr(output, "外科") || 
        strstr(output, "骨科") || 
        strstr(output, "普外") || 
        strstr(output, "神经外科")) {
        prefix = "外科";
    }

    // 第四步：构造标准格式
    if (!strstr(output, "内科") && !strstr(output, "外科")) {
        char temp[100];
        if (strstr(output, "消化")) sprintf(temp, "%s-消化科", prefix);
        else if (strstr(output, "心血管")) sprintf(temp, "%s-心血管科", prefix);
        else if (strstr(output, "呼吸")) sprintf(temp, "%s-呼吸科", prefix);
        else if (strstr(output, "骨科")) strcpy(temp, "外科-骨科");
        else if (strstr(output, "普外")) strcpy(temp, "外科-普外科");
        else if (strstr(output, "神经外科")) strcpy(temp, "外科-神经外科");
        else {
            strcpy(temp, output); // 无法识别的保持原样
            return;
        }
        strcpy(output, temp);
    }

    // 第五步：最终格式校验
    if (output[0] == '-') memmove(output, output+1, strlen(output));
}


void viewAppointments() {
    if (recordCount == 0) {
        printf("没有预约记录。\n");
        return;
    }

    // 打印带编号的预约记录
    printf("\n=== 预约记录 ===\n");
    for (int i = 0; i < recordCount; i++) {
        printf("[%d] %s 预约 %s 医生 %s %s\n",
               i+1,
               appointmentRecords[i].patientName,
               appointmentRecords[i].docName,
               appointmentRecords[i].timeOfDay == 0 ? "上午" : "下午",
               appointmentRecords[i].deptName);
    }

    // 添加取消功能
    int choice;
    printf("\n输入要取消的预约编号（0返回）: ");
    if(scanf("%d", &choice) != 1) {
        while(getchar() != '\n');
        printf("输入无效\n");
        return;
    }

    if(choice == 0) return;

    if(choice <1 || choice > recordCount) {
        printf("无效的编号\n");
        return;
    }

    // 获取要取消的记录索引
    int index = choice - 1;
    AppointmentRecord record = appointmentRecords[index];

    // 二次确认
    char confirm;
    printf("\n确认取消以下预约吗？(y/其他取消)\n");
    printf("患者: %s\n科室: %s\n医生: %s\n时间: 星期%d %s\n",
           record.patientName,
           record.deptName,
           record.docName,
           record.dayOfWeek,
           record.timeOfDay == 0 ? "上午" : "下午");
    printf("请确认: ");
    scanf(" %c", &confirm);

    if(tolower(confirm) == 'y') {
        // 恢复号源
        int restored = 0;
        for(int i=0; i<departmentCount; i++) {
            if(strcmp(departments[i].name, record.deptName) == 0) {
                for(int j=0; j<departments[i].doctorCount; j++) {
                    if(strcmp(departments[i].doctors[j].name, record.docName) == 0) {
                        // 检查时间有效性
                        if(record.dayOfWeek >=1 && record.dayOfWeek <=7 &&
                           record.timeOfDay >=0 && record.timeOfDay <=1) {
                            departments[i].doctors[j].appointments[record.dayOfWeek-1][record.timeOfDay]++;
                            restored = 1;
                        }
                        break;
                    }
                }
                break;
            }
        }

        // 删除记录
        for(int i=index; i<recordCount-1; i++) {
            appointmentRecords[i] = appointmentRecords[i+1];
        }
        recordCount--;

        // 结果反馈
        if(restored) {
            printf("取消成功！号源已释放\n");
        } else {
            printf("取消完成（注：未能恢复号源，可能科室或医生信息已变更）\n");
        }
    } else {
        printf("操作已取消\n");
    }
}

	// 疾病库管理函数
void diseaseManagement() {
    int choice;
    do {
        printf("\n=== 疾病库管理 ===\n");
        printf("1. 添加疾病对应关系\n");
        printf("2. 修改现有疾病分类\n");
        printf("3. 删除疾病条目\n");
        printf("4. 查看全部疾病对应\n");
        printf("0. 返回上级菜单\n");
        printf("请选择操作: ");
        
        if(scanf("%d", &choice) != 1) {
            while(getchar() != '\n');
            printf("输入无效，请重新选择\n");
            continue;
        }

        switch (choice) {
            case 1: {
    char newDisease[50] = {0}, rawDept[50] = {0}, normalizedDept[50] = {0};
    
    printf("\n请输入疾病名称（0返回）: ");
    if(scanf("%49s", newDisease) != 1) {
        printf("输入无效\n");
        break;
    }
    if(strcmp(newDisease, "0") == 0) break;

    printf("请输入对应科室（0返回）: ");
    if(scanf("%49s", rawDept) != 1) {
        printf("输入无效\n");
        break;
    }
    if(strcmp(rawDept, "0") == 0) break;

    // 执行标准化
    normalizeDepartment(rawDept, normalizedDept);
    
    // 检查科室是否存在
    int deptValid = 0;
    for(int i=0; i<departmentCount; i++){
        // 使用模糊匹配
        if(strstr(departments[i].name, normalizedDept) || 
           strstr(normalizedDept, departments[i].name)) {
            strcpy(normalizedDept, departments[i].name);
            deptValid = 1;
            break;
        }
    }

    if(!deptValid){
        printf("错误：无法识别科室「%s」，请使用标准格式（如：内科-消化科）\n", rawDept);
        printf("当前有效科室：\n");
        for(int i=0; i<departmentCount; i++){
            printf("- %s\n", departments[i].name);
        }
        break;
    }

    // [修正点] 将 newDept 改为 normalizedDept
    // 检查科室容量
    int deptDiseaseCount = 0;
    for(int i=0; i<diseaseCount; i++){
        if(strcmp(diseaseDatabase[i].department, normalizedDept) == 0){ // 修改此处
            deptDiseaseCount++;
        }
    }
    if(deptDiseaseCount >= MAX_DEPT_DISEASES){
        printf("错误：该科室疾病数量已达上限（%d种）\n", MAX_DEPT_DISEASES);
        break;
    }

    // 执行添加
    strcpy(diseaseDatabase[diseaseCount].disease, newDisease);
    strcpy(diseaseDatabase[diseaseCount].department, normalizedDept);
    diseaseCount++;
    printf("添加成功！标准化科室名称：%s\n", normalizedDept);
    break;
}
            case 2: {
                // 显示带编号的表格
                printf("\n=== 疾病科室对应表 ===\n");
                char *mainDepartments[] = {
                    "内科-消化科", "内科-心血管科", "内科-呼吸科",
                    "外科-骨科", "外科-普外科", "外科-神经外科"
                };
                const int deptCount = 6;

                // 列宽计算
                int colWidths[6] = {0};
                for(int d=0; d<deptCount; d++){
                    int deptNameLen = strlen(mainDepartments[d]) * 2;
                    colWidths[d] = deptNameLen;
                    
                    for(int i=0; i<diseaseCount; i++){
                        if(strcmp(diseaseDatabase[i].department, mainDepartments[d]) == 0){
                            int diseaseLen = strlen(diseaseDatabase[i].disease) * 2;
                            if(diseaseLen > colWidths[d]){
                                colWidths[d] = diseaseLen;
                            }
                        }
                    }
                    colWidths[d] += 4;
                }

                // 打印表头
                for(int d=0; d<deptCount; d++){
                    printf("%-*s", colWidths[d], mainDepartments[d]);
                }
                printf("\n");

                // 收集带索引的数据
                struct DeptDiseases {
                    int indexes[10];
                    char diseases[10][50];
                    int count;
                } deptData[6] = {0};

                for(int d=0; d<deptCount; d++){
                    for(int i=0; i<diseaseCount; i++){
                        if(strcmp(diseaseDatabase[i].department, mainDepartments[d]) == 0){
                            strcpy(deptData[d].diseases[deptData[d].count], diseaseDatabase[i].disease);
                            deptData[d].indexes[deptData[d].count] = i;
                            deptData[d].count++;
                        }
                    }
                }

                // 打印带全局编号的表格
                int globalIndex = 1;
                for(int line=0; line<10; line++) {
                    for(int d=0; d<deptCount; d++){ 
                        if(line < deptData[d].count){
                            printf("%2d.%-*s", 
                                globalIndex++,
                                colWidths[d]-3,
                                deptData[d].diseases[line]);
                        } else {
                            printf("%-*s", colWidths[d], "");
                        }
                    }
                    printf("\n");
                }

                // 修改逻辑
                int selectedIndex;
                printf("\n请选择要修改的疾病编号（0返回）: ");
                if(scanf("%d", &selectedIndex) != 1) {
                    while(getchar() != '\n');
                    printf("输入无效\n");
                    break;
                }
                if(selectedIndex == 0) break;

                if(selectedIndex<1 || selectedIndex>diseaseCount){
                    printf("无效的编号\n");
                    break;
                }

                int originalIndex = selectedIndex - 1;
                char currentDisease[50], currentDept[50];
                strcpy(currentDisease, diseaseDatabase[originalIndex].disease);
                strcpy(currentDept, diseaseDatabase[originalIndex].department);

                // 修改疾病名称
                char newDisease[50] = {0};
                printf("\n当前疾病名称: %s\n", currentDisease);
                printf("输入新名称（直接回车保留）: ");
                getchar();
                fgets(newDisease, 50, stdin);
                newDisease[strcspn(newDisease, "\n")] = '\0';
                
                if(strlen(newDisease) > 0) {
                    // 检查新名称是否重复
                    int exists = 0;
                    for(int i=0; i<diseaseCount; i++){
                        if(i != originalIndex && strcmp(diseaseDatabase[i].disease, newDisease) == 0){
                            exists = 1;
                            break;
                        }
                    }
                    if(exists){
                        printf("错误：疾病名称已存在\n");
                        break;
                    }
                    strcpy(diseaseDatabase[originalIndex].disease, newDisease);
                }

                // 修改科室
                char newDept[50] = {0};
                printf("\n当前科室: %s\n", currentDept);
                printf("输入新科室（直接回车保留）: ");
                fgets(newDept, 50, stdin);
                newDept[strcspn(newDept, "\n")] = '\0';

                if(strlen(newDept) > 0) {
                    // 检查科室存在
                    int deptValid = 0;
                    for(int i=0; i<departmentCount; i++){
                        if(strcmp(departments[i].name, newDept) == 0){
                            deptValid = 1;
                            break;
                        }
                    }
                    if(!deptValid){
                        printf("错误：科室不存在\n");
                        // 回滚疾病名称修改
                        if(strlen(newDisease)>0) strcpy(diseaseDatabase[originalIndex].disease, currentDisease);
                        break;
                    }

                    // 检查新科室容量
                    int deptDiseaseCount = 0;
                    for(int i=0; i<diseaseCount; i++){
                        if(strcmp(diseaseDatabase[i].department, newDept) == 0){
                            deptDiseaseCount++;
                        }
                    }
                    if(deptDiseaseCount >= 6){
                        printf("错误：该科室已满6种疾病\n");
                        // 回滚修改
                        if(strlen(newDisease)>0) strcpy(diseaseDatabase[originalIndex].disease, currentDisease);
                        break;
                    }

                    strcpy(diseaseDatabase[originalIndex].department, newDept);
                }

                printf("修改成功！\n");
                break;
            }

            case 3: {
                printf("\n当前疾病库：\n");
                for(int i=0; i<diseaseCount; i++){
                    printf("%d. %s -> %s\n", i+1, 
                        diseaseDatabase[i].disease,
                        diseaseDatabase[i].department);
                }

                int index;
                printf("\n请选择要删除的条目编号（0返回）: ");
                if(scanf("%d", &index) != 1) {
                    while(getchar() != '\n');
                    printf("输入无效\n");
                    break;
                }
                if(index == 0) break;

                if(index<1 || index>diseaseCount){
                    printf("无效的编号\n");
                    break;
                }

                char confirm;
                printf("确认删除「%s」吗？(y/其他取消): ", diseaseDatabase[index-1].disease);
                scanf(" %c", &confirm);
                if(tolower(confirm) != 'y') {
                    printf("操作已取消\n");
                    break;
                }

                // 执行删除
                for(int i=index-1; i<diseaseCount-1; i++){
                    diseaseDatabase[i] = diseaseDatabase[i+1];
                }
                diseaseCount--;
                printf("删除成功！\n");
                break;
            }

            case 4: {
                printf("\n=== 疾病科室对应表 ===\n");
                char *mainDepartments[] = {
                    "内科-消化科", "内科-心血管科", "内科-呼吸科",
                    "外科-骨科", "外科-普外科", "外科-神经外科"
                };
                const int deptCount = 6;

                // 列宽计算
                int colWidths[6] = {0};
                for(int d=0; d<deptCount; d++){
                    int deptNameLen = strlen(mainDepartments[d]) * 2;
                    colWidths[d] = deptNameLen;
                    
                    for(int i=0; i<diseaseCount; i++){
                        if(strcmp(diseaseDatabase[i].department, mainDepartments[d]) == 0){
                            int diseaseLen = strlen(diseaseDatabase[i].disease) * 2;
                            if(diseaseLen > colWidths[d]){
                                colWidths[d] = diseaseLen;
                            }
                        }
                    }
                    colWidths[d] += 4;
                }

                // 打印表头
                for(int d=0; d<deptCount; d++){
                    printf("%-*s", colWidths[d], mainDepartments[d]);
                }
                printf("\n");

                // 收集数据
                struct DeptDiseases {
                    char diseases[10][50];
                    int count;
                } deptData[6] = {0};

                for(int d=0; d<deptCount; d++){
                    for(int i=0; i<diseaseCount; i++){
                        if(strcmp(diseaseDatabase[i].department, mainDepartments[d]) == 0){
                            strcpy(deptData[d].diseases[deptData[d].count], diseaseDatabase[i].disease);
                            deptData[d].count++;
                        }
                    }
                }

                // 打印表格
                int maxLines = 0;
for(int d=0; d<deptCount; d++){
    if(deptData[d].count > maxLines){
        maxLines = deptData[d].count;
    }
}
for(int line=0; line<maxLines; line++){
                    for(int d=0; d<deptCount; d++){ 
                        if(line < deptData[d].count){
                            printf("%2d.%-*s", 
                                line+1,
                                colWidths[d]-3,
                                deptData[d].diseases[line]);
                        } else {
                            printf("%-*s", colWidths[d], "");
                        }
                    }
                    printf("\n");
                }

                printf("按任意键返回...");
                getchar();
                getchar();
                break;
            }

            case 0:
                printf("返回上级菜单\n");
                break;

            default:
                printf("无效的选择\n");
                break;
        }
    } while (choice != 0);
}
	
	void searchByDisease(int isDeveloperMode) {
	    char disease[50];
	    printf("\n请输入疾病名称（0返回）: ");
	    scanf("%s", disease);
	    
	    if(strcmp(disease, "0") == 0) return;
	
	    // 查找科室
	    char targetDept[50] = "";
	    for(int i=0; i<diseaseCount; i++){
	        if(strcmp(diseaseDatabase[i].disease, disease) == 0){
	            strcpy(targetDept, diseaseDatabase[i].department);
	            break;
	        }
	    }
	
	    // 处理未找到情况
	    if(strlen(targetDept) == 0){
	        printf("未找到相关科室信息，建议咨询导诊台\n");
	        if(isDeveloperMode){
	            printf("[开发者提示] 可前往疾病库管理添加对应关系\n");
	        }
	        return;
	    }
	
	    // 显示结果
	    printf("\n【%s】属于【%s】\n", disease, targetDept);
	    
	    // 查找科室信息
	    int deptIndex = -1;
	    for(int i=0; i<departmentCount; i++){
	        if(strcmp(departments[i].name, targetDept) == 0){
	            deptIndex = i;
	            break;
	        }
	    }
	
	    if(deptIndex == -1){
	        printf("当前没有开设该专科\n");
	        return;
	    }
	
	    // 显示科室医生
	    printf("\n=== 推荐专家 ===\n");
	    for(int j=0; j<departments[deptIndex].doctorCount; j++){
	        Doctor doc = departments[deptIndex].doctors[j];
	        printf("%d. %s %s（%s）\n", 
	              j+1, doc.name, doc.title, doc.specialty);
	    }
	
	    // 患者模式下进入预约流程
	    if(!isDeveloperMode){
	        int docChoice;
	        printf("\n请选择医生查看详情（0返回）: ");
	        scanf("%d", &docChoice);
	        
	        if(docChoice>0 && docChoice<=departments[deptIndex].doctorCount){
	            Doctor doc = departments[deptIndex].doctors[docChoice-1];
	            
	            // 显示医生详情
	            printf("\n=== 医生详情 ===\n");
	            printf("姓名：%s\n职称：%s\n", doc.name, doc.title);
	            printf("专业方向：%s\n", doc.specialty);
	            printf("挂号费：%.1f元\n", doc.fee);
	            
	            // 显示可预约时间
	            printf("\n可预约时间：\n");
	            char *days[] = {"周一","周二","周三","周四","周五","周六","周日"};
	            int hasAvailable = 0;
	            for(int d=0; d<7; d++){
	                for(int t=0; t<2; t++){
	                    if(doc.appointments[d][t] >0){
	                        printf("%s %s：剩余%d号\n", 
	                              days[d], 
	                              t==0 ? "上午" : "下午", 
	                              doc.appointments[d][t]);
	                        hasAvailable = 1;
	                    }
	                }
	            }
	            if(!hasAvailable) printf("近期号源已满\n");
	            
	            // 进入预约流程
	            char confirm;
	            printf("\n是否立即预约？(y/其他返回): ");
	            scanf(" %c", &confirm);
	            if(tolower(confirm) == 'y'){
	                char name[50], phone[20];
	                printf("请输入姓名: ");
	                scanf("%s", name);
	                printf("请输入联系电话: ");
	                scanf("%s", phone);
	                
	                // 自动预约最早可用的时段
	                int booked = 0;
	                for(int d=0; d<7 && !booked; d++){
	                    for(int t=0; t<2; t++){
	                        if(doc.appointments[d][t] >0){
	                            makeAppointment(name, phone, deptIndex+1, docChoice, d+1, t);
	                            booked = 1;
	                            break;
	                        }
	                    }
	                }
	                if(booked) printf("预约成功！\n");
	            }
	        }
	    }
	}
	
void searchDoctor() {
    char name[50];
    printf("请输入要查找的医生姓名: ");
    scanf("%s", name);
    
    int found = 0;
    for(int i=0; i<departmentCount; i++) {
        for(int j=0; j<departments[i].doctorCount; j++) {
            Doctor doc = departments[i].doctors[j];
            if(strcmp(doc.name, name) == 0) {
                printf("\n=== 医生详细信息 ===\n");
                printf("所属科室: %s\n", departments[i].name);
                printf("姓名: %s\n", doc.name);
                printf("职称: %s\n", doc.title);
                printf("专业领域: %s\n", doc.specialty);
                printf("挂号费: %.1f元\n", doc.fee);
                printf("年龄: %d\n", doc.age);
                
                printf("\n可预约时间:\n");
                char *days[] = {"周一","周二","周三","周四","周五","周六","周日"};
                int hasAvailable = 0;
                for(int d=0; d<7; d++){
                    for(int t=0; t<2; t++){
                        if(doc.appointments[d][t] >0){
                            printf("%s %s：剩余%d号\n", 
                                  days[d], 
                                  t==0 ? "上午" : "下午", 
                                  doc.appointments[d][t]);
                            hasAvailable = 1;
                        }
                    }
                }
                if(!hasAvailable) printf("当前无可用号源\n");
                
                printf("\n患者评价:\n");
                for(int k=0; k<3; k++){
                    if(strlen(doc.reviews[k]) > 0)
                        printf("%d. %s\n", k+1, doc.reviews[k]);
                }
                found = 1;
                break;
            }
        }
        if(found) break;
    }
    if(!found) {
        printf("未找到名为 %s 的医生\n", name);
    }
}	
	
	
	
	
	
	
	
void initDepartments() {
    // 初始科室数据
    char *deptNames[] = {
        "内科-消化科", "内科-心血管科", "内科-呼吸科",
        "外科-骨科", "外科-普外科", "外科-神经外科"
    };
    const int initialDeptCount = sizeof(deptNames)/sizeof(deptNames[0]);

    // 预定义的医生姓名（每个科室2位，共12位）
    char *deptDoctors[][2] = {
        {"张建国", "王桂芳"},    // 消化科
        {"李德海", "赵丽华"},    // 心血管科
        {"陈志强", "刘秀英"},    // 呼吸科
        {"周建军", "黄淑兰"},    // 骨科
        {"吴国强", "徐玉兰"},    // 普外科
        {"孙建华", "马晓梅"}     // 神经外科
    };

    // 专业方向数据
    char *specialties[][3] = {
        {"胃炎", "肝硬化", "消化道肿瘤"},
        {"冠心病", "动脉硬化", "心力衰竭"},
        {"肺炎", "哮喘", "慢性阻塞性肺疾病"},
        {"骨折", "关节置换", "脊柱外科"},
        {"阑尾炎", "疝气", "胃肠肿瘤"},
        {"脑肿瘤", "脑血管病", "神经创伤"}
    };

    // 随机数据池（保留其他随机属性）
    char *reviewPool[] = {
        "问诊细致，解释清楚", "治疗效果好，推荐", "非常有耐心",
        "用药精准，复诊安排合理", "候诊时间稍长但值得", "对老年人特别关照",
        "检查建议非常专业", "会认真听患者描述", "术后随访很及时",
        "诊断准确，用药后明显好转", "对儿童患者很有办法", "治疗方案经济有效"
    };

    srand(time(NULL));
    
    // 初始化6个科室
    for (int i = 0; i < initialDeptCount; i++) {
        // 初始化科室信息
        strcpy(departments[i].name, deptNames[i]);
        departments[i].doctorCount = 2; // 每个科室初始2位医生

        // 初始化医生信息
        for (int j = 0; j < 2; j++) {
            Doctor *doc = &departments[i].doctors[j];
            
            // 使用预设的固定姓名
            strcpy(doc->name, deptDoctors[i][j]);
            
            // 设置职称和费用（保留原有职称生成逻辑）
            char *titles[] = {"主任医师", "副主任医师", "主治医师"};
            int titleIndex = (j == 0) ? 0 : (rand()%2 + 1);
            strcpy(doc->title, titles[titleIndex]);
            doc->fee = (titleIndex == 0) ? 50.0 : (titleIndex == 1) ? 30.0 : 15.0;
            
            // 专业方向
            strcpy(doc->specialty, specialties[i][j%3]);
            
            // 随机年龄（保留年龄生成逻辑）
            doc->age = (titleIndex == 0) ? 55+rand()%5 : 
                      (titleIndex == 1) ? 45+rand()%5 : 35+rand()%5;

            // 初始化预约时间
            for(int d=0; d<7; d++){
                for(int t=0; t<2; t++){
                    doc->appointments[d][t] = (rand()%3 == 0) ? 5 : 0;
                }
            }

            // 随机评价（保留评价生成逻辑）
            int used[12] = {0};
            for(int k=0; k<3; k++){
                int r;
                do { r = rand()%12; } while(used[r]);
                strcpy(doc->reviews[k], reviewPool[r]);
                used[r] = 1;
            }
        }

        // 剩余医生位置保持空
        for(int j=2; j<MAX_DOCTORS; j++){
            departments[i].doctors[j].name[0] = '\0';
        }
    }

    // 初始化剩余科室为空
    for(int i=initialDeptCount; i<MAX_DEPARTMENTS; i++){
        departments[i].name[0] = '\0';
        departments[i].doctorCount = 0;
    }

    departmentCount = initialDeptCount;
}
	
	/* 修改后的显示函数 */
	void showDepartmentsAndMakeAppointment() {
	    printf("\n=== 科室列表 ===\n");
	    for(int i=0; i<departmentCount; i++){ // 只显示实际存在的科室
	        printf("%d. %s", i+1, departments[i].name);
	        if (departments[i].doctorCount == 0) {
	            printf(" (暂未开放)");
	        }
	        printf("\n");
	    }
	    
	    int choice;
	    printf("请选择科室查看专家（0返回）: ");
	    scanf("%d", &choice);
	    
	    if(choice == 0) return;
	    
	    if(choice >0 && choice <= departmentCount){
	        Department *dept = &departments[choice-1];
	        if(dept->doctorCount == 0) {
	            printf("该科室暂未开放，请联系医院管理部门\n");
	            return;
	        }
	        
	        printf("\n=== %s专家列表 ===\n", dept->name);
	        for(int j=0; j<dept->doctorCount; j++){
	            Doctor doc = dept->doctors[j];
	            printf("%d. %s %s（挂号费：%.1f元）\n", 
	                  j+1, doc.name, doc.title, doc.fee);
	        }
	        
	        int docChoice;
	        printf("选择医生查看详情或预约（0返回）: ");
	        scanf("%d", &docChoice);
	        
	        if(docChoice == 0) return;
	        
	        if(docChoice >0 && docChoice <= departments[choice-1].doctorCount){
	            Doctor doc = departments[choice-1].doctors[docChoice-1];
	            
	            // 显示医生详情
	            printf("\n=== 医生详情 ===\n");
	            printf("姓名：%s\n职称：%s\n专业：%s\n", doc.name, doc.title, doc.specialty);
	            printf("挂号费：%.1f元\n", doc.fee);
	            
	            // 显示可预约时间
	            printf("\n可预约时间：\n");
	            char *days[] = {"周一","周二","周三","周四","周五","周六","周日"};
	            int hasAvailable = 0;
	            for(int d=0; d<7; d++){
	                for(int t=0; t<2; t++){
	                    if(doc.appointments[d][t] >0){
	                        printf("%s %s：剩余%d号\n", 
	                              days[d], 
	                              t==0 ? "上午" : "下午", 
	                              doc.appointments[d][t]);
	                        hasAvailable = 1;
	                    }
	                }
	            }
	            if(!hasAvailable) printf("本周暂无可用号源\n");
	            
	            // 显示患者评价
	            printf("\n患者评价：\n");
	            for(int k=0; k<3; k++){
	                printf("· %s\n", doc.reviews[k]);
	            }
	
	            // 预约流程
	            char patientName[50], phoneNumber[20];
	            int dayOfWeek, timeOfDay;
	            
	            // 输入姓名
	            printf("请输入您的姓名（0返回）: ");
	            scanf("%s", patientName);
	            if(strcmp(patientName, "0") == 0) return;
	
	            // 输入电话
	            printf("请输入您的电话号码（0返回）: ");
	            scanf("%s", phoneNumber);
	            if(strcmp(phoneNumber, "0") == 0) return;
	
	            // 确认预约
	            printf("是否现在预约？(y/n/0返回): ");
	            char confirm;
	            scanf(" %c", &confirm);
	            if(confirm == '0') return;
	            if(tolower(confirm) == 'n') return;
	
	            if(tolower(confirm) == 'y'){
	                // 选择日期
	                printf("选择预约日期(1-7周一到周日，0返回): ");
	                if(scanf("%d", &dayOfWeek) != 1){
	                    while(getchar() != '\n');
	                    printf("输入无效\n");
	                    return;
	                }
	                if(dayOfWeek == 0) return;
	                if(dayOfWeek <1 || dayOfWeek>7){
	                    printf("无效的日期选择\n");
	                    return;
	                }
	
	                // 选择时间段
	                printf("选择时间段（上午0，下午1，其他返回）: ");
	                if(scanf("%d", &timeOfDay) != 1){
	                    while(getchar() != '\n');
	                    printf("输入无效\n");
	                    return;
	                }
	                if(timeOfDay <0 || timeOfDay>1){
	                    printf("无效的时间选择\n");
	                    return;
	                }
	
	                // 执行预约
	                makeAppointment(patientName, phoneNumber, choice, docChoice, dayOfWeek, timeOfDay);
	            }
	        } else {
	            printf("无效的选择\n");
	        }
	    } else {
	        printf("无效的选择\n");
	    }
	}
	// 增强版的makeAppointment函数
	void makeAppointment(char patientName[], char phoneNumber[], int deptChoice, int docChoice, int dayOfWeek, int timeOfDay) {
	    // 检查预约冲突前先检查返回信号
	    if(strcmp(patientName, "0") == 0 || strcmp(phoneNumber, "0") == 0 || dayOfWeek == 0) {
	        printf("已取消预约\n");
	        return;
	    } // 补全这个闭合括号
	
	    // 原有预约逻辑
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
	
	// 修改后的完整showMenu函数
	void showMenu(int mode) {
	    int choice;
	    do {
	        printf("\n=== 主菜单 ===\n");
	        printf("1. 查看科室并预约\n");
	        printf("2. 查看预约记录\n");
	        printf("3. 查找医生\n");
	        printf("4. 按疾病查找科室\n");
	        
	        if(mode == 1) {
	            printf("5. 修改医生信息\n");
	            printf("6. 删除医生信息\n");
	            printf("7. 添加医生信息\n");
	            printf("8. 疾病库管理\n");
	            printf("9. 添加科室\n");  // 新增菜单项
	        }
	        
	        printf("0. 返回上级菜单\n");
	        printf("请选择操作: ");
	        scanf("%d", &choice);
	
	        switch (choice) {
	            case 1: showDepartmentsAndMakeAppointment(); break;
	            case 2: viewAppointments(); break;
	            case 3: searchDoctor(); break;
	            case 4: searchByDisease(mode); break;
	            case 5: if(mode) modifyDoctor(); else printf("无效选择\n"); break;
	            case 6: if(mode) deleteDoctor(); else printf("无效选择\n"); break;
	            case 7: if(mode) addDoctor(); else printf("无效选择\n"); break;
	            case 8: if(mode) diseaseManagement(); else printf("无效选择\n"); break;
	            case 9: if(mode) addDepartment(); else printf("无效选择\n"); break; // 新增case
	            case 0: break;
	            default: printf("无效的选择\n");
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
	        printf("%d. %s", i + 1, departments[i].name);
	        if (departments[i].doctorCount >= MAX_DOCTORS) {
	            printf(" (已满)");
	        }
	        printf("\n");
	    }
	    
	    int deptChoice;
	    printf("请选择要添加医生的科室(输入0返回): ");
	    scanf("%d", &deptChoice);
	
	     if(deptChoice >0 && deptChoice <= departmentCount){
	        Department *dept = &departments[deptChoice - 1];
	        
	        if(dept->doctorCount >= MAX_DOCTORS) {
	            printf("该科室已达最大医生数量（%d）\n", MAX_DOCTORS);
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
	        newDoc.age = 35 + ((strcmp(newDoc.title, "主任医师") == 0) ? 0 : 
	                         (strcmp(newDoc.title, "副主任医师") == 0) ? 5 : 10) + rand()%5;
	
	        // 初始化预约时间
	        for(int d=0; d<7; d++){
	            for(int t=0; t<2; t++){
	                newDoc.appointments[d][t] = (rand()%2 == 0) ? 5 : 0;
	            }
	        }
	
	        // 初始化评价
	        strcpy(newDoc.reviews[0], "暂无患者评价");
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
