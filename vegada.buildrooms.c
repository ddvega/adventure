/*******************************************************************************
** Program:       Assignment 1 - CS344
** Author:        David Vega
** Date:          6/24/20
** Description:   This program creates an array of struct rooms and randomly
**                assigns names and connections to each room. Once the array is
**                created, the data is then written to a file for
**                vegada.adventure to read from.
*******************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <sys/stat.h>
#include <time.h>
#include <string.h>
#include "vegada.buildrooms.h"

/*******************************************************************************
**  Main function
*******************************************************************************/
int main()
{
    //seed the random function
    srand(time(0));

    // get random number between 0 and 100000
    char *num = int2string(rand() % 100001, 20);

    // concat name of directory and random number
    char dirname[30]; // holds space for directory name
    strcpy(dirname, "vegada.rooms."); // directory name
    strcat(dirname, num); // finish directory name by adding random number
    free(num);

    // create a directory
    mkdir(dirname, 0755);

    // array of room structs
    struct Room rooms[7];

    // populate array of room structs
    initRooms(rooms);

    // build connections between rooms in array of room structs
    while (graphFull(rooms) == 0)
    {
        addRandomConnection(rooms);
    }

    // print info on each room to terminal
    printRoomInfo(rooms);

    // create files and fill with room data
    write2files(dirname, rooms);

    return 0;
}

/*******************************************************************************
**  reads the value of an integer and converts it to string
*******************************************************************************/
char *int2string(int x, int bufferSize)
{

    // create space in heap for char *
    char *str = malloc(bufferSize * sizeof(char));

    // copy int to string
    sprintf(str, "%d", x);
    return str;
}

/*******************************************************************************
**  Checks if two structs are the same by checking their id
*******************************************************************************/
int isSameRoom(struct Room *x, struct Room *y)
{
    // return 1 (yes) if id's match
    return (x->id == y->id) ? 1 : 0;
}

/*******************************************************************************
**  creates connections between rooms by assigning id's to connections array
*******************************************************************************/
void connectRoom(struct Room *x, struct Room *y)
{
    int connected[] = {0, 0}; // tracks if rooms have been connected
    for (int i = 0; i < 6; i++)
    {

        // checks for an empty array in x's connection list and adds y
        if (x->connections[i] == -1 && connected[0] == 0)
        {
            x->connections[i] = y->id;
            connected[0] = 1;
        }

        // checks for an empty array in y's connection list and adds x
        if (y->connections[i] == -1 && connected[1] == 0)
        {
            y->connections[i] = x->id;
            connected[1] = 1;
        }
    }
}

/*******************************************************************************
**  checks if a connection exists between two rooms
*******************************************************************************/
int connectionExists(struct Room *x, struct Room *y)
{
    // loop through all 6 possible connections
    for (int i = 0; i < 6; i++)
    {
        // checks if rooms are connected by checking connections and id
        if (x->connections[i] == y->id || y->connections[i] == x->id)
        {
            return 1;
        }
    }
    return 0;
}

/*******************************************************************************
**  checks if the connections array in struct room is at capacity (6)
*******************************************************************************/
int connectionAvailable(struct Room *x)
{
    // loop through 6 possible connections
    for (int i = 0; i < 6; i++)
    {
        // checks for an empty slot in the connections array
        if (x->connections[i] == -1)
        {
            return 1;
        }
    }
    return 0;
}

/*******************************************************************************
**  returns a random room from the array of rooms
*******************************************************************************/
struct Room *getRandom(struct Room *rooms)
{
    // random number between 0 and 6 used to select a random room from rooms arr
    return &rooms[rand() % 7];
}

/*******************************************************************************
**  randomly connects rooms together by checking if connection is available
**  and making sure that the rooms being checked are not the same room
*******************************************************************************/
void addRandomConnection(struct Room *rooms)
{
    // empty structs will be replaced with rooms from rooms array
    struct Room *A;
    struct Room *B;

    while (true)
    {
        A = getRandom(rooms); // room A filled with random room

        // checks if random room A has a connection available in its connections
        if (connectionAvailable(A) == 1)
        {
            break; // exit loop when room with an available connection is found
        }
    }

    do
    {
        B = getRandom(rooms); // room B filled with random room
    }
        // loop as long as connection is available, room not same and connection
        // doesn't already exist
    while (connectionAvailable(B) == 0 || isSameRoom(A, B) == 1 ||
           connectionExists(A, B) == 1);

    connectRoom(A, B); //conditions met, connect rooms
}

