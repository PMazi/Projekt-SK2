#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <pthread.h>




int main(int argc, char* argv[]){

    struct sockaddr_in addrToConnect;
    memset(&addrToConnect, 0, sizeof addrToConnect);
    addrToConnect.sin_family = AF_INET;
    addrToConnect.sin_port = htons(atoi(argv[2]));
    addrToConnect.sin_addr.s_addr = inet_addr(argv[1]);

    int clientSocket = socket(AF_INET, SOCK_STREAM, 0);
    if( clientSocket == -1){
        perror("Blad w socket");
        exit(EXIT_FAILURE);
    }

    if( connect(clientSocket, (struct sockaddr*)&addrToConnect, sizeof addrToConnect) == -1){
        perror("Blad w connect");
        close(clientSocket);
        exit(EXIT_FAILURE);
    }

    char buff[1024];
    int data_size;
    while(1){
        recv(clientSocket, &data_size, sizeof data_size, 0);
        memset(buff, 0, sizeof buff);
        recv(clientSocket, buff, data_size, 0);
        printf("%s\n", buff);

        if(strcmp(buff, "Polaczono z przeciwnikiem.") == 0){
            break;
        }
    }

    char board[10][10];
    int rowsNumber = sizeof board / sizeof board[0];
    int columnsNumber = sizeof board[0] / sizeof board[0][0];

    char message[100];
    char move[100];
    
    int bytes;
    while (1) {

        recv(clientSocket, &data_size, sizeof data_size, 0);
        
        memset(board, 0, sizeof board);
        recv(clientSocket, board, data_size, 0);
        
        // Wyświetlanie planszy
        for (int row = 0; row < rowsNumber; row++) {
            if (row == 0) {
                printf("   | 0 | 1 | 2 | 3 | 4 | 5 | 6 | 7 | 8 | 9 |\n");
            }
            printf("--------------------------------------------\n");
            for (int column = 0; column < columnsNumber; column++) {
                if (column == 0) {
                    printf(" %d |", row);
                }
                printf(" %c |", board[row][column]);
            }
            printf("\n");
            if (row + 1 == rowsNumber){
                printf("--------------------------------------------\n");
            } 
        }

        recv(clientSocket, &data_size, sizeof data_size, 0);

        memset(message, 0, sizeof message);
        recv(clientSocket, message, data_size, 0);
        printf("%s\n", message);

        if (strcmp(message, "Podaj ruch (wiersz kolumna): ") == 0) {
            int allowedMove = 0;
            while (!allowedMove) {
                
                memset(move, 0, sizeof move);
                fgets(move, sizeof move, stdin);
                move[strlen(move) - 1] = '\0';

                data_size = strlen(move);
                send(clientSocket, &data_size, sizeof data_size, 0);
                send(clientSocket, move, data_size, 0);

                recv(clientSocket, &data_size, sizeof data_size, 0);
                memset(message, 0, sizeof message);
                recv(clientSocket, message, data_size, 0);
                if (strcmp(message, "Podales dane w zlym formacie!") == 0) {
                    printf("Zly format danych. Podaj 2 liczby w przedziale 0-9.\n");
                    continue;
                } 
                else if(strcmp(message, "Podales nieprawidlowe wspolrzedne!") == 0){
                    printf("Podales zle wspolrzedne, mozesz podac liczby w przedziale 0-9\n");
                    continue;
                }
                else if(strcmp(message, "Podales wspolrzedne, ktore sa juz zajete!") == 0){
                    printf("Podales wspolrzedne, ktore sa juz zajete!\n");
                    continue;
                }
                else {
                    allowedMove = 1;
                }
            }
        }
        else if(strcmp(message, "Oczekiwanie na ruch przeciwnika...") == 0){
            memset(message, 0, sizeof message);
            recv(clientSocket, &data_size, sizeof data_size, 0);
            recv(clientSocket, message, data_size, 0);

            if(strcmp(message, "Rozlaczenie przeciwnika") == 0){
                printf("Rozlaczono przeciwnika, wygrales\n");
                break;
            }
        }

        memset(message, 0, sizeof message);
        bytes = recv(clientSocket, message, sizeof message, 0);
        
        message[bytes] = '\0';
        
        if (strcmp(message, "Gratulacje, wygrales") == 0){
            printf("%s \n", message);
            break;
        }

        else if (strcmp(message, "Przegrales :(") == 0){
            printf("%s \n", message);
            break;
        }

        data_size = strlen("Poprawnie odebrano wiadomosc");
        send(clientSocket, &data_size, sizeof data_size, 0);
        send(clientSocket, "Poprawnie odebrano wiadomosc", data_size, 0);

        memset(message, 0, sizeof message);
        recv(clientSocket, &data_size, sizeof data_size, 0);
        recv(clientSocket, message, data_size, 0);

        if(strcmp(message, "Rozlaczenie przeciwnika") == 0){
            printf("Rozlaczono przeciwnika, wygrales\n");
            break;
        }

    }   

    close(clientSocket);
    return 0;
}
