/*******************************************************************************
** Program:       Assignment 1 
** Author:        David Vega
** Date:          6/24/20
** Description:   This program reads data from 7 files in a folder. The data
**                contains information about digital rooms that a user must
**                travel through in order to reach the end room. User starts
**                in "START_ROOM" and needs to find "END_ROOM" by typing the
**                names of the next room they wish to enter.
*******************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <time.h>
#include <string.h>
#include <dirent.h>
#include "vegada.adventure.h"

/*******************************************************************************
**  Main function
*******************************************************************************/
int main()
{
    //initialize array of struct Rooms
    struct Room rooms[7];
    initRooms(rooms);

    //get data from files
    downloadRoomData(rooms);

    //initiate game
    gameInterface(rooms);

    return 0;
}

/*******************************************************************************
**  interface for user input
*******************************************************************************/
void gameInterface(struct Room *rooms)
{
    int currentRoom = -1; // index location of room in rooms array
    int steps = 0; // number of steps taken to find end room
    char *path[100] = {}; // stores rooms visited


    // loop through rooms array to find "start room"
    for (int i = 0; i < 7; i++)
    {
        if (strcmp(rooms[i].type, "START_ROOM") == 0)
        {
            currentRoom = i; // set currentRoom to index of start room
            break;
        }
    }

    char input[100] = ""; // stores user input
    int check = checkRoomType(rooms, currentRoom, "END_ROOM");

    // loop until "end room" is reached
    while (!check)
    {
        getUserInput(input, rooms, &currentRoom); // user chooses room
        check = checkRoomType(rooms, currentRoom, "END_ROOM");
        path[steps] = rooms[currentRoom].name; // record rooms visited
        steps++; // record movement to next room
    }

    printf("\nYOU HAVE FOUND THE END ROOM. CONGRATULATIONS!");
    printf("\nYOU TOOK %d STEPS. YOUR PATH TO VICTORY WAS: ", steps);


    // print rooms visited
    for (int i = 0; i < steps; i++)
    {
        printf("\n%s", path[i]);
    }
    printf("\n");
}

/*******************************************************************************
**  checks if room type matches string.
*******************************************************************************/
int checkRoomType(struct Room *rooms, int cloc, char *name)
{
    // access rooms struct attribute type to determine if type matches string
    return strcmp(rooms[cloc].type, name) == 0 ? 1 : 0;
}

/*******************************************************************************
**  loop through array to get number of elements in the array.
*******************************************************************************/
int getArrSize(char *arr[])
{
    for (int i = 0; i < 6; i++)
    {
        // vacant space in array was initialized to ""
        if (strcmp(arr[i], "") == 0)
        {
            return i;
        }
    }
    return 6; // array is full
}

/*******************************************************************************
**  gets the current room's connection list and builds a sentence out of it.
*******************************************************************************/
void getConnections(struct Room *rooms, int currentRoom, char *conList)
{
    // need array size to know where to end the sentence with a period
    int size = getArrSize(rooms[currentRoom].connections);

    // read room's connection list and copy elements to array conList
    for (int i = 0; i < size; i++)
    {
        // first element uses strcpy and the rest use strcat
        if (i == 0)
        {
            strcpy(conList, rooms[currentRoom].connections[i]);
            strcat(conList, ", ");
        } else if (i == size - 1) // check if last element in array
        {
            strcat(conList, rooms[currentRoom].connections[i]);
            strcat(conList, ".");
            break;
        } else
        {
            strcat(conList, rooms[currentRoom].connections[i]);
            strcat(conList, ", ");
        }
    }
}

/*******************************************************************************
**  checks if input matches connection and sets value of index associated w room
*******************************************************************************/
int validRoom(struct Room *rooms, char *input, char *conArr[])
{
    // loop through room connections
    for (int j = 0; j < 6; j++)
    {
        // found connection
        if (strcmp(input, conArr[j]) == 0)
        {
            // loop through room names in array to get index value
            for (int i = 0; i < 7; i++)
            {
                // found room
                if (strcmp(input, rooms[i].name) == 0)
                {
                    return i;
                }
            }
        }
    }

    return -1; // user input does not match any rooms in list
}

/*******************************************************************************
**  get user input and set index value associated with current room.
*******************************************************************************/
void getUserInput(char *input, struct Room *rooms, int *cloc)
{
    int i = -1; // default room as invalid
    char conList[300] = ""; // string holds a sentence listing all connections
    do // loops until a valid room is entered
    {
        printf("\nCURRENT LOCATION: ");
        printf("%s", rooms[*cloc].name); // print current room's name
        printf("\n");
        printf("POSSIBLE CONNECTIONS: ");
        getConnections(rooms, *cloc, conList); // get current room's connections
        printf("%s", conList);
        printf("\n\n");
        printf("WHERE TO? >");

        // flush stdin and add null terminator to input from user
        fflush(stdin);
        fgets(input, 100, stdin);
        if (input[strlen(input) - 1] == '\n')
            input[strlen(input) - 1] = '\0';

        // check if room is a connection and set room's index
        i = validRoom(rooms, input, rooms[*cloc].connections); // -1 or index

        if (i == -1)
            printf("\nHUH? I DON'T UNDERSTAND THAT ROOM. TRY AGAIN.\n");

    } while (i == -1);
    *cloc = i; // update current location with index value
}

