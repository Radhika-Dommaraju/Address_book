#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "contact.h"
#include "file.h"
//#include "populate.h"

void listContacts(AddressBook *addressBook) 
{
    // Sort contacts based on the chosen criteria

    Contact temp;
    int i, j;
    for(i = 0; i < addressBook->contactCount-1; i++)
    {
        for(j = 0; j < addressBook->contactCount - i - 1; j++)
        {
            if((strcasecmp(addressBook->contacts[j].name , addressBook->contacts[j+1].name)) > 0)
            {
                temp = addressBook->contacts[j];
                addressBook->contacts[j] = addressBook->contacts[j+1];
                addressBook->contacts[j+1] = temp;
            }
        }
    }

    for(i = 0; i < addressBook->contactCount; i++)
    {
        printf("%d | %s | %s | %s \n", i+1, addressBook->contacts[i].name,addressBook->contacts[i].phone, addressBook->contacts[i].email );
    }

}

void initialize(AddressBook *addressBook) {
    addressBook->contactCount = 0;
    //populateAddressBook(addressBook);
    
    // Load contacts from file during initialization (After files)
    loadContactsFromFile(addressBook);
}


void saveAndExit(AddressBook *addressBook) {
    saveContactsToFile(addressBook); // Save contacts to file
    exit(EXIT_SUCCESS); // Exit the program
}

int validate_name(char *name)
{
    int i = 0;

    for( i = 0; name[i] != '\0'; i++)
    {
        if((name[i] >= 'a' && name[i] <= 'z') || (name[i] >= 'A' && name[i] <= 'Z')  )
        {
            return 1;
        }
        else if (name [i] == ' ' || name[i] == '.')
        {
            return 1;
        }
       
    }
   // LOOP till name[i] != '\0'
     //   check name[i] is not in the range of A to Z, a to z, ' ', '.'

       //     return 0; // invalid
    //END LOOP
    printf("\033[1;31m");
    printf("ERROR : Invalid input\n");
    printf("\033[0m");
    
    return 0;
  // valid.
}

int validate_phone(char *str, AddressBook *addressBook)
{
    //check the length
         // != 10  ==> return 0 (invalid)
     int i = 0;
     int len ;
     len = strlen(str);
     if(len != 10)
     {
        
        printf("\033[1;31m");
        printf("ERROR : ");
        printf("\033[0m");
        
        printf(" \033[1;34m number should exactly contain 10 digits \033[0m \n");
        return 0;
     }


    // Loop till str[i] != '\0'
    //     check the str[i] is not in the rage '0' to '9'
    //         return 0; invalid
    while(str[i] != '\0')
    {
        if(!(str[i] >= '0' && str[i] <= '9'))
        {
            
        printf("\033[1;31m");
        printf("ERROR : ");
        printf("\033[0m");
        printf("\033[1;34m number should only contain digits \033[0m\n");
        return 0;
        
        }
    
        i++;

    }


    // check unique or not
    //     not => return 0;
    int res;
    //condition for loop
    for(i = 0; i < addressBook->contactCount; i++)
    {
    res = strcmp(addressBook->contacts[i].phone, str);
    
    if(res == 0)
    {
        
        printf("\033[1;31m");
        printf("ERROR : ");
        printf("\033[0m");
        
        printf("\033[1;34m This phone number already exists \033[0m\n");
        return 0;
    }
    }

    return 1;  // valid;
}

