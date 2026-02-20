#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_LINE 100
#define MOV_LINE_LENGTH 7
#define MAX_REGISTERS 4
#define SMALL_LETTER_SHIFTING 97
#define MAX_REGISTER_LENGTH 10
#define MAX_LOGICAL_OPERATORS 3
#define MAX_ACTION_LENGTH 10
#define MAX_DATA_OPERATORS 4
#define MAX_SHIFT_OPERATORS 2
#define SHIFT_OP_MAX_SIZE 3

void handle_mov_instruction(char *line, char **register_database) {
    char *word = strtok(line, " ");
    char *destination_operand = word;

    word = strtok(NULL, " ");
    word = strtok(NULL, " ");
    char *source_operand = word;
    source_operand[strlen(source_operand) - 2] = 0;

    if (source_operand[strlen(source_operand) - 1] == ';') {
        source_operand[strlen(source_operand) - 1] = 0;
    }
    if (atoi(source_operand)) {
        printf("MOV %s, %s\n", register_database[(int)destination_operand[0] - SMALL_LETTER_SHIFTING], source_operand);
    } else {
        printf("MOV %s, %s\n", register_database[(int)destination_operand[0] - SMALL_LETTER_SHIFTING],
                             register_database[(int)source_operand[0] - SMALL_LETTER_SHIFTING]);
    }
}

void handle_logical_instruction(char *line, char **register_database) {
    char *word = strtok(line, " ");
    char *destination_operand = word;

    word = strtok(NULL, " ");
    word = strtok(NULL, " ");

    char *first_operand = word;
    word = strtok(NULL, " ");
    char *operator = word;
    word = strtok(NULL, " ");
    
    char* second_operator = word;
    char *action = calloc(MAX_ACTION_LENGTH, 1);
    
    if (operator[0] == '|') {
        strcpy(action, "OR");
    } else if (operator[0] == '&') {
        strcpy(action, "AND");
    } else {
        strcpy(action, "XOR");
    }
    
    second_operator[strlen(second_operator) - 2] = 0;

    if (second_operator[strlen(second_operator) - 1] == ';') {
        second_operator[strlen(second_operator) - 1] = 0;
    }
    if (atoi(second_operator)) {
        printf("%s %s, %s\n", action, register_database[(int)destination_operand[0] - SMALL_LETTER_SHIFTING],
                                     second_operator);
    } else {
        if (second_operator[0] == destination_operand[0]) {
            printf("%s %s, %s\n", action, register_database[(int)destination_operand[0] - SMALL_LETTER_SHIFTING],
            register_database[(int)first_operand[0] - SMALL_LETTER_SHIFTING]);
        } else {
            printf("%s %s, %s\n", action, register_database[(int)destination_operand[0] - SMALL_LETTER_SHIFTING],
                    register_database[(int)second_operator[0] - SMALL_LETTER_SHIFTING]);
        }
    }
    
    free(action);
    action = NULL;
}

void handle_data_instruction(char *line, char **register_database) {
    char *word = strtok(line, " ");
    char *destination_operand = word;

    word = strtok(NULL, " ");
    word = strtok(NULL, " ");

    char *first_operand = word;
    word = strtok(NULL, " ");
    char *operator = word;
    word = strtok(NULL, " ");
    
    char* second_operator = word;
    char *action = calloc(MAX_ACTION_LENGTH, 1);

    if (operator[0] == '+') {
        strcpy(action, "ADD");
    } else if (operator[0] == '-') {
        strcpy(action, "SUB");
    } else if (operator[0] == '*') {
        strcpy(action, "MUL");
    } else {
        strcpy(action, "DIV");
    }

    second_operator[strlen(second_operator) - 2] = 0;
    if (second_operator[strlen(second_operator) - 1] == ';') {
        second_operator[strlen(second_operator) - 1] = 0;
    }

    if (!strcmp(action, "ADD") || !strcmp(action, "SUB")) {
        if (atoi(second_operator)) {
            printf("%s %s, %s\n", action, register_database[(int)first_operand[0] - SMALL_LETTER_SHIFTING],
                                         second_operator);
        } else {
            if (second_operator[0] == destination_operand[0]) {
                printf("%s %s, %s\n", action, register_database[(int)destination_operand[0] - SMALL_LETTER_SHIFTING],
                        register_database[(int)first_operand[0] - SMALL_LETTER_SHIFTING]);
            } else {
                printf("%s %s, %s\n", action, register_database[(int)first_operand[0] - SMALL_LETTER_SHIFTING],
                        register_database[(int)second_operator[0] - SMALL_LETTER_SHIFTING]);
            }
        }
    } else {
        if (second_operator[0] == '0' || atoi(second_operator)) {
            if (second_operator[0] == '0' && !strcmp(action, "DIV")) {
                printf("Error\n");
            } else {
                if (strcmp("eax", register_database[(int)first_operand[0] - SMALL_LETTER_SHIFTING])) {
                    printf("MOV eax, %s\n", register_database[(int)first_operand[0] - SMALL_LETTER_SHIFTING]);
                    printf("%s %s\n", action, second_operator);
                    printf("MOV %s, eax\n", register_database[(int)first_operand[0] - SMALL_LETTER_SHIFTING]);
                } else {
                    printf("%s %s\n", action, second_operator);
                }
            }
        } else {
            printf("MOV eax, %s\n", register_database[(int)first_operand[0] - SMALL_LETTER_SHIFTING]);
            printf("%s %s\n", action, register_database[(int)second_operator[0] - SMALL_LETTER_SHIFTING]);
            printf("MOV %s, eax\n", register_database[(int)first_operand[0] - SMALL_LETTER_SHIFTING]);
        }
    }

    free(action);
}

