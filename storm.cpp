/*
 * Description: Uses queries to build arrays of structs and BSTs to store information from a spreadsheet 
 * Author: Jack Hosier
 * Date: 10/23/2021
 * Language: C++
 * Class: CSE 310 T/TH 3:00-4:15PM
*/


#include <iostream>
#include <fstream>
#include <sstream> 
#include <cstring>
#include <string>
#include "defns.h"
#include "prime.cc" 

using namespace std;

//function prototypes

//builds array of structs given the year(s)
annual_storms* buildStructs(string, int);

//build an array of fatality_events that will later be turned into linkedList nodes and assigned to storm_events
fatality_event* buildFatalityArray(int);  

//fatality_event* buildLinkedList(int, fatality_event*); 
 
//convert structArray to BST
bst* buildBST(bst*, annual_storms*);

//constructor for BST nodes
bst* Node(char*, int, int); 


//print contents of BST
void printBST(bst*, string, string, annual_storms); 

//hash table funcs

//global variables
string line, token, function, year, field_name, low, high; 

int stormArrSize, eventArrSize, fatalArraySize;  

//function definitions
hash_table_entry* newEntry(int, int, int); 


fatality_event* buildFatalityArray(int year) {
	
		ifstream fatality_file; 
		
		string fileName = "/home/student/Desktop/CSE310Project2/fatalities-" + to_string(year) + ".csv";
		
		fatality_file.open(fileName);
		
		getline(fatality_file,line); //skip the very first line (has the names of the fields) 
		
		int otherNumLines = 0; 
		//loop to count # of lines.
		
		while (!fatality_file.eof()){
		
			getline(fatality_file,line); 
			otherNumLines++; 
		} //end while 
		
		fatalArraySize = otherNumLines; 
	
		fatality_file.close(); //close file 
		
		//create array of fatality events 
		
		fatality_event *fatalities = new fatality_event[fatalArraySize]; 
	
		//now open file to read data 
		
		fatality_file.open(fileName); 
		
		getline(fatality_file,line); //skip first line 
		
		for (int j = 0; j < fatalArraySize; j++) {
		
			while(getline(fatality_file,line)){
			
				
				istringstream ss(line);
				
				int count = 0; 
				
				string fatLineArr[6] = {"0", "0", "0", "0", "0", "0" }; //array of parsed out strings 
				
				
				//load lineArr with parsed strings
			
				while(ss.good() && count < 6){
					
					int front = 0; 
					int end = line.length(); //char at the end of each line
					
					int comma = line.find(","); 
					
					cout << "comma: " << comma << endl; 
					
					token = line.substr(front, comma);
					
					fatLineArr[count] = token; //load string into array (this works) 
					
					line = line.substr(comma + 1, end); 
					
					
					//cout << "token: " << token << endl; 
					//cout << "lineArray: " << lineArr[i] << endl; 
					
					//cout << "Line:" << line << endl; 
					
					count++;
				} //end inner-while loop (do this for each line in the .csv file)
				
				//assign variables here 
				
				fatalities[j].fatality_id = stoi(fatLineArr[0]); 
				
				fatalities[j].event_id = stoi(fatLineArr[1]);
				
				string fatality_typeStr = fatLineArr[2]; 
				
				fatalities[j].fatality_type = fatality_typeStr[0]; 
				
				strcpy(fatalities[j].fatality_date, fatLineArr[3].c_str()); 
				
				//check for empty fields
				if (fatLineArr[4] == "") {
					
					fatalities[j].fatality_age = -1; 
					
				} else {
				
					fatalities[j].fatality_age = stoi(fatLineArr[4]); 
				
				}
				
				if (fatLineArr[5] == "") {
					
					fatalities[j].fatality_sex = 'U'; 
					
				} else {
					
					string fatality_sexStr = fatLineArr[5];
				
					fatalities[j].fatality_sex = fatality_sexStr[0]; 
				
				}
				
				if (fatLineArr[6] == "") {
					
					string fatality_locationStr = "UNKNOWN";
					
					strcpy(fatalities[j].fatality_location, fatality_locationStr.c_str()); 
					
				} else {
				
					strcpy(fatalities[j].fatality_location, fatLineArr[6].c_str()); 
				
				}
			
			}// end of outer while-loop 
		
		}//end for loop
		
		cout << "Closing file" << endl; 
			
		fatality_file.close(); //close the file 
			
		return fatalities;
}




