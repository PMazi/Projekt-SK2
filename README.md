## Kompilacja programu

```sh
gcc gomoku_server.c -o gomoku_server -pthread
gcc gomoku_client.c -o gomoku_client
```

## Uruchomienie programu

**Składnia uruchomienia serwera:**
```sh
./gomoku_server <adres IP> <port>
```

**Uruchomienie servera:**
```sh
./gomoku_server 127.0.0.1 2000
```

**Składnia uruchomienia klienta:**
```sh
./gomoku_client <adres IP> <port>
```

**Uruchomienie klienta:**
```sh
./gomoku_client 127.0.0.1 2000
```

