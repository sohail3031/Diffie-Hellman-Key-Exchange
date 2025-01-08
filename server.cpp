#include <iostream>
#include <cstdlib>
#include <ctime>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <bits/stdc++.h>

using namespace std;

// function to create a server and accept a connection from a client
int createServer(int port) {
	int sersock = socket(AF_INET, SOCK_STREAM, 0); // create a TCP socket

	// condition to check if the socket is created or not
	if (sersock < 0) {
		cerr << "\nFailed to create socket" << endl;

		exit(EXIT_FAILURE);
	}

	// address structure
	struct sockaddr_in addr = {0};
	addr.sin_family = AF_INET;
	addr.sin_port = htons(port);
	addr.sin_addr.s_addr = INADDR_ANY;

	// condition to check the binding failure
	if (bind(sersock, (struct sockaddr *) &addr, sizeof(addr)) < 0) {
		cerr << "\nBinding failed" << endl;

		close(sersock);
		exit(EXIT_FAILURE);
	}

	cout << "\nServer is running. Waiting for client..." << endl;

	// condition for listening failure
	if (listen(sersock, 5) < 0) {
		cerr << "\nListening failed" << endl;

		close(sersock);
		exit(EXIT_FAILURE);
	}

	int sock = accept(sersock, NULL, NULL); // accept a connection from a client

	// condition for accept failure
	if (sock < 0) {
		cerr << "\nAccept failed" << endl;

		close(sersock);
		exit(EXIT_FAILURE);
	}

	cout << "\nConnection established" << endl;

	return sock;
}

// function to generate a random number
int randInRange(int low, int high) {
	return rand() % (high - low - 1) + (low + 1);
}

// function to compute modular exponentation
long powermod(long a, long b, long q) {
	long res = 1;

	for (long i = 0; i < b; i++) {
		res = (res * a) % q;
	}

	return res;
}

// function to check if "P" is prime
bool isPrime(long n) {
	if (n <= 1) {
		return false;
	} 

	if (n <= 3) {
		return true;
	}

	if (n % 2 == 0 || n % 3 == 0) {
		return false;
	}

	for (long i = 5; i * i <= n; i += 6) {
		if (n % i == 0 || n % (i + 2) == 0) {
			return false;
		}
	}

	return true;
}

// function to check the entered input is a number
bool isNumber(const string &s) {
	return !s.empty() && all_of(s.begin(), s.end(), ::isdigit);
}

int main() {
	string portstr;

	cout << "\nEnter port number: ";
	cin >> portstr;

	while (!isNumber(portstr)) {
		cout << "Invalid input. Please enter a valid port number: ";
		cin >> portstr;
	}
	
	int port = stoi(portstr);
	int sock = createServer(port); // create a server and wait for the client connection
	long P, G;
	string strg;

	do {
		cout << "\nEnter a prime number P: ";
		cin >> P;

		if (cin.fail()) {
            cin.clear(); // clear the error flag
            cin.ignore(numeric_limits<streamsize>::max(), '\n'); // discard invalid input

            cerr << "Invalid input. Please enter a valid prime number." << endl;
            
			continue;
        }

		if (!isPrime(P)) {
			cerr << "The number entered is not a prime. Please enter a prime number" << endl;
		}
	} while (!isPrime(P));

	while(true) {
		cout << "\nEnter primitive root of P (generator) G: ";
		cin >> G;

		if (cin.fail()) {
            cin.clear(); // clear the error flag
            cin.ignore(numeric_limits<streamsize>::max(), '\n'); // discard invalid input

            cerr << "Invalid input. Please enter a valid number for G" << endl;
            
			continue;
        } else if (G <= 0) {
			cerr << "The generator should be greater than zero" << endl;

			continue;
		} else {
			break;
		}
	}

	long B;

	// condition to handle the public key received from the client
	if (recv(sock, &B, sizeof(B), 0) <= 0) {
		cerr << "\nFailed to receive client's public key" << endl;

		close(sock);
		exit(EXIT_FAILURE);
	}

	cout << "\nClient's Public Key, B = "<< B << endl;

	srand(static_cast<unsigned int>(time(0))); // seed to generate random number
	
	long a = randInRange(1, P); // server's private key

	cout << "\nServer's Private Key, a = " << a << endl;

	long A = powermod(G, a, P); // server's public key

	// send server's public key
	if (send(sock, &A, sizeof(A), 0) <= 0) {
		cerr << "\nFailed to send server's public key" << endl;

		close(sock);
		exit(EXIT_FAILURE);
	}

	cout << "\nServer's Public Key, A = " << A << endl;

	long S = powermod(B, a, P); // shared secret

	cout << "\nSecret Key, S = " << S << endl;

	long msg;

	while(true) {
		cout << "\nEmter a message (number) to send: ";
		cin >> msg;

		if (cin.fail()) {
            cin.clear(); // clear the error flag
            cin.ignore(numeric_limits<streamsize>::max(), '\n'); // discard invalid input

            cerr << "Invalid input. Please enter a valid number for message" << endl;
            
			continue;
        } else {
			break;
		}
	}

	long cipher = msg ^ S; // encrypts the message using XOR with shared secret 

	// condition to handle the send message
	if (send(sock, &cipher, sizeof(cipher), 0) <= 0) {
		cerr << "\nFailes to send encrypted message" << endl;

		close(sock);
		exit(EXIT_FAILURE);
	}

	cout << "\nEncrypted message send to client: " << cipher << endl;

	// close the client socket
	close(sock);

	return 0;
}