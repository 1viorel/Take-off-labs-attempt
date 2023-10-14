//i think i broke my mind writing all of this
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include <errno.h>


//structures
typedef struct student {
	int id;
	char name[30];
	char university[10];
} stud;

typedef struct grade {
	float grades;
	int s_id;
} gr;
//structures

//compares for qsort
int compare_stud(const void* a, const void* b) {
    const stud* stud_a = (const stud*)a;
    const stud* stud_b = (const stud*)b;
    return strcmp(stud_a->university, stud_b->university);
}

int compare_grades(const void* a, const void* b) {
    gr* grade_a = (gr*)a;
    gr* grade_b = (gr*)b;

    if (grade_a->grades < grade_b->grades)
        return 1;
    else if (grade_a->grades > grade_b->grades)
        return -1;
    else
        return 0;
}
//compares for qsort

void GPA_calculator(int M, int N, stud *s, gr* g, int opt2) {
    int counter=1;
    char temp[10];
    char** univ;
    int counter2=0;
    float top_grades[2] = { -1.0, -1.0 };
    stud top_students[2] = { {0} };

    //sort by university
    qsort(s, N, sizeof(stud), compare_stud);

    //finds out the number of universities
    strcpy_s(temp, sizeof(temp), s[0].university);
    for (int i = 1; i < N; i++) {
        if (strcmp(temp, s[i].university) != 0) {
            counter++;
            strcpy_s(temp,sizeof(temp), s[i].university);
        }
    }
    printf("There are %d universities. \n", counter);
    //finds out the number of universities

    //sort by grade
    qsort(g, M, sizeof(gr), compare_grades);

    //dynamic alloc for  univ string array
    univ = (char**)malloc(counter * sizeof(char*));
    if (univ == NULL) {
        printf("Cannot allocate memory for universities.\n");
        exit(1);
    }
    for (int i = 0; i < counter; i++) {
        univ[i] = (char*)malloc(10 * sizeof(char));
        if (univ[i] == NULL) {
            printf("Cannot allocate memory for university name %d.\n", i);
            exit(1);
        }
    }
    //dynamic alloc for  univ string array


    //stores each student info in array (sorted by the grade)
    for (int i = 0; i < M; i++) {
        int student_id = g[i].s_id;
        stud* student = NULL;
        for (int j = 0; j < N; j++) {
            if (s[j].id == student_id) {
                student = &s[j];
                break;
            }
        }
        //transfers grade info from struct to array
        for (int i = 0; i < M; i++) {
            int student_id = g[i].s_id;
            stud* student = NULL;
            for (int j = 0; j < N; j++) {
                if (s[j].id == student_id) {
                    student = &s[j];
                    break;
                }
            }
        }
        //transfers grade info from struct to array


        //student info for function 1
        if (student != NULL) {
            int univ_i = -1;
            for (int j = 0; j < counter2; j++) {
                if (strcmp(univ[j], student->university) == 0) {
                    univ_i = j;
                    break;
                }
            }

            if (univ_i == -1) {
                //checks counter2 not to exceed the intended value
                if (counter2 >= counter) {
                    printf("Counter2 is too big! \n");
                    exit(1);
                }
                strcpy_s(univ[counter2], sizeof(univ[counter2]), student->university);
                univ_i = counter2;
                counter2++;
            }
            //student info for function 1

            //top grade tracker
            if (g[i].grades > top_grades[0]) {
                top_grades[1] = top_grades[0];
                top_students[1] = top_students[0];
                top_grades[0] = g[i].grades;
                top_students[0] = *student;
            }
            else if (g[i].grades > top_grades[1]) {
                top_grades[1] = g[i].grades;
                top_students[1] = *student;
            }
            //top grades tracker

            //to round down I made the round_down funtion, but its not working so this will do for now
            if (opt2 == 1) {
                printf("%s \t %s \t \t %.1f \n", univ[univ_i], student->name, g[i].grades);
            }
        }
    }
    
   //print the top 2 students
    if (opt2 == 2) {
        printf("Top 2 students: \n");
        printf("%s:  %s\n", top_students[0].university, top_students[0].name);
        printf("%s:  %s\n", top_students[1].university, top_students[1].name);
    }
    //print the top 2 students

} 

