#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <winsock2.h>
#include "cJSON.c"
#define MAX 80
#define PORT 12345
#define SA struct sockaddr

#define max_asami 200
#define max_string 10000
#define max_token 200
#define max_message 5000
int socket_maker();
void account_menu();
void main_menu(char []);
void chat_menu(char []);
void dash ();
void ce();
int main()
{
    int client_socket = socket_maker();
    closesocket(client_socket);
    ce();
    account_menu();
}
int socket_maker ()
{
    int client_socket, server_socket;
	struct sockaddr_in servaddr, cli;

	WORD wVersionRequested;
    WSADATA wsaData;
    int err;

	// Use the MAKEWORD(lowbyte, highbyte) macro declared in Windef.h
    wVersionRequested = MAKEWORD(2, 2);

    err = WSAStartup(wVersionRequested, &wsaData);
    if (err != 0) {
        // Tell the user that we could not find a usable Winsock DLL.
        printf("WSAStartup failed with error: %d\n", err);
        return 1;
    }

	// Create and verify socket
	client_socket = socket(AF_INET, SOCK_STREAM, 0);
	if (client_socket == -1) {
		printf("Socket creation failed...\n");
		exit(0);
	}

	// Assign IP and port
	memset(&servaddr, 0, sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	servaddr.sin_addr.s_addr = inet_addr("127.0.0.1");
	servaddr.sin_port = htons(PORT);

	// Connect the client socket to server socket
	if (connect(client_socket, (SA*)&servaddr, sizeof(servaddr)) != 0) {
		printf("Connection to the server failed...\n");
		exit(0);
	}
    return client_socket;

}

void account_menu ()
{

    int a_menu=0;
    dash();
    printf("Account menu : \n1: Register \n2: login \n3: Exit \n" );
    dash();
    scanf("%d", & a_menu);
    int client_socket = socket_maker();
    switch(a_menu)
    {
    case 1:
    {
        printf("Welcome!!! \nYou just need to type a username and a password to register \n");
        char username [max_asami] , password [max_asami] ,result [2*max_asami]= "register ";
        printf("Enter your username please:\n");
        scanf("%s", username);
        printf("Enter your password please:\n");
        scanf("%s", password);

        strcat(result , username);
        strcat(result , ", ");
        strcat(result , password);
        strcat(result , "\n");
        send(client_socket,result, sizeof(result), 0);

        char string[max_string];
        recv(client_socket ,string , sizeof(string), 0);

        cJSON * json = cJSON_CreateObject();
        json = cJSON_Parse(string);
        char state [20] , content[50];
        strcpy(state,cJSON_GetObjectItem( json , "type")->valuestring);
        strcpy(content ,cJSON_GetObjectItem( json , "content")->valuestring);
        if(strcasecmp(state , "Error") == 0)
            printf("%s\n", content);
        else
            printf("Your Account Was Succesfully Created\n");

        break;
    }
    case 2:
    {
        char username [max_asami] , password [max_asami] ,result [2*max_asami] = "login ";
        printf("Enter your username please:\n");
        scanf("%s", username);
        printf("Enter your password please:\n");
        scanf("%s", password);

        strcat(result , username);
        strcat(result , ", ");
        strcat(result , password);
        strcat(result , "\n");

        send(client_socket,result, sizeof(result), 0);

        char string[max_string];
        recv(client_socket ,string , sizeof(string), 0);

        cJSON * json = cJSON_CreateObject();
        json = cJSON_Parse(string);

        char state [20] , token[max_token];
        strcpy(state,cJSON_GetObjectItem( json , "type")->valuestring);
        strcpy(token,cJSON_GetObjectItem( json , "content")->valuestring);

        if(strcmp(state , "Error")== 0)
            printf("%s\n", token);
        else
        {
            main_menu(token);
            return;
        }
        break;


    }
    case 3:
    {
        printf("hope to see you again!\n");
        return;
    }

    default:
        printf("Invalid command\n");
    }
    closesocket(client_socket);
    account_menu();

}

void main_menu (char token[])
{
    int m_menu =0;
    dash();
    printf ("Main menu : \n1: Create Channel \n2: Join Channel \n3: Logout\n");
    dash();
    scanf("%d", & m_menu);
    int client_socket = socket_maker();
    switch(m_menu)
    {
    case 1:
    {
        char channel_name [max_asami] ,result [max_asami]= "create channel ";
        printf("Enter the channel's name please:\n");
        scanf("%s", channel_name);

        strcat(result , channel_name);
        strcat(result , ", ");
        strcat(result , token);
        strcat(result , "\n");
        send(client_socket,result, sizeof(result), 0);

        char string[max_string];
        recv(client_socket ,string , sizeof(string), 0);
        cJSON * json = cJSON_CreateObject();
        json = cJSON_Parse(string);
        char state [20] , content[50];
        strcpy(state,cJSON_GetObjectItem( json , "type")->valuestring);
        strcpy(content ,cJSON_GetObjectItem( json , "content")->valuestring);
        if(strcasecmp(state , "Error") == 0)
            printf("%s\n", content);
        else
        {
            printf("Channel Was Succesfully Created\n");
            chat_menu(token);
            return;
        }

        break;

    }
    case 2:
    {
        char channel_name [max_asami] ,result [max_asami]= "join channel ";
        printf("Enter the channel's name please:\n");
        scanf("%s", channel_name);

        strcat(result , channel_name);
        strcat(result , ", ");
        strcat(result , token);
        strcat(result , "\n");
        send(client_socket,result, sizeof(result), 0);

        char string[max_string];
        recv(client_socket ,string , sizeof(string), 0);
        cJSON * json = cJSON_CreateObject();
        json = cJSON_Parse(string);
        char state [20] , content[50];
        strcpy(state,cJSON_GetObjectItem( json , "type")->valuestring);
        strcpy(content ,cJSON_GetObjectItem( json , "content")->valuestring);
        if(strcasecmp(state , "Error") == 0)
            printf("%s\n", content);
        else
        {
            printf("You have Succesfully joined to %s\n",channel_name);
            chat_menu(token);
            return;
        }

        break;
    }
    case 3:
    {
        char result [max_asami]= "logout ";
        strcat(result , token);
        strcat(result , "\n");
        send(client_socket,result, sizeof(result), 0);
        account_menu();
        return;
        break;
    }
    }

    closesocket(client_socket);
    main_menu(token);
}
void chat_menu(char token[])
{
    int c_menu;
    dash();
    printf("Chat Menu : \n1: Send Message \n2: Refresh \n3: Channel Members \n4: Leave Channel\n");
    dash();
    scanf("%d" , & c_menu);
    int client_socket = socket_maker();
    switch(c_menu)
    {
    case 1:
    {
        char message[max_message] ,result [max_message+40]= "send ";
        printf("Enter your message:\n");
        scanf(" %[^\n]", message);
        strcat(result , message);
        strcat(result , ", ");
        strcat(result , token);
        strcat(result , "\n");
        send(client_socket,result, sizeof(result), 0);
        break;

    }
    case 2:
    {
        char result [max_token]= "refresh ";
        strcat(result ,token);
        strcat(result , "\n");
        send(client_socket,result, sizeof(result), 0);

        char string [max_string];
        recv(client_socket ,string , sizeof(string), 0);

        cJSON * json = cJSON_CreateObject();
        json = cJSON_Parse(string);
        char state [20] ,sender [max_asami] , message[max_message];
        strcpy(state,cJSON_GetObjectItem( json , "type")->valuestring);
        if(strcmp(state, "Error") == 0 )
            printf("%s" , cJSON_GetObjectItem(json , "content"));
        else
        {
            cJSON * array = cJSON_GetObjectItem( json , "content");

            for (int i =0 ;i <cJSON_GetArraySize(array) ; i++)
            {
                cJSON * arrayitem = cJSON_GetArrayItem(array , i);
                strcpy(sender , cJSON_GetObjectItem(arrayitem , "sender")->valuestring);
                strcpy(message, cJSON_GetObjectItem(arrayitem , "content")->valuestring);
                printf("%s : %s\n",sender, message);
            }
        }
        break;
    }
    case 3:
    {
        char result [max_token]= "channel members ";
        strcat(result ,token);
        strcat(result , "\n");
        send(client_socket,result, sizeof(result), 0);

        char string [max_string];
        recv(client_socket ,string , sizeof(string), 0);

        cJSON * json = cJSON_CreateObject();
        json = cJSON_Parse(string);
        char state [20] ;
        strcpy(state,cJSON_GetObjectItem( json , "type")->valuestring);
        if(strcmp(state, "Error") == 0)
        {
            printf("%s", cJSON_GetObjectItem(json , "content")->valuestring);
        }
        cJSON * members =cJSON_GetObjectItem(json , "content");
        printf("Channel Members: ");
        for (int i=0 ; i< cJSON_GetArraySize(members)-1 ; i++)
        {
            printf("%s ," , cJSON_GetArrayItem( members , i)->valuestring);
        }
        printf("%s " , cJSON_GetArrayItem( members , cJSON_GetArraySize(members)-1)->valuestring);
        printf("\n");
        break;
    }
    case 4:
    {
        char result [max_token]= "leave ";
        strcat(result ,token);
        strcat(result , "\n");
        send(client_socket,result, sizeof(result), 0);
        main_menu(token);
        return;
    }
    }
    closesocket(client_socket);
    chat_menu(token);
}
void dash()
{
    printf("------------------------------------\n");
}
void ce()
{
    printf("        /\\\\\\\\\\\\\\\\\\  /\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\      \n");
    printf("       /\\\\\\//////// \/\\\\\\///////////      \n");
    printf("      /\\\\\\/          \/\\\\\\                \n");
    printf("     /\\\\\\             \/\\\\\\\\\\\\\\\\\\\\\\\\\\\\     \n");
    printf("     \\/\\\\\\             \/\\\\\\//////////     \n");
    printf("      \\//\\\\\\            \/\\\\\\             \n");
    printf("        \\///\\\\\\          \/\\\\\\            \n");
    printf("           \\////\\\\\\\\\\\\\\\\\\ \/\\\\\\\\\\\\\\\\\\\\\\\\  \n");
    printf("              \\/////////  \///////////// \n");
}
