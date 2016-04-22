<?php
require 'Slim/Slim.php';
\Slim\Slim::registerAutoloader();
$app = new \Slim\Slim();



$app->post('/data', function() use ($app) {

	 $req = $app->request();
	 $data = $req->post('data');
	echo '{"flag": "true","data": "'.$data.'"}';

	
});


$app->run();
?>
