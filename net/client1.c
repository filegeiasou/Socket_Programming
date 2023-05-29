#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define BUFFER_SIZE 1024

int main(int argc , char **argv) {
    int client_fd, server_len;
    struct sockaddr_un server_address;
    char buffer[BUFFER_SIZE];
    int sequence_length;


    // Αρχικοποίηση της διεύθυνσης του server
    server_address.sun_family = AF_UNIX;
    strcpy(server_address.sun_path, argv[1]);

    
    while (1) 
    {
        if ((client_fd = socket(AF_UNIX, SOCK_STREAM, 0)) == -1) 
	{
		perror("Σφάλμα στη δημιουργία του socket");
		exit(EXIT_FAILURE);
    	}
      	if (connect(client_fd, (struct sockaddr *)&server_address, sizeof(struct sockaddr_un)) < 0) 
	{
		perror("Σφάλμα στη σύνδεση με τον server");
		exit(EXIT_FAILURE);
	}
    	printf("Δώσε τη μήκος της ακολουθίας (0 για έξοδο): \n");
        scanf("%d", &sequence_length);
        int *buffer1=(int*)malloc(sequence_length*sizeof(int));
        // Αποστολή του μήκους της ακολουθίας
        if (write(client_fd, &sequence_length, sizeof(sequence_length)) < 0) {
            perror("Σφάλμα στην αποστολή του μήκους της ακολουθίας");
            exit(EXIT_FAILURE);
        }
        if(sequence_length==0)
        	break;

        // Ανάγνωση της ακολουθίας από το χρήστη
        printf("Δώσε τα στοιχεία της ακολουθίας:\n");
        for (int i = 0; i < sequence_length; i++) {
            scanf("%d", &buffer1[i]);
        }
        

        // Αποστολή των στοιχείων της ακολουθίας
        if (write(client_fd, buffer1,sequence_length*sizeof(buffer1)) < 0) {
            perror("Σφάλμα στην αποστολή των στοιχείων της ακολουθίας");
            exit(EXIT_FAILURE);
        }

        // Αποδοχή της απάντησης από τον server
	if (read(client_fd, buffer, sizeof(buffer))<0) {
		perror("Σφάλμα στην αποδοχή της απάντησης από τον server");
		exit(EXIT_FAILURE);
	}
	printf("Απάντηση από τον server: %s\n", buffer);
	close(client_fd);
    }

close(client_fd);

return 0;
}