annual_storms* buildStructs(string startYear, int numYears){
	
	int numLines = 0; //number of lines in file
	
	string cz_typeStr, damage_propertyStr;
	
	//if year == all, fill year array with all the years. 
	
	int year = stoi(startYear); //convert start year to int for filename
	
	stormArrSize = numYears; //pass numYears to global var
 
	annual_storms *storms = new annual_storms[numYears]; //struct array of numYears length. 


	//check to make sure year is in range because we skip from 1953 to 1966, then to 1996. 
	
	if(year > 1953) {
		year = 1966; 
	} else if (year > 1966) {
		year = 1996; 
	}
	
	for (int j = 0; j < numYears; j++){
		
		string filename = "/home/student/Desktop/CSE310Project2/details-" + to_string(year) + ".csv"; //create filename for each file
		
		ifstream file; //open files
		
		file.open(filename); 
		
		getline(file,line); //skip the very first line (has the names of the fields) 
	 
		
		//loop to count # of lines.
		
		while (!file.eof()){
		
			getline(file,line); 
			numLines++; 
			//cout << "Number of Lines: " << numLines << endl; 
		} //end while 
	
		file.close(); //close file 
		
		storms[j].year = year; //assign the year in the storm array 
		
		//cout << "Year in storm array: " << storms[j].year << endl; 
		
		storms[j].events = new storm_event[numLines]; //allocate an array of events within the annual_storm array element 
		
		//cout << "EventArray Pointer: " << storms[j].events << endl; 
	
		file.open(filename); //reopen file to read each line 
		
		getline(file,line); //skip first line in file 
		
		int countEvents = 0; //index var for building events array 
		//------------------------------------------------------------------------------------------------------------------------------------------------------------------
		
		while(getline(file,line)){
		
			//cout << "String: " << line << "" << endl; //read each line 
			
			istringstream ss(line);
			string token;
			
			int i = 0; 
			
			string lineArr[12] = {"0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0",}; //array of parsed out strings 
			
			
			//load lineArr with parsed strings
		
			while(ss.good() && i < 12){
				
				int front = 0; 
				int end = line.length(); //char at the end of each line
				
				int comma = line.find(","); 
				
				//cout << "comma: " << comma << endl; 
				
				token = line.substr(front, comma);
				
				lineArr[i] = token; //load string into array (this works) 
				
				line = line.substr(comma + 1, end); 
				
				
				//cout << "token: " << token << endl; 
				//cout << "lineArray: " << lineArr[i] << endl; 
				
				//cout << "Line:" << line << endl; 
				
				i++;
			} //end inner-while loop (do this for each line in the .csv file)
			
			storms[j].events[countEvents].event_id = stoi(lineArr[0]); 

			strcpy(storms[j].events[countEvents].state, lineArr[1].c_str());
			
			storms[j].events[countEvents].year = stoi(lineArr[2]);
			
			strcpy(storms[j].events[countEvents].month_name, lineArr[3].c_str());
			
			strcpy(storms[j].events[countEvents].event_type, lineArr[4].c_str()); 
			
			cz_typeStr = lineArr[5];

			storms[j].events[countEvents].cz_type = cz_typeStr[0]; 
			 
			strcpy(storms[j].events[countEvents].cz_name, lineArr[6].c_str());
			 
			storms[j].events[countEvents].injuries_direct = stoi(lineArr[7]);
			
			storms[j].events[countEvents].injuries_indirect = stoi(lineArr[8]); 
			 
			storms[j].events[countEvents].deaths_direct = stoi(lineArr[9]);
			
			storms[j].events[countEvents].deaths_indirect = stoi(lineArr[10]); 
			
			// convert "K" and "M" values to their integer equivalents 
			damage_propertyStr = lineArr[11];
			
			char intDelimiter = damage_propertyStr[damage_propertyStr.length()-1]; //delimiter is at the end of the string 
			
			if (intDelimiter == 'K') {
				
				//delete last char in string 
				damage_propertyStr.erase(damage_propertyStr.end()-1); 
				//convert string to int 
				storms[j].events[countEvents].damage_property = stof(damage_propertyStr) * 1000; //multiply int by a thousand 
				
			} else if (intDelimiter == 'M') {
				
				//delete last char in string 
				damage_propertyStr.erase(damage_propertyStr.end()-1); 
				//convert string to int 
				storms[j].events[countEvents].damage_property = stof(damage_propertyStr) * 1000000; //multiply int by a million 
				
			}
			
			//storms[j].events[countEvents].damage_crops = stoi(lineArr[12]); //Why do I have to comment this out for the program to work?
			
			/* This is where I was going to allocate fatality_events 
			 * 
			 */
			
			countEvents++; //increment counter 	
		
			eventArrSize = countEvents; //pass value of countEvents to global var eventArrSize
			
			cout << "Size of event array: " << eventArrSize << endl; 
		
		}// end of outer while-loop 
		
			cout << "Closing file" << endl; 
			
			file.close(); //close the file 
			
			//increment year int to go to the next year 
			year += 1; 
		
	} //end of outer for-loop	
	
	
	
	cout << "return pointer to array of storms" << endl;
	
	cout << "Array pointer: " << storms << endl; 
	
	return storms; //return pointer to array of annual_storms
	 
	
} //end of buildStructs method

