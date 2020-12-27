#!/bin/bash
echo -e "login:test:puissant0x12" | nc -u 51.210.41.122 11102
n=1
while [ $n -le 50 ]
do
	echo -e "getClientData:test0x12" | nc -u 51.210.41.122 11102
done
echo -e "logout:test0x12" | nc -u 51.210.41.122 11102