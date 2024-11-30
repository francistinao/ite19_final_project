// By Francis Tin-ao
// 13 user-defined functions
// 2 user-defined structures
// 3 user-defined constant macro

#include <stdio.h>  // input/output library for basinc I/O operations
#include <stdlib.h> // standard library for memory allocation and deallocation

#define DECIMAL_BASE 10   // constant macro for decimal base since we are converting to decimal
#define MAX_EQUATIONS 100 // constant macro for the maximum number of equations, initialized to 100 for now
#define MAX_LENGTH 50     // max number of characters in a single equation

// dictionary or object of english notation of the numbers
const char *ones[] = {"", "One", "Two", "Three", "Four", "Five", "Six", "Seven", "Eight", "Nine"};
const char *teens[] = {"Ten", "Eleven", "Twelve", "Thirteen", "Fourteen", "Fifteen", "Sixteen", "Seventeen", "Eighteen", "Nineteen"};
const char *tens[] = {"", "", "Twenty", "Thirty", "Forty", "Fifty", "Sixty", "Seventy", "Eighty", "Ninety"};
const char *thousands[] = {"", "Thousand", "Million", "Billion"};

// to a more organized and coherent code, we will define the structures and functions first before the main function
// utilizing linked list to store the roman numerals and equations
typedef struct Node
{
    char value;        // the character value of the node which is the roman numeral 'I', 'V', 'X', 'L', 'C', 'D', 'M'
    struct Node *next; // pointer
} Node;

// structure for the equation to combine the operands and the operator
typedef struct Equation
{
    Node *operand1;        // pointer to the first operand (linked list of characters)
    Node *operand2;        // pointer to the second operand (linked list of characters)
    char operator;         // The operator character (+, -, *, /)
    struct Equation *next; // pointer to the next equation in the linked list
} Equation;

// function to create a new node with a given character value
Node *create_node(char value)
{
    Node *new_node = (Node *)malloc(sizeof(Node)); // allocate memory for the new node
    new_node->value = value;                       // set the value of the node
    new_node->next = NULL;                         // initialize the next pointer to NULL
    return new_node;                               // return the new node
}

// Function to create a new equation
Equation *create_equation()
{
    Equation *new_equation = (Equation *)malloc(sizeof(Equation)); // allocate memory for the new equation
    new_equation->operand1 = NULL;                                 // initialize the first operand to NULL
    new_equation->operand2 = NULL;                                 // initialize the second operand to NULL
    new_equation->operator= '\0';                                  // initialize the operator to the null character
    new_equation->next = NULL;                                     // initialize the next pointer to NULL
    return new_equation;                                           // return the new equation
}

int is_valid_roman_numeral(char *roman_numeral)
{
    int i = 0; // initialize the index for the Roman numeral
    while (roman_numeral[i] != '\0')
    { // iterate to the charcaters one by one in the Roman numeral
        switch (roman_numeral[i])
        { // character checking
        case 'I':
        case 'V':
        case 'X':
        case 'L':
        case 'C':
        case 'D':
        case 'M':
        case ' ':
        case '+':
        case '-':
        case '*':
        case '/':
        case '%':
        case '^':
        case '\n':
            break; // if it passes these cases, its a valid roman numeral characters
        default:
            return 0; // else invalid Roman numeral character
        }
        i++; // move to the next character
    }
    return 1; // thus, all characters are valid Roman numerals
}

// function to append a node with a given character value to the end of a linked list
void append_node(Node **head, char value)
{
    Node *new_node = create_node(value); // create a new node with the given value
    if (*head == NULL)
    {                     // check if list is empty
        *head = new_node; // set the new node as the head of the list
    }
    else
    {                          // if the list is not empty
        Node *current = *head; // start at the head of the list
        while (current->next != NULL)
        {                            // traverse the list until the end
            current = current->next; // move to the next node
        }
        current->next = new_node; // append the new node to the end of the list
    }
}

