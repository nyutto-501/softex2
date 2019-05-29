#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct student_records{
    float gpa;
    int credit;
    char name[200];
}SRec;



int reading(char *file, int *number, SRec **student){                     //ファイル読み込み
    FILE *fp_read;
    int i=0;

    if((fp_read = fopen(file, "r")) == NULL){    //(エラー処理) ファイルがあるかどうか
            printf("There is no file.");
            return 0;
        }

    fscanf(fp_read, "%d", number);
    if((*student = calloc(*number,sizeof(SRec))) == NULL){      //(エラー処理) メモリの確保
        printf("Fail to get memory.");
    }

    while(i < *number){
        if(feof(fp_read)){                                //(エラー処理) データの個数が少ない
            printf("There are few datas.");
            return 0;
        }

        fscanf(fp_read, "%f", &((*student + i)->gpa));
        fscanf(fp_read, "%d", &((*student + i)->credit));
        fscanf(fp_read, "%s", (*student + i)->name);

        if(strlen((*student + i)->name) >= 200){              //(エラー処理) 名前の長さが200文字以上
            printf("Name is too long.");
            return 0;
        }

        i++;
    }

    float k;
    if(fscanf(fp_read, "%f", &k) != EOF){                  //(エラー処理)データの数が多い
        printf("There are many datas.");
        return 0;
    }

    fclose(fp_read);

    return 1;
}


void writing(char *file, int number, SRec *student){                   //ファイル書き出し
    FILE *fp_write;
    int i;

    fp_write = fopen(file, "w");

    fprintf(fp_write, "%d\n", number);

    for(i = 0; i != number; i++){
        fprintf(fp_write, "%f\n", (student + i)->gpa);
        fprintf(fp_write, "%d\n", (student + i)->credit);
        fprintf(fp_write, "%s\n", (student + i)->name);
    }

    printf("Finished.");

    fclose(fp_write);
}

void sort(SRec **student, int number, int (*compare)(const void *c1, const void *c2)){
    SRec tmp;
    int k, l;
    int x = number - 1;

    for(k = 0;x > 0;x--){
        for(l = 1;l <= x;l++){     //大小比較
            if(compare(*student + k, *student + l) < 0){
                k = l;
            }
        }

        l--;                  //lの調整

        tmp = *(*student + k);            //入れ替え
        *(*student + k) = *(*student + l);
        *(*student + l) = tmp;

        k = 0;              //kの調整
    }
}

int compare_gpa(const void *c1, const void *c2){
    SRec stu1 = *(SRec *)c1;
    SRec stu2 = *(SRec *)c2;

    float x = stu1.gpa;
    float y = stu2.gpa;

    if((x-y)> 0) return 1;
    else if((x-y)< 0) return -1;
    else return 0;
}

int compare_credit(const void *c1, const void *c2){
    SRec stu1 = *(SRec *)c1;
    SRec stu2 = *(SRec *)c2;

    int x = stu1.credit;
    int y = stu2.credit;

    return x - y;
}

int compare_name(const void *c1, const void *c2){
    SRec stu1 = *(SRec *)c1;
    SRec stu2 = *(SRec *)c2;

    return strcmp(stu1.name, stu2.name);
}

int main(int argc, char *argv[]) {
    int number = 0;                             //データの個数
    SRec student_data;
    SRec *student;

    student = &student_data;

    if(reading(argv[2], &number, &student) == 0){
        return 0;
    }

    int (*compare)();

    if(strcmp("gpa", argv[1]) == 0){
        compare = compare_gpa;
        printf("You choose gpa.\n");
    }
    else if(strcmp("credit", argv[1]) == 0){
        compare = compare_credit;
        printf("You choose credit.\n");
    }
    else if(strcmp("name", argv[1]) == 0){
        compare = compare_name;
        printf("You choose name.\n");
    }
    else{                                                       //(エラー処理)
        printf("Command line argument is wrong.");
        return 0;
    }

    sort(&student, number, compare);

    writing(argv[3], number, student);

    free(student);

    return 0;
}
