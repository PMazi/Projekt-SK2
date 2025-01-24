#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <pthread.h>


int game_over_check(char (*board)[10]){

    //sprawdzanie czy jest 5 figur takich samych obok siebie w kolumnie
    for(int row = 0; row <= 5; row++){
        for(int column = 0; column <= 9; column++){
            if(board[row][column] == 'X' &&
               board[row+1][column] == 'X' &&
               board[row+2][column] == 'X' &&
               board[row+3][column] == 'X' &&
               board[row+4][column] == 'X')
            {
                return 1;    
            }
            if(board[row][column] == 'O' &&
               board[row+1][column] == 'O' &&
               board[row+2][column] == 'O' &&
               board[row+3][column] == 'O' &&
               board[row+4][column] == 'O')
            {
                return 1;    
            }
        }
    }

    //sprawdzanie czy jest 5 figur takich samych obok siebie w rzedzie
    for(int row = 0; row <= 9; row++){
        for(int column = 0; column <= 5; column++){
            if(board[row][column] == 'X' &&
               board[row][column+1] == 'X' &&
               board[row][column+2] == 'X' &&
               board[row][column+3] == 'X' &&
               board[row][column+4] == 'X')
            {
                return 1;
            }
            if(board[row][column] == 'O' &&
               board[row][column+1] == 'O' &&
               board[row][column+2] == 'O' &&
               board[row][column+3] == 'O' &&
               board[row][column+4] == 'O')
            {
                return 1;
            }
        }
    }

    //sprawdzanie czy jest 5 figur pod rzad na ukos    (10 - 5 + 1), 10 to rozmiar (10x10), 5 to ile pod rzad ma byc
    
    //sprawdzanie po przekatnej od lewego gornego rogu w dol
    int count_x, count_o;
    for(int i = 0; i < 10; i++){
        count_x = 0;
        count_o = 0;
        if(i >= 6){
            break;
        }
        for(int j = 0; j < 10 - i; j++){  

            //printf("%c\n", board[j+i][i]);
            if(board[j+i][j] == 'X'){
                count_x++;
            }
            else{
                count_x = 0;
            }
            if(count_x == 5){
                return 1;
            }

            if(board[j+i][j] == 'O'){
                count_o++;
            }
            else{
                count_o = 0;
            }

            if(count_o == 5){
                return 1;
            }
        }
    }

    //sprawdzanie po przekatnej od lewego gornego rogu w gore
    for(int i = 0; i < 10; i++){
        count_x = 0;
        count_o = 0;
        if(i >= 6){
            break;
        }
        for(int j = 0; j < 10 - i; j++){  

            //printf("%c\n", board[j+i][i]);
            if(board[j][j+i] == 'X'){
                count_x++;
            }
            else{
                count_x = 0;
            }
            if(count_x == 5){
                return 1;
            }

            if(board[j][j+i] == 'O'){
                count_o++;
            }
            else{
                count_o = 0;
            }
            if(count_o == 5){
                return 1;
            }
            
        }
    }

    //sprawdzanie od lewego gornego rogu w dol
    for(int i = 0; i < 10; i++){
        count_x = 0;
        count_o = 0;
        if(i >= 6){
            break;
        }
        for(int j = 0; j < 10 - i; j++){
            if(board[i+j][10-1-j] == 'X'){
                count_x++;
            }
            else{
                count_x = 0;
            }
            if(count_x == 5){
                return 1;
            }

            if(board[i+j][10-1-j] == 'O'){
                count_o++;
            }
            else{
                count_o = 0;
            }
            if(count_o == 5){
                return 1;
            }
        }
    }

    //sprawdzanie od lewego gornego rogu w gore
    for(int i = 0; i < 10; i++){
        count_x = 0;
        count_o = 0;
        if(i >= 6){
            break;
        }
        for(int j = 0; j < 10 - i; j++){
            if(board[j][10-1-i-j] == 'X'){
                count_x++;
            }
            else{
                count_x = 0;
            }
            if(count_x == 5){
                return 1;
            }

            if(board[j][10-1-i-j] == 'O'){
                count_o++;
            }
            else{
                count_o = 0;
            }
            if(count_o == 5){
                return 1;
            }
        }
    }

    //zwraca 0 jezeli nie ma konca gry
    return 0;
}