// function to read equations from a file and store them in an array
void read_roman_numeral_equation_from_file(char equations[MAX_EQUATIONS][MAX_LENGTH])
{
    FILE *file = fopen("input.txt", "r"); // open the file in reading type
    if (file == NULL)
    {                                                            // if the file could not be opened
        printf("Error: Could not open file. File not found.\n"); // just display an error message
        exit(1);                                                 // exit the program with an error code
    }

    int i = 0; // Initialize the index for the equations array
    while (i < MAX_EQUATIONS && fgets(equations[i], MAX_LENGTH, file) != NULL)
    {                // read each line from the file
        int len = 0; // initialize the length of the current equation
        while (equations[i][len] != '\0')
        { // Traverse the characters in the equation

            // if the roman numeral is in lower case, convert it to upper case
            if (equations[i][len] >= 'a' && equations[i][len] <= 'z')
            {
                equations[i][len] = equations[i][len] - 'a' + 'A';
            }

            // if the roman numeral is not a valid roman numeral, print an error message and exit the program
            if (!is_valid_roman_numeral(equations[i]))
            {
                printf(
                    "Error: Invalid roman numeral at position %d in %d%s equation.\n",
                    len,
                    i + 1,
                    (i + 1) % 10 == 1 && (i + 1) % 100 != 11 ? "st" : (i + 1) % 10 == 2 && (i + 1) % 100 != 12 ? "nd"
                                                                  : (i + 1) % 10 == 3 && (i + 1) % 100 != 13   ? "rd"
                                                                                                               : "th");
                exit(1);
            }

            if (equations[i][len] == '\n')
            {                             // if a newline character is found
                equations[i][len] = '\0'; // replace it with the null character
                break;                    // exit the loop
            }
            len++; // move to the next character
        }
        i++; // move to the next equation
    }

    fclose(file); // close the file to avoid memory leaks
}

// function to format equations into a linked list of Equation structs
void format_equations(char equations[MAX_EQUATIONS][MAX_LENGTH], Equation **equation_list)
{
    for (int i = 0; i < MAX_EQUATIONS && equations[i][0] != '\0'; i++)
    {                                                       // iterate through each equation
        Equation *new_equation = create_equation();         // create a new equation
        int eq_index = 0;                                   // initialize the index for the current equation
        Node **current_operand = &(new_equation->operand1); // start with the first operand

        while (equations[i][eq_index] != '\0')
        {                                     // traverse the characters in the equation
            char ch = equations[i][eq_index]; // get the current character
            if (ch == ' ')
            {               // if the character is a space
                eq_index++; // move to the next character
                continue;   // skip the space
            }

            if (ch == '+' || ch == '-' || ch == '*' || ch == '/')
            {                                                // if the character is an operator
                new_equation->operator= ch;                  // set the operator of the equation
                current_operand = &(new_equation->operand2); // switch to the second operand
                eq_index++;                                  // move to the next character
                continue;                                    // skip the operator
            }

            append_node(current_operand, ch); // append the character to the current operand
            eq_index++;                       // move to the next character
        }

        new_equation->next = *equation_list; // add the new equation to the list
        *equation_list = new_equation;       // update the head of the list
    }
}

// function to convert a Roman numeral character to its integer value
int roman_to_value(char roman)
{
    switch (roman)
    { // check the Roman numeral character
    case 'I':
        return 1; // return 1 for 'I'
    case 'V':
        return 5; // return 5 for 'V'
    case 'X':
        return 10; // return 10 for 'X'
    case 'L':
        return 50; // return 50 for 'L'
    case 'C':
        return 100; // return 100 for 'C'
    case 'D':
        return 500; // return 500 for 'D'
    case 'M':
        return 1000; // return 1000 for 'M'
    default:
        return 0; // return 0 for any other character
    }
}

