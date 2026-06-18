#include <stdio.h>
#include "file.h"

void saveContactsToFile(AddressBook *addressBook) 
{
  FILE *fptr;
  fptr = fopen("contacts.csv", "w");
  int i;
  fprintf(fptr, "#%d\n", addressBook->contactCount);

  for(i = 0; i < addressBook->contactCount; i++)
  {
    fprintf(fptr, " %s, %s, %s\n",addressBook->contacts[i].name,addressBook->contacts[i].phone,addressBook->contacts[i].email);
  }
  fclose(fptr);
}

void loadContactsFromFile(AddressBook *addressBook)
{
printf("loading file ....\n");
FILE *fptr;
fptr = fopen("contacts.csv", "r");
if(fptr == NULL)
{
  printf("File not found \n");
  return;
}
int i;
fscanf(fptr, "#%d\n", &addressBook->contactCount);
printf("------------%d\n", addressBook->contactCount);
for(i = 0; i < addressBook->contactCount; i++)
{
  fscanf(fptr, "%[^,],%[^,],%s\n",addressBook->contacts[i].name, addressBook->contacts[i].phone, addressBook->contacts[i].email);
}
fclose(fptr);
}
