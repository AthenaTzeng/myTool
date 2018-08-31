<?php

// 2018.08.23.

function solution($A) {
    $number = count($A);
    $min_avg = 0;
    $min_j = 0;
    $set_min = 0;
    
    $sumA[0] = $A[0];
    for($i=1; $i<$number; $i++) {
        $sumA[$i] = $sumA[$i-1] + $A[$i];
    }
    
    foreach($sumA as $k => $v) {
        for( $j=0; $j<$k; $j++) {
            if( $j==0 ) $avg = $v/($k+1);
            else $avg = ($v - $sumA[$j-1])/($k-$j+1);
            if( $set_min==0 ) {
                $min_avg = $avg;
                $min_j = $j;
                $set_min = 1;
            } else {
                if( $avg<$min_avg ) {
                    $min_avg = $avg;
                    $min_j = $j;
                }
            }
        }
    }
    
    return $min_j;
}

?>

