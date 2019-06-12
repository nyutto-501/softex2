#include <stdio.h>
#include <stdlib.h>
#include <string.h>


typedef struct school_record SRec;
struct school_record{
    float gpa;
    int credit;
    char name[200];
    SRec *next;
};


int input(char *file, SRec **head, SRec **tail){
    FILE *fp_read;
    SRec *p, *q;
    float gpa = 0;
    p = NULL;
    q = NULL;

    if((fp_read = fopen(file, "r")) == NULL){
        printf("There is no file.");
        return 1;
    }

    while(1){
        if(fscanf(fp_read, "%f", &gpa) == EOF){
            break;
        }

        if((p = malloc(sizeof(SRec))) == NULL){
            printf("Fail to get memory.");
        }

        if(*head == NULL){
            *head = p;
        }

        p->gpa = gpa;
        fscanf(fp_read, "%d", &(p->credit));
        fscanf(fp_read, "%s", p->name);
        p->next = NULL;

        if(strlen(p->name) >= 200){
            printf("Name is too long.");
            return 1;
        }

        if(q != NULL){
            q->next = p;
        }
        q = p;
    }
    *tail = p;

    float k;
    if(fscanf(fp_read, "%f", &k) != EOF){
        printf("There are many datas.");
        return 1;
    }
    fclose(fp_read);
    return 0;
}


void output(char *file, SRec **student){
    FILE *fp_write;
    SRec *p = *student;

    fp_write = fopen(file, "w");

    while(1){
        fprintf(fp_write, "%f ", p->gpa);
        fprintf(fp_write, "%d ", p->credit);
        fprintf(fp_write, "%s\n", p->name);
        if(p->next == NULL){
            free(p);
            break;
        }
        p = p->next;
        free(*student);
        *student = p;
    }
    printf("Finished.");
    fclose(fp_write);
}


//void sort(SRec **student, int (*compare)()){
//    SRec tmp;
//    int i, k, m;
//
//    for(k = 1; k < stu_num; k++){
//        for(m = 0; m < k; m++){
//            if(compare((*student)[m], (*student)[k]) > 0){
//                break;
//            }
//        }
//        tmp = (*student)[k];
//        for(i = k; i > m; i--){
//            (*student)[i] = (*student)[i-1];
//        }
//        (*student)[m] = tmp;
//    }
//}


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
    SRec *head = NULL;
    SRec *tail = NULL;

    if(input(argv[2], &head, &tail) == 1){
        return 1;
    }

//    int (*compare)();
//
//    if(strcmp("gpa", argv[1]) == 0){
//        compare = compare_gpa;
//        printf("You choose gpa.\n");
//    }
//    else if(strcmp("credit", argv[1]) == 0){
//        compare = compare_credit;
//        printf("You choose credit.\n");
//    }
//    else if(strcmp("name", argv[1]) == 0){
//        compare = compare_name;
//        printf("You choose name.\n");
//    }
//    else{
//        printf("Command line argument is wrong.");
//        return 0;
//    }
//
//    sort(&head, compare);
    output(argv[3], &head);
//    free();
    return 0;
}