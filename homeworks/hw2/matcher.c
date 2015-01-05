#include "matcher.h"

/**
 * Returns a number representing the total length
 * of a pointer
 */

int size_pattern(char *ptr) {
    int count = 0;
    while (*ptr++) { //while we have not hit null pointer
        count += 1;
    }
    return count;
} 

/**
 * Implementation of a helper function matches, which
 * will be called by the rgrep_matches program.
 *
 * It takes as parameters the current line position, pattern position,
 * line length, pattern length, pointer to line, and pointer to pattern
 */

int matches(int linePos, int patternPos, int line_length, int pattern_length, char* line, char* pattern) {
    if (linePos >= line_length && patternPos < pattern_length) { //the pattern was not finished
        if (pattern[patternPos] == '+') {     // pattern == '+'
            return matches(linePos - 1, patternPos + 1, line_length, pattern_length, line, pattern); //should be good
        } else if (pattern[patternPos] == '?') { // pattern == '?'
            return matches(linePos - 1, patternPos + 1, line_length, pattern_length, line, pattern); //should be good
        }
        return 0; //no pattern matched  works!!!
    } else if (patternPos > pattern_length && linePos < line_length) { //pattern finished
        return 1;
    } else if (patternPos > pattern_length && linePos > line_length) {  //this and the two on top are my base cases
        return 1;
    } else if (pattern[patternPos] == '+') {
        if (pattern[patternPos - 1] == line[linePos] && pattern[patternPos - 1] != '\\') {  //so previous char in pattern matches linePos and is not nullifier
            return matches(linePos + 1, patternPos, line_length, pattern_length, line, pattern); // || //there was an or here
        }
        return matches(0, patternPos + 1, line_length, pattern_length, line, pattern); //we did encounter the nullifier \\ and move forward in our patternPos //0 or linePos // or linePos+1
    } else if (pattern[patternPos] == '?') { //should be correct
        if (line[linePos - 1] == line[linePos]) {
            return matches(linePos + 1, 0, line_length, pattern_length, line, pattern); //should be good
        } else if (pattern[patternPos - 1] == line[linePos] && pattern[patternPos - 1]  != '\\') {  //had before pattern[patternPos - 1] == line[linePos]
            return matches(linePos, patternPos + 1, line_length, pattern_length, line, pattern); // || //should handle this return first don't know about second so commenting it out, THERE WAS AN OR HERE
        }
        return matches(0, patternPos + 1, line_length, pattern_length, line, pattern);
    } else if (pattern[patternPos] == '\\') { //should be good
        if (pattern[patternPos + 1] == line[linePos]) {
            return matches(linePos + 1, patternPos + 2, line_length, pattern_length, line, pattern);
        } 
        return matches(linePos + 1, 0, line_length, pattern_length, line, pattern);  //should be good
    } else if (pattern[patternPos] == line[linePos] || pattern[patternPos] == '.') {
        return matches(linePos + 1, patternPos + 1, line_length, pattern_length, line, pattern); //this is the period case or when they are just matching ///good///
    }
    return matches(linePos + 1, patternPos, line_length, pattern_length, line, pattern); //no line has matched pattern yet ///good///
}

/**
 * Implementation of your matcher function, which
 * will be called by the main program.
 *
 * You may assume that both line and pattern point
 * to reasonably short, null-terminated strings.
 */

int rgrep_matches(char *line, char *pattern) {
    return matches(0, 0, size_pattern(line), size_pattern(pattern), line, pattern);
}  