void handle_shift_instruction(char *line, char **register_database) {
    char *word = strtok(line, " ");
    char *destination_operand = word;

    word = strtok(NULL, " ");
    word = strtok(NULL, " ");

    char *first_operand = word;
    word = strtok(NULL, " ");
    char *operator = word;
    word = strtok(NULL, " ");
    
    char* second_operator = word;
    char *action = calloc(MAX_ACTION_LENGTH, 1);

    if (!strcmp(operator, "<<")) {
        strcpy(action, "SHL");
    } else {
        strcpy(action, "SHR");
    }

    second_operator[strlen(second_operator) - 2] = 0;
    if (second_operator[strlen(second_operator) - 1] == ';') {
        second_operator[strlen(second_operator) - 1] = 0;
    }

    if (second_operator[0] == '0' || atoi(second_operator)) {
        printf("%s %s, %s\n", action, register_database[(int)destination_operand[0] - SMALL_LETTER_SHIFTING],
                                     second_operator);
    } else {
        if (second_operator[0] == destination_operand[0]) {
            printf("%s %s, %s\n", action, register_database[(int)destination_operand[0] - SMALL_LETTER_SHIFTING],
                    register_database[(int)first_operand[0] - SMALL_LETTER_SHIFTING]);
        } else {
            printf("%s %s, %s\n", action, register_database[(int)destination_operand[0] - SMALL_LETTER_SHIFTING],
                    register_database[(int)second_operator[0] - SMALL_LETTER_SHIFTING]);
        }
    }

    free(action);
}

void handle_if_instruction(char *line, char **registers) {
    char *word = strtok(line, " ");
    word = strtok(NULL, " ");

    char first_operand = word[1];
    word = strtok(NULL, " ");

    char *sign = word;
    word = strtok(NULL, " ");

    char *second_operand = word;
    second_operand[strlen(second_operand) - 1] = 0;

    char *first_action = calloc(MAX_ACTION_LENGTH, 1);
    char *second_action = calloc(MAX_ACTION_LENGTH, 1);
    strcpy(first_action, "CMP");

    if (!strcmp(sign, "==")) {
        strcpy(second_action, "JNE");
    } else if (!strcmp(sign, "<=")) {
        strcpy(second_action, "JG");
    } else if (!strcmp(sign, "<")) {
        strcpy(second_action, "JGE");
    } else if (!strcmp(sign, ">=")) {
        strcpy(second_action, "JL");
    } else {
        strcpy(second_action, "JLE");
    }

    if (second_operand[0] == '0' || atoi(second_operand)) {
        printf("%s %s, %s\n", first_action,  registers[(int)first_operand - SMALL_LETTER_SHIFTING], second_operand);
        printf("%s end_label\n", second_action);
    } else {
        printf("%s %s, %s\n", first_action,  registers[(int)first_operand - SMALL_LETTER_SHIFTING], 
                                    registers[(int)second_operand[0] - SMALL_LETTER_SHIFTING]);
        printf("%s end_label\n", second_action);
    }

    free(first_action);
    first_action = NULL;

    free(second_action);
    second_action = NULL;
}