int validate_email(char *mail, AddressBook *addressBook)
{

    if(!(mail[0] >= 'a' && mail[0] <= 'z'))
    {
    
        printf("\033[1;31m");
        printf("ERROR : ");
        printf("\033[0m");
        printf(" \033[1;34m mail id should start with a lower case alphabet \033[0m\n");
        return 0;
    }
int i; 
for(i = 1; mail[i] != '\0'; i++)
{
    
    
    if(!((mail[i] >= 'a' && mail[i] <= 'z') || (mail[i] >= '0' && mail[i] <= '9') || (mail[i] == '@' || mail[i] == '.')))
    {
        printf("\033[1;31m");
        printf("Error : invalid input\n");
        printf("\033[0m");
    
        return 0;
    }

    

}
int count = 0;
for(i = 0; mail[i] != '\0'; i++)
{
    if(mail[i] == '@')
    {
        count++;

    }

}
if(count > 1)
{

    printf("\033[1;31m");
    printf("ERROR : ");
    printf("\033[0m");

    printf("\033[1;34m Invalid input email should have only one @ \033[0m\n");
    return 0;
}
char *at;
   //check for @_.com
at = strchr(mail,'@');

if(at == NULL)
{
    printf("\033[1;31m");
    printf("ERROR : Invalid input\n");
    printf("\033[0m");
    
    return 0;
}

if(*(at + 1) == '.')
{
    printf("\033[1;31m");
    printf("ERROR : ");
    printf("\033[0m");
    printf(" \033[1;34m Invalid input(expects a character after '@' eg: @g.com) \033[0m\n");
    return 0;
}

int len, res;
len = strlen(mail);
res = strcmp( &mail[len-4] , ".com");
if(res > 0 || res < 0)
{
     printf("\033[1;31m");
     printf("ERROR : ");
     printf("\033[0m");
    printf("\033[1;34m Invalid input email id should end with .com \033[0m\n");

    return 0;
}
   

   //check for duplicate

   for(i = 0; i < addressBook->contactCount ; i++)
   {
    if(!(strcmp(addressBook->contacts[i].email,mail)))
    {
        printf("\033[1;");
        printf("ERROR : this email already exists\n");
        printf("\033[0m");
    
    
    return 0;
    }
   }
   
   

   return 1;

}


void createContact(AddressBook *addressBook)
{
	/* Define the logic to create a Contacts */

    char name[20];
    int valid;

    while(1)
    {
        printf("Enter the name : ");
        scanf(" %[^\n]", name);
        

        valid = validate_name(name);
        
        if(valid == 1)
           break; // valid

    }

    char mobile[11];

    while(1)
    {
        printf("Enter the phone number : ");
        scanf(" %[^\n]", mobile);
        

        valid = validate_phone(mobile, addressBook);
        if(valid == 1)// valid
          break;
        
    }

    char mail[30];

   while(1)
    {
        printf("Enter the email : ");
        scanf(" %[^\n]", mail);
        
      valid = validate_email(mail, addressBook);
       if(valid == 1)
        break;
       

    }


  
    
    strcpy(addressBook->contacts[addressBook->contactCount].name, name);
    strcpy(addressBook->contacts[addressBook->contactCount].phone, mobile);
    strcpy(addressBook->contacts[addressBook->contactCount].email, mail);

    addressBook->contactCount++;
    printf("\033[0;32m");
    printf("Contact created successfully!\n");
    printf("\033[0m");

    
}

int search_by_name(AddressBook *addressBook)
{
    
    char name[20];
    printf("Enter the name : ");
    scanf(" %[^\n]",name);

    int res, i = 0, count = 0, ind = 0;
    for ( i = 0; i < addressBook->contactCount; i++)
    {
    res = strcmp(addressBook->contacts[i].name, name);
    if(res == 0)
    {
        printf("%s %s %s \n", addressBook->contacts[i].name,addressBook->contacts[i].phone,addressBook->contacts[i].email );
        ind = i;
        count = count + 1;
    
    }
    }
    if (count == 0)
    {
        printf("\033[1;31m");
        printf("ERROR : ");
        printf("\033[0m");
        printf("\033[1;33m No match found\033[0m \n");
        return -1;
    }
    if(count == 1)
    {
        return ind;
    }
    if(count > 1)
    {
        return search_by_phone(addressBook);
    }
 return -1;
}


int search_by_phone(AddressBook *addressBook)
{
    char num[15];
        printf("Enter phone number :");
        scanf("%s", num);
        int i, count = 0;
        for(i = 0; i < addressBook->contactCount; i++)
        {
            if(!(strcmp(addressBook->contacts[i].phone, num)))
            {
            printf("%s %s %s", addressBook->contacts[i].name, addressBook->contacts[i].phone, addressBook->contacts[i].email);
            count++;
            return i;
            }
            
        }
        if(count == 0)
        {
            printf("\033[1;31m");
            printf("ERROR : ");
            printf("\033[0m\n");
            printf("\033[1;33mNo match found\033[0m\n");
            return -1;
        }
}

int search_by_mail(AddressBook *addressBook)
{
    char mail[20];
        printf("Enter your mail :");
        scanf("%s", mail);
        int i, count = 0;
        for(i = 0; i < addressBook->contactCount; i++)
        {
            if(!(strcmp(addressBook->contacts[i].email, mail)))
            printf("%s %s %s", addressBook->contacts[i].name, addressBook->contacts[i].phone, addressBook->contacts[i].email);
            count++;
            return i;
        }
        if(count == 0)
        {
            printf("\033[1;31m");
            printf("ERROR : ");
            printf("\033[0m\n");
             printf("\033[1;33mNo match found\033[0m\n");
            return 0;
        }
        
}

