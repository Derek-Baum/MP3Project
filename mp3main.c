#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdbool.h>
#include "serialize.h"


typedef struct mp3node mp3node;

char* readFile(char* filename);
mp3node* readMp3Node(char* filename, ser_tra_t* translator);
void writeNodeToFile(mp3node* node, char* filename, ser_tra_t* translator);
void startup(ser_tra_t* translator, mp3node* firstNode);
void printList(mp3node* list);
void printNode(mp3node* singleNode);
bool processInput(ser_tra_t* translator, mp3node* start);
int getUserInput();
int indexOf(char* str, char c);
void insertIndex(mp3node* start);
void insertToEnd(mp3node* start);
void removeIndex(mp3node* start);
void printTitleFilter(mp3node* start);
void printArtistFilter(mp3node* start);
void printYearSort(mp3node* start);
void printForwards(mp3node* start);
void printBackwards(mp3node* start);
void save(ser_tra_t* translator, mp3node* start);



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
  
  
  ser_tra_t* tra;
  mp3node* first;

  startup(tra,first);
  bool exit = false;

  if(first){
    printf("Initial List Loaded From File\nPrinting... \n");
    printList(first);
  }else{
    printf("No list loaded from file.\n");
  }

  while(!exit){
    exit = processInput(tra,first);
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

  if(tra != NULL){
  
    printf("Let's see what we got...\n");

  }
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
mp3node* readMp3Node(char* filename, ser_tra_t* translator){

  char* out = readFile(filename);

  mp3node* output = ser_parse(translator,"node",out,NULL);

  free(out);
  
  return output;
}
void writeNodeToFile(mp3node* node, char* filename, ser_tra_t* translator){

  FILE * fp;
  
  char* raw = ser_ialize(translator,"node",node,NULL,0);

  printf("writing to file...\n");
  printf("%s\n",raw);

  fp = fopen(filename, "w");

  fprintf(fp,raw);
  fclose(fp);
}
void startup(ser_tra_t* translator, mp3node* firstNode){
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
    firstNode = readMp3Node(FILE_STORE, translator);
  }else{
    printf("No list found on file system.\n");
    firstNode = NULL;
  }

}

void printList(mp3node* list){

  mp3node* tmp;
  *tmp = *list;
  while(tmp){
    printNode(tmp);
    tmp = tmp->next;
  }
  
}
void printNode(mp3node* singleNode){
  printf("%s - %s (%d)\n",singleNode->title,singleNode->name,singleNode->year);
  printf("Runtime: %d\n",singleNode->runtime);
  printf("----------------------------------------\n");
}
/*
 * Return is boolean
 * True: stop running : 1
 * False: Continue running : 0
 */
bool processInput(ser_tra_t* translator, mp3node* start){
  //Write a new function, that handles all of the input, and puts it into a String. It handles displaying the input
  //options, aswell as preventing the user from messing up.
  int userChoice = getUserInput();
  if(userChoice == 1){
    //insert an mp3, with index.
    insertIndex(start);
  }else if(userChoice == 2){
    //insert an mp3, without index.
    insertToEnd(start);
  }else if(userChoice == 3){
    //remove mp3, by its index only.
    removeIndex(start);
  }else if(userChoice == 4){
    //print filter by title
    printTitleFilter(start);
  }else if(userChoice == 5){
    //print filter by artist name
    printArtistFilter(start);
  }else if(userChoice == 6){
    //print sorted by year
    printYearSort(start);
  }else if(userChoice == 7){
    //print list forwards
    printForwards(start);
  }else if(userChoice == 8){
    //print list backwards
    printBackwards(start);
  }else if(userChoice == 9){
    //save
    save(translator,start);
  }else if(userChoice == 10){
    //exit
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
  printf("What would you like to do?");
  printf("P : Print the list.(Many different options.)\n");
  printf("I : Insert an mp3.\n");
  printf("R : Remove an mp3 by its index.\n");
  printf("S : Save current list to file, to be loaded up on program startup.\n");
  printf("E : Exit Program.\n");


  while(strchr(filter, *tmp)==NULL){
    fgets(input,2,stdin);
    tmp = input;
  }
  int ind = (indexOf(filter,input[0]) % 5);


  if(ind == 0){
    printf("How would yo like to print the list?\n");
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
      //prints hte list, sorted by year.
      output = 6;
    }

  }else if(ind == 1){
    //insert
    printf("Input i to insert based on index, any other input will result in insertion to end of list.\n");
    fgets(input,2,stdin);
    if(input[0] == 'i' || input[0] == 'I'){
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
void insertIndex(mp3node* start){

}
void insertToEnd(mp3node* start){

}
void removeIndex(mp3node* start){

}
void printTitleFilter(mp3node* start){

}
void printArtistFilter(mp3node* start){

}
void printYearSort(mp3node* start){

}
void printForwards(mp3node* start){

}
void printBackwards(mp3node* start){

}
void save(ser_tra_t* translator, mp3node* start){
  writeNodeToFile(start,FILE_STORE,translator);
}
