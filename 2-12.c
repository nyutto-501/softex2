#include <stdio.h>
#include <stdlib.h>
#include <string.h>


typedef struct school_record SRec;
struct school_record{
    float gpa;
    int credit;
    char name[200];
    SRec *next;
    SRec *left, *right;
};


int input(char *file, SRec **head){
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
        p->left = NULL;
        p->right = NULL;

        if(strlen(p->name) >= 200){
            printf("Name is too long.");
            return 1;
        }

        if(q != NULL){
            q->next = p;
        }
        q = p;
    }

    fclose(fp_read);
    return 0;
}


void output(char *file, SRec **head){
    FILE *fp_write;
    SRec *p = *head;

    fp_write = fopen(file, "w");

    while(p != NULL){
        fprintf(fp_write, "%f ", p->gpa);
        fprintf(fp_write, "%d ", p->credit);
        fprintf(fp_write, "%s\n", p->name);
//        printf("node:%p, left:%p, right:%p \n", p, p->left, p->right);
        if(p->next == NULL){
            free(p);
            break;
        }
        p = p->next;
        free(*head);
        *head = p;
    }
    printf("Finished.");
    fclose(fp_write);
}


void sort(SRec *node, SRec **head, SRec **tail){
    if(node == NULL){
        return;
    }

    if(node->left != NULL){
        sort(node->left, head, tail);
    }

    if(*head == NULL) {
        *head = node;
    }
    if(*tail != NULL){
        (*tail)->next = node;
    }
    *tail = node;
    node->next = NULL;

    if(node->right != NULL){
        sort(node->right, head, tail);
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


void search_tree(SRec *p, SRec *node, int (*compare)()){
    if(compare(p, node) < 0){
        if(node->left == NULL){
            node->left = p;
        }
        else{
            search_tree(p, node->left, compare);
        }
    }
    else{
        if(node->right == NULL){
            node->right = p;
        }
        else{
            search_tree(p, node->right, compare);
        }
    }
}


SRec *make_tree(SRec *head, int (*compare)()){
    SRec *root = NULL;
    SRec *p = NULL;

    if(head == NULL){
        return root;
    }
    root = head;

    for(p = head->next; p != NULL; p = p->next){
        search_tree(p, root, compare);
    }

    return root;
}


int main(int argc, char *argv[]) {
    SRec *head = NULL;
    SRec *root = NULL;
    SRec *tail = NULL;

    if(input(argv[2], &head) == 1){
        return 1;
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
    else{
        printf("Command line argument is wrong.");
        return 0;
    }

    root = make_tree(head, compare);
    head = NULL;
    sort(root, &head, &tail);
    output(argv[3], &head);
    return 0;
}