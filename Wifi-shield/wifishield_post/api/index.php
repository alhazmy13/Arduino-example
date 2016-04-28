<?php
require 'Slim/Slim.php';
\Slim\Slim::registerAutoloader();
$app = new \Slim\Slim();



$app->post('/data', function() use ($app) {

	
	$req = $app->request();
	$json = $req->post('data');
    $data = json_decode($json, true);
    $data = $data["data"];
	//var_dump($data);
    $list = array();
	for ($i=0; $i < count($data); $i++) {
		$value = $data[$i];
		$splitValue = explode(",", $value); 
		$nstdlist = array($splitValue[0],getDifTimes($splitValue[1]));
		array_push($list, $nstdlist);
	}
	$fp = fopen('data.csv', 'a+');
	foreach ($list as $fields) {
	    fputcsv($fp, $fields);
	}
	fclose($fp);
	echo "DONE";
});

$app->get('/data', function() use ($app) {

$file = fopen("data.csv","r");
$result["data"] = array();
while(! feof($file))
  {
  	$row = fgetcsv($file);
  	if(count($row) == 1 and $row[0] === null) 
      { 
         continue;  // skip empty rows 
      } 
      $test["status"] = $row[0];
      $test["date"] = $row[1];
  	array_push($result["data"],$test);
  }

echo json_encode($result);
fclose($file);

});
function getDifTimes($time){
	$start = new DateTime();
	$end = date_create($time);
	$diff = strtotime(date_format($start, 'Y-m-d H:i:s')) - strtotime(date_format($end, 'Y-m-d H:i:s'));
	$diff2 = strtotime(date_format($end, 'Y-m-d H:i:s')) + $diff;
	 return date('Y-m-d H:i:s', $diff2);
}


$app->run();
?>
