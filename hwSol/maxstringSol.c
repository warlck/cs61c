#include <stdio.h>

int charcount(char* str, char c);
char* maxstring(char* str_arr[], int num, char c);

/*
 * Modify this in part B to accept command-line arguments.
 */
int main(int argc, char* argv[]) {
    if (argc == 1) {
        char* strings[] = {"hello world!", "", "welcome to cs61c", "good day"};
        printf("If the char is 'd', maxstring() should return 'good day'. Result:\n%s\n", maxstring(strings, 4, 'd'));
        printf("If the char is ' ', maxstring() should return 'welcome to cs61c'. Result:\n%s\n", maxstring(strings, 4, ' '));
        printf("If the char is 'b', maxstring() should return ''. Result:\n%s\n", maxstring(strings, 4, 'b'));
        printf("If the char is 'o', maxstring() should return 'hello world!'. Result:\n%s\n", maxstring(strings, 4, 'o'));
        return 0;
    }

    printf("%s\n", maxstring(argv + 2, argc - 2, argv[1][0]));
    return 0;
}

/*
 * Counts the number of times a given character is present in a string.
 */
int charcount(char *str, char c) {
    int ret = 0;
    while(*str++) {
        ret += (str[-1] == c);
    }
    return ret;
}

/*
 * Takes an array of strings and returns the string with the maximum number of occurences
 * of a given character. In case of a tie, the first string is returned. If the array is
 * empty or if the character does not exist in any of the strings, an empty string is returned.
 */
char *maxstring(char* str_arr[], int num, char c) {
    int best_score = 0;
    char *ret = "";
    for (int i = 0; i < num; i += 1) {
        int score = charcount(str_arr[i], c);
        if (score > best_score) {
            best_score = score;
            ret = str_arr[i];
        }
    }

    return ret;
}