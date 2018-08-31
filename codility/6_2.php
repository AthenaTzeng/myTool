<?php

// 6_2, 50 min, 3 times, after class
// 2018.08.23.

function solution($A) {
    $leng = count($A)-1;
    sort($A);
    
    $ans_p = $A[$leng] * $A[$leng-1] * $A[$leng-2];
    $ans_n = $A[$leng] * $A[0] * $A[1];
    
    return ( $ans_p>$ans_n ? $ans_p : $ans_n ) ;
}

?>

