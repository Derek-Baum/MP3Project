#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdbool.h>
#include "serialize.h"


typedef struct mp3node mp3node;

char* readFile(char* filename);
mp3node* readMp3Node(char* filename);
void writeNodeToFile();
void startup();
void printList();
void printListBackwards();
void printNode(mp3node* singleNode,int ind);
bool processInput();
int getUserInput();
int indexOf(char* str, char c);
void insertIndex();
void insertToEnd();
void removeIndex();
void printTitleFilter();
void printArtistFilter();
void printYearSort();
void printForwards();
void printBackwards();
void save();
mp3node* buildNodeFromUserInput();
void clearInput();


char* FILE_STORE = "testserialize.txt";
struct mp3node
{
  char* name;
  char* title;
  int year;
  int runtime;
  mp3node* next;
  mp3node* prev;
};

mp3node* MP3_HEAD;
ser_tra_t* translator;
int main()
{
  /*
    Set up the translator.
    
    Note that the third argument to ser_new_field is 1; this will make
    the translator treat this field as a pointer to another structure.

    When a structure is serialized it will add all its references to
    the list of things to be serialized. We only need to pass the
    first node and the rest will follow.
  */
  
 
  startup();
  bool exit = false;

  if(MP3_HEAD){
    printf("Initial List Loaded From File\nPrinting... \n");
    printList();
  }else{
    printf("No list loaded from file.\n");
  }

  while(!exit){
    exit = processInput();
  }
  
  /*
    Populate a short list.
    a will point to the start of the list.
  */

  /*

  char tmp[50];
  char tmptitle[50];
  mp3node* a;
  mp3node* b;
  int i;

  a = NULL;

  for (i = 5; i > 0; i--)
  {
    b = malloc(sizeof(mp3node));
    sprintf(tmp, "node %d", i);
    sprintf(tmptitle, "title %d", i);
    b->name = strdup(tmp);
    b->title = strdup(tmptitle);
    b->year=2000;
    b->runtime=10;
    b->next = a;
    if(a != NULL)
      a->prev=b;
    a = b;
  }

  */
  /*
    Serialize the list.

    This should print the nodes we created above.
    The last should end with "next NULL;".
  */

  /*
  
  char* result;

  result = ser_ialize(tra, "node", a, NULL, 0);

  printf("Result:\n%s\n", result);

  while (a){
    b = a->next;
    free(a->name);
    free(a->title);
    free(a);
    a = b;
  }
  */

  /*
    Now we will read a short list. The input intentionally mixes up
    the order of the nodes; the serializer will get this right.
  */

  //a = ser_parse(tra,"node", result, NULL);

  /*
  while (a){
    printf("%s - %s\n", a->name, a->title);
    b = a->next;
    free(a->name);
    free(a->title);
    free(a);
    a = b;
  }
  */

 
  return 0;
}
char* readFile(char* filename){
  char * buffer = 0;
  long length;
  FILE * f = fopen (filename, "rb");

  if (f) {
      fseek (f, 0, SEEK_END);
      length = ftell (f);
      fseek (f, 0, SEEK_SET);
      buffer = malloc (length+1);
      if (buffer) {
	  fread (buffer, 1, length, f);
	}
      fclose (f);
      buffer[length]='\0';
    }
  return buffer;
}
mp3node* readMp3Node(char* filename){

  char* out = readFile(filename);

  mp3node* output = ser_parse(translator,"node",out,NULL);

  free(out);
  
  return output;
}
void writeNodeToFile(){

  FILE * fp;
  
  char* raw = ser_ialize(translator,"node",MP3_HEAD,NULL,0);
  
  printf("writing to file...\n");
  printf("%s\n",raw);

  fp = fopen(FILE_STORE, "w");

  fprintf(fp,raw);
  fclose(fp);
}
void startup(){
  printf("Initializing...\n");
  translator = ser_new_tra("node", sizeof(mp3node), NULL);
  ser_new_field(translator, "string", 0, "name", offsetof(mp3node, name));
  ser_new_field(translator, "string", 0, "title", offsetof(mp3node, title));
  ser_new_field(translator, "int", 0, "year", offsetof(mp3node, year));
  ser_new_field(translator, "int", 0, "runtime", offsetof(mp3node, runtime));
  ser_new_field(translator, "node",   1, "next",  offsetof(mp3node, next));
  ser_new_field(translator, "node",   1, "prev", offsetof(mp3node, prev));


  if(access(FILE_STORE, F_OK)!= -1){
    printf("We have found a local mp3 list.\nLoading...\n");
    MP3_HEAD = readMp3Node(FILE_STORE);
  }else{
    printf("No list found on file system.\n");
    MP3_HEAD = NULL;
  }

}

