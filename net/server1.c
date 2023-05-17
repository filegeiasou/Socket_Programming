#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define SOCKET_PATH "/tmp/socket"
#define MAX_SEQUENCE_SIZE 100

void calculate_mean(int* sequence, int size, double* mean) {
    int sum = 0;
    for (int i = 0; i < size; i++) {
        sum += sequence[i];
    }
    *mean = (double)sum / size;
}

int main() 
{
    int server_fd, client_fd, len;
    struct sockaddr_un server_addr, client_addr;
    char buffer[MAX_SEQUENCE_SIZE];
    int ra;
    double mean;
    pid_t pid;

    // Δημιουργία του UNIX-domain socket
    server_fd = socket(AF_UNIX, SOCK_STREAM, 0);
    if (server_fd == -1) {
        perror("Σφάλμα στη δημιουργία του socket");
        exit(EXIT_FAILURE);
    }

    unlink(SOCKET_PATH);
    server_addr.sun_family = AF_UNIX;
    strncpy(server_addr.sun_path, SOCKET_PATH, sizeof(server_addr.sun_path) - 1);

    // Σύνδεση του socket με την διεύθυνση
    if (bind(server_fd, (struct sockaddr*)&server_addr, sizeof(server_addr)) == -1) {
        perror("Σφάλμα στη bind");
        exit(EXIT_FAILURE);
    }

    // Ετοιμασία του socket για αποδοχή συνδέσεων
    if (listen(server_fd, 10) == -1) {
        perror("Σφάλμα στη listen");
        exit(EXIT_FAILURE);
    }

    printf("Ο server εκτελείται. Περιμένει συνδέσεις...\n");
    int rval ,rval1, n ,*a,rval2; 
    while (1) 
    {
    	    //int in=fork();
	    // Αποδοχή σύνδεσης από client
            client_fd = accept(server_fd, NULL, NULL);
	    if (client_fd == -1) 
	    {
		perror("Σφάλμα στην accept");
		exit(EXIT_FAILURE);
	    }
	    printf("Νέα σύνδεση από client.\n");
	    if((pid=fork())==0)
	    {
	    	close(server_fd);
		while (1) 
    		{
	    			// Αποδοχή της ακολουθίας από τον client
		    if (rval = read(client_fd, &n, sizeof(int)) < 0)
		    	perror("reading stream message");
		    if(n>0)
		    {
			    
			    a = (int *) malloc(n*sizeof(int));
			    if (rval1 = read(client_fd, a, sizeof(a)) < 0)
				perror("reading stream message");
					
			    // Υπολογισμός του μέσου όρου
			    calculate_mean(a, n, &mean);

			    // Έλεγχος και αποστολή του αποτελέσματος στον client
			    if (mean > 20) 
			    {
				snprintf(buffer, sizeof(buffer), "%lf %s", mean, "Sequence Ok");
			    } 
			    else 
			    {
				snprintf(buffer, sizeof(buffer), "%s", "Check Failed");
			    }

			    if (send(client_fd, buffer, sizeof(buffer), 0) == -1) {
				perror("Σφάλμα στην αποστολή της απάντησης στον client");
				exit(EXIT_FAILURE);
			    }
		    }
		    close(client_fd);
    		
    		}
	    }
	    
		
	    // Αποδοχή της ακολουθίας από τον client
	   /* if (rval = read(client_fd, &n, sizeof(int)) < 0)
	    	perror("reading stream message");
	    if(n>0)
	    {
		    
		    a = (int *) malloc(n*sizeof(int));
		    if (rval1 = read(client_fd, a, sizeof(a)) < 0)
			perror("reading stream message");
				
		    // Υπολογισμός του μέσου όρου
		    calculate_mean(a, n, &mean);

		    // Έλεγχος και αποστολή του αποτελέσματος στον client
		    if (mean > 20) 
		    {
			snprintf(buffer, sizeof(buffer), "%lf %s", mean, "Sequence Ok");
		    } 
		    else 
		    {
			snprintf(buffer, sizeof(buffer), "%s", "Check Failed");
		    }

		    if (send(client_fd, buffer, sizeof(buffer), 0) == -1) {
			perror("Σφάλμα στην αποστολή της απάντησης στον client");
			exit(EXIT_FAILURE);
		    }
	    }
	    close(client_fd);*/
	}
close(server_fd);
unlink(SOCKET_PATH);

return 0;
}
