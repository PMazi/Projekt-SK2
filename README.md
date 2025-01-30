#Kompilacja programu
gcc gomoku_server.c -o gomoku_server -pthread
gcc gomoku_client.c -o gomoku_client

#Uruchomienie programu ( ./gomoku_client <adres IP> <port> ):
./gomoku_server 127.0.0.1 2000
./gomoku_client 127.0.0.1 2000
