<?php
	$top = 20; // check $top news desc list by SendDate
	
	/* get source from ntpc gov open platform API */
	$ch = curl_init();
	$finalurl = "http://data.ntpc.gov.tw/od/data/api/DF4825E5-9E52-4281-BCAC-10542B3088EF?\$format=json&\$orderby=SendDate%20DESC&\$top=".$top;//."\$skip=5";
	echo "===> exec: ".$finalurl."<br>\n";
	curl_setopt($ch, CURLOPT_RETURNTRANSFER, 1);
	curl_setopt($ch, CURLOPT_URL, $finalurl);
	$response = curl_exec($ch);
	curl_close($ch);
	/* end of data get */
	
	$j_data = json_decode($response, true);
	echo "count = ".COUNT($j_data)."<br>";

	foreach( $j_data as $oneContext ) {
		echo "<br>PublishDept: ".$oneContext["PublishDept"];
		if($oneContext["PublishDeptId"]!=1140300) continue; // skip if not from 1140300 新聞局

		date_default_timezone_set("Asia/Taipei");
		$post_date = ( isset($oneContext["SendDate"]) ) ? $oneContext["SendDate"] : date("Y/m/d\TH:i:s.0") ;
		echo "<br>post_date = ".$post_date." and current = ".date("Y/m/d\TH:i:s.0")."<br>";
		if( strcmp(date("Y/m/d\TH:i:s.0"),$post_date)>=0 ) ; // in the future...
		else continue;

		$post_title = $oneContext["Subject_"];
		$post_content = isset($oneContext["Content"]) ? htmlspecialchars( preg_replace("/&#?[a-z0-9]+;/i","", strip_tags($oneContext["Content"])) ) : "" ;
		if( $post_content=="" ) { 
			//echo "<br>The ID:".$post_id." with empty content, so DO NOT Handle it<br>\n";
			if( !isset($oneContext["SubCategory"]) || strcmp($oneContext["SubCategory"], "圖片新聞")!=0 ) continue;
			$post_content = "此新聞無說明。";
		}
		$post_content = "<p>".$post_content."</p>";

		if( !isset($oneContext["cb_ContactData"]) || $oneContext["cb_ContactData"]=="" ) $post_contact = "";
		else {
			$post_contact = "<ul>";
			foreach( preg_split("/[\},]+/", $oneContext["cb_ContactData"]) as $xml_form ) {
				$p = xml_parser_create();
				xml_parse_into_struct($p, $xml_form, $xml_info);
				xml_parser_free($p);
				foreach( $xml_info as $real_data ) {
					switch( $real_data["tag"] ) {
					case "CONTACTNAME" :
						$post_contact .= "<li>聯絡人：".$real_data['value']."</li>";
						break;
					case "CONTACTTEL" :
						$post_contact .= "<li>聯絡電話：".$real_data['value']."</li>";
						break;
					case "CONTACTMAIL" :
						$post_contact .= "<li>電子信箱：".$real_data['value']."</li>";
						break;
					case "CONTACTDEPT" :
						$post_contact .= "<li>聯絡單位：".$real_data['value']."</li>";
						break;
					default:
						break;
					}
				}
			}
		}
		if( !isset($oneContext["EndDate"]) || $oneContext["EndDate"]=="" ) ;
		else $post_contact .= "<li>公告結束時間：".$oneContext["EndDate"]."</li>";
		$post_contact .= "</ul>";

		if( !isset($oneContext["cb_Attachment"]) || $oneContext["cb_Attachment"]=="" ) $post_attach = "";
		else {
			$post_attach = "<div>";
			$att_cnt = 1;
			foreach( preg_split("/[\},]+/", $oneContext["cb_Attachment"]) as $xml_form ) {
				$p = xml_parser_create();
				xml_parse_into_struct($p, $xml_form, $xml_info);
				xml_parser_free($p);
				foreach( $xml_info as $real_data ) {
					switch( $real_data["tag"] ) {
					case "FILEURL" :
						$att_url = (isset($real_data["value"])) ? $real_data['value'] : "" ; break;
					case "FILEDESC" : 
						$att_des = (isset($real_data["value"])) ? $real_data['value'] : "" ; break;
					case "FILETYPE" : 
						$att_type = (isset($real_data["value"])) ? $real_data['value'] : "" ; break;
					default: 
						break;
					}
				}
				if( $att_type!=2 ) continue; // not img news
				$post_attach .= "<img src=\"$att_url\" alt=\"$att_des\" width=\"30%\">&nbsp;&nbsp;";
				if( ($att_cnt%3)==0 ) $post_attach .= "<br><br>";
				$att_cnt += 1;
			}
			$post_attach .= "</div>";
		}
		$post_content .= $post_contact.$post_attach;
		$post_id = $oneContext["Id"];
		$in_post = check_ID_Duplicated($post_id);
		if( $in_post==0 ) {
			echo "<br>ID: ".$post_id." NOT in DB, Insert it<br>\n";
			insert_into_DB($post_id, $post_title, $post_content, $post_date);
		} else {
			echo "<br>ID: ".$post_id." already in DB  --> update data no metter what<p>\n";
			update_DB($in_post, $post_title, $post_content);
		}
	}
	
	/* Check if this news has posted before. Return post id or 0. */
	function check_ID_Duplicated( $ID ) {
		$res = 0;
		$mysqli = new mysqli('localhost', 'xxx', '', 'xxx', 000);
		if ($mysqli->connect_errno) {
			echo "Failed to connect to MySQL: (" . $mysqli->connect_errno . ") " . $mysqli->connect_error;
			die();
		}
		echo $mysqli->host_info . "<br>\n";
		$mysqli->set_charset("utf8");
		
		$sql = "SELECT ID, post_name from wp_posts where post_name=$ID;";
		$stmt = $mysqli->query($sql);
		if( $mysqli->errno )
			echo "Table creation failed: (" . $mysqli->errno . ") " . $mysqli->error;

		if( $stmt->num_rows == 0 ) ;
		else { //already in DB
			$result = $stmt->fetch_assoc();
			$res = $result["ID"];
		}

		$mysqli->close();
		return $res;
	}
	
	/* if this news has not posted before, just insert it */
	function insert_into_DB( $post_id, $post_title, $post_content, $post_date ) {
		echo "try insert: post_title=".$post_title."<p>\n";
		
		$mysqli = new mysqli('localhost', 'xxx', '', 'xxx', 000);
		if( $mysqli->connect_errno ) {
			echo "Failed to connect to MySQL: (" . $mysqli->connect_errno . ") " . $mysqli->connect_error;
			die();
		}
		echo $mysqli->host_info . "<br>\n";
		$mysqli->set_charset("utf8");

		$sql = "INSERT INTO `wp_posts` (post_author, post_title, post_content, post_date, post_name, guid) VALUES (1, '$post_title', '$post_content', '$post_date', $post_id, '');";
		$mysqli->query($sql);
		$qID = $mysqli->insert_id;
		if( $mysqli->errno )
			echo "Table creation failed: (" . $mysqli->errno . ") " . $mysqli->error;
		else {
			$sql = "UPDATE `wp_posts` SET `guid` = 'http://www.info.ntpc.gov.tw/?p=$qID' where ID=$qID";
			$mysqli->query($sql);
		    if( $mysqli->errno )
			    echo "Table creation failed: (" . $mysqli->errno . ") " . $mysqli->error;
		}

		// Wite to wp-term_relationships table
		$sql = "INSERT INTO `wp_term_relationships` (object_id, term_taxonomy_id, term_order) VALUES ($qID, 3, 0)";
		$mysqli->query($sql);
		$mysqli->close();
	}

	/* if this news has posted before, try update data (in case of contant change) */
	function update_DB( $in_post, $post_title, $post_content ) {
		echo "try update: post_title=".$post_title."<p>\n";
		$mysqli = new mysqli('localhost', 'xxx', '', 'xxx', 000);
		if( $mysqli->connect_errno ) {
			echo "Failed to connect to MySQL: (" . $mysqli->connect_errno . ") " . $mysqli->connect_error;
			die();
		}
		echo $mysqli->host_info . "<br>\n";
		$mysqli->set_charset("utf8");

		$sql = "UPDATE `wp_posts` SET `post_title`='$post_title', post_content='$post_content' WHERE `ID`=$in_post;";
		$mysqli->query($sql);
		$qID = $mysqli->insert_id;
		if( $mysqli->errno )
			echo "update failed: (" . $mysqli->errno . ") " . $mysqli->error;
		
		$mysqli->close();
	}

?>