void handle_while_instruction(char* line, char **registers) {
    char *word = strtok(line, " ");
    word = strtok(NULL, " ");

    char *first_op = word;
    char first_operator = first_op[1];

    word = strtok(NULL, " ");
    char *sign = word;
    word = strtok(NULL, " ");

    char *second_op = word;
    second_op[strlen(second_op) - 1] = 0;
    
    char *first_action = calloc(MAX_ACTION_LENGTH, 1);
    char *second_action = calloc(MAX_ACTION_LENGTH, 1);
    strcpy(first_action, "CMP");

    if (!strcmp(sign, "==")) {
        strcpy(second_action, "JNE");
    } else if (!strcmp(sign, "<=")) {
        strcpy(second_action, "JG");
    } else if (!strcmp(sign, "<")) {
        strcpy(second_action, "JGE");
    } else if (!strcmp(sign, ">=")) {
        strcpy(second_action, "JL");
    } else {
        strcpy(second_action, "JLE");
    }

    printf("start_loop:\n");

    if (second_op[0] == '0' || atoi(second_op)) {
        printf("%s %s, %s\n", first_action,  registers[(int)first_operator - SMALL_LETTER_SHIFTING], second_op);
        printf("%s end_label\n", second_action);
    } else {
        printf("%s %s, %s\n", first_action,  registers[(int)first_operator - SMALL_LETTER_SHIFTING], 
                                    registers[(int)second_op[0] - SMALL_LETTER_SHIFTING]);
        printf("%s end_label\n", second_action);
    }

    free(first_action);
    free(second_action);
}

char handle_for_instruction(char *line, char **registers_database) {
    char *word = strtok(line, " ");
    word = strtok(NULL, " ");

    char *first_op = word;
    char first_operator = first_op[1];

    word = strtok(NULL, " ");
    word = strtok(NULL, " ");

    word[strlen(word) - 1] = 0;
    int initial_value = atoi(word);

    word = strtok(NULL, " ");
    word = strtok(NULL, " ");

    char *second_operator = word;

    word = strtok(NULL, " ");
    word[strlen(word) - 1] = 0;
    int final_value = atoi(word);

    word = strtok(NULL, " ");

    
    char *first_action = calloc(MAX_ACTION_LENGTH, 1);
    char *second_action = calloc(MAX_ACTION_LENGTH, 1);
    char *third_action = calloc(MAX_ACTION_LENGTH, 1);
    
    strcpy(first_action, "MOV");
    strcpy(second_action, "CMP");

    if (!strcmp(second_operator, "==")) {
        strcpy(third_action, "JNE");
    } else if (!strcmp(second_operator, "<=")) {
        strcpy(third_action, "JG");
    } else if (!strcmp(second_operator, "<")) {
        strcpy(third_action, "JGE");
    } else if (!strcmp(second_operator, ">=")) {
        strcpy(third_action, "JL");
    } else {
        strcpy(third_action, "JLE");
    }
    
    printf("%s %s, %d\n", first_action, registers_database[(int)first_operator - SMALL_LETTER_SHIFTING], initial_value);
    printf("start_loop:\n");
    printf("%s %s, %d\n", second_action, registers_database[(int)first_operator - SMALL_LETTER_SHIFTING], final_value);
    printf("%s end_label\n", third_action);

    free(first_action);
    first_action = NULL;
    
    free(second_action);
    second_action = NULL;

    free(third_action);
    third_action = NULL;

    return first_operator;
}

char *create_logical_operators_table() {
    char *logical_operators = calloc(MAX_LOGICAL_OPERATORS + 1, 1);
    logical_operators[0] = '&';
    logical_operators[1] = '|';
    logical_operators[2] = '^';

    return logical_operators;
}

/**
 * creates an array of data operators specific to the C language
 */

char *create_data_operators() {
    char *data_operators = calloc(MAX_DATA_OPERATORS, 1);
    data_operators[0] = '+';
    data_operators[1] = '-';
    data_operators[2] = '/';
    data_operators[3] = '*';

    return data_operators;
}

/**
 * creates an array of shifting operators specific to this language
 */

char **create_shift_operators() {
    char **shift_operators = calloc(MAX_SHIFT_OPERATORS, sizeof(char *));

    shift_operators[0] = calloc(SHIFT_OP_MAX_SIZE, 1);
    shift_operators[1] = calloc(SHIFT_OP_MAX_SIZE, 1);

    strcpy(shift_operators[0], "<<");
    strcpy(shift_operators[1], ">>");

    return shift_operators;
}

int is_logical_operator(char *logical_operators, char sign) {
    for (int i = 0; i < MAX_LOGICAL_OPERATORS; i++) {
        if (logical_operators[i] == sign) {
            return 1;
        }
    }

    return 0;
}

