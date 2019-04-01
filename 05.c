#include <stdio.h>
#include <stdlib.h>
#include <signal.h>

typedef struct Node_s {
	struct Node_s *next;
	unsigned int val;
} Node_s, *Node;

Node new_node(unsigned int val, Node prevNode) {
	Node node = (Node)malloc(sizeof(Node_s));
	if (prevNode != NULL) {
		prevNode->next = node;
	}
	node->next = NULL;
	node->val = val;

	return node;
}

void destroyLinkedList(Node head) {
	Node current = head;
	Node temp;
	while(current != NULL) {
		temp = current->next;
		free(current);
		current = temp;
	}
}



// Utility for percentage calculation.
float percent(unsigned int part, unsigned int max) {
	return ( (float)part / max )*100.0;
}


// Keep track of completition status.
unsigned int completition;
unsigned int max;

// Artificially slow down this thing
int weight = 1;


// Sieve of Eratosthenes. Returns a dynamically allocated linked list of prime numbers
// up to n. It updates completition with numbers analyzed until now (either found
// to be prime or composite), so n can be used as max in the percentage calculation.
Node sieve(size_t n) {
	unsigned int *table = (unsigned int *)malloc( (sizeof(unsigned int)*n) + 1 );
	
	table[0] = 0;
	table[1] = 0;
	completition++;

	size_t i;
	for(i = 2; i < n+1; i++) {
		table[i] = 1;
	}

	size_t j;
	for(i = 2; i < n+1; i++) {
		//printf("p: %d | %d\n", i, table[i]);
		//fflush(stdout);
		if(table[i]) {
			completition++;

			for(j = i*i; j < n+1; j += i) {
				table[j] = 0;
				sleep(weight);
				completition++;
			}
		}
	}

	Node head = NULL;
	Node current = NULL;
	for(i = 2; i < n+1; i++) {
		//printf("Here %d : %d\n", i, table[i]);
		//fflush(stdout);
		if(table[i]) {
			current = new_node((unsigned int)i, current);
			
			if(i == 2) {
				head = current;
				//printf("Got head: %p\n", head);
				//fflush(stdout);
			}
		}
	}

	free(table);

	return head;

}

void print_primes(Node head) {
	printf("{ ");
	while(head != NULL) {
		printf("%u ", head->val);
		head = head->next;
	}
	printf("}\n");
}

// SIGINT handle
void sigint_handler(int sig) {
	printf("Calculating, done %%%.2f\n", percent(completition, max));
}

int main(int argc, char *argv[]) {
	if (argc < 2) {
		fprintf(stderr, "use: 05.exe N\n");
		fprintf(stderr, "\nCalculate prime numbers up to N. Press CTR-C (SIGINT) for a status report\n");
		exit(0);
	}

	signal(SIGINT, sigint_handler);
	max = (unsigned int)atol(argv[1]);
	completition = 0;

	Node head = sieve(max);	

	printf("Primes:\n");
	print_primes(head);
	destroyLinkedList(head);

	return 0;
}
