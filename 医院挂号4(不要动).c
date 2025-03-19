#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define MAX_DOCTORS 2
#define MAX_DEPARTMENTS 6

typedef struct {
    char name[50];
    int age;
    char title[30];      // ְ��
    char specialty[50];  // ����רҵ
    float fee;           // �Һŷ�
    int appointments[7][2]; 
    char reviews[3][100]; // ������������
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

// ��ʼ������
// ��������
void initDepartments();
void showMenu(int mode);
void showDepartmentsAndMakeAppointment();
void viewAppointments();
void makeAppointment(char patientName[], char phoneNumber[], int deptChoice, int docChoice, int dayOfWeek, int timeOfDay);
void deleteDoctor();
void addDoctor(); // ��������������addDoctor����
void modifyDoctor();

// ��ʼ������
void initDepartments() {
    char *deptNames[MAX_DEPARTMENTS] = {
        "�ڿ�-������", "�ڿ�-��Ѫ�ܿ�", "�ڿ�-������",
        "���-�ǿ�", "���-�����", "���-�����"
    };

    char *specialties[6][3] = {
        {"θ������", "�ε�����", "����������"},       // ������
        {"���Ĳ�", "����ʧ��", "����˥��"},          // ��Ѫ�ܿ�
        {"����", "����", "���������Էμ���"},        // ������
        {"����", "�ؽ��û�", "�������"},            // �ǿ�
        {"��β��", "����", "θ������"},              // �����
        {"������", "��Ѫ�ܲ�", "�񾭴���"}           // �����
    };

    char *surnames[] = {"��", "��", "��", "��", "��", "��"};
    char *titles[] = {"����ҽʦ", "������ҽʦ", "����ҽʦ"};
    float fees[] = {50.0, 30.0, 15.0};
    char *reviewPool[] = {
        "ҽ����տ�����׼ȷ",
        "����ϸ�£�������",
        "��ҩЧ���������Ƽ�",
        "����ʱ���Գ���ֵ��",
        "�Դ����߷ǳ�����"
    };

    srand(time(NULL));
    
    for (int i = 0; i < MAX_DEPARTMENTS; i++) {
        strcpy(departments[i].name, deptNames[i]);
        
        for (int j = 0; j < MAX_DOCTORS; j++) {
            Doctor *doc = &departments[i].doctors[j];
            
            sprintf(doc->name, "%s%s", surnames[rand()%6], (j == 0) ? "����" : "ҽ��");
            strcpy(doc->specialty, specialties[i][rand()%3]);
            int titleLevel = (j == 0) ? 0 : (1 + rand()%2);
            strcpy(doc->title, titles[titleLevel]);
            doc->fee = fees[titleLevel];
            doc->age = 35 + titleLevel * 5 + rand()%5;
            
            // ��ʼ������ʱ���Ϊ����ԤԼ
            for(int d=0; d<7; d++){
                for(int t=0; t<2; t++){
                    doc->appointments[d][t] = 0;
                }
            }
            
            // �������5����ԤԼʱ��
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

// ��ʾ�����б��ṩԤԼѡ��
void showDepartmentsAndMakeAppointment() {
    printf("\n=== �����б� ===\n");
    for(int i=0; i<departmentCount; i++){
        printf("%d. %s\n", i+1, departments[i].name);
    }
    
    int choice;
    printf("��ѡ����Ҳ鿴ר�ң�0���أ�: ");
    scanf("%d", &choice);
    
    if(choice >0 && choice <= departmentCount){
        printf("\n=== %sר���б� ===\n", departments[choice-1].name);
        for(int j=0; j<departments[choice-1].doctorCount; j++){
            Doctor doc = departments[choice-1].doctors[j];
            printf("%d. %s %s���Һŷѣ�%.1fԪ��\n", 
                  j+1, doc.name, doc.title, doc.fee);
        }
        
        int docChoice;
        printf("ѡ��ҽ���鿴�����ԤԼ��0���أ�: ");
        scanf("%d", &docChoice);
        
        if(docChoice >0 && docChoice <= departments[choice-1].doctorCount){
            Doctor doc = departments[choice-1].doctors[docChoice-1];
            printf("\n=== ҽ������ ===\n");
            printf("������%s\nְ�ƣ�%s\nרҵ��%s\n", doc.name, doc.title, doc.specialty);
            printf("�Һŷѣ�%.1fԪ\n", doc.fee);
            
            printf("\n��ԤԼʱ�䣺\n");
            char *days[] = {"��һ","�ܶ�","����","����","����","����","����"};
            int hasAvailable = 0;
            for(int d=0; d<7; d++){
                for(int t=0; t<2; t++){
                    if(doc.appointments[d][t] >0){
                        printf("%s %s��ʣ��%d��\n", days[d], t==0 ? "����" : "����", doc.appointments[d][t]);
                        hasAvailable = 1;
                    }
                }
            }
            if(!hasAvailable) printf("�������޿��ú�Դ\n");
            
            printf("\n�������ۣ�\n");
            for(int k=0; k<3; k++){
                printf("�� %s\n", doc.reviews[k]);
            }

            // �ṩԤԼѡ��
            char patientName[50], phoneNumber[20];
            printf("��������������: ");
            scanf("%s", patientName);
            printf("���������ĵ绰����: ");
            scanf("%s", phoneNumber);
            int dayOfWeek, timeOfDay;
            printf("�Ƿ�����ԤԼ��(y/n): ");
            char confirm;
            scanf(" %c", &confirm); // ע��ǰ����һ���ո����Ե����з�
            
            if(confirm == 'y' || confirm == 'Y'){
                printf("ѡ��ԤԼ����(���ڼ�, ����1-7): ");
                scanf("%d", &dayOfWeek);
                printf("ѡ��ʱ���(��������0, ��������1): ");
                scanf("%d", &timeOfDay);

                makeAppointment(patientName, phoneNumber, choice, docChoice, dayOfWeek, timeOfDay);
            }
        }
    }
}

// �޸�makeAppointment����ǩ���Խ��ܸ������
void makeAppointment(char patientName[], char phoneNumber[], int deptChoice, int docChoice, int dayOfWeek, int timeOfDay) {
    // ����Ƿ�������ͬԤԼ
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

void viewAppointments() {
    if (recordCount == 0) {
        printf("û��ԤԼ��¼��\n");
    } else {
        for (int i = 0; i < recordCount; i++) {
            printf("ԤԼ��: %s, ��ϵ��ʽ: %s, ����: %s, ҽ��: %s, ʱ��: ����%d %s\n",
                   appointmentRecords[i].patientName,
                   appointmentRecords[i].phoneNumber,
                   appointmentRecords[i].deptName,
                   appointmentRecords[i].docName,
                   appointmentRecords[i].dayOfWeek,
                   appointmentRecords[i].timeOfDay == 0 ? "����" : "����");
        }
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

void showMenu(int mode) {
    int choice;
    do {
        printf("\n1. �鿴���Ҳ�ԤԼ\n2. �鿴ԤԼ��¼\n");
        if(mode == 1) {
            printf("3. �޸�ҽ����Ϣ\n4. ɾ��ҽ����Ϣ\n5. ���ҽ����Ϣ\n");
        }
        printf("0. �����ϼ��˵�\n��ѡ��: ");
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
                    printf("��Ч��ѡ��\n");
                }
                break;
            case 4:
                if(mode == 1) {
                    deleteDoctor();
                } else {
                    printf("��Ч��ѡ��\n");
                }
                break;
            case 5:
                if(mode == 1) {
                    addDoctor();
                } else {
                    printf("��Ч��ѡ��\n");
                }
                break;
            case 0:
                break;
            default:
                printf("��Ч��ѡ��\n");
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
        printf("%d. %s\n", i + 1, departments[i].name);
    }
    
    int deptChoice;
    printf("��ѡ��Ҫ���ҽ���Ŀ���(����0����): ");
    scanf("%d", &deptChoice);

    if(deptChoice > 0 && deptChoice <= departmentCount){
        Department *dept = &departments[deptChoice - 1];
        
        if(dept->doctorCount >= MAX_DOCTORS) {
            printf("�ÿ����Ѵ����ҽ���������޷���ӡ�\n");
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
        newDoc.age = 35 + ((strcmp(newDoc.title, "����ҽʦ") == 0) ? 0 : (strcmp(newDoc.title, "������ҽʦ") == 0) ? 5 : 10) + rand()%5;

        // ��ʼ������ʱ���Ϊ����ԤԼ
        for(int d=0; d<7; d++){
            for(int t=0; t<2; t++){
                newDoc.appointments[d][t] = 0;
            }
        }

        // �������5����ԤԼʱ��
        int available = 0;
        while(available < 5) {
            int day = rand()%7;
            int time = rand()%2;
            if(newDoc.appointments[day][time] == 0){
                newDoc.appointments[day][time] = 5;
                available++;
            }
        }

        strcpy(newDoc.reviews[0], "��������");
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
            // ��������֣�������뻺�������˳�
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