void edit_by_name(AddressBook *addressBook , int res)
{
    int valid;
    char name[20];
    
    while(1)
    {
        printf("Enter the name :");
        scanf(" %[^\n]", name);

        valid = validate_name(name);
        if(valid == 1)
        break;
    }
    strcpy(addressBook->contacts[res].name, name);
    printf("\033[0;32m");
    printf("Your contact is updated successfully !\n");
    printf("\033[0m");
}


void edit_by_phone(AddressBook *addressBook , int res)
{
    char phone[11];
    int valid;
    while(1)
    {
        printf("\nEnter the phone number :\n");
        scanf(" %[^\n]", phone);

        valid = validate_phone(phone,addressBook);
        if(valid == 1)
        break;
    }
    strcpy(addressBook->contacts[res].phone, phone);
    printf("\033[0;32m");
    printf("Your contact is updated successfully !\n");
    printf("\033[0m");

}


void edit_by_mail(AddressBook *addressBook , int res)
{
    char mail[30];
    int valid;
    while(1)
    {
        printf("Enter the mail :");
        scanf(" %[^\n]", mail);

        valid = validate_email(mail, addressBook);
        if(valid == 1)
        break;
    }
    strcpy(addressBook->contacts[res].email, mail);
    printf("\033[0;32m");
    printf("Your contact is updated successfully !\n");
    printf("\033[0m");
}



void searchContact(AddressBook *addressBook) 
{
    int choice, res;
    /* Define the logic for search */
   printf("Search by : \n");
   printf("1. By name \n");
   printf("2. By number\n");
   printf("3. By email\n");
   printf("Enter your choice : ");
   scanf("%d", &choice);
   switch(choice)
   {
    case 1:
    res = search_by_name(addressBook);
    break;

    case 2:
    //search  by number
    res =search_by_phone(addressBook);
    break;

    case 3:
    //search by email
    res = search_by_mail(addressBook);
    break;

   }
}

void editContact(AddressBook *addressBook)
{
	/* Define the logic for Editcontact */
    int choice, res;
    /* Define the logic for search */
   printf("Search by : \n");
   printf("1. By name \n");
   printf("2. By number\n");
   printf("3. By email\n");
   printf("Enter your choice : ");
   scanf("%d", &choice);
   switch(choice)
   {
    case 1:
    res = search_by_name(addressBook);
    break;

    case 2:
    //search  by number
    res =search_by_phone(addressBook);
    break;

    case 3:
    //search by email
    res = search_by_mail(addressBook);
    break;

    default:
    printf("Invalid input\n");
    break;


   }
   
   
   if(res == -1)
   {

   }
else{
   printf("enter the field you want to edit :\n");
   printf("1.Name\n2.Phone number\n3.Email\n");
   printf("Enter your choice :");
   scanf("%d", &choice);

   switch(choice)
   {
    case 1:
    edit_by_name(addressBook, res);
    break;

    case 2:
    edit_by_phone(addressBook,res);
    break;

    case 3:
    edit_by_mail(addressBook, res);
    break;

    default:
    printf("Invalid input\n");
    break;
   }
    
}
}

void deleteContact(AddressBook *addressBook)
{
	/* Define the logic for deletecontact */

    
	/* Define the logic for Editcontact */
    int choice, res, i;
    /* Define the logic for search */
   printf("Search by : \n");
   printf("1. By name \n");
   printf("2. By number\n");
   printf("3. By email\n");
   printf("Enter your choice : ");
   scanf("%d", &choice);
   switch(choice)
   {
    case 1:
    res = search_by_name(addressBook);
    break;

    case 2:
    //search  by number
    res =search_by_phone(addressBook);
    break;

    case 3:
    //search by email
    res = search_by_mail(addressBook);
    break;

    default:
   
    printf("Invalid input\n");
    break;

   }
   if(res == -1)
   {

   }
   else
   {

    for (i = res; i < addressBook->contactCount - 1; i++)
    {
        addressBook->contacts[i] = addressBook->contacts[i+1];
    }
    addressBook->contactCount --;

    printf("\033[0;32m");
    printf("\nContact deleted successfully\n");
    printf("\033[0m");
   }
   
}