/*******************************************************************************
**  checks if every room in the array has the minimum number of connections
**  to other rooms in the array.
*******************************************************************************/
int graphFull(struct Room *rooms)
{
    // loop through all rooms in rooms array
    for (int i = 0; i < 7; i++)
    {
        int count = 0;

        // loop through connections array in each room
        for (int j = 0; j < 6; j++)
        {
            // checks if connection slot is not empty and increments count
            if (rooms[i].connections[j] != -1)
            {
                count++;
            }
        }

        // room has less than the minimum 3 connections required
        if (count < 3)
        {
            return 0; // graph is not full
        }

    }
    return 1; // graph is full
}

/*******************************************************************************
**  initializes array of rooms with default values
*******************************************************************************/
void initRooms(struct Room *rooms)
{
    // array of ints with random elements between 0 and 6
    int *rChoice = randArray(7);

    // loop through all rooms
    for (int i = 0; i < 7; i++)
    {
        if (i == 0) //start room
        {
            rooms[i].type = ROOM_TYPES[0];
        } else if (i == 6) //end room
        {
            rooms[i].type = ROOM_TYPES[2];
        } else // mid rooms
        {
            rooms[i].type = ROOM_TYPES[1];
        }

        // assign room names randomly
        rooms[i].name = ROOM_NAMES[rChoice[i]];

        // id corresponds to position in ROOM_NAMES
        rooms[i].id = rChoice[i];
        ROOM_IDS[i] = rChoice[i];


        // initialize connections to -1 to distinguish empty elements
        for (int j = 0; j < 6; j++)
        {
            rooms[i].connections[j] = -1;
        }
    }
    free(rChoice);

}

/*******************************************************************************
**  packages info about room to be written to files
*******************************************************************************/
void roomInfo(char *data, struct Room x)
{
    // copy and concat info to data string
    strcpy(data, "ROOM NAME: ");
    strcat(data, x.name);
    strcat(data, "\n");

    // loop through all possible conections
    for (int i = 0; i < 6; i++)
    {
        // stop when empty slot is found
        if (x.connections[i] == -1)
            break;

        // concat info about connections
        strcat(data, "CONNECTION ");

        // convert int i to string and append it to connection
        char *num = int2string(i + 1, 10);
        strcat(data, num);
        strcat(data, ": ");
        strcat(data, ROOM_NAMES[x.connections[i]]);
        strcat(data, "\n");

        // number string was returned from heap and must be freed
        free(num);
    }
    strcat(data, "ROOM TYPE: ");
    strcat(data, x.type);
}

/*******************************************************************************
**  returns a 7 element array with randomly selected elements
*******************************************************************************/
int *randArray(int arrSize)
{
    // create space in heap for array of ints
    int *arr = malloc(arrSize * sizeof(int));

    // keeps track of element that needs to be filled with random number
    int size = 0;

    // loop until all elements are filled
    while (size < 7)
    {
        // there are 10 possible rooms to choose from
        int add = rand() % 10; // random int between 0 and 9
        int exists = 0; // tracks if value already exists in array

        // loop through numbers already in array
        for (int k = 0; k < size; k++)
        {
            // checks if random value add is present
            if (add == arr[k])
            {
                exists = 1;
                break;
            }
        }
        if (!exists) // not present so adds add to array
        {
            arr[size] = add;
            size++;
        }
    }
    return arr;
}

/*******************************************************************************
**  prints info about all rooms in the rooms array
*******************************************************************************/
void printRoomInfo(struct Room *rooms)
{
    printf("\nHello CS344 TA. Thank you for testing my Adventure Program.");
    printf("\nThe following info gives details on randomly selected rooms");
    printf("\nand they're connections.\n\nI know this print out is not");
    printf(" required but I thought it would help with testing.\n");

    // loop through all the rooms in the array and print info
    for (int i = 0; i < 7; i++)
    {
        printf("Room number: ");
        printf("%d", rooms[i].id);
        printf("-----");
        printf("%s", rooms[i].name);
        printf("-----");
        printf("%s", rooms[i].type);
        printf("-----  ");

        // loop through connections for each room
        for (int j = 0; j < 6; j++)
        {
            if (rooms[i].connections[j] != -1)
            {
                printf("%d", rooms[i].connections[j]);
                printf(" ");
            }
        }
        printf("\n");
    }

    printf("\nPlease run vegada.adventure to start game with newly ");
    printf("created rooms. Thank you.\n");
}

/*******************************************************************************
**  writes room info to files
*******************************************************************************/
void write2files(char *dirname, struct Room *rooms)
{
    for (int i = 0; i < 7; i++)
    {
        // holds value of directory path
        char dirPath[100];
        snprintf(dirPath, 100, "%s/%s", dirname, ROOM_FILES[i]);

        // points to open file
        FILE *filePointer = fopen(dirPath, "w+");

        // holds room information
        char roomData[1000];

        // fill roomData with room info
        roomInfo(roomData, rooms[i]);

        // write to file
        fprintf(filePointer, "%s", roomData);

        // close open file
        fclose(filePointer);

    }
}
