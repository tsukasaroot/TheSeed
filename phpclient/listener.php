<?php
$server = '127.0.0.1';
$port = 7801;

if (!($sock = socket_create(AF_INET, SOCK_DGRAM, 0))) {
	$errorcode = socket_last_error();
	$errormsg = socket_strerror($errorcode);

	die("Couldn't create socket: [$errorcode] $errormsg \n");
}

socket_bind($sock, "0.0.0.0", 9000) or onSocketFailure("Failed to bind to 0.0.0.0:9000", $sock);

function arrange_opcodes($buffer): array
{
	$array_of_bufer = explode(':', $buffer);
	$i = 0;

	if ($array_of_bufer[0] !== '' && $array_of_bufer[0][0] === 'C') {
		$assoc_opcodes = [
			'opcode' => $array_of_bufer[0]
		];
		$i++;
	}

	for (; $i < count($array_of_bufer); $i++) {
		$arr = explode('{', $array_of_bufer[$i]);
		if (count($arr) > 1) {
			$assoc_opcodes[$arr[0]] = $arr[1];
			$arr[1] = rtrim($arr[1], '}');
			$assoc_opcodes[$arr[0]] = $arr[1];
		}
	}
	return $assoc_opcodes;
}

while (1) {

	socket_recvfrom($sock, $buffer, 32768, 0, $ip, $port);
	$buffer = str_replace('0x12', '', $buffer);

	if (count($arrays = explode('[', $buffer)) > 1) {
		for ($i = 0; $i < count($arrays); $i++) {
			$arrays[$i] = rtrim($arrays[$i], ':]:');
		}
	}

	$assoc_opcodes = [];

	if (count($arrays) > 1) {
		for ($i = 0; $i < count($arrays); $i++) {
			$assoc_opcodes[$i] = arrange_opcodes($arrays[$i]);
		}
	} else {
		$assoc_opcodes = arrange_opcodes($buffer);
	}

	foreach ($assoc_opcodes as $key => $value) {
		if (is_array($value)) {
			echo "array number $key\n";
			foreach ($value as $k => $v) {
				echo $k . " : " . $v . "\n";
			}
		} else
			echo $key . " : " . $value . "\n";
		echo "\n";
	}
}
