# Secure Key Exchange Using Diffie-Hellman Algorithm

This project implements a secure key exchange mechanism using the Diffie-Hellman algorithm. It consists of a server and client program, enabling the exchange of a secret key that is subsequently used for encrypted communication. The key exchange and encryption are implemented using modular arithmetic and XOR operations.

## Features
- **Key Exchange:** Implements the Diffie-Hellman key exchange protocol.
- **Encryption:** Uses XOR operation with the shared secret key for secure message exchange.
- **Prime Number Validation:** Ensures that valid prime numbers and primitive roots are used for the key exchange.
- **Socket Programming:** Establishes a TCP connection for communication between the client and server.

## Requirements
- C++ compiler
- Linux-based OS (preferred) or a compatible environment for socket programming
- Basic knowledge of socket programming and cryptography concepts

## Files
- `server.cpp`: Contains the implementation of the server program.
- `client.cpp`: Contains the implementation of the client program.

## How It Works
1. The server and client both generate their private keys.
2. Using the private keys, each calculates a public key using the formula:

PublicKey = (G^PrivateKey) % P

where `G` is the generator and `P` is a prime number.
3. The public keys are exchanged between the server and client.
4. Both parties calculate the shared secret key:

SecretKey = (OtherPartyPublicKey^OwnPrivateKey) % P

5. The shared secret key is used to encrypt and decrypt messages using the XOR operation.

## Usage
### Server
1. Compile the server program:
g++ -o server server.cpp

## Run the server:
./server

Enter the port number, a prime number P, and a generator G as prompted.

## Client
Compile the client program:
g++ -o client client.cpp

## Run the client:
./client
Enter the server's IP address, port number, a prime number P, and a generator G as prompted.

