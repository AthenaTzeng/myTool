<?php

// Fish - N voracious fish are moving along a river. Calculate how many fish are alive.
// Score: once, about 20 min, 100% done, time complexity O(N)

/*
  Goal: Given two non-empty arrays A and B consisting of N integers (range [1..100,000]), returns the number of alive fish.

  Array A are the sizes of the fish. All its elements are unique and within the range [0..1,000,000,000]. 
  Array B are the directions of the fish. It contains only 0s and/or 1s, where:
    - 0 represents a fish flowing upstream,
    - 1 represents a fish flowing downstream.
  If two fish move in opposite directions, they will eventually meet each other. Then only the bigger fish can stay alive.

  For example, consider arrays A and B as: ([4,3,2,1,5],[0,1,0,0,0])
  The function should return 2.

  Copyright 2009–2018 by Codility Limited. All Rights Reserved. Unauthorized copying, publication or disclosure prohibited.
*/

function solution($A, $B) {
    $stack = array();
    $total = count($A)-1;
    $ans = 0;
    
    for($idx=$total; $idx>=0; $idx-- ) {
        if( $B[$idx]==0 )
            array_push($stack, $A[$idx]);
        else {
            while( ($next_fish=array_pop($stack))!==NULL ) {
                if( $next_fish<$A[$idx] ) ;
                else {
                    array_push($stack, $next_fish);
                    $ans--;
                    break;
                }
            }
            $ans++;
        }
    }
    
    $ans += count($stack);
    return $ans;
}

?>