// Function to convert a linked list of Roman numeral characters to a decimal integer
int convert_roman_to_decimal(Node *roman_numeral)
{
    int decimal = 0;               // initialize the decimal value
    Node *current = roman_numeral; // start at the beginning of the linked list

    while (current != NULL)
    {                                                                                 // traverse the linked list
        int current_value = roman_to_value(current->value);                           // get the value of the current character
        int next_value = roman_to_value(current->next ? current->next->value : '\0'); // get the value of the next character

        if (next_value > current_value)
        {                                                         // if the next value is greater than the current value
            decimal += (next_value - current_value);              // subtract the current value from the next value and add to the decimal
            current = current->next ? current->next->next : NULL; // move to the character after the next
        }
        else
        {                             // if the next value is not greater than the current value
            decimal += current_value; // add the current value to the decimal
            current = current->next;  // move to the next character
        }
    }

    return decimal; // return the decimal value
}

int get_pow(int base, int exponent)
{
    int result = 1;
    for (int i = 0; i < exponent; i++)
    {
        result *= base;
    }
    return result;
}

// function to calculate the result of an equation
int calculate_result(int operand1, int operand2, char operator)
{
    switch (operator)
    { // Check the operator
    case '+':
        return operand1 + operand2; // Return the sum for '+'
    case '-':
        return operand1 < operand2 ? operand2 - operand1 : operand1 - operand2;
    case '*':
        return operand1 * operand2;                     // Return the product for '*'
    case '/':                                           // For division
        return operand2 != 0 ? operand1 / operand2 : 0; // Return the quotient if the divisor is not zero, otherwise return 0
    case '%':                                           // For modulo
        return operand2 != 0 ? operand1 % operand2 : 0; // Return the remainder if the divisor is not zero, otherwise return 0
    case '^':
        return get_pow(operand1, operand2);                // Return the power for '^'
    default:                                               // For any other operator
        printf("Error: Unknown operator %c.\n", operator); // Print an error message
        exit(1);                                           // Exit the program with an error code
    }
}

// function to append a word with a space to a result string
void append_with_space(char *result, const char *word)
{
    int result_len = 0; // initialize the length of the result string
    while (result[result_len] != '\0')
    {                 // traverse the result string
        result_len++; // move to the next character
    }

    if (result_len > 0)
    {                                  // if the result string is not empty
        result[result_len] = ' ';      // add a space to the end of the result string
        result[result_len + 1] = '\0'; // add the null character to the end of the result string
        result_len++;                  // increment the length of the result string
    }

    int i = 0; // Initialize the index for the word
    while (word[i] != '\0')
    {                                     // traverse the word
        result[result_len + i] = word[i]; // add the word to the end of the result string
        i++;                              // move to the next character
    }
    result[result_len + i] = '\0'; // add the null character to the end of the result string
}

// function to convert a number less than 1000 to words
void convert_hundreds(int num, char *result)
{
    if (num >= 100)
    {                                               // if the number is 100 or greater
        append_with_space(result, ones[num / 100]); // append the hundreds place to the result string
        append_with_space(result, "Hundred");       // append the word "Hundred" to the result string
        num %= 100;                                 // get the remainder after dividing by 100
    }
    if (num >= 20)
    {                                              // if the number is 20 or greater
        append_with_space(result, tens[num / 10]); // append the tens place to the result string
        num %= 10;                                 // get the remainder after dividing by 10
    }
    else if (num >= 10)
    {                                               // if the number is between 10 and 19
        append_with_space(result, teens[num - 10]); // append the teens place to the result string
        return;                                     // exit the function
    }
    if (num > 0)
    {                                         // if the number is greater than 0
        append_with_space(result, ones[num]); // append the ones place to the result string
    }
}