//function that finds the index of a storm_event within the array 
int findStormEvent (int event_id, annual_storms stormArray[]){
	
	int eventIndex; 
	
	for (int a = 0; a < eventArrSize; a++) {
		
		if (stormArray->events[a].event_id == event_id) {
			eventIndex = a; 
		} 
	}
	
	return eventIndex; 
}

//-----------------------BST Functions-------------------------//


bst* Node(char* s, int year, int event_id) {
	
	bst* temp = (bst*) malloc(sizeof(bst)); //create new node
	
	temp->s = s;
	cout << "s: " << s << endl;  
	temp->year = year;
	cout << "year: " << year << endl; 
	temp->event_id = event_id; 
	cout << "event_id: " << event_id << endl; 
	temp->left = temp->right = NULL; 
	
	return temp; //return the node 
}




//convert struct array to BST

bst* buildBST(bst* node, annual_storms stormArray[]){
	
	int numNodes = 0; //int for total number of nodes in BST
	
	int treeRootHeight = 0; 
	int leftLevelCount = 0; //height of left subtree
	int rightLevelCount = 0; //height of right subtree
	
	//loop through event array and convert each event to a BST node
	for(int c = 0; c < eventArrSize; c++) {
		
		//create new node for each event
		
		//if (field_name == "state") {
			bst* newEvent = Node(stormArray->events[c].state, stormArray->events[c].year, stormArray->events[c].event_id); 
			
		//} else {
			//bst* newEvent = Node(stormArray->events[c].month_name, stormArray->events[c].year, stormArray->events[c].event_id); 
	
		//}
		numNodes++; //increment for each new node created
		
		bst* position = node; //index for traversing BST 
		
		while(position != NULL) {
			
			//if second cstring is greater than first string
			if(strcmp(newEvent->s, position->s) < 0) {
				
				if(position->left == NULL) {
					position->left = newEvent; 
					break; 
				} 
					position = position->left;
					leftLevelCount++;  
				
			} else if(strcmp(newEvent->s, position->s) > 0) {
				
				if(position->right == NULL) {
					position->right = newEvent; 
					break;
				} 
					position = position->right;
					rightLevelCount++;
				 
				
			} else {
				//if field_names are the same then compare by event_id
				if(newEvent->event_id < position->event_id) {
					
					if(position->left == NULL) {
						position = newEvent; 
						break;
					} 
						position = position->left; 
						leftLevelCount++; 
					
				} else {
					
					if(position->right == NULL) {
						position = newEvent;
					} 
						position = position->right; 
						rightLevelCount++;
					
				}
			} //end big else
		
		} //end while-loop
	
	} //end for-loop
	
	cout << "Total Number of Nodes in tree: " << numNodes << endl;  
	
	cout << "Tree Root Height: " << treeRootHeight << endl; 
	
	cout << "Left Subtree Height: " << leftLevelCount << endl; 
	
	cout << "Right Subtree Height: " << rightLevelCount << endl; 
	
	cout << "Pointer to BST? " << node << endl; 
	
	return node; 
}

