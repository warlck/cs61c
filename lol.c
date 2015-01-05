/**
 * The contents of this file are only a suggestion.
 * Feel free to change anything.
 */

#include "matcher.h"

/**
 * Returns true if partial_line matches pattern, starting from
 * the first char of partial_line.
 */
/* int matches_leading(char *partial_line, char *pattern) {
  // You can use this recommended helper function, or not.  //covers substring cases
    while (*partial_line++ || *pattern++) { //try && next
        if (*partial_line == *pattern) {
            return matches_leading(partial_line + 1, pattern + 1);
        } else if (*pattern_line == '\0' ) {
            
        }
        return 0;
    }
  return 0;
} */

/**
 * Implementation of your matcher function, which
 * will be called by the main program.
 *
 * You may assume that both line and pattern point
 * to reasonably short, null-terminated strings.
 */

int size_pattern(char *ptr) {
	int count = 0;
	while (*ptr++) { //while we have not hit null pointer
		count += 1;
	}
	return count;
}

int matches(int linePos, int patternPos, int line_length, int pattern_length, char* line, char* pattern) {
	if (linePos >= line_length && patternPos < pattern_length) { //the pattern was finished
		if (pattern[patternPos] == '+') { //*pattern == '+'
			return matches(linePos + 1, patternPos + 1, line_length, pattern_length);
		} else if (pattern[patternPos] == '?') { //*pattern == '?'
            return matches(linePos + 1, patternPos + 1, line_length, pattern_length);
        }
		return 0;
	} else if (patternPos > pattern_length && linePos < line_length) { //pattern finished
		return 1;
	} else if (patternPos > pattern_length && linePos > line_length) {  //this and the ones on top I assume are our base cases
		return 1;
	} else if (pattern[patternPos] == '+') {
        if (pattern[patternPos - 1] == line[linePos] && pattern[patternPos] != '\\') {   //only pattern pointer can '\\'or all patterns
            return (matches(linePos + 1, patternPos, line_length, pattern_length, line, pattern) || 
                matches(linePos - 1, patternPos, line_length, pattern_length, line, pattern);
        }
        return matches(0, patternPos + 1, ine_length, pattern_length, line, pattern); //we did encounter the nullifier \\ and move forward in our patternPos //0 or linePos // or linePos+1
    } else if (pattern[patternPos] == '?') { //should be correct
        if (pattern[patternPos - 1] == line[linePos] && pattern[patternPos - 1]  != '\\') {
            return (matches(linePos + 1, patternPos, line_length, pattern_length, line, pattern) || 
                matches(linePos + 1, patternPos + 1, line_length, pattern_length, line, pattern));
        }
        return matches(0, patternPos + 1, line_length, pattern_length, line, pattern);
	} else if (pattern[patternPos] == '\\') {
        if (pattern[pattern + 1] == line[linePos] {
            return matches(linePos + 1, patternPos + 1, line_length, pattern_length, line, pattern);
        }
        return matches(linePos, patternPos + 1, line_length, pattern_length, line, pattern);  //if it does not work it should be linePos+1, chech code
	} else if (pattern[patternPos] == line[linePos] || pattern[patternPos] == '.') {
		return matches(linePos + 1, patternPos + 1, line_length, pattern_length, line, pattern); //this is the period case or when they are just matching
	}
    return matches(linePos + 1, patternPos, line_length, pattern_length, line, pattern); //no line has matched pattern yet
}

int rgrep_matches(char *line, char *pattern) {
	return matches(0, 0, size_pattern(line), size_pattern(pattern), line, pattern);
}

    /* for (int i = 0; pattern[i] != '\0'; i++) {
        if (pattern[i] == '\\') {

        }
    return 0;
    } */ 
    // TODO put your code here.
    //
 /*   int pattern_length = malloc(sizeof(line));
    //maybe keep track of index?
    int i = 0;
    while (*pattern++) { //while we have not hit null character
        //from piazza one approach is to use another pointer to tell you what is ahead(next) char   or if not have a set value that is the beginnig and end of the line?
        if (*pattern == '?') {
            //have to handle 2 cases, 1 where you actually have letter value & one where the optional char is not there but both still work
            matches_leading(partial_line + 1, pattern + 1);
        } else if (*pattern == ) {
            matches_leading(partial_line + 1, pattern + 1);
        } else if (*pattern == '.') {
        
        } else if (*pattern == '+') {
                       
        } else {
            return 0;
        }
        return 1;
    }
    return 0;

    
    //recursion approach
    //or base case can be 
    if (*pattern == '\0') {                        /////////////Maybe include this base case
        return 0         
    }
    if () {
        return 1;
    } else {
        while (*pattern++ ) {
            rgrep_matches(line + 1, pattern + 1);
        }
        rgrep_matches(line + 1, pattern + 1);
    }
} */
