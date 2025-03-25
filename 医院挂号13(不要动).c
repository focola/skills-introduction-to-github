	#include <stdio.h>
	#include <stdlib.h>
	#include <string.h>
	#include <time.h>
	#include <ctype.h>
	
	
	#define MAX_DOCTORS 10      // ÿ���������10λҽ��
	#define MAX_DEPARTMENTS 20  // ��������20
// �޸�Ϊ������ʵ�����������
#define MAX_DISEASES 1000  // �ܼ�������
#define MAX_DEPT_DISEASES 50  // ����������󼲲���
	// ����-���Ҷ�Ӧ�ṹ
	typedef struct {
	    char disease[50];
	    char department[50];
	} DiseaseMap;
	
	
// ����-���Ҷ�Ӧ�ṹ
DiseaseMap diseaseDatabase[MAX_DISEASES] = {
    // �ڿ�-�����ƣ�10�֣�
    {"θ����", "�ڿ�-������"},
    {"θ����", "�ڿ�-������"},
    {"θ��", "�ڿ�-������"},
    {"��Ӳ��", "�ڿ�-������"},
    {"��������Ѫ", "�ڿ�-������"},
    {"���׼��ۺ���", "�ڿ�-������"},

    // �ڿ�-��Ѫ�ܿƣ�10�֣�
    {"���Ĳ�", "�ڿ�-��Ѫ�ܿ�"},
    {"�Ľ�ʹ", "�ڿ�-��Ѫ�ܿ�"},
    {"�ļ�����", "�ڿ�-��Ѫ�ܿ�"},
    {"����ʧ��", "�ڿ�-��Ѫ�ܿ�"},
    {"��Ѫѹ", "�ڿ�-��Ѫ�ܿ�"},
    {"����Ӳ��", "�ڿ�-��Ѫ�ܿ�"},

    // �ڿ�-�����ƣ�10�֣�
    {"����", "�ڿ�-������"},
    {"����", "�ڿ�-������"},
    {"���������Էμ���", "�ڿ�-������"},
    {"�ν��", "�ڿ�-������"},
    {"������", "�ڿ�-������"},
    {"��Ĥ��", "�ڿ�-������"},

    // ���-�ǿƣ�10�֣�
    {"����", "���-�ǿ�"},
    {"��������", "���-�ǿ�"},
    {"��׵����ͻ��", "���-�ǿ�"},
    {"�ؽ���", "���-�ǿ�"},
    {"������", "���-�ǿ�"},
    {"��׵��", "���-�ǿ�"},

    // ���-����ƣ�10�֣�
    {"��β��", "���-�����"},
    {"����", "���-�����"},
    {"���ҽ�ʯ", "���-�����"},
    {"������", "���-�����"},
    {"���ٰ�", "���-�����"},
    {"��״�ٽ��", "���-�����"},

    // ���-����ƣ�10�֣�
    {"������", "���-�����"},
    {"�Գ�Ѫ", "���-�����"},
    {"������", "���-�����"},
    {"�Ի�ˮ", "���-�����"},
    {"����ɭ��", "���-�����"},
    {"������ʹ", "���-�����"},

};

int diseaseCount = 36;  // ��ǰ����������6�����ҡ�10�ּ�����


	
	// ҽ���ṹ
	typedef struct {
	    char name[50];
	    int age;
	    char title[30];      // ְ��
	    char specialty[50]; // רҵ����
	    float fee;           // �Һŷ�
	    int appointments[7][2]; 
	    char reviews[3][100]; // ��������
	} Doctor;
	
	// ���ҽṹ
	typedef struct {
	    char name[50];
	    Doctor doctors[MAX_DOCTORS];
	    int doctorCount;
	} Department;
	
	Department departments[MAX_DEPARTMENTS];
	int departmentCount = 0;
	
	// ԤԼ��¼�ṹ
	typedef struct {
	    char patientName[50];
	    char phoneNumber[20];
	    char deptName[50];
	    char docName[50];
	    int dayOfWeek;
	    int timeOfDay;
	} AppointmentRecord;  // ��ȫ�ṹ��պ�
	
	AppointmentRecord appointmentRecords[100];
	int recordCount = 0;
	// ��������
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
	void addDepartment();  // ������������
	void normalizeDepartment(const char *input, char *output);  // ��������
	void cancelAppointment(int index);

	
	void addDepartment() {
	    if (departmentCount >= MAX_DEPARTMENTS) {
	        printf("���������Ѵ����ޣ�%d��\n", MAX_DEPARTMENTS);
	        return;
	    }
	    
	    char name[50];
	    printf("�������¿�������: ");
	    scanf("%s", name);
	    
	    // ����ظ�
	    for (int i = 0; i < departmentCount; i++) {
	        if (strcmp(departments[i].name, name) == 0) {
	            printf("�ÿ����Ѵ���\n");
	            return;
	        }
	    }
	    
	    strcpy(departments[departmentCount].name, name);
	    departments[departmentCount].doctorCount = 0;
	    departmentCount++;
	    printf("������ӳɹ���\n");
	}

