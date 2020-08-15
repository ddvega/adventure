/*******************************************************************************
** Program:       Assignment 1 - CS344
** Author:        David Vega
** Date:          6/24/20
** Description:   This program creates an array of struct rooms and randomly
**                assigns names and connections to each room. Once the array is
**                created, the data is then written to a file for
**                vegada.adventure to read from.
*******************************************************************************/

#ifndef VEGADA_BUILDROOMS_H
#define VEGADA_BUILDROOMS_H

/*******************************************************************************
**  Struct and global variable definitions
*******************************************************************************/
struct Room
{
   char *name;
   char *type;
   int id;
   int connections[6];
};

int ROOM_IDS[10] = {};
char *ROOM_FILES[7] = {"dark_room", "loud_room", "small_room", "cold_room",
                       "hot_room", "digital_room", "wet_room"};

char *ROOM_NAMES[10] = {"bali", "tendoz", "rolo", "magilond", "bashtir",
                        "bescan", "kudrow", "yandiz", "lumpturn", "dungy"};

char *ROOM_TYPES[3] = {"START_ROOM", "MID_ROOM", "END_ROOM"};

/*******************************************************************************
**  Function declarations
*******************************************************************************/
void roomInfo(char *, struct Room);
int *randArray(int);
void initRooms(struct Room *rooms);
int isSameRoom(struct Room *, struct Room *);
void connectRoom(struct Room *, struct Room *);
int connectionExists(struct Room *, struct Room *);
int connectionAvailable(struct Room *);
struct Room *getRandom(struct Room *);
void addRandomConnection(struct Room *);
int graphFull(struct Room *rooms);
void printRoomInfo(struct Room *);
void write2files(char *, struct Room *);
char *int2string(int, int);

#endif