void printInOrder(bst* node) {
	//base case (if tree is empty) 
	if (node == NULL) {
		cout << "Bottom" << endl; 
		return; 
	}
	
	//traverse left subtree
	printInOrder(node->left); 
	
			//print out data in range 
	cout << "State: " << node->s << endl; 
	
	//traverse right subtree
	printInOrder(node->right); 
}

//print out contents of BST within the range of front and end
void printBST(bst* node, string low, string high, annual_storms stormArray[]){
	 
	//convert string low and high to char arrays to resolve type mismatch  

	const char *lowerBound = low.c_str(); 
	const char *upperBound = high.c_str(); 
	
	//base case (if tree is empty) 
	if (node == NULL) { 
		return; 
	}
	
	//traverse left subtree
	printBST(node->left, low, high, stormArray); 
	 
	//cout << "LowerBound:" << strcmp(node->s, lowerBound) << endl; 
	
	//cout << "UpperBound: " << strcmp(node->s, upperBound) << endl; 
	
	if(strcmp(node->s, lowerBound) >= 0 && strcmp(node->s, upperBound) <= 0) {
		
		//print out data in range 
		if(field_name == "state") {
			cout << "State: " << node->s << endl; 
		} else if (field_name == "month_name") {
			cout << "Month Name: " << node->s << endl; 
		}
		
		cout << "Event ID: " << node->event_id << endl; 
		
		cout << "Year: " << node->year << endl; 
		
		//figure out how to extract event_type, county/zone_type, and cz_name
			
		cout << "Event Type: " << stormArray->events->event_type << endl; 
			
		cout << "County/Zone Type: " << stormArray->events->cz_type << endl; 
			
		cout << "County/Zone Name: " << stormArray->events->cz_name << endl; 
	} else {
		//cout << "No storm events found for the given range" << endl; 
	}
	
	//traverse right subtree
	printBST(node->right, low, high, stormArray); 
	
}