/*******************************************************************************
** read room files and build array of struct Room rooms. Info about strtok
** obtained from https://stackoverflow.com/questions/26597977/split-string-
** with-multiple-delimiters-using-strtok-in-c
*******************************************************************************/
void buildRoom(char *filePath, int position, struct Room *rooms)
{
    FILE *fPointer; // points to file in directory
    fPointer = fopen(filePath, "r"); // open file inside vegada.rooms.xxxxxx
    if (fPointer == NULL)
    {
        perror("Unable to open file!");
        exit(1);
    }

    char buffer[200]; // stores the contents of a line in the file being read
    char delimit[] = " \t\r\n\v\f"; // white space characters
    int j = 0; // keep track of connections
    int k = 0; // keep track of the first line

    // read each line in file and assign corresponding data to room attributes
    while (fgets(buffer, 200, fPointer) != NULL)
    {
        //strtok returns a pointer to a location in string, not a new string!
        //char *token = strtok(buffer, " "); // points to first space
        char *token = strtok(buffer, delimit);

        // first row contains room name
        if (k == 0)
        {
            // sets token to characters between second space and end of line
            token = strtok(NULL, delimit); // points to second space
            token = strtok(NULL, delimit); // points to end of line
            for (int i = 0; i < 10; i++)
            {
                if (strcmp(token, ROOM_NAMES[i]) == 0)
                {
                    rooms[position].name = ROOM_NAMES[i];
                    break;
                }
            }
        }

        // connections to other rooms
        if (strcmp(token, "CONNECTION") == 0)
        {
            token = strtok(NULL, delimit); // points to second space
            token = strtok(NULL, delimit); // points to end of line

            for (int i = 0; i < 10; i++)
            {
                if (strcmp(token, ROOM_NAMES[i]) == 0)
                {
                    rooms[position].connections[j] = ROOM_NAMES[i];
                    j++;
                    break;
                }
            }
        }

        // type of room
        if (strcmp(token, "ROOM") == 0)
        {
            token = strtok(NULL, delimit); // points to second space
            token = strtok(NULL, delimit); // points to end of line
            for (int i = 0; i < 3; i++)
            {
                // token matches room type
                if (strcmp(token, ROOM_TYPES[i]) == 0)
                {
                    rooms[position].type = ROOM_TYPES[i];
                    break;
                }
            }
        }
        k++;
    }
    fclose(fPointer);
}

/*******************************************************************************
**  search directories in current folder and download data from files
*******************************************************************************/
void downloadRoomData(struct Room *rooms)
{
    char dirName[100] = {0};
    newestFolder(dirName); //assign name of newest folder to dirName
    DIR *newestDir;
    struct dirent *dirFile;
    newestDir = opendir(dirName); // open newest folder
    int i = 0;
    if (newestDir) // check if directory exists
    {
        dirFile = readdir(newestDir); // point to first file in directory
        while (dirFile != NULL) // loop until there's no more files in directory
        {
            if (dirFile->d_name[0] != '.') // ignore hidden files
            {
                // set filepath name by combining directory and filename
                char filePath[400];
                snprintf(filePath, 400, "%s/%s", dirName, dirFile->d_name);

                // assign data to room struct in array
                buildRoom(filePath, i, rooms);
                i++; // move to next room in the rooms array
            }
            dirFile = readdir(newestDir); // next file
        }
        closedir(newestDir);
    }
}

/*******************************************************************************
**  get the name of the most newly created folder
*******************************************************************************/
void newestFolder(char *directoryName)
{
    DIR *currentDir = opendir("."); // points to current directory
    struct dirent *dir; // struct pointer gets folder name
    time_t currentTime; // holds the time folder was created
    struct stat dirStat; // get time
    int lastTime = 0;

    // reads all directories in home directory and updates directoryName with
    // each encounter of a newer directory. Once all directories have been
    // checked, the newest directory will be stored in directoryName variable
    while ((dir = readdir(currentDir)) != NULL)
    {

        // ignore directories that don't start with "vegada.buildrooms"
        if (!stat(dir->d_name, &dirStat) && !strncmp(dir->d_name, "vegada"
                                                                  ".rooms", 12))
        {
            currentTime = dirStat.st_mtime; // time folder was created
            if (currentTime > lastTime)
            {
                strcpy(directoryName, dir->d_name);
                lastTime = currentTime;
            }
        }
    }
    closedir(currentDir);
}

/*******************************************************************************
**  prints info on all the rooms in the rooms array
*******************************************************************************/
void printRoomInfo(struct Room *rooms)
{
    // loop through rooms in rooms array
    for (int i = 0; i < 7; i++)
    {
        printf("%s", rooms[i].name);
        printf("-----");
        printf("%s", rooms[i].type);
        printf("-----  ");
        for (int j = 0; j < 6; j++)
        {
            // make sure element in connections isn't empty
            if (strcmp(rooms[i].connections[j], "") != 0)
            {
                printf("%s", rooms[i].connections[j]);
                printf(" ");
            }
        }
        printf("\n");
    }
}

/*******************************************************************************
**  initializes the values of the rooms in the array of struct rooms
*******************************************************************************/
void initRooms(struct Room *rooms)
{
    // 7 rooms in the rooms array
    for (int i = 0; i < 7; i++)
    {
        // initialize struct attributes
        rooms[i].name = "";
        rooms[i].type = "";

        // initialize connections 6 max
        for (int j = 0; j < 6; j++)
        {
            rooms[i].connections[j] = "";
        }
    }
}