// function to convert a total number to words
char *format_result(int total)
{
    if (total == 0)
    {                  // if the total is 0
        return "Zero"; // return the word "Zero"
    }

    char *result = (char *)malloc(256); // allocate memory for the result string
    result[0] = '\0';                   // initialize the result string to the empty string

    int thousand_counter = 0; // initialize the counter for thousands

    while (total > 0)
    { // While the total is greater than 0
        if (total % 1000 != 0)
        {                                         // if the remainder after dividing by 1000 is not 0
            char part[256] = "";                  // initialize a temporary string for the current part
            convert_hundreds(total % 1000, part); // convert the current part to words
            if (thousand_counter > 0)
            {                                                         // if the counter for thousands is greater than 0
                append_with_space(part, thousands[thousand_counter]); // append the thousands place to the current part
            }
            if (result[0] != '\0')
            {                   // if the result string is not empty
                char temp[256]; // initialize a temporary string for the result
                int i = 0;      // initialize the index for the result
                while (result[i] != '\0')
                {                        // traverse the result string
                    temp[i] = result[i]; // copy the result string to the temporary string
                    i++;                 // move to the next character
                }
                temp[i] = '\0'; // add the null character to the end of the temporary string

                int j = 0; // initialize the index for the current part
                while (part[j] != '\0')
                {                        // traverse the current part
                    result[j] = part[j]; // copy the current part to the result string
                    j++;                 // move to the next character
                }
                result[j] = ' ';      // add a space to the end of the result string
                result[j + 1] = '\0'; // add the null character to the end of the result string

                int k = 0; // initialize the index for the temporary string
                while (temp[k] != '\0')
                {                                // traverse the temporary string
                    result[j + 1 + k] = temp[k]; // copy the temporary string to the result string
                    k++;                         // move to the next character
                }
                result[j + 1 + k] = '\0'; // add the null character to the end of the result string
            }
            else
            {              // if the result string is empty
                int i = 0; // initialize the index for the current part
                while (part[i] != '\0')
                {                        // traverse the current part
                    result[i] = part[i]; // copy the current part to the result string
                    i++;                 // move to the next character
                }
                result[i] = '\0'; // add the null character to the end of the result string
            }
        }
        total /= 1000;      // divide the total by 1000
        thousand_counter++; // increment the counter for thousands
    }

    return result; // return the result string
}

// function to reverse the linked list of equations
Equation *reverse_equation_list(Equation *head)
{
    Equation *prev = NULL;    // initialize the previous pointer to NULL
    Equation *current = head; // start at the head of the linked list
    Equation *next = NULL;    // initialize the next pointer to NULL
    while (current != NULL)
    {                         // traverse the linked list
        next = current->next; // save the next node
        current->next = prev; // reverse the current node's next pointer
        prev = current;       // move the previous pointer to the current node
        current = next;       // move to the next node
    }
    return prev; // return the new head of the reversed list
}

// Function to write the results to a file
void write_results_to_file(Equation *equation_list)
{
    FILE *file = fopen("output.txt", "w"); // open the output file for writing
    if (file == NULL)
    {                                                   // if the file could not be opened
        printf("Error: Could not open output file.\n"); // print an error message
        exit(1);                                        // exit the program with an error code
    }

    Equation *current = equation_list; // start at the head of the linked list
    while (current != NULL)
    {                                                                         // traverse the linked list
        int operand1 = convert_roman_to_decimal(current->operand1);           // convert the first operand to a decimal integer
        int operand2 = convert_roman_to_decimal(current->operand2);           // convert the second operand to a decimal integer
        int result = calculate_result(operand1, operand2, current->operator); // calculate the result of the equation
        char *result_str = format_result(result);                             // convert the result to words

        fprintf(file, "%s\n", result_str); // write the result to the file
        printf("%s\n", result_str);        // print the result to the console
        free(result_str);                  // free the memory allocated for the result string

        current = current->next; // move to the next equation
    }

    fclose(file); // close the output file
}

int main()
{
    char equations[MAX_EQUATIONS][MAX_LENGTH] = {{0}}; // initialize the array to store the equations
    Equation *equation_list = NULL;                    // initialize the linked list of equations

    read_roman_numeral_equation_from_file(equations);     // read the equations from the file
    format_equations(equations, &equation_list);          // format the equations into a linked list
    equation_list = reverse_equation_list(equation_list); // reverse the linked list to maintain the correct order
    write_results_to_file(equation_list);                 // write the results to the output file

    return 0;
}