#ifndef MAP
#define MAP
#include <stdlib.h>
#include <map>

void planMap(){
	int rooms[16];
	for (int i = 0; i < sizeof(rooms); i++){
		if (i == 0) rooms[i] = 1;
		int room_type = rand() % 4;
		rooms[i] = room_type;
	}
}

void makeBorder(){
	std::map<int, int[80]> built_rooms;
	int rooms[16];
	int room[80];
	for (int i = 0; i < sizeof(rooms); i++){
		for (int j = 0; j < sizeof(room); j++){
			if (i < 4){
				if (j < 10) room[j] = 1;
			}
			if (i % 4 == 0){
				if (j % 10 == 0) room[j] = 1;
			}
			if (i > 9){
				if (j > 69) room[j] = 1;
			}
			if (i % 3 == 0){
				if (j % 9 == 0) room[j] = 1;
			}
			else{ room[j] = 0; }
		}
		built_rooms.insert(std::pair<int,int[80]>(rooms[i],room));
	}
}

void makeIndividualRooms(){

}



#endif