#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int i = 0;
int j;
int *n;

typedef struct student_records{
    float gpa;
    int credit;
    char name[200];
}srec;

srec *student;

int reading(char *file){                     //ファイル読み込み
    FILE *fp_read;
    int number;                             //データの個数

    if((fp_read = fopen(file, "r")) == NULL){    //(エラー処理) ファイルがあるかどうか
            printf("There is no file.");
            return 0;
        }

    fscanf(fp_read, "%d", &number);
    if((student = calloc(number,sizeof(srec))) == NULL){      //(エラー処理) メモリの確保
        printf("Fail to get memory.");
    }

    while(i < number){
    	if(feof(fp_read)){                                //(エラー処理) データの個数が違う
    		printf("There are few datas.");
    		return 0;
    	}

        fscanf(fp_read, "%f", &student[i].gpa);
        fscanf(fp_read, "%d", &student[i].credit);
        fscanf(fp_read, "%s", student[i].name);

        if(strlen(student[i].name) >= 200){              //(エラー処理) 名前の長さが200文字以上
        	printf("Name is too long.");
        	return 0;
        }

        i++;
    }

    float k;
    if(fscanf(fp_read, "%f", &k) != EOF){
    	printf("There are many datas.");
    	return 0;
    }

    fclose(fp_read);

    return 1;
}


void writing(char *file){                   //ファイル書き出し
    FILE *fp_write;

    fp_write = fopen(file, "w");

    for(i = 0; i != j; i++){
        fprintf(fp_write, "%f\n", student[i].gpa);
        fprintf(fp_write, "%d\n", student[i].credit);
        fprintf(fp_write, "%s\n", student[i].name);
    }

    printf("Finished.");

    fclose(fp_write);
}

int compare_gpa(const void *c1, const void *c2){
    srec stu1 = *(srec *)c1;
    srec stu2 = *(srec *)c2;

    double x = stu1.gpa;
    double y = stu2.gpa;

    return x - y;
}

int compare_credit(const void *c1, const void *c2){
	srec stu1 = *(srec *)c1;
	srec stu2 = *(srec *)c2;

	int x = stu1.credit;
	int y = stu2.credit;

	return x - y;
}

int compare_name(const void *c1, const void *c2){
	srec stu1 = *(srec *)c1;
	srec stu2 = *(srec *)c2;

	return strcmp(stu1.name, stu2.name);
}

int main(int argc, char *argv[]) {
    if(reading(argv[2]) == 0){
        return 0;
    }

    j = i;

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

    qsort(student, i, sizeof(srec), compare);

    writing(argv[3]);

    free(n);

    return 0;
}
