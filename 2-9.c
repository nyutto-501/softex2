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


void sort(SRec **head, int (*compare)()){
	SRec *new_head = NULL;
	SRec *k = NULL;
	SRec *p1 = NULL;
	SRec *p2 = NULL;

	/* リスト数が 0 or 1 */
	if(*head == NULL){
		return;
	}
	if((*head)->next == NULL){
		return;
	}

	new_head = *head;
	*head = (*head)->next;
	new_head->next = NULL;

	while(*head != NULL){
		k = *head;
		p1 = new_head;
		p2 = new_head;
		if((*head)->next == NULL){
			*head = NULL;
		}
		else{
			*head = (*head)->next;
		}

		while(p2 != NULL){
			if(compare(p2, k) > 0){
				break;
			}

			p1 = p2;
			if(p2->next == NULL){
				p2 = NULL;
			}
			else{
				p2 = p2->next;
			}
		}

		if(p2 == new_head){
			new_head = k;
			k->next = p2;
		}
		else{
			p1->next = k;
			if(p2 == NULL){
				k->next = NULL;
			}
			else{
				k->next = p2;
			}
		}
	}

	*head = new_head;
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
	SRec *head = NULL;

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

	sort(&head, compare);
	output(argv[3], &head);
	return 0;
}