//displays the data
void print_struct(int N, stud *s) {
	for (int i = 0; i < N; i++) {
        printf("Student %d: \n", i+1);
		printf("ID: %d \n", s[i].id);
		printf("NAME: %s \n", s[i].name);
		printf("UNIVERSITY: %s \n", s[i].university);
        printf("\n");
	}
}

void print_struct2 (int M, gr* g) {
    for (int i = 0; i < M; i++) {
        printf("Grade %d: \n", i + 1);
        printf("GRADE: %.1f \n", g[i].grades);
        printf("ID: %d \n", g[i].s_id);
        printf("\n");
    }
}
//displays the data

//tranfer data from the txt file to the struct
void file_to_struct (int N, FILE* f, stud* s) {
	char buffer[100];
	//skips the first line, the N value is on the first line
	fgets(buffer, sizeof(buffer), f);
	//skips the first line
	
	for (int i = 0; i < N; i++) {
		if (fgets(buffer, sizeof(buffer), f) != NULL) {
			if (sscanf_s(buffer, "%d %s %s", &s[i].id, s[i].name, (unsigned int)sizeof(s[i].name),s[i].university, (unsigned int)sizeof(s[i].university)) != 3) {
				printf("Error reading line.\n");
			}
		}
	}
}

void file_to_struct2(int N, int M, FILE* f, gr* g) {
    char buffer[100];
    //skips a line, it needs to be skipped because here is where the M value is located
    fgets(buffer, sizeof(buffer), f);
    //skips a line

    for (int i = 0; i < M; i++) {
        if (fgets(buffer, sizeof(buffer), f) != NULL) {
            if (sscanf_s(buffer, "%f %d", &g[i].grades, &g[i].s_id)!=2) {
                printf("Error reading line.\n");
            }
        }
    }
}
//tranfer data from the txt file to the struct

/* Does not work in calls, to do later
float round_down(float x) {
	float round_down = floor(x * 10.0) / 10.0;
	return round_down;
}
*/

int main() {
    int N = 0, M=0, opt, opt2=0;
    char buffer[100];
    FILE* f;
    errno_t error = NULL;
    stud* s = NULL;
    gr* g = NULL;

    // file open
    error = fopen_s(&f, "input.txt", "r");
    if (error != 0 || f == NULL) {
        printf("Couldn't open file. Error: %d\n", error);
        return 1;
    }
    // file open

    // dynamic alloc for students
    fscanf_s(f, "%d", &N);
    s = malloc(N * sizeof(stud));
    if (s == NULL) {
        printf("Cannot allocate memory for students.\n");
        fclose(f);
        return 1;
    }
    // dynamic alloc for students

    // dynamic alloc for grades
    for (int i = 0; i <= N; i++) {
        fgets(buffer, sizeof(buffer), f);
    }
    fscanf_s(f, "%d", &M);
    g = malloc(M * sizeof(gr));
    if (g == NULL) {
        printf("Cannot allocate memory for grades.\n");
        fclose(f);
        return 1;
    }
    // dynamic alloc for grades
    
    //resets the file pointer
    fseek(f, 0, SEEK_SET);

    //tranfering info from files to structs
    file_to_struct(N, f, s);
    file_to_struct2(N, M, f, g);

    //menu
    do {
        printf("Student manager v1 by Soitu Viorel\n");
        printf("Press 1 for subtask 1. (Partial solution) \n");
        printf("Press 2 for subtask 2. (Partial solution) \n");
        printf("Press 3 for subtask 3. (Missing) \n");
        printf("Press 4 to display current data.\n");
        scanf_s("%d", &opt);
        if (opt > 4 || opt < 0) printf("Invalid option! \n");
        switch (opt)
        {
        case 4:
            print_struct(N, s);
            print_struct2(M, g);
            break;
        case 3:
            printf("Missing. \n");
            break;
        case 2:
            opt2 = 2;
            GPA_calculator(M, N, s, g, opt2);
            break;
        case 1:
            opt2 = 1;
            GPA_calculator(M, N, s, g, opt2);
            break;
        }
    } while (opt);
    //menu

    fclose(f);
    free(s);
    return 0;
}