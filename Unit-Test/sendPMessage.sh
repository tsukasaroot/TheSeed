#!/bin/bash
echo -e "login:test:puissant0x12" | nc -u -w 1 51.210.41.122 11102
echo -e "login:testo:test0x12" | nc -u -w 1 51.210.41.122 11102

echo -e "privateMessage:testo:test:teeeeeeeest0x12" | nc -u -w 1 51.210.41.122 11102
echo -e "privateMessage:test:testo:teeeeeeeest0x12" | nc -u -w 1 51.210.41.122 11102

echo -e "logout:test0x12" | nc -u -w 1 51.210.41.122 11102
echo -e "logout:testo0x12" | nc -u -w 1 51.210.41.122 11102