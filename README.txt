Charalambos Kyriakou

UDP_Ex
1.  Go to the UDP client directory and compile the .c file (gcc UDP_client.c -o client.out)
2.  Go to the UDP server directory and compile the .c file (gcc UDP_server.c -o server.out)
3.  Open 2 terminals.
4.  On the first, navigate to the UDP server directory and enter the following command:
"./server.out 'argument1'" as 'argument1' enter the port number.
5.  On the second, navigate to the UDP client directory and enter the following command:
"./client.out argument1 argument2" as 'argument1' enter the port number (must be the same as the server is using)
as 'argument2' enter the host name, in our case enter "localhost".

TCP_Ex
1.  Go to the TCP client directory and compile the .c file (gcc TCP_client.c -o client.out)
2.  Go to the TCP server directory and compile the .c file (gcc TCP_server.c -o server.out)
3.  Open 2 terminals.
4.  On the first, navigate to the TCP server directory and enter the following command:
"./server.out 'argument1'" as 'argument1' enter the port number.
5.  On the second, navigate to the TCP client directory and enter the following command:
"./client.out argument1 argument2" as 'argument1' enter the port number (must be the same as the server is using)
as 'argument2' enter the host name, in our case enter "localhost".

BOTH SERVER DIRECTORIES CONTAIN A FILE NAMED file.c THAT IS AN EXAMPLE FILE THAT
CAN BE USED WHEN THE CLIENT ASKS FOR A FILE NAME IN ORDER TO DOWNLOAD IT.

Question : Do you observer any differences in the file transfer process?

Answer : In our case we do not, that is because we are using the same NIC
to transfer the data, so we do not observe any packet loses. If it was a real client-server 
application the UDP would not be reliable when downloading a file. That is because UDP does not
ensure that all packets sent between the client and the server will be delivered.
On the other hand TCP does. 