int validateMove(char (*board)[10], char* move, int socketFd){
    int row, column;
    int data_size;

    if(sscanf(move, "%d %d", &row, &column) != 2){
        printf("Gracz podal dane w niepoprawnym formacie\n");
        data_size = strlen("Podales dane w zlym formacie!");
        send(socketFd, &data_size, sizeof data_size, 0);
        send(socketFd, "Podales dane w zlym formacie!", data_size, 0);
        return 0;
    }

    if(row < 0 || row > 9 || column < 0 || column > 9){
        printf("Gracz podal nieprawidlowe wspolrzedne planszy\n");
        
        data_size = strlen("Podales nieprawidlowe wspolrzedne!");
        send(socketFd, &data_size, sizeof data_size, 0);
        send(socketFd, "Podales nieprawidlowe wspolrzedne!", data_size, 0);
        return 0;
    }

    if(board[row][column] != ' '){
        printf("Gracz podal pole ktore jest juz zajete\n");
        
        data_size = strlen("Podales wspolrzedne, ktore sa juz zajete!");
        send(socketFd, &data_size, sizeof data_size, 0);
        send(socketFd, "Podales wspolrzedne, ktore sa juz zajete!", data_size, 0);
        return 0;
    }
    data_size = strlen("Ruch wykonany poprawnie");
    send(socketFd, &data_size, sizeof data_size, 0);
    send(socketFd, "Ruch wykonany poprawnie", data_size, 0);
    printf("Gracz o deskryptorze numer %d wykonal ruch\n", socketFd);
    return 1;

}

void* gomokuGame(void* players) {
    char board[10][10];
    char buff[1024];
    memset(buff, 0, sizeof buff);
    int* playersFd = (int*)players;

        
    printf("Rozpoczeto nowa gre\n");
    printf("Deskryptory graczy: %d %d\n", playersFd[0], playersFd[1]);


    // Inicjalizacja planszy
    int rowsNumber = sizeof board / sizeof board[0];
    int columnsNumber = sizeof board[0] / sizeof board[0][0];
    for (int row = 0; row < rowsNumber; row++) {
        for (int column = 0; column < columnsNumber; column++) {
            board[row][column] = ' ';
        }
    }

    int round = 1;
    char move[10];
    int row, column;
    int data_size;
    int bytes;
    while (1) {
        if (round % 2 == 1) {

            data_size = sizeof board;
            send(playersFd[0], &data_size, sizeof data_size, 0);
            send(playersFd[0], board, data_size, 0);

            send(playersFd[1], &data_size, sizeof data_size, 0);
            send(playersFd[1], board, data_size, 0);

            data_size = strlen("Podaj ruch (wiersz kolumna): ");
            send(playersFd[0], &data_size, sizeof data_size, 0);
            send(playersFd[0], "Podaj ruch (wiersz kolumna): ", data_size, 0);

            data_size = strlen("Oczekiwanie na ruch przeciwnika...");
            send(playersFd[1], &data_size, sizeof data_size, 0);
            send(playersFd[1], "Oczekiwanie na ruch przeciwnika...", data_size, 0);

            recv(playersFd[0], &data_size, sizeof data_size, 0);
            memset(move, 0, sizeof move);
            recv(playersFd[0], move, data_size, 0);

            while (validateMove(board, move, playersFd[0]) != 1) {
                recv(playersFd[0], &data_size, sizeof data_size, 0);
                memset(move, 0, sizeof move);
                recv(playersFd[0], move, data_size, 0);
            }
            sscanf(move, "%d %d", &row, &column);
            board[row][column] = 'O';

            if(game_over_check(board) == 1){
                printf("Koniec gry, wygral gracz o deskryptorze numer %d\n", playersFd[0]);

                data_size = strlen("Gratulacje, wygrales");
                send(playersFd[0], &data_size, sizeof data_size, 0);
                send(playersFd[0], "Gratulacje, wygrales", data_size, 0);

                data_size = strlen("Przegrales :(");
                send(playersFd[1], &data_size, sizeof data_size, 0);
                send(playersFd[1], "Przegrales :(", data_size, 0);

                break;
            } 
            else {
                data_size = strlen("Brak zwyciezcy");
                send(playersFd[0], &data_size, sizeof data_size, 0);
                send(playersFd[0], "Brak zwyciezcy", data_size, 0);

                send(playersFd[1], &data_size, sizeof data_size, 0);
                send(playersFd[1], "Brak zwyciezcy", data_size, 0);
            }
            
        } else {

            data_size = sizeof board;
            send(playersFd[1], &data_size, sizeof data_size, 0);
            send(playersFd[1], board, sizeof board, 0);

            send(playersFd[0], &data_size, sizeof data_size, 0);
            send(playersFd[0], board, sizeof board, 0);

            data_size = strlen("Podaj ruch (wiersz kolumna): ");
            send(playersFd[1], &data_size, sizeof data_size, 0);
            send(playersFd[1], "Podaj ruch (wiersz kolumna): ", data_size, 0);
            
            data_size = strlen("Oczekiwanie na ruch przeciwnika...");
            send(playersFd[0], &data_size, sizeof data_size, 0);
            send(playersFd[0], "Oczekiwanie na ruch przeciwnika...", data_size, 0);

            recv(playersFd[1], &data_size, sizeof data_size, 0);
            memset(move, 0, sizeof move);
            recv(playersFd[1], move, data_size, 0);

            while (validateMove(board, move, playersFd[1]) != 1) {

                recv(playersFd[1], &data_size, sizeof data_size, 0);
                memset(move, 0, sizeof move);
                recv(playersFd[1], move, data_size, 0);
            }
            sscanf(move, "%d %d", &row, &column);
            board[row][column] = 'X';
            
            if(game_over_check(board) == 1){
                printf("Koniec gry, wygral gracz o deskryptorze numer %d\n", playersFd[1]);

                data_size = strlen("Gratulacje, wygrales");
                send(playersFd[1], &data_size, sizeof data_size, 0);
                send(playersFd[1], "Gratulacje, wygrales", data_size, 0);

                data_size = strlen("Przegrales :(");
                send(playersFd[0], &data_size, sizeof data_size, 0);
                send(playersFd[0], "Przegrales :(", data_size, 0);

                break;
            }
            else {
                data_size = strlen("Brak zwyciezcy");
                send(playersFd[0], &data_size, sizeof data_size, 0);
                send(playersFd[0], "Brak zwyciezcy", data_size, 0);

                send(playersFd[1], &data_size, sizeof data_size, 0);
                send(playersFd[1], "Brak zwyciezcy", data_size, 0);
            }
            
        }

        round++;

    }

    close(playersFd[0]);
    close(playersFd[1]);

    pthread_exit(NULL);
}
    