int is_data_operator(char *data_operators, char sign) {
    for (int i = 0; i < MAX_DATA_OPERATORS; i++) {
        if (data_operators[i] == sign) {
            return 1;
        }
    }

    return 0;
}

int is_shift_operator(char **shift_operators, char *operator) {
    for (int i = 0; i < MAX_SHIFT_OPERATORS; i++) {
        if (!strcmp(shift_operators[i], operator)) {
            return 1;
        }
    }

    return 0;
}

/**
 * responsible for freeing all memory resources used by this program
 */
void free_resources(char **registers, char *logical_operators, char *data_operators, char **shift_operators) {
    free(logical_operators);
    logical_operators = NULL;

    free(data_operators);
    data_operators = NULL;

    for (int i = 0; i < MAX_REGISTERS; i++) {
        free(registers[i]);
        registers[i] = NULL;
    }

    for (int i = 0; i < MAX_SHIFT_OPERATORS; i++) {
        free(shift_operators[i]);
        shift_operators[i] = NULL;
    }

    free(shift_operators);
    shift_operators = NULL;

    free(registers);
    registers = NULL;
}


char **create_registers_database() {
    char **register_database = calloc(MAX_REGISTERS, sizeof(*register_database));
    for (int i = 0; i < MAX_REGISTERS; i++) {
        register_database[i] = calloc(MAX_REGISTER_LENGTH, 1);
        register_database[i][0] = 'e';
        register_database[i][2] = 'x';
        register_database[i][1] = (char)(i + SMALL_LETTER_SHIFTING);
    }

    return register_database;
}

int main() {
    char *line = calloc(MAX_LINE, 1);
    char *duplicate_line = calloc(MAX_LINE, 1);
    char **register_database = create_registers_database();
    char *logical_operators = create_logical_operators_table();
    char *data_operators = create_data_operators();
    char **shift_operators = create_shift_operators();
    char *auxiliar_line = calloc(MAX_LINE, 1);

    int inside_if = 0, inside_while = 0, inside_for = 0;
    char saved_register = 'a';

    while (fgets(line, 99, stdin) != NULL) {
        if (line[0] == '\t') {
            auxiliar_line[0] = 0;
            int first_space = 1, index = 0;
            int has_newline = (line[strlen(line) - 1] == '\n');
        
            for (unsigned int i = 0; i < strlen(line); i++) {
                if (line[i] != '\t') {
                    first_space = 0;
                }
        
                if (!first_space) {
                    auxiliar_line[index] = line[i];
                    index++;
                }
            }
        
            if (has_newline) {
                auxiliar_line[index] = '\n';
                index++;
            }
        
            auxiliar_line[index] = '\0';
        
            strcpy(line, auxiliar_line);
        }

        strcpy(duplicate_line, line);
        if (duplicate_line[0] == '}') {
            if (inside_if == 1) {
                inside_if = 0;
                printf("end_label:\n");
            } else if (inside_while == 1) {
                inside_while = 0;
                printf("JMP start_loop\n");
                printf("end_label:\n");
            } else if (inside_for == 1) {
                inside_for = 0;
                printf("ADD %s, 1\n", register_database[saved_register - SMALL_LETTER_SHIFTING]);
                printf("JMP start_loop\n");
                printf("end_loop:\n");
            }

            continue;
        }
        char *word = strtok(duplicate_line, " ");

        if (!strcmp(word, "if")) {
            handle_if_instruction(line, register_database);
            inside_if = 1;
            continue;
        } else if (!strcmp(word, "while")) {
            handle_while_instruction(line, register_database);
            inside_while = 1;
            continue;
        } else if (!strcmp(word, "for")) {
            saved_register = handle_for_instruction(line, register_database);
            inside_for = 1;
            continue;
        }

        word = strtok(NULL, " ");
        word = strtok(NULL, " ");
        word = strtok(NULL, " ");
        
        if (!word) {
            // we have a simple attribution
            handle_mov_instruction(line, register_database);
        } else if (is_logical_operator(logical_operators, word[0])) {
            handle_logical_instruction(line, register_database);
        } else if (is_data_operator(data_operators, word[0])) {
            handle_data_instruction(line, register_database);
        } else if (is_shift_operator(shift_operators, word)) {
            handle_shift_instruction(line, register_database);
        }

        line[0] = 0;
        duplicate_line[0] = 0;
        auxiliar_line[0] = 0;
    }

    free(line);
    free(duplicate_line);
    free(auxiliar_line);

    free_resources(register_database, logical_operators, data_operators, shift_operators);
    return 0;
}