void printList(){

  mp3node* tmp = MP3_HEAD;
  int i = 0;
  printf("****************************************\n");
  while(tmp){
    printNode(tmp, i);
    tmp = tmp->next;
    i++;
  }
  
}
void printNode(mp3node* singleNode, int ind){
  printf("%s - %s (%d)\n",singleNode->title,singleNode->name,singleNode->year);
  int minutes = (singleNode->runtime)/60;
  int seconds = (singleNode->runtime)%60;
  printf("Time: %d:%d\n",minutes,seconds);
  printf("Playlist Index: %d\n",ind);
  if(singleNode->next == NULL){
    printf("****************************************\n");
  }else{
    printf("----------------------------------------\n");
  }
}
void printListBackwards(){
  mp3node* tmp = MP3_HEAD;
  int i = 0;

  while(tmp->next){
    i++;
    tmp = tmp->next;
  }
  
  while(tmp){
    printNode(tmp,i);
    tmp = tmp->prev;
    i--;
  }  
}
/*
 * Return is boolean
 * True: stop running : 1
 * False: Continue running : 0
 */
bool processInput(){
  //Write a new function, that handles all of the input, and puts it into a String. It handles displaying the input
  //options, aswell as preventing the user from messing up.
  int userChoice = getUserInput();
  if(userChoice == 1){
    //insert an mp3, with index.
    insertIndex();
  }else if(userChoice == 2){
    //insert an mp3, without index.
    insertToEnd();
  }else if(userChoice == 3){
    //remove mp3, by its index only.
    removeIndex();
  }else if(userChoice == 4){
    //print filter by title
    printTitleFilter();
  }else if(userChoice == 5){
    //print filter by artist name
    printArtistFilter();
  }else if(userChoice == 6){
    //print sorted by year
    printYearSort();
  }else if(userChoice == 7){
    //print list forwards
    printForwards();
  }else if(userChoice == 8){
    //print list backwards
    printBackwards();
  }else if(userChoice == 9){
    //save
    save();
  }else if(userChoice == 10){
    //exit
    printf("Exiting without saving.\n");
    return true;
  }

  return false;
}

/*
 * The purpose of this function is to ultimately ask the user for input, and get their choice of operation.
 * These are the possibities:
 * 1 : Insert an mp3, with index
 * 2 : Insert an mp3, without index
 * 3 : Remove mp3, by its index only.
 * 4 : Print Filter by title
 * 5 : Print Filter by Artist name
 * 6 : Print Sorted By Year
 * 7 : Print List Forwards
 * 8 : Print List Backwards
 * 9 : Save Current List To File
 * 10 : Exit Program
 *
 */
