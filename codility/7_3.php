<?php

// Brackets - Determine whether a given string of parentheses (multiple types) is properly nested.
// Score: once, about 15 min, 100% done, time complexity O(N)

/*
  Goal: given a string S consisting of N characters (within the range [0..200,000]), returns 1 if S is properly nested and 0 otherwise.

  String S consists only of the following characters: "(", "{", "[", "]", "}" and/or ")".

  A string is considered to be properly nested if any of the following conditions is true:
    - S is empty;
    - S has the form "(U)" or "[U]" or "{U}" where U is a properly nested string;
    - S has the form "VW" where V and W are properly nested strings.

  For example, the string "{[()()]}" is properly nested but "([)()]" is not.

  Copyright 2009–2018 by Codility Limited. All Rights Reserved. Unauthorized copying, publication or disclosure prohibited.
*/

function solution($S) {
    $stack = array();
    
    foreach( str_split($S) as $test_char ) {
        switch( $test_char ) {
        case '(' :
        case '[' :
        case '{' :
            array_push($stack, $test_char);
            break;
        case ')' :
            if( array_pop($stack)==='(' ) ;
            else return 0;
            break;
        case ']' :
            if( array_pop($stack)==='[' ) ;
            else return 0;
            break;
        case '}' :
            if( array_pop($stack)==='{' ) ;
            else return 0;
            break;
        default : break; // should never be here
        }
    }
    
    if( array_pop($stack)!==NULL ) return 0;
    return 1;
}

?>