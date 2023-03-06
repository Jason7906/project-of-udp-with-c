# UDP Client-Server Communication
This is a simple implementation of UDP client-server communication in C programming language. The client sends a message to the server, and the server echoes the same message back to the client.

## Getting Started
To run this program, you need to have a C compiler installed on your system. You can use any C compiler of your choice.

Clone the repository:
```bash=
git clone https://github.com/Jason7906/udp-client-server.git
```

Compile the client and server programs:
```bash=
gcc client.c -o client -lm
gcc server.c -o server
```
Run the server:

```bash=
./server <port>
```

Where <port> is the port number on which the server will listen for incoming connections.

Run the client:

```bash=
./client <server-ip> <server-port> <message>
```
Where <server-ip> is the IP address of the server, <server-port> is the port number on which the server is listening, and <message> is the message you want to send to the server.

## How to do retry test
use traggic tool on Linux to simulate a congested network with packet loss
in my case because i use local network so the <network interface name> is lo
```bash=
sudo tc qdisc add dev <network interface name> root netem loss 50%
```
then it will be like this!
[image](https://user-images.githubusercontent.com/90664935/223033841-aff63f25-8085-4bcc-b88c-8b5c525179bf.png)
  
and if you want it stop. You can use the following instruction
```bash=
sudo tc qdisc del dev <network interface name> root netem
```

## How It Works
The client sends a message to the server using the sendto() function. The server receives the message using the recvfrom() function and echoes the same message back to the client using the sendto() function.

The client uses a timeout mechanism to handle situations where the server does not respond within a certain amount of time. The client will retry sending the message up to a maximum number of times.
## Demo video
https://youtu.be/Ze8CAZni9mc
## License
This project is licensed by Jason
