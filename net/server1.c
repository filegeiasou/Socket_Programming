/*
Εργαστήριο ΛΣ 2 / Άσκηση 2 / Ερώτημα 3/ 2022-23
Ονοματεπώνυμο:Άγγελος Κωνσταντίνος Μεντζέλος
ΑΜ:21390132
Τρόπος μεταγλώττισης: gcc -o server1 server1.c -pthread
*/



#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

#define SOCKET_PATH "/tmp/socket"
#define MAX_SEQUENCE_SIZE 1024

void * connection(void *);

void calculate_mean(int* sequence, int size, double* mean) {
    int sum = 0;
    for (int i = 0; i < size; i++) {
        sum += sequence[i];
    }
    *mean = (double)sum / size;
}

    	

int main() 
{
    int server_fd, len,client_fd;
    struct sockaddr_un server_addr, client_addr;
    
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
    if (listen(server_fd, 5) == -1) {
        perror("Σφάλμα στη listen");
        exit(EXIT_FAILURE);
    }
    
    printf("Ο server εκτελείται. Περιμένει συνδέσεις...\n");
    //int rval ,rval1, n ,*a,rval2; 
    int i = 0;
    pthread_t thread1;
    while (client_fd = accept(server_fd, NULL, NULL)) 
    {
	    // Αποδοχή σύνδεσης από client
    
	    printf("Νέα σύνδεση από client.\n");
    	
	    
	    
	    if (pthread_create(&thread1, NULL, (void *) connection, (void *)&client_fd) != 0) 
	    {
		perror("pthread_create");
		exit(1);
	    }
	}
close(server_fd);
unlink(SOCKET_PATH);

return 0;
}



void * connection(void * client_fd)
{	
double mean;
int rval ,rval1, n ,*a,rval2; 
char buffer[MAX_SEQUENCE_SIZE];
int int_p = *(int * ) client_fd;

    // Αποδοχή της ακολουθίας από τον client
	   if (rval = read(int_p, &n, sizeof(int)) < 0)
	   {
	   	
	    	perror("reading stream message");
	    	return NULL;
	    }
		    
		    a = (int *) malloc(n*sizeof(int));
		    if (rval1 = read(int_p, a, n*sizeof(a)) < 0)
		    {
		    
			perror("reading stream message");
			return NULL;
		    }
			
		    // Υπολογισμός του μέσου όρου
		    calculate_mean(a, n, &mean);

		    // Έλεγχος και αποστολή του αποτελέσματος στον client
		    if (mean > 20) 
		    {
			snprintf(buffer, sizeof(buffer), "%.2lf %s", mean, "Sequence Ok");
		    } 
		    else 
		    {
			snprintf(buffer, sizeof(buffer), "%s", "Check Failed");
		    }

		    if (send(int_p, buffer, sizeof(buffer), 0) == -1) {
			perror("Σφάλμα στην αποστολή της απάντησης στον client");
			return NULL;
			exit(EXIT_FAILURE);
		    }
   close(int_p);
   pthread_exit(NULL);
}
