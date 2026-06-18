#ifndef CONTACT_H
#define CONTACT_H

typedef struct {
    char name[50];
    char phone[20];
    char email[50];
} Contact;

typedef struct {
    Contact contacts[100];
    int contactCount ;
} AddressBook;

void createContact(AddressBook *addressBook);
void searchContact(AddressBook *addressBook);
void editContact(AddressBook *addressBook);
void deleteContact(AddressBook *addressBook);
void listContacts(AddressBook *addressBook);
void initialize(AddressBook *addressBook);
void saveContactsToFile(AddressBook *AddressBook);
void loadContactsFromFile(AddressBook *addressBook);
int validate_name(char *name);
int validate_phone(char *str, AddressBook *addressBook);
int validate_email(char *mail, AddressBook *addressBook);



#endif