// �����������������Ʊ�׼������
void normalizeDepartment(const char *input, char *output) {
    char buffer[100];
    int j = 0;
    
    // ��һ�����Ƴ����з������ַ��Ϳո�
    for (int i = 0; input[i] != '\0'; i++) {
        if (isalpha((unsigned char)input[i]) || 
            isdigit((unsigned char)input[i]) || 
            input[i] == ' ' || 
            input[i] == '-') {
            buffer[j++] = tolower(input[i]);
        }
    }
    buffer[j] = '\0';

    // �ڶ�����ͳһ�ָ���Ϊ�����ַ�
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

    // �����������ܲ�ȫ����ǰ׺
    const char *prefix = "�ڿ�";
    if (strstr(output, "���") || 
        strstr(output, "�ǿ�") || 
        strstr(output, "����") || 
        strstr(output, "�����")) {
        prefix = "���";
    }

    // ���Ĳ��������׼��ʽ
    if (!strstr(output, "�ڿ�") && !strstr(output, "���")) {
        char temp[100];
        if (strstr(output, "����")) sprintf(temp, "%s-������", prefix);
        else if (strstr(output, "��Ѫ��")) sprintf(temp, "%s-��Ѫ�ܿ�", prefix);
        else if (strstr(output, "����")) sprintf(temp, "%s-������", prefix);
        else if (strstr(output, "�ǿ�")) strcpy(temp, "���-�ǿ�");
        else if (strstr(output, "����")) strcpy(temp, "���-�����");
        else if (strstr(output, "�����")) strcpy(temp, "���-�����");
        else {
            strcpy(temp, output); // �޷�ʶ��ı���ԭ��
            return;
        }
        strcpy(output, temp);
    }

    // ���岽�����ո�ʽУ��
    if (output[0] == '-') memmove(output, output+1, strlen(output));
}