int main(int argc, char* argv[]){

    struct sockaddr_in server;
    memset(&server, 0, sizeof server);
    server.sin_family = AF_INET;
    server.sin_port = htons(atoi(argv[2]));
    server.sin_addr.s_addr = inet_addr(argv[1]);

    int serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    if( serverSocket == -1 ){
        perror("Blad w socket");
        exit(EXIT_FAILURE);
    }

    if( bind(serverSocket, (struct sockaddr*)&server, sizeof server) == -1 ){
        perror("Blad w bind");
        close(serverSocket);
        exit(EXIT_FAILURE);
    }

    if( listen(serverSocket, 2) == -1 ){
        perror("Blad w listen");
        close(serverSocket);
        exit(EXIT_FAILURE);
    }


    int numberOfPlayers = 0;
    
    char* message = malloc(sizeof(char) * 1024);

    int data_size;
    while(1){

        int* clientSockets = malloc(sizeof(int) * 2); // Alokacja dla pary graczy
        
        for (int i = 0; i < 2; i++){

            clientSockets[i] = accept(serverSocket, NULL, NULL);
            if( clientSockets[numberOfPlayers] == -1){
                perror("Blad w accept");
                free(clientSockets);
                clientSockets = NULL;
                continue;
            }

            if( i == 0){
                printf("Gracz o deskryptorze numer %d oczekuje na przeciwnika\n", clientSockets[0]);

                message = "Udalo ci sie polaczyc z serverem, oczekiwanie na przeciwnika...";
                data_size = strlen(message);
                
                send(clientSockets[i], &data_size, sizeof data_size, 0);
                send(clientSockets[i], message, data_size, 0);
            }
        }


        message = "Polaczono z przeciwnikiem.";
        data_size = strlen(message);
        send(clientSockets[0], &data_size, sizeof data_size, 0);
        send(clientSockets[0], message, data_size, 0);

        send(clientSockets[1], &data_size, sizeof data_size, 0);
        send(clientSockets[1], message, data_size, 0);

        pthread_t thread;
        pthread_create(&thread, NULL, gomokuGame, clientSockets);
        pthread_detach(thread);
 
    }
    printf("Koniec dzialania serwera\n");

    free(message);
    close(serverSocket);

    return 0;
}