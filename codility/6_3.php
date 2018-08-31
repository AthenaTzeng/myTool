<?php

// 6_3, 10 min, once
// 2018.08.23.

function solution($A) {
    $leng = count($A);
    if( $leng<3 ) return 0;
    
    $gotOne = 0;
    sort($A);
    
    for( $i=2; $i<$leng; $i++ ) {
        if( $A[$i]<($A[$i-1] + $A[$i-2]) ) return 1;
    }
    
    return 0;
}

?>