int getUserInput(){
  int output = -1;
  char input[2];
  input[0] = 'd';
  input[1] = '\0';
  char *tmp = input;

  char* filter = "pirsePIRSE";
  
  printf("Input a character to perform an action.\n");
  printf("What would you like to do?\n");
  printf("P : Print the list.(Many different options.)\n");
  printf("I : Insert an mp3.\n");
  printf("R : Remove an mp3 by its index.\n");
  printf("S : Save current list to file, to be loaded up on program startup.\n");
  printf("E : Exit Program.\n");


  while(strchr(filter, *tmp)==NULL){
    clearInput();
    fgets(input,2,stdin);
    tmp = input;
  }
  int ind = (indexOf(filter,input[0]) % 5);
  printf("The index of your input in the PIRSE is: %d.\n",ind);


  if(ind == 0){
    printf("How would you like to print the list?\n");
    printf("P : Prints the list forwards.\n");
    printf("B : Prints the list backwards.\n");
    printf("T : Prints the filtered list, filtered by a title.\n");
    printf("A : Prints the filtered list, filtered by an artist name.\n");
    printf("Y : Prints the list, sorted by year.\n");

    filter = "PBTAYpbtay";
    input[0] = 'd';
    input[1] = '\0';
    tmp = input;

    while(strchr(filter, *tmp)==NULL){
      clearInput();
      fgets(input,2,stdin);
      tmp = input;
    }
    int ind_print = (indexOf(filter,input[0]) % 5);
    if(ind_print == 0){
      //print the list forwards
      output = 7;
    }else if(ind_print == 1){
      //print the list backwards
      output = 8;
    }else if(ind_print == 2){
      //print the list filtered, with title.
      output = 4;
    }else if(ind_print == 3){
      //print the list, filtered by an artist name.
      output = 5;
    }else if(ind_print == 4){
      //prints the list, sorted by year.
      output = 6;
    }

  }else if(ind == 1){
    //insert
    printf("Would you like to insert with an index, or to the end of the list?\n");
    printf("I : insert with index.\n");
    printf("E : insert to end.\n");
    char inputTwo[2];
    inputTwo[0] = '\n';
    inputTwo[1] = '\0';
    clearInput();
    fgets(inputTwo,2,stdin);
    while(inputTwo[0] == '\n'){
      printf("Please enter input:\n");
      fgets(inputTwo,2,stdin);
    }
    if(inputTwo[0] == 'i' || inputTwo[0] == 'I'){
      //with index
      output = 1;
    }else{
      //without index
      output = 2;
    }
  }else if(ind == 2){
    //remove an item from the list, by index.
    output = 3;
  }else if(ind == 3){
    //save
    output = 9;
  }else if(ind == 4){
    //exit
    output = 10;
  }
  return output;
}

