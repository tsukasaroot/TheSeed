<?php

/*
	Simple php udp socket client
*/

$server = '127.0.0.1';
$port = 7801;

if (!($sock = socket_create(AF_INET, SOCK_DGRAM, 0))) {
	$errorcode = socket_last_error();
	$errormsg = socket_strerror($errorcode);

	die("Couldn't create socket: [$errorcode] $errormsg \n");
}

echo "Socket created \n";
$packet = "";
$bytesRead = 0;
$disponibleCommands = json_decode(file_get_contents("commands.json"), true);

while (1) {
	//Take some input to send
	echo 'Enter a command to send : ';

	$input = readline();

	if ($input === "help") {
		echo "Available commands : \n";
		foreach ($disponibleCommands as $key => $value) {
			echo $key . "\n";
		}
		continue;
	}

	if ($input === "" || empty($disponibleCommands[$input]))
		continue;

	if (!socket_sendto($sock, $disponibleCommands[$input] . '0x12' . "\n", strlen($disponibleCommands[$input] . '0x12' . "\n"), 0, $server, $port)) {
		$errorcode = socket_last_error();
		$errormsg = socket_strerror($errorcode);

		die("Could not send data: [$errorcode] $errormsg \n");
	}
	echo "command sent \n";
}