void deleteBST(bst* node) {
	
	if (node == NULL) {
		return; 
	}
	
	//traverse tree in postorder
	deleteBST(node->left);
	deleteBST(node->right); 
	
	//delete node
	delete node; 
}
/*-------------------Hash Table Functions-------------------*/
//I was still working on these 
/*
//class for hashTable
struct hashTable {
	
	hash_table_entry** entries; //array of pointers to hash_table_entries 
	fatality_event** fatalities; //array of fatality_event linked list pointers
	
	int size; 
	int countEntries;
	
};


//size of hash table is the first prime number after eventArrSize * 3 
//unsigned because eventArrSize should not be negative
unsigned int HTSize = eventArrSize * 3; 

//increase HTSize while not prime
while(TestForPrime(HTSize) == false){
	HTSize++; 
}

//hash function generator
unsigned int hash_func(int event_id){
	
	unsigned int hash = event_id % HTSize; 
	
	return hash; 
}


//constructor for hash_table entry 
hash_table_entry* newEntry(int event_id, int year, int event_index) {
	
	//allocate memory for hash table
	hash_table_entry* entry = (hash_table_entry*) malloc (sizeof(hash_table_entry));
	
	entry->event_id = event_id;
	entry->year = year;
	entry->event_index = event_index;
	
	return entry; 
	
}
//build the hash table 
hashTable* buildHT(int size) {
	
	//allocate memory for the table
	hashTable* HT = (hashTable*) malloc (sizeof(hashTable)); 
	HT->size = size; 
	HT->countEntries = 0; 
	
	//set size of each entry into the HT
	HT->entries = (hash_table_entry**) calloc (HT->size, sizeof(hash_table_entry*)); 
	
	for (int b = 0; b < HT->size; b++) {
		HT->entries[b] = NULL;
	}
	
	return HT; 
}



//insert elements into hash table 

void insertHTEntry(hashTable* ht, int event_id, int year, int event_index){
	
	//create the entry 
	hash_table_entry* entry = newEntry(event_id, year, event_index); 
	
	//find the index into the table 
	int index = hash_func(event_id);
	
	hash_table_entry* currentEntry = ht->entries[index]; 
	
	//check if entry is empty
	if(currentEntry == NULL) {
		//if HT is FULL
		if (ht->countEntries == ht->size) { 
			
			cout << "Error. Hash Table is at max capacity." << endl; 
		}
	
	
	//insert entry into hash table 
	ht->entries[index] = entry; 
	ht->countEntries++; 
	
	} else {
	//check for collisions
	
		//collision_check(ht, entry); 
	
	 
	}
	
}

//find the event within the hash table using the event_id and return all info on it
void findEvent (hashTable ht, int event_id){
	
	//search hash table for event with matching event_id 
	int index = hash_func(event_id); 
	
	hash_table_entry* entry = ht->entries[index]; 
	
	//check if the event isn't NULL
	if (entry != NULL) {
		//if found, print out all info on event like fatality events 
		if(entry->event_id == event_id) {
			cout << "Event ID: " << entry->event_id << endl; 
			cout << "Fatality ID: " << entry->fat
		}
		
	} else { 
		cout << "Error. Event Not Found." << endl; 
	}
	
	
}

//fatality_event stuff


fatality_event* makeListOfFatalities() {
	fatality_event* fatalities = (fatality_event*) malloc (sizeof(fatality_event)); //allocate size of list
	return fatalities;
}

fatality_event* insertFatality(fatality_event* fatalityList, int fatality_id, int event_id, char* fatality_type, char* fatality_date, int fatality_age, char fatality_sex, char fatality_location) {


		
	//insert items into fatality_event linked list
	if (fatalityList == NULL) {
		//if list empty, make a new one
		
		fatality_event* head = makeListOfFatalities(); 
		head->fatality
	}
	
	
}
*/

/*-------------------MAIN-------------------*/
int main (int argc, char** argv){
	
	int numYears; 
	
	string startYear; 
	
	cout << "Hello, please enter the start year you want look at, and the number of years you want to view " << endl; 
	
	//startYear = argv[0]; //read in command-line input for starting year
	
	//numYears = atoi(argv[1]); //read in command-line input for number of years
	
	cin >> startYear; 
	cin >> numYears; 
	
	if(startYear == "all") {
		numYears = 6; 
	}
	
	startYear = "1950"; //I don't feel like constantly filling in thus data.  
	
	int firstYear = stoi(startYear); //for quick error check
	
	if(firstYear < 1950 && firstYear > 1996) {
		
		cout << "Sorry, we don't have any data for that year. We only have data for 1950, 1951, 1952, 1953, 1966, and 1996" << endl; 
	}
	
	cout << "Building structs" << endl; 
	
	 //create structArray from year input
	 
	annual_storms *stormArrayPtr = buildStructs(startYear, numYears); 
			
	cout << "What function do your want to perform?" << endl; 
	
	//read in user input
	 
		cin >> function;
		
		 
		cin >> year; 
		cin >> field_name; 
		cin >> low; 
		cin >> high;
			
		
		if(function == "range") {
			
			bst* root; //pointer to root of the tree
		
			root = NULL; //initialize root to NULL
			
			cout << "Building the BST" << endl; 
			
			//create root 	
			root = Node(stormArrayPtr[0].events[0].state, stormArrayPtr[0].events[0].year, stormArrayPtr[0].events[0].event_id); 
			
			root = buildBST(root, stormArrayPtr); //convert structArray to BST 	
			//print out elements within range set by low and high
			
			cout << "Printing BST IN-ORDER" << endl; 
			
			printInOrder(root); 
			

			cout << "Printing BST" << endl; 
			printBST(root, low, high, stormArrayPtr);
			
			cout << "Deleting BST" << endl; 
			deleteBST(root); 
			
		} else if (function == "find event") {
			
			//create hash-table of storm_events 
			//hashTable *table = buildHT(HTSize); 
			
			
			
		}
		
	cout << "Goodbye!" << endl; 
	
	return 0; 

}