int indexOf(char* str, char c){
  char* ptr;
  int index;

  ptr = strchr(str,c);
  if(ptr==NULL){
    return -1;
  }

  index = ptr - str;
  
  return index;
}
void insertIndex(){
  if(MP3_HEAD == NULL){
    printf("You can only insert to one index, since the list is empty!\nBringing you to insert to end.\n");
    insertToEnd();
    return;
  }
  //insert the node at tempnodes next. if tempnodes next isn't null, put tempnodes next on new tempnodes next first.

  
  printf("Inserting a record.\n");
  mp3node* newnode = buildNodeFromUserInput();

  printf("Pick an index to insert your new record into.\nThese are 0 based indeces, and if your index is too high, the insertion will fail.\n");
  
  int uind;
  clearInput();
  scanf("%10d",&uind);

  printf("Attempting to insert at index %d.\n",uind);


  int i = 0;
  if(uind == 0){
    /*
     * take the new node, set a tmp node to start, make start to point to the new node, set new->next = tmp.
     */
    mp3node* temp = MP3_HEAD;
    MP3_HEAD = newnode;
    newnode->next = temp;
    temp->prev = newnode;
    return;
  }
  mp3node* tempnode = MP3_HEAD;
  while(i < (uind-1)){
    if(tempnode != NULL){
      tempnode = tempnode->next;
    }else{
      printf("The index you input was too large. Check the list and try again.\n");
      return;
    }
    i++;
  }
  //test again
  if(tempnode == NULL){
    printf("The index you input was too large. Check the list and try again.\n");
    return;
  }
  
  newnode->prev = tempnode;
  newnode->next = tempnode->next;
  tempnode->next = newnode;

  printf("Successfully inserted:\n");
  printNode(newnode,uind);
}

  
void insertToEnd(){
  printf("Inserting node.\n");
  mp3node* newnode = buildNodeFromUserInput();
  int ind = 0;
  if(MP3_HEAD == NULL){
    MP3_HEAD = newnode;
  }else{
    mp3node* tmpnode = MP3_HEAD;

    ind = 1;
    while(tmpnode->next != NULL){
      tmpnode = tmpnode -> next;
      ind++;
    }
    tmpnode->next = newnode;
    newnode->prev = tmpnode;
    newnode->next = NULL;
  }

  printf("Successfully inserted record:\n");
  printNode(newnode,ind);
}
void removeIndex(){
  int uind;
  clearInput();

  printf("Please enter the index you would like to remove.\n");
  clearInput();
  scanf("%4d",&uind);
  while(uind <  0){
    printf("Recieved Bad input, please enter the index you would like to remove?\n");
    scanf("%4d",&uind);
  }
  printf("Attempting to remove record at index %d.\n",uind);
 
  if(uind == 0){
    if(MP3_HEAD != NULL){
      mp3node* tmp = MP3_HEAD;
      MP3_HEAD = MP3_HEAD->next;
      free(tmp);
      printf("Successfully removed the entry at index 0.\n");
    }else{
      printf("List is empty. Can't remove...\n");
    }
    return;
  }else{
    int i = 0;
    mp3node* tmpnode = MP3_HEAD;
    while(i < uind){
      tmpnode = tmpnode->next;
      i++;
    }
    mp3node* prevnode = tmpnode->prev;
    mp3node* nextnode = tmpnode->next;
    if(prevnode != NULL){
      prevnode->next = nextnode;
    }
    if(nextnode != NULL){
      nextnode->prev = prevnode;
    }
    free(tmpnode);
    printf("Successfully removed the entry at index %d.\n",i);
  }
}
void printTitleFilter(){
  printf("We will print the list, only those whose title contain the string that you enter.\n");
  char input[140];


  printf("What title would you like to search for?\n");
  input[0] = '\0';
  clearInput();
  fgets(input,140,stdin);
  while(input[0] == '\0' || input[0] == '\n'){
    printf("Recieved Bad input, what title would you like to search for?\n");
    fgets(input,140,stdin);
  }  
  
  char* inputcopy = strdup(input);
  strtok(inputcopy,"\n");


  printf("Searching for: %s\n",inputcopy);

  mp3node* tmp = MP3_HEAD;
  int i = 0;
  bool found = false;
  while(tmp){
    if(strstr(tmp->title,inputcopy) != NULL){
      printNode(tmp, i);
      found = true;
    }
    tmp = tmp->next;
    i++;
  }
  if(!found){
    printf("No results found. Consider altering your search.\n");
  }
}
void printArtistFilter(){
  printf("We will print the list, only those whose artist name contain the string that you enter.\n");

  char input[140];

  printf("What artist would you like to search for?\n");
  input[0] = '\0';

  clearInput();
  fgets(input,140,stdin);

  while(input[0] == '\0' || input[0] == '\n'){
    printf("Recieved bad input, what arist would you like to search for?\n");
    fgets(input,140,stdin);
  }
  char* inputcopy = strdup(input);

  strtok(inputcopy,"\n");
  printf("Searching for: %s\n",inputcopy);
  mp3node* tmp = MP3_HEAD;

  int i = 0;
  bool found = false;
  while(tmp){
    if(strstr(tmp->name,inputcopy) != NULL){
      printNode(tmp, i);
      found = true;
    }
    tmp = tmp->next;
    i++;
  }
  if(!found){
    printf("No results found. Consider altering your search.\n");
  }

}
void printYearSort(){
  //TODO
}
void printForwards(){
  printf("Printing the mp3 list...\n");
  printList();
  
}
void printBackwards(){
  printf("Printing the mp3 list backwards...\n");
  printListBackwards();
}
void save(){
  writeNodeToFile();
}
mp3node* buildNodeFromUserInput(){
  mp3node* newnode;
  newnode = malloc(sizeof(mp3node));

  printf("First, what is the artist's name?\n");
  char input[140];
  input[0] = '\0';
  clearInput();
  fgets(input,140,stdin);
  while(input[0] == '\0' || input[0] == '\n'){
    printf("Recieved Bad input, what is the artist's name?\n");
    fgets(input,140,stdin);
  }
  newnode->name = strdup(input);
  strtok(newnode->name, "\n");
  
  input[0] = '\0';
  printf("Next, what is the title of the song?\n");
  clearInput();
  fgets(input,140,stdin);
  while(input[0] == '\0' || input[0] == '\n'){
    printf("Recieved Bad input, what is the title of the song?\n");
    fgets(input,140,stdin);
  }
  newnode->title = strdup(input);
  strtok(newnode->title, "\n");

  printf("Next, what year was the song released?\n");
  int yearinput = -1;
  clearInput();
  scanf("%4d",&yearinput);
  while(yearinput <=  0){
    printf("Recieved Bad input, what year was the song released?\n");
    scanf("%4d",&yearinput);
  }
  newnode->year = yearinput;

  printf("Finally, how long (in seconds) is the song?\n");

  int timeinput = -1;
  clearInput();
  scanf("%10d",&timeinput);
  while(timeinput <= 0){
    printf("Recieved Bad input, how long (in seconds) is the song?\n");
    scanf("%10d",&timeinput);
  }
  newnode->runtime = timeinput;

  return newnode;
}
void clearInput(){
  fseek(stdin,0,SEEK_END);
  fflush(stdin);
}