void viewAppointments() {
    if (recordCount == 0) {
        printf("û��ԤԼ��¼��\n");
        return;
    }

    // ��ӡ����ŵ�ԤԼ��¼
    printf("\n=== ԤԼ��¼ ===\n");
    for (int i = 0; i < recordCount; i++) {
        printf("[%d] %s ԤԼ %s ҽ�� %s %s\n",
               i+1,
               appointmentRecords[i].patientName,
               appointmentRecords[i].docName,
               appointmentRecords[i].timeOfDay == 0 ? "����" : "����",
               appointmentRecords[i].deptName);
    }

    // ���ȡ������
    int choice;
    printf("\n����Ҫȡ����ԤԼ��ţ�0���أ�: ");
    if(scanf("%d", &choice) != 1) {
        while(getchar() != '\n');
        printf("������Ч\n");
        return;
    }

    if(choice == 0) return;

    if(choice <1 || choice > recordCount) {
        printf("��Ч�ı��\n");
        return;
    }

    // ��ȡҪȡ���ļ�¼����
    int index = choice - 1;
    AppointmentRecord record = appointmentRecords[index];

    // ����ȷ��
    char confirm;
    printf("\nȷ��ȡ������ԤԼ��(y/����ȡ��)\n");
    printf("����: %s\n����: %s\nҽ��: %s\nʱ��: ����%d %s\n",
           record.patientName,
           record.deptName,
           record.docName,
           record.dayOfWeek,
           record.timeOfDay == 0 ? "����" : "����");
    printf("��ȷ��: ");
    scanf(" %c", &confirm);

    if(tolower(confirm) == 'y') {
        // �ָ���Դ
        int restored = 0;
        for(int i=0; i<departmentCount; i++) {
            if(strcmp(departments[i].name, record.deptName) == 0) {
                for(int j=0; j<departments[i].doctorCount; j++) {
                    if(strcmp(departments[i].doctors[j].name, record.docName) == 0) {
                        // ���ʱ����Ч��
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

        // ɾ����¼
        for(int i=index; i<recordCount-1; i++) {
            appointmentRecords[i] = appointmentRecords[i+1];
        }
        recordCount--;

        // �������
        if(restored) {
            printf("ȡ���ɹ�����Դ���ͷ�\n");
        } else {
            printf("ȡ����ɣ�ע��δ�ָܻ���Դ�����ܿ��һ�ҽ����Ϣ�ѱ����\n");
        }
    } else {
        printf("������ȡ��\n");
    }
}

	// �����������
void diseaseManagement() {
    int choice;
    do {
        printf("\n=== ��������� ===\n");
        printf("1. ��Ӽ�����Ӧ��ϵ\n");
        printf("2. �޸����м�������\n");
        printf("3. ɾ��������Ŀ\n");
        printf("4. �鿴ȫ��������Ӧ\n");
        printf("0. �����ϼ��˵�\n");
        printf("��ѡ�����: ");
        
        if(scanf("%d", &choice) != 1) {
            while(getchar() != '\n');
            printf("������Ч��������ѡ��\n");
            continue;
        }

        switch (choice) {
            case 1: {
    char newDisease[50] = {0}, rawDept[50] = {0}, normalizedDept[50] = {0};
    
    printf("\n�����뼲�����ƣ�0���أ�: ");
    if(scanf("%49s", newDisease) != 1) {
        printf("������Ч\n");
        break;
    }
    if(strcmp(newDisease, "0") == 0) break;

    printf("�������Ӧ���ң�0���أ�: ");
    if(scanf("%49s", rawDept) != 1) {
        printf("������Ч\n");
        break;
    }
    if(strcmp(rawDept, "0") == 0) break;

    // ִ�б�׼��
    normalizeDepartment(rawDept, normalizedDept);
    
    // �������Ƿ����
    int deptValid = 0;
    for(int i=0; i<departmentCount; i++){
        // ʹ��ģ��ƥ��
        if(strstr(departments[i].name, normalizedDept) || 
           strstr(normalizedDept, departments[i].name)) {
            strcpy(normalizedDept, departments[i].name);
            deptValid = 1;
            break;
        }
    }

    if(!deptValid){
        printf("�����޷�ʶ����ҡ�%s������ʹ�ñ�׼��ʽ���磺�ڿ�-�����ƣ�\n", rawDept);
        printf("��ǰ��Ч���ң�\n");
        for(int i=0; i<departmentCount; i++){
            printf("- %s\n", departments[i].name);
        }
        break;
    }

    // [������] �� newDept ��Ϊ normalizedDept
    // ����������
    int deptDiseaseCount = 0;
    for(int i=0; i<diseaseCount; i++){
        if(strcmp(diseaseDatabase[i].department, normalizedDept) == 0){ // �޸Ĵ˴�
            deptDiseaseCount++;
        }
    }
    if(deptDiseaseCount >= MAX_DEPT_DISEASES){
        printf("���󣺸ÿ��Ҽ��������Ѵ����ޣ�%d�֣�\n", MAX_DEPT_DISEASES);
        break;
    }

    // ִ�����
    strcpy(diseaseDatabase[diseaseCount].disease, newDisease);
    strcpy(diseaseDatabase[diseaseCount].department, normalizedDept);
    diseaseCount++;
    printf("��ӳɹ�����׼���������ƣ�%s\n", normalizedDept);
    break;
}
            case 2: {
                // ��ʾ����ŵı��
                printf("\n=== �������Ҷ�Ӧ�� ===\n");
                char *mainDepartments[] = {
                    "�ڿ�-������", "�ڿ�-��Ѫ�ܿ�", "�ڿ�-������",
                    "���-�ǿ�", "���-�����", "���-�����"
                };
                const int deptCount = 6;

                // �п����
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

                // ��ӡ��ͷ
                for(int d=0; d<deptCount; d++){
                    printf("%-*s", colWidths[d], mainDepartments[d]);
                }
                printf("\n");

                // �ռ�������������
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

                // ��ӡ��ȫ�ֱ�ŵı��
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

                // �޸��߼�
                int selectedIndex;
                printf("\n��ѡ��Ҫ�޸ĵļ�����ţ�0���أ�: ");
                if(scanf("%d", &selectedIndex) != 1) {
                    while(getchar() != '\n');
                    printf("������Ч\n");
                    break;
                }
                if(selectedIndex == 0) break;

                if(selectedIndex<1 || selectedIndex>diseaseCount){
                    printf("��Ч�ı��\n");
                    break;
                }

                int originalIndex = selectedIndex - 1;
                char currentDisease[50], currentDept[50];
                strcpy(currentDisease, diseaseDatabase[originalIndex].disease);
                strcpy(currentDept, diseaseDatabase[originalIndex].department);

                // �޸ļ�������
                char newDisease[50] = {0};
                printf("\n��ǰ��������: %s\n", currentDisease);
                printf("���������ƣ�ֱ�ӻس�������: ");
                getchar();
                fgets(newDisease, 50, stdin);
                newDisease[strcspn(newDisease, "\n")] = '\0';
                
                if(strlen(newDisease) > 0) {
                    // ����������Ƿ��ظ�
                    int exists = 0;
                    for(int i=0; i<diseaseCount; i++){
                        if(i != originalIndex && strcmp(diseaseDatabase[i].disease, newDisease) == 0){
                            exists = 1;
                            break;
                        }
                    }
                    if(exists){
                        printf("���󣺼��������Ѵ���\n");
                        break;
                    }
                    strcpy(diseaseDatabase[originalIndex].disease, newDisease);
                }

                // �޸Ŀ���
                char newDept[50] = {0};
                printf("\n��ǰ����: %s\n", currentDept);
                printf("�����¿��ң�ֱ�ӻس�������: ");
                fgets(newDept, 50, stdin);
                newDept[strcspn(newDept, "\n")] = '\0';

                if(strlen(newDept) > 0) {
                    // �����Ҵ���
                    int deptValid = 0;
                    for(int i=0; i<departmentCount; i++){
                        if(strcmp(departments[i].name, newDept) == 0){
                            deptValid = 1;
                            break;
                        }
                    }
                    if(!deptValid){
                        printf("���󣺿��Ҳ�����\n");
                        // �ع����������޸�
                        if(strlen(newDisease)>0) strcpy(diseaseDatabase[originalIndex].disease, currentDisease);
                        break;
                    }

                    // ����¿�������
                    int deptDiseaseCount = 0;
                    for(int i=0; i<diseaseCount; i++){
                        if(strcmp(diseaseDatabase[i].department, newDept) == 0){
                            deptDiseaseCount++;
                        }
                    }
                    if(deptDiseaseCount >= 6){
                        printf("���󣺸ÿ�������6�ּ���\n");
                        // �ع��޸�
                        if(strlen(newDisease)>0) strcpy(diseaseDatabase[originalIndex].disease, currentDisease);
                        break;
                    }

                    strcpy(diseaseDatabase[originalIndex].department, newDept);
                }

                printf("�޸ĳɹ���\n");
                break;
            }

            case 3: {
                printf("\n��ǰ�����⣺\n");
                for(int i=0; i<diseaseCount; i++){
                    printf("%d. %s -> %s\n", i+1, 
                        diseaseDatabase[i].disease,
                        diseaseDatabase[i].department);
                }

                int index;
                printf("\n��ѡ��Ҫɾ������Ŀ��ţ�0���أ�: ");
                if(scanf("%d", &index) != 1) {
                    while(getchar() != '\n');
                    printf("������Ч\n");
                    break;
                }
                if(index == 0) break;

                if(index<1 || index>diseaseCount){
                    printf("��Ч�ı��\n");
                    break;
                }

                char confirm;
                printf("ȷ��ɾ����%s����(y/����ȡ��): ", diseaseDatabase[index-1].disease);
                scanf(" %c", &confirm);
                if(tolower(confirm) != 'y') {
                    printf("������ȡ��\n");
                    break;
                }

                // ִ��ɾ��
                for(int i=index-1; i<diseaseCount-1; i++){
                    diseaseDatabase[i] = diseaseDatabase[i+1];
                }
                diseaseCount--;
                printf("ɾ���ɹ���\n");
                break;
            }

            case 4: {
                printf("\n=== �������Ҷ�Ӧ�� ===\n");
                char *mainDepartments[] = {
                    "�ڿ�-������", "�ڿ�-��Ѫ�ܿ�", "�ڿ�-������",
                    "���-�ǿ�", "���-�����", "���-�����"
                };
                const int deptCount = 6;

                // �п����
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

                // ��ӡ��ͷ
                for(int d=0; d<deptCount; d++){
                    printf("%-*s", colWidths[d], mainDepartments[d]);
                }
                printf("\n");

                // �ռ�����
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

                // ��ӡ���
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

                printf("�����������...");
                getchar();
                getchar();
                break;
            }

            case 0:
                printf("�����ϼ��˵�\n");
                break;

            default:
                printf("��Ч��ѡ��\n");
                break;
        }
    } while (choice != 0);
}
	
	void searchByDisease(int isDeveloperMode) {
	    char disease[50];
	    printf("\n�����뼲�����ƣ�0���أ�: ");
	    scanf("%s", disease);
	    
	    if(strcmp(disease, "0") == 0) return;
	
	    // ���ҿ���
	    char targetDept[50] = "";
	    for(int i=0; i<diseaseCount; i++){
	        if(strcmp(diseaseDatabase[i].disease, disease) == 0){
	            strcpy(targetDept, diseaseDatabase[i].department);
	            break;
	        }
	    }
	
	    // ����δ�ҵ����
	    if(strlen(targetDept) == 0){
	        printf("δ�ҵ���ؿ�����Ϣ��������ѯ����̨\n");
	        if(isDeveloperMode){
	            printf("[��������ʾ] ��ǰ�������������Ӷ�Ӧ��ϵ\n");
	        }
	        return;
	    }
	
	    // ��ʾ���
	    printf("\n��%s�����ڡ�%s��\n", disease, targetDept);
	    
	    // ���ҿ�����Ϣ
	    int deptIndex = -1;
	    for(int i=0; i<departmentCount; i++){
	        if(strcmp(departments[i].name, targetDept) == 0){
	            deptIndex = i;
	            break;
	        }
	    }
	
	    if(deptIndex == -1){
	        printf("��ǰû�п����ר��\n");
	        return;
	    }
	
	    // ��ʾ����ҽ��
	    printf("\n=== �Ƽ�ר�� ===\n");
	    for(int j=0; j<departments[deptIndex].doctorCount; j++){
	        Doctor doc = departments[deptIndex].doctors[j];
	        printf("%d. %s %s��%s��\n", 
	              j+1, doc.name, doc.title, doc.specialty);
	    }
	
	    // ����ģʽ�½���ԤԼ����
	    if(!isDeveloperMode){
	        int docChoice;
	        printf("\n��ѡ��ҽ���鿴���飨0���أ�: ");
	        scanf("%d", &docChoice);
	        
	        if(docChoice>0 && docChoice<=departments[deptIndex].doctorCount){
	            Doctor doc = departments[deptIndex].doctors[docChoice-1];
	            
	            // ��ʾҽ������
	            printf("\n=== ҽ������ ===\n");
	            printf("������%s\nְ�ƣ�%s\n", doc.name, doc.title);
	            printf("רҵ����%s\n", doc.specialty);
	            printf("�Һŷѣ�%.1fԪ\n", doc.fee);
	            
	            // ��ʾ��ԤԼʱ��
	            printf("\n��ԤԼʱ�䣺\n");
	            char *days[] = {"��һ","�ܶ�","����","����","����","����","����"};
	            int hasAvailable = 0;
	            for(int d=0; d<7; d++){
	                for(int t=0; t<2; t++){
	                    if(doc.appointments[d][t] >0){
	                        printf("%s %s��ʣ��%d��\n", 
	                              days[d], 
	                              t==0 ? "����" : "����", 
	                              doc.appointments[d][t]);
	                        hasAvailable = 1;
	                    }
	                }
	            }
	            if(!hasAvailable) printf("���ں�Դ����\n");
	            
	            // ����ԤԼ����
	            char confirm;
	            printf("\n�Ƿ�����ԤԼ��(y/��������): ");
	            scanf(" %c", &confirm);
	            if(tolower(confirm) == 'y'){
	                char name[50], phone[20];
	                printf("����������: ");
	                scanf("%s", name);
	                printf("��������ϵ�绰: ");
	                scanf("%s", phone);
	                
	                // �Զ�ԤԼ������õ�ʱ��
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
	                if(booked) printf("ԤԼ�ɹ���\n");
	            }
	        }
	    }
	}
	
void searchDoctor() {
    char name[50];
    printf("������Ҫ���ҵ�ҽ������: ");
    scanf("%s", name);
    
    int found = 0;
    for(int i=0; i<departmentCount; i++) {
        for(int j=0; j<departments[i].doctorCount; j++) {
            Doctor doc = departments[i].doctors[j];
            if(strcmp(doc.name, name) == 0) {
                printf("\n=== ҽ����ϸ��Ϣ ===\n");
                printf("��������: %s\n", departments[i].name);
                printf("����: %s\n", doc.name);
                printf("ְ��: %s\n", doc.title);
                printf("רҵ����: %s\n", doc.specialty);
                printf("�Һŷ�: %.1fԪ\n", doc.fee);
                printf("����: %d\n", doc.age);
                
                printf("\n��ԤԼʱ��:\n");
                char *days[] = {"��һ","�ܶ�","����","����","����","����","����"};
                int hasAvailable = 0;
                for(int d=0; d<7; d++){
                    for(int t=0; t<2; t++){
                        if(doc.appointments[d][t] >0){
                            printf("%s %s��ʣ��%d��\n", 
                                  days[d], 
                                  t==0 ? "����" : "����", 
                                  doc.appointments[d][t]);
                            hasAvailable = 1;
                        }
                    }
                }
                if(!hasAvailable) printf("��ǰ�޿��ú�Դ\n");
                
                printf("\n��������:\n");
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
        printf("δ�ҵ���Ϊ %s ��ҽ��\n", name);
    }
}	
	
	
	
	
	
	
	
void initDepartments() {
    // ��ʼ��������
    char *deptNames[] = {
        "�ڿ�-������", "�ڿ�-��Ѫ�ܿ�", "�ڿ�-������",
        "���-�ǿ�", "���-�����", "���-�����"
    };
    const int initialDeptCount = sizeof(deptNames)/sizeof(deptNames[0]);

    // Ԥ�����ҽ��������ÿ������2λ����12λ��
    char *deptDoctors[][2] = {
        {"�Ž���", "����"},    // ������
        {"��º�", "������"},    // ��Ѫ�ܿ�
        {"��־ǿ", "����Ӣ"},    // ������
        {"�ܽ���", "������"},    // �ǿ�
        {"���ǿ", "������"},    // �����
        {"�ｨ��", "����÷"}     // �����
    };

    // רҵ��������
    char *specialties[][3] = {
        {"θ��", "��Ӳ��", "����������"},
        {"���Ĳ�", "����Ӳ��", "����˥��"},
        {"����", "����", "���������Էμ���"},
        {"����", "�ؽ��û�", "�������"},
        {"��β��", "����", "θ������"},
        {"������", "��Ѫ�ܲ�", "�񾭴���"}
    };

    // ������ݳأ���������������ԣ�
    char *reviewPool[] = {
        "����ϸ�£��������", "����Ч���ã��Ƽ�", "�ǳ�������",
        "��ҩ��׼�����ﰲ�ź���", "����ʱ���Գ���ֵ��", "���������ر����",
        "��齨��ǳ�רҵ", "����������������", "������úܼ�ʱ",
        "���׼ȷ����ҩ�����Ժ�ת", "�Զ�ͯ���ߺ��а취", "���Ʒ���������Ч"
    };

    srand(time(NULL));
    
    // ��ʼ��6������
    for (int i = 0; i < initialDeptCount; i++) {
        // ��ʼ��������Ϣ
        strcpy(departments[i].name, deptNames[i]);
        departments[i].doctorCount = 2; // ÿ�����ҳ�ʼ2λҽ��

        // ��ʼ��ҽ����Ϣ
        for (int j = 0; j < 2; j++) {
            Doctor *doc = &departments[i].doctors[j];
            
            // ʹ��Ԥ��Ĺ̶�����
            strcpy(doc->name, deptDoctors[i][j]);
            
            // ����ְ�ƺͷ��ã�����ԭ��ְ�������߼���
            char *titles[] = {"����ҽʦ", "������ҽʦ", "����ҽʦ"};
            int titleIndex = (j == 0) ? 0 : (rand()%2 + 1);
            strcpy(doc->title, titles[titleIndex]);
            doc->fee = (titleIndex == 0) ? 50.0 : (titleIndex == 1) ? 30.0 : 15.0;
            
            // רҵ����
            strcpy(doc->specialty, specialties[i][j%3]);
            
            // ������䣨�������������߼���
            doc->age = (titleIndex == 0) ? 55+rand()%5 : 
                      (titleIndex == 1) ? 45+rand()%5 : 35+rand()%5;

            // ��ʼ��ԤԼʱ��
            for(int d=0; d<7; d++){
                for(int t=0; t<2; t++){
                    doc->appointments[d][t] = (rand()%3 == 0) ? 5 : 0;
                }
            }

            // ������ۣ��������������߼���
            int used[12] = {0};
            for(int k=0; k<3; k++){
                int r;
                do { r = rand()%12; } while(used[r]);
                strcpy(doc->reviews[k], reviewPool[r]);
                used[r] = 1;
            }
        }

        // ʣ��ҽ��λ�ñ��ֿ�
        for(int j=2; j<MAX_DOCTORS; j++){
            departments[i].doctors[j].name[0] = '\0';
        }
    }

    // ��ʼ��ʣ�����Ϊ��
    for(int i=initialDeptCount; i<MAX_DEPARTMENTS; i++){
        departments[i].name[0] = '\0';
        departments[i].doctorCount = 0;
    }

    departmentCount = initialDeptCount;
}
	
	/* �޸ĺ����ʾ���� */
	void showDepartmentsAndMakeAppointment() {
	    printf("\n=== �����б� ===\n");
	    for(int i=0; i<departmentCount; i++){ // ֻ��ʾʵ�ʴ��ڵĿ���
	        printf("%d. %s", i+1, departments[i].name);
	        if (departments[i].doctorCount == 0) {
	            printf(" (��δ����)");
	        }
	        printf("\n");
	    }
	    
	    int choice;
	    printf("��ѡ����Ҳ鿴ר�ң�0���أ�: ");
	    scanf("%d", &choice);
	    
	    if(choice == 0) return;
	    
	    if(choice >0 && choice <= departmentCount){
	        Department *dept = &departments[choice-1];
	        if(dept->doctorCount == 0) {
	            printf("�ÿ�����δ���ţ�����ϵҽԺ������\n");
	            return;
	        }
	        
	        printf("\n=== %sר���б� ===\n", dept->name);
	        for(int j=0; j<dept->doctorCount; j++){
	            Doctor doc = dept->doctors[j];
	            printf("%d. %s %s���Һŷѣ�%.1fԪ��\n", 
	                  j+1, doc.name, doc.title, doc.fee);
	        }
	        
	        int docChoice;
	        printf("ѡ��ҽ���鿴�����ԤԼ��0���أ�: ");
	        scanf("%d", &docChoice);
	        
	        if(docChoice == 0) return;
	        
	        if(docChoice >0 && docChoice <= departments[choice-1].doctorCount){
	            Doctor doc = departments[choice-1].doctors[docChoice-1];
	            
	            // ��ʾҽ������
	            printf("\n=== ҽ������ ===\n");
	            printf("������%s\nְ�ƣ�%s\nרҵ��%s\n", doc.name, doc.title, doc.specialty);
	            printf("�Һŷѣ�%.1fԪ\n", doc.fee);
	            
	            // ��ʾ��ԤԼʱ��
	            printf("\n��ԤԼʱ�䣺\n");
	            char *days[] = {"��һ","�ܶ�","����","����","����","����","����"};
	            int hasAvailable = 0;
	            for(int d=0; d<7; d++){
	                for(int t=0; t<2; t++){
	                    if(doc.appointments[d][t] >0){
	                        printf("%s %s��ʣ��%d��\n", 
	                              days[d], 
	                              t==0 ? "����" : "����", 
	                              doc.appointments[d][t]);
	                        hasAvailable = 1;
	                    }
	                }
	            }
	            if(!hasAvailable) printf("�������޿��ú�Դ\n");
	            
	            // ��ʾ��������
	            printf("\n�������ۣ�\n");
	            for(int k=0; k<3; k++){
	                printf("�� %s\n", doc.reviews[k]);
	            }
	
	            // ԤԼ����
	            char patientName[50], phoneNumber[20];
	            int dayOfWeek, timeOfDay;
	            
	            // ��������
	            printf("����������������0���أ�: ");
	            scanf("%s", patientName);
	            if(strcmp(patientName, "0") == 0) return;
	
	            // ����绰
	            printf("���������ĵ绰���루0���أ�: ");
	            scanf("%s", phoneNumber);
	            if(strcmp(phoneNumber, "0") == 0) return;
	
	            // ȷ��ԤԼ
	            printf("�Ƿ�����ԤԼ��(y/n/0����): ");
	            char confirm;
	            scanf(" %c", &confirm);
	            if(confirm == '0') return;
	            if(tolower(confirm) == 'n') return;
	
	            if(tolower(confirm) == 'y'){
	                // ѡ������
	                printf("ѡ��ԤԼ����(1-7��һ�����գ�0����): ");
	                if(scanf("%d", &dayOfWeek) != 1){
	                    while(getchar() != '\n');
	                    printf("������Ч\n");
	                    return;
	                }
	                if(dayOfWeek == 0) return;
	                if(dayOfWeek <1 || dayOfWeek>7){
	                    printf("��Ч������ѡ��\n");
	                    return;
	                }
	
	                // ѡ��ʱ���
	                printf("ѡ��ʱ��Σ�����0������1���������أ�: ");
	                if(scanf("%d", &timeOfDay) != 1){
	                    while(getchar() != '\n');
	                    printf("������Ч\n");
	                    return;
	                }
	                if(timeOfDay <0 || timeOfDay>1){
	                    printf("��Ч��ʱ��ѡ��\n");
	                    return;
	                }
	
	                // ִ��ԤԼ
	                makeAppointment(patientName, phoneNumber, choice, docChoice, dayOfWeek, timeOfDay);
	            }
	        } else {
	            printf("��Ч��ѡ��\n");
	        }
	    } else {
	        printf("��Ч��ѡ��\n");
	    }
	}
	// ��ǿ���makeAppointment����
	void makeAppointment(char patientName[], char phoneNumber[], int deptChoice, int docChoice, int dayOfWeek, int timeOfDay) {
	    // ���ԤԼ��ͻǰ�ȼ�鷵���ź�
	    if(strcmp(patientName, "0") == 0 || strcmp(phoneNumber, "0") == 0 || dayOfWeek == 0) {
	        printf("��ȡ��ԤԼ\n");
	        return;
	    } // ��ȫ����պ�����
	
	    // ԭ��ԤԼ�߼�
	    for (int i = 0; i < recordCount; i++) {
	        if (strcmp(appointmentRecords[i].patientName, patientName) == 0 &&
	            strcmp(appointmentRecords[i].docName, departments[deptChoice - 1].doctors[docChoice - 1].name) == 0 &&
	            appointmentRecords[i].dayOfWeek == dayOfWeek &&
	            appointmentRecords[i].timeOfDay == timeOfDay) {
	            printf("���Ѿ�����ͬ��ʱ���ԤԼ�˸�ҽ������ѡ������ʱ���ҽ����\n");
	            return;
	        }
	    }
	
	    if (departments[deptChoice - 1].doctors[docChoice - 1].appointments[dayOfWeek - 1][timeOfDay] == 0) {
	        printf("�Բ��𣬸�ʱ���������\n");
	    } else {
	        departments[deptChoice - 1].doctors[docChoice - 1].appointments[dayOfWeek - 1][timeOfDay]--;
	        strcpy(appointmentRecords[recordCount].patientName, patientName);
	        strcpy(appointmentRecords[recordCount].phoneNumber, phoneNumber);
	        strcpy(appointmentRecords[recordCount].deptName, departments[deptChoice - 1].name);
	        strcpy(appointmentRecords[recordCount].docName, departments[deptChoice - 1].doctors[docChoice - 1].name);
	        appointmentRecords[recordCount].dayOfWeek = dayOfWeek;
	        appointmentRecords[recordCount].timeOfDay = timeOfDay;
	        recordCount++;
	        printf("ԤԼ�ɹ�!\n");
	    }
	}
	
	
	
	void deleteDoctor() {
	    printf("\n=== ɾ��ҽ����Ϣ ===\n");
	    for (int i = 0; i < departmentCount; i++) {
	        printf("%d. %s\n", i + 1, departments[i].name);
	    }
	    
	    int deptChoice;
	    printf("��ѡ��Ҫ�����Ŀ���(����0����): ");
	    scanf("%d", &deptChoice);
	    
	    if(deptChoice > 0 && deptChoice <= departmentCount){
	        Department *dept = &departments[deptChoice - 1];
	        
	        printf("\n=== %sר���б� ===\n", dept->name);
	        for(int j=0; j<dept->doctorCount; j++){
	            printf("%d. %s\n", j+1, dept->doctors[j].name);
	        }
	        
	        int docChoice;
	        printf("ѡ��Ҫɾ����ҽ�����: ");
	        scanf("%d", &docChoice);
	        
	        if(docChoice > 0 && docChoice <= dept->doctorCount){
	            // ����ҽ��������ԤԼҲһ��ɾ��
	            for(int i=recordCount-1; i>=0; i--){
	                if(strcmp(appointmentRecords[i].docName, dept->doctors[docChoice-1].name) == 0){
	                    // �ƶ�����ļ�¼��ǰ����
	                    for(int j=i; j<recordCount-1; j++){
	                        appointmentRecords[j] = appointmentRecords[j+1];
	                    }
	                    recordCount--;
	                }
	            }
	
	            // �����һ��ҽ������Ϣ�ƶ�����ɾ��ҽ����λ��
	            dept->doctors[docChoice-1] = dept->doctors[dept->doctorCount-1];
	            dept->doctorCount--;
	
	            printf("ҽ����Ϣɾ���ɹ���\n");
	        } else {
	            printf("��Ч��ѡ��\n");
	        }
	    }
	}
	
	// �޸ĺ������showMenu����
	void showMenu(int mode) {
	    int choice;
	    do {
	        printf("\n=== ���˵� ===\n");
	        printf("1. �鿴���Ҳ�ԤԼ\n");
	        printf("2. �鿴ԤԼ��¼\n");
	        printf("3. ����ҽ��\n");
	        printf("4. ���������ҿ���\n");
	        
	        if(mode == 1) {
	            printf("5. �޸�ҽ����Ϣ\n");
	            printf("6. ɾ��ҽ����Ϣ\n");
	            printf("7. ���ҽ����Ϣ\n");
	            printf("8. ���������\n");
	            printf("9. ��ӿ���\n");  // �����˵���
	        }
	        
	        printf("0. �����ϼ��˵�\n");
	        printf("��ѡ�����: ");
	        scanf("%d", &choice);
	
	        switch (choice) {
	            case 1: showDepartmentsAndMakeAppointment(); break;
	            case 2: viewAppointments(); break;
	            case 3: searchDoctor(); break;
	            case 4: searchByDisease(mode); break;
	            case 5: if(mode) modifyDoctor(); else printf("��Чѡ��\n"); break;
	            case 6: if(mode) deleteDoctor(); else printf("��Чѡ��\n"); break;
	            case 7: if(mode) addDoctor(); else printf("��Чѡ��\n"); break;
	            case 8: if(mode) diseaseManagement(); else printf("��Чѡ��\n"); break;
	            case 9: if(mode) addDepartment(); else printf("��Чѡ��\n"); break; // ����case
	            case 0: break;
	            default: printf("��Ч��ѡ��\n");
	        }
	    } while (choice != 0);
	}
	
	void modifyDoctor() {
	    printf("\n=== �޸�ҽ����Ϣ ===\n");
	    
	    // ѡ�����
	    printf("��ѡ����Ҫ�޸ĵ�ҽ����������:\n");
	    for (int i = 0; i < departmentCount; i++) {
	        printf("%d. %s\n", i+1, departments[i].name);
	    }
	    int deptChoice;
	    printf("������ұ�ţ�0���أ�: ");
	    scanf("%d", &deptChoice);
	    if (deptChoice < 1 || deptChoice > departmentCount) return;
	
	    Department *dept = &departments[deptChoice-1];
	    
	    // ѡ��ҽ��
	    printf("\n=== %sר���б� ===\n", dept->name);
	    for (int j = 0; j < dept->doctorCount; j++) {
	        printf("%d. %s\n", j+1, dept->doctors[j].name);
	    }
	    int docChoice;
	    printf("����ҽ����ţ�0���أ�: ");
	    scanf("%d", &docChoice);
	    if (docChoice < 1 || docChoice > dept->doctorCount) return;
	
	    Doctor *doc = &dept->doctors[docChoice-1];
	    
	    int option;
	    do {
	        printf("\n��ѡ��Ҫ�޸ĵ���Ϣ��:\n");
	        printf("1.���� 2.���� 3.ְ�� 4.רҵ 5.�Һŷ� 6.���� 0.����\nѡ��: ");
	        scanf("%d", &option);
	
	        switch(option) {
	            case 1: {
	                printf("��ǰ����: %s\n������: ", doc->name);
	                scanf("%s", doc->name);
	                printf("�����޸ĳɹ�\n");
	                break;
	            }
	            case 2: {
	                printf("��ѡ���¿���:\n");
	                for (int i = 0; i < departmentCount; i++) {
	                    printf("%d. %s\n", i+1, departments[i].name);
	                }
	                int newDeptChoice;
	                printf("�����¿��ұ�ţ�0ȡ����: ");
	                scanf("%d", &newDeptChoice);
	                
	                if (newDeptChoice > 0 && newDeptChoice <= departmentCount) {
	                    Department *newDept = &departments[newDeptChoice-1];
	                    if (newDept->doctorCount >= MAX_DOCTORS) {
	                        printf("Ŀ���������\n");
	                        break;
	                    }
	                    
	                    // Ǩ��ҽ�����¿���
	                    newDept->doctors[newDept->doctorCount++] = *doc;
	                    
	                    // ��ԭ����ɾ��
	                    for (int j = docChoice-1; j < dept->doctorCount-1; j++) {
	                        dept->doctors[j] = dept->doctors[j+1];
	                    }
	                    dept->doctorCount--;
	                    
	                    // ����ԤԼ��¼
	                    for (int i = 0; i < recordCount; i++) {
	                        if (strcmp(appointmentRecords[i].docName, doc->name) == 0) {
	                            strcpy(appointmentRecords[i].deptName, newDept->name);
	                        }
	                    }
	                    printf("�����޸ĳɹ�\n");
	                    return; // ���ҽṹ�ѱ������Ҫ�˳�����
	                }
	                break;
	            }
	            case 3: {
	                printf("��ǰְ��: %s\n��ְ��: ", doc->title);
	                scanf("%s", doc->title);
	                printf("ְ���޸ĳɹ�\n");
	                break;
	            }
	            case 4: {
	                printf("��ǰרҵ: %s\n��רҵ: ", doc->specialty);
	                scanf("%s", doc->specialty);
	                printf("רҵ�޸ĳɹ�\n");
	                break;
	            }
	            case 5: {
	                printf("��ǰ�Һŷ�: %.1f\n�¹Һŷ�: ", doc->fee);
	                scanf("%f", &doc->fee);
	                printf("�Һŷ��޸ĳɹ�\n");
	                break;
	            }
	            case 6: {
	                printf("��ǰ����:\n");
	                for (int i = 0; i < 3; i++) {
	                    printf("%d. %s\n", i+1, doc->reviews[i]);
	                }
	                int reviewChoice;
	                printf("ѡ��Ҫ�޸ĵ����ۣ�1-3, 0ȡ����: ");
	                scanf("%d", &reviewChoice);
	                
	                if (reviewChoice >= 1 && reviewChoice <= 3) {
	                    printf("����������: ");
	                    getchar(); // ������뻺����
	                    fgets(doc->reviews[reviewChoice-1], 100, stdin);
	                    doc->reviews[reviewChoice-1][strcspn(doc->reviews[reviewChoice-1], "\n")] = 0; // ȥ�����з�
	                    printf("�����޸ĳɹ�\n");
	                }
	                break;
	            }
	            case 0:
	                break;
	            default:
	                printf("��Ч��ѡ��\n");
	        }
	    } while (option != 0);
	}
	void addDoctor() {
	    printf("\n=== ���ҽ����Ϣ ===\n");
	    for (int i = 0; i < departmentCount; i++) {
	        printf("%d. %s", i + 1, departments[i].name);
	        if (departments[i].doctorCount >= MAX_DOCTORS) {
	            printf(" (����)");
	        }
	        printf("\n");
	    }
	    
	    int deptChoice;
	    printf("��ѡ��Ҫ���ҽ���Ŀ���(����0����): ");
	    scanf("%d", &deptChoice);
	
	     if(deptChoice >0 && deptChoice <= departmentCount){
	        Department *dept = &departments[deptChoice - 1];
	        
	        if(dept->doctorCount >= MAX_DOCTORS) {
	            printf("�ÿ����Ѵ����ҽ��������%d��\n", MAX_DOCTORS);
	            return;
	        }
	        Doctor newDoc;
	        printf("������ҽ������: ");
	        scanf("%s", newDoc.name);
	        printf("������ҽ��ְ��(����ҽʦ/������ҽʦ/����ҽʦ): ");
	        scanf("%s", newDoc.title);
	        printf("������ҽ��רҵ����: ");
	        scanf("%s", newDoc.specialty);
	        printf("������Һŷ�: ");
	        scanf("%f", &newDoc.fee);
	        newDoc.age = 35 + ((strcmp(newDoc.title, "����ҽʦ") == 0) ? 0 : 
	                         (strcmp(newDoc.title, "������ҽʦ") == 0) ? 5 : 10) + rand()%5;
	
	        // ��ʼ��ԤԼʱ��
	        for(int d=0; d<7; d++){
	            for(int t=0; t<2; t++){
	                newDoc.appointments[d][t] = (rand()%2 == 0) ? 5 : 0;
	            }
	        }
	
	        // ��ʼ������
	        strcpy(newDoc.reviews[0], "���޻�������");
	        strcpy(newDoc.reviews[1], "");
	        strcpy(newDoc.reviews[2], "");
	
	        dept->doctors[dept->doctorCount++] = newDoc;
	        printf("ҽ����Ϣ��ӳɹ���\n");
	    } else if(deptChoice != 0) {
	        printf("��Ч��ѡ��\n");
	    }
	}
	
	int main() {
	    int mode;
	    int running = 1;
	
	    initDepartments(); // ��ʼ������
	
	    while (running) {
	        printf("\nѡ��ģʽ (0: ����ģʽ, 1: ������ģʽ, ���������˳�): ");
	        int result = scanf("%d", &mode);
	        if (result != 1) {
	            while (getchar() != '\n');
	            printf("�˳�����\n");
	            break;
	        }
	
	        if (mode == 0 || mode == 1) {
	            showMenu(mode); 
	        } else {
	            printf("�˳�����\n");
	            running = 0;
	        }
	    }
	    return 0;
	}
