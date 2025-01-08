#include <iostream>
#include <cstdlib>
#include <ctime>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <cstring>
#include <bits/stdc++.h>

using namespace std;

int connectToServer(const char* ip, int port) {
	int sock = socket(AF_INET, SOCK_STREAM, 0); // create a TCP socket

	// condition to handle socket creation 
	if (sock < 0) {
		cerr << "\nFailed to create socket" << endl;

		exit(EXIT_FAILURE);
	}

	// address structure
	struct sockaddr_in addr = {0};
	addr.sin_family = AF_INET;
	addr.sin_port = htons(port);
	addr.sin_addr.s_addr = inet_addr(ip);

	// condition to handle the server port
	if (connect(sock, (struct sockaddr *) &addr, sizeof(addr)) < 0) {
		cerr << "\nRun server program first or check the port number" << endl;

		close(sock);
		exit(EXIT_FAILURE);
	}

	cout << "\nClient is connected to Server" << endl;

	return sock;
}

// function to generate a random number
int randInRange(int low, int high) {
	return rand() % (high - low - 1) + (low + 1);
}

// function to compute the modular exponentation
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
	char ip[50];

	cout << "\nEnter sever's IP address: ";
	cin >> ip;

	// check if the IP address is empty
	if (strlen(ip) == 0) {
		cerr << "IP address is empty" << endl;

		exit(EXIT_FAILURE);
	}

	// check if the IP address is 127.0.0.1
	if (strcmp(ip, "127.0.0.1") != 0) {
		cerr << "IP address is invalid. It should be 127.0.0.1" << endl;

		exit(EXIT_FAILURE);
	}

	string portstr;

	cout << "\nEnter port number: ";
	cin >> portstr;

	while (!isNumber(portstr)) {
		cout << "Invalid input. Please enter a valid port number: ";
		cin >> portstr;
	}
	
	int port = stoi(portstr);
	int sock = connectToServer(ip, port); // connect to the server

	long P, G;

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

	srand(static_cast<unsigned int>(time(0))); // seed to generate random numbers

	long b = randInRange(1, P); // client's private key

	cout << "\nClient's Private Key, b = " << b << endl;

	long B = powermod(G, b, P); // client's public key

	// send client's public key
	if (send(sock, &B, sizeof(B), 0) <= 0) {
		cerr << "\nFailed to send client's public key" << endl;

		close(sock);
		exit(EXIT_FAILURE);
	}

	cout << "\nClient's Public Key, B = " << B << endl;

	long A;

	// condition to handle server's public key
	if (recv(sock, &A, sizeof(A), 0) <= 0) {
		cerr << "\nFailed to receive server's public key" << endl;

		close(sock);
		exit(EXIT_FAILURE);
	}

	cout << "\nServer's Public Key, A = " << A << endl;

	long S = powermod(A, b, P); // shared secret key

	cout << "\nSecret Key, S = " << S << endl;

	long cipher;

	// condition to handle the received message from serevr
	if (recv(sock, &cipher, sizeof(cipher), 0) <= 0) {
		cerr << "\nFailed to receive encrypted message" << endl;

		close(sock);
		exit(EXIT_FAILURE);
	}

	cout << "\nMessage received from Server: " << cipher << endl;

	long decipher = cipher ^ S; // decrypt the message using XOR with the shared secret key

	cout << "\nDecrypted message: " << decipher << endl;

	// close the server socket
	close(sock);

	return 0;
}