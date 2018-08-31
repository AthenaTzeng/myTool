<?php

// 6_4, 300 min, 7 times
// error [1,1,1] => 3
// error [1, 0, 1, 0, 1] => 6
// error [1, 2147483647, 0] => 2
// error [0, 0, 0] => 0 & if( $ans>10000000 ) return -1;
// 75% 殘念 => 空間複雜度不過
// 100% YA~~~ 2018.08.30.

function solution($A) {
    $p_in = array(); // 所有的in
    $p_out = array(); // 所有的out

    $ans = 0; // 答案
    foreach( $A as $k => $v ) {
        array_push($p_in, $k-$v);
        array_push($p_out, $k+$v);
    }
    sort($p_in);
    sort($p_out);
    
    $idx_in = 0; // 現在檢查到in的位置
    $idx_out = 0; // 現在檢查到out的位置
    $current_num = 0; // 現在有多少個圈
    while( isset($p_in[$idx_in]) ) {
        $pt_num = ( $p_in[$idx_in]<=$p_out[$idx_out] ? $p_in[$idx_in] : $p_out[$idx_out] );
        $num_in = 0;
        $num_out = 0;
        while( isset($p_in[$idx_in]) && $p_in[$idx_in]==$pt_num ) {
            $num_in++;
            $idx_in++;
        }
        while( isset($p_out[$idx_out]) && $p_out[$idx_out]==$pt_num ) {
            $num_out++;
            $idx_out++;
        }

        if( $num_in>0 )
            $ans += ($current_num*$num_in) + ($num_in*($num_in-1))/2;
        $current_num += ($num_in-$num_out);
        if( $ans>10000000 ) return -1; // 例外條件
    }
    
    return $ans;
}

?>

