/*******************************************************************************
** Program:       Assignment 1 - CS344
** Author:        David Vega
** Date:          6/24/20
** Description:   This program reads data from 7 files in a folder. The data
**                contains information about digital rooms that a user must
**                travel through in order to reach the end room. User starts
**                in "START_ROOM" and needs to find "END_ROOM" by typing the
**                names of the next room to enter.
*******************************************************************************/

#ifndef VEGADA_ADVENTURE_H
#define VEGADA_ADVENTURE_H

/*******************************************************************************
**  Struct and global variable definitions
*******************************************************************************/
struct Room
{
   char *name;
   char *type;
   char *connections[6];
};

char *ROOM_TYPES[3] = {"START_ROOM", "MID_ROOM", "END_ROOM"};

char *ROOM_NAMES[10] = {"bali","tendoz","rolo","magilond","bashtir","bescan",
                        "kudrow","yandiz","lumpturn","dungy"};

/*******************************************************************************
**  Function declarations
*******************************************************************************/
void getUserInput(char *input, struct Room *rooms, int *cloc);
void newestFolder(char *);
void gameInterface(struct Room *);
void printRoomInfo(struct Room *);
void initRooms(struct Room *);
void downloadRoomData(struct Room *);
void buildRoom(char *, int, struct Room *);
void getConnections(struct Room *, int, char *);
int getArrSize(char *[]);
int validRoom(struct Room *, char *, char*[]);
int checkRoomType(struct Room *, int, char *);
void setString(char *, int);

#endif