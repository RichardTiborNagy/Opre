#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <time.h>
#include <signal.h>

//Signal handler.
void handler(int signumber){
  //printf("\nSignal with number %i has arrived",signumber);
  return;
}

//A structure that contains all the data for a participant registration.
struct Registration {
    char fname[20];
    char lname[20];
    char email[30];
    char phone[15];
    char route[20];
    int people;
    char date[25];        
};

//Function declarations.
void MainMenu(); 
void Add();
void Edit();
void Delete();
void ListAll();
void ListByRoute();
void StartRoute();

//The program starts by displaying the main menu.
int main(int argc,char *argv[]){    
        signal(SIGRTMIN, handler); 
        MainMenu();    
        return 0;
}

void MainMenu(){
    char input[100];
    //Loop until a correct option is given.
    while(1){
        int correct;
        do{
            correct = 1;
            printf("\n\na:\tAdd a new registration");
            printf("\ne:\tEdit an existing registration");
            printf("\nd:\tDelete an existing registration");
            printf("\nl:\tList all registrations");
            printf("\nr:\tList all registrations by route");
            printf("\ns:\tStart a route");
            printf("\nq:\tQuit");
            printf("\nChoose an option: ");
            scanf("%s",input);
            if(strcmp(input,"a") == 0){
                Add();
            } 
            else if(strcmp(input,"e") == 0){
                Edit();
            }
            else if(strcmp(input,"d") == 0){
                Delete();
            }
            else if(strcmp(input,"l") == 0){
                ListAll();
            }
            else if(strcmp(input,"r") == 0){
                ListByRoute();
            } 
            else if(strcmp(input,"s") == 0){
                StartRoute();
            }
            else if(strcmp(input,"q") == 0){
                return;
            }
            else {
                correct = 0;
                printf("\nNo such option, try again!");
            }
        } while (correct == 0);
    }   
}

//Handles adding a new registration to the data file.
void Add(){    
    //Open the data file.
    int f = open("data.txt", O_WRONLY | O_APPEND | O_CREAT, S_IRUSR | S_IWUSR);
    if (f<0){perror("File opening error\n"); exit(1);}
    
    //Create a new struct, and fill it with data from the user.
    struct Registration reg;

    printf("Adding a new registration.");
	printf("\nFirst name:\t");
    scanf("%s",&(reg.fname));
    printf("\nLast name:\t");
	scanf("%s",&(reg.lname));
	printf("\nEmail:\t");
	scanf("%s",&(reg.email));
	printf("\nPhone number:\t");
    scanf("%s",&(reg.phone));
    printf("\nNumber of other participants:\t");
    scanf("%d",&(reg.people));
    
    //The route can only be one of 3 things:
    int correct;    
    do{
        char route[100];
        correct = 1;
        printf("\nRoute:\t 1:Parlament 2:Hősök tere 3:Vár\t");    
        scanf("%s",&route);
        if (strcmp(route,"1") == 0){
            strcpy(reg.route , "Parlament");
        } else if (strcmp(route,"2") == 0){
            strcpy(reg.route , "Hősök tere");
        } else if (strcmp(route,"3") == 0){
            strcpy(reg.route , "Vár");
        } else {
            correct = 0;
            printf("\nNo such route, try again!");
        }
    } while (correct == 0);
    
    //Get the current time and store it as well.
    time_t rawtime;
    struct tm * timeinfo;  
    time (&rawtime);
    timeinfo = localtime (&rawtime);  
    strftime (reg.date,25,"%x %X",timeinfo);

    //Write the struct into the data file, give an error if it did not succeed.
    if(write(f,&reg, sizeof(reg)) != sizeof(reg)){perror("\nAn error occured while writing to the file.\n");exit(1);};

    //Close the data file and return to the main menu.
    close(f);
    return;
}

//Handles editing an existing registration.
void Edit(){
    //Get the email address of the registration the user wants to edit.
    char email[50];
    printf("\nType the email address of the registration you want to edit:\t");
    scanf("%s",&email);
    int emailFound = 1;

    struct Registration reg;

    //Open the data file and a temporary helper file.
    int f = open("data.txt", O_RDONLY);
    if (f<0){perror("File opening error\n"); exit(1);}
    int g = open("temp.txt", O_WRONLY | O_APPEND | O_CREAT, S_IRUSR | S_IWUSR);
    if (g<0){perror("File opening error\n"); exit(1);}

    //Loop trough the data file.
    while(read(f,&reg,sizeof(reg))){
        //If the email of a registration is the same as the given one, get the new info.
        if (strcmp(email,reg.email) == 0){
            emailFound = 0;
            struct Registration ereg;    
            printf("\nFill in the new information:");
            printf("\nFirst name:\t");
            scanf("%s",&(ereg.fname));
            printf("\nLast name:\t");
            scanf("%s",&(ereg.lname));
            printf("\nEmail:\t");
            scanf("%s",&(ereg.email));
            printf("\nPhone number:\t");
            scanf("%s",&(ereg.phone));
            printf("\nNumber of other participants:\t");
            scanf("%d",&(ereg.people));
            int correct;    
            do{
                char route[100];
                correct = 1;
                printf("\nRoute:\t 1:Parlament 2:Hősök tere 3:Vár\t");    
                scanf("%s",&route);
                if (strcmp(route,"1") == 0){
                    strcpy(ereg.route , "Parlament");
                } else if (strcmp(route,"2") == 0){
                    strcpy(ereg.route , "Hősök tere");
                } else if (strcmp(route,"3") == 0){
                    strcpy(ereg.route , "Vár");
                } else {
                    correct = 0;
                    printf("\nNo such route, try again!");
                }
            } while (correct == 0);    
            time_t rawtime;
            struct tm * timeinfo;  
            time (&rawtime);
            timeinfo = localtime (&rawtime);  
            strftime (ereg.date,25,"%x %X",timeinfo);
            //Copy the edited registration in the temporary file.
            if(write(g,&ereg, sizeof(ereg)) != sizeof(ereg)){perror("\nAn error occured while writing to the file.\n");exit(1);};
        } else {
            //Copy the registration in the temporary file.
            if(write(g,&reg, sizeof(reg)) != sizeof(reg)){perror("\nAn error occured while writing to the file.\n");exit(1);};
        }         
    } 

    //Check if there was a registration with the given email address.
    if (emailFound == 1) printf("\nNo registration found with the given email.");

    //Close the files.
    close(f);
    close(g);

    //Delete the old data file.
    remove("data.txt");
    //Rename the helper file to be the new data file.
    rename("temp.txt", "data.txt");

    //Return to the main menu.
    return;
}

//Handles the deletion of an existing registration
void Delete(){
    //Get the email address of the registration the user wants to delete.
    char email[50];
    printf("\nType the email address of the registration you want to delete:\t");
    scanf("%s",&email);
    int emailFound = 1;
    
    struct Registration reg;

    //Open the data and a helper file.
    int f = open("data.txt", O_RDONLY);
    if (f<0){perror("File opening error\n"); exit(1);}
    int g = open("temp.txt", O_WRONLY | O_APPEND | O_CREAT, S_IRUSR | S_IWUSR);
    if (g<0){perror("File opening error\n"); exit(1);}

    //Loop through the registrations, if the email address is the same as the given one, skip it, otherwise copy them in the helper file.
    while(read(f,&reg,sizeof(reg))){
        if (strcmp(email,reg.email) == 0){
            emailFound = 0;
        } else {
            if(write(g,&reg, sizeof(reg)) != sizeof(reg)){perror("\nAn error occured while writing to the file.\n");exit(1);};
        }         
    } 

    //Check if there was a registration with the given email address.
    if (emailFound == 1) printf("\nNo registration found with the given email.");

    //Close the files.
    close(f);
    close(g);

    //Delete the old data file.
    remove("data.txt");
    //Rename the helper file to be the new data file.
    rename("temp.txt", "data.txt");

    //Return to the main menu.
    return;
}

//Handles the listing of all the registrations.
void ListAll(){
    //Open the data file.
    int f = open("data.txt", O_RDONLY);
    if (f<0){perror("File opening error\n"); exit(1);}
    struct Registration reg;
    printf("\nListing all registrations");
    printf("\nOrder of data: \tName\tEmail\tPhone number\tRoute\tPeople\tDate\n");
    //Loop through the registrations, write them out to the screen.
    while(read(f,&reg,sizeof(reg))){
        printf("\n%s %s    \t%s \t%s \t%s     \t%d \t%s", reg.fname, reg.lname, reg.email, reg.phone, reg.route, reg.people, reg.date);
    }  

    //Close the data file.
    close(f);

    //Return to the main menu.
    return;
}

//Handles the listing of the registrations on a specific route.
void ListByRoute(){
    //Get the route the user wants.
    char route[15];
    int correct;    
    do{
        correct = 1;
        printf("\nChoose a route:\t 1:Parlament 2:Hősök tere 3:Vár\t");    
        scanf("%s",&route);
        if (strcmp(route,"1") == 0){
            strcpy(route , "Parlament");
        } else if (strcmp(route,"2") == 0){
            strcpy(route , "Hősök tere");
        } else if (strcmp(route,"3") == 0){
            strcpy(route , "Vár");
        } else {
            correct = 0;
            printf("\nNo such route, try again!");
        }
    } while (correct == 0);

    //Open the data file.
    int f = open("data.txt", O_RDONLY);
    if (f<0){perror("File opening error\n"); exit(1);}
    struct Registration reg;

    printf("\nListing all registrations on route: %s", route);
    printf("\nOrder of data: \tName\tEmail\tPhone number\tRoute\tPeople\tDate\n");
    
    //Loop through the registrations, write out the ones that are on the given route.
    while(read(f,&reg,sizeof(reg))){
        if (strcmp(route,reg.route) == 0){
            printf("\n%s %s    \t%s \t%s \t%s     \t%d \t%s", reg.fname, reg.lname, reg.email, reg.phone, reg.route, reg.people, reg.date);
        }            
    }  

    //Close the data file.
    close(f);

    //Return to the main menu.
    return;
}

void StartRoute(){

    //Get the route from the user.
    char chosenRoute[20];
    printf("\nPlease choose a route:");
    //The route can only be one of 3 things:
    int correct;    
    do{
        char route[100];
        correct = 1;
        printf("\nRoute:\t 1:Parlament 2:Hősök tere 3:Vár\t");    
        scanf("%s",&route);
        if (strcmp(route,"1") == 0){
            strcpy(chosenRoute , "Parlament");
        } else if (strcmp(route,"2") == 0){
            strcpy(chosenRoute , "Hősök tere");
        } else if (strcmp(route,"3") == 0){
            strcpy(chosenRoute , "Vár");
        } else {
            correct = 0;
            printf("\nNo such route, try again!");
        }
    } while (correct == 0);

    //Count the registrations on the chosen route.
    int registrationCount = 0;
    int peopleCount = 0;
    //Open the data file.
    int f = open("data.txt", O_RDONLY);
    if (f<0){perror("File opening error\n"); exit(1);}

    struct Registration reg;    
    //Loop through the registrations, increment the counting variable if necessary.
    while(read(f,&reg,sizeof(reg))){
        if (strcmp(reg.route,chosenRoute) == 0){
            registrationCount++;
            peopleCount += reg.people;
        } 
    }  
    //Close the data file.
    close(f);

    //The route will only start if there is at least 10 paticipants.
    if (peopleCount < 10) {
        printf("\nNot enough participants to start this route.");
        return;
    }

    //Create pipes.
    int pipePtoC[2]; //Parent to child
    int pipeCtoP[2]; //Child to parent
    if (pipe(pipePtoC) == -1 || pipe(pipeCtoP) == -1) {
        perror("Error during pipe opening!");
        exit(1);
    }
    

    pid_t child = fork();
	if(child < 0) {
		perror("Error during fork!");
		exit(1);
	}


    if (child > 0){      //Parent process, SKB
        //Close unused pipe endings
        close(pipePtoC[0]);
        close(pipeCtoP[1]);

        printf("\nSKB: Starting route to %s, waiting for SBD to get ready.\n", chosenRoute);
        pause();
        printf("\nSKB: Sending data to SDB.\n");

        //Collect the registrations on the current route.
        struct Registration registrations[registrationCount];
        int i = 0;
        //Open the data file.
        int f = open("data.txt", O_RDONLY);
        if (f<0){perror("File opening error\n"); exit(1);}

        struct Registration reg;    
        //Loop through the registrations, collect the ones on the current route.
        while(read(f,&reg,sizeof(reg))){
            if (strcmp(reg.route,chosenRoute) == 0){
                registrations[i] = reg;
                i++;
            }            
        }  
        //Close the data file.
        close(f);

        int price = 1000;

        write(pipePtoC[1], &registrationCount, sizeof(int));
        write(pipePtoC[1], &peopleCount, sizeof(int));
        write(pipePtoC[1], &registrations, registrationCount * sizeof(struct Registration));
        write(pipePtoC[1], &price, sizeof(int));
        write(pipePtoC[1], &chosenRoute, 20 * sizeof(char));

        printf("\nSKB: Waiting for financials.\n");

        int beer = 0;
        read(pipeCtoP[0], &beer, sizeof(int));
        int income = 0;
        read(pipeCtoP[0], &income, sizeof(int));

        printf("\nSKB: Received the following financial info:");
        printf("\nSKB: Liters of beer bought: %d", beer);
        printf("\nSKB: Total income earned: %d", income);

        printf("\nSKB: Waiting for the tour to finish.\n");

        pause();

        printf("\nSKB: The tour finished successfully.\n");

        wait();
    }
    else {      //Child pricess, SBD
        //Close unused pipe endings
        close(pipePtoC[1]);
        close(pipeCtoP[0]);

        printf("\nSBD: Getting ready for shopping.\n");
        sleep(1);
        int parent = getppid();
        printf("\nSBD: Ready for shopping, notifying SKB.\n");
        kill(parent, SIGRTMIN);
        
        registrationCount = 0;
        read(pipePtoC[0], &registrationCount, sizeof(int));
        int customerCount = 0;
        read(pipePtoC[0], &customerCount, sizeof(int));
        struct Registration customers[customerCount];
        read(pipePtoC[0], &customers, registrationCount * sizeof(struct Registration));
        int price = 0;
        read(pipePtoC[0], &price, sizeof(int));
        char currentRoute[20];
        read(pipePtoC[0], &currentRoute, 20 * sizeof(char));

        printf("\nSBD: Received the following data:");
        printf("\nSBD: Number of registrations: %d", registrationCount);
        printf("\nSBD: Total number of customers: %d", customerCount);
        printf("\nSBD: Cost of the tour per person (without a discount): %d", price);
        printf("\nSBD: Name of the current route: %s", currentRoute);
        printf("\nSBD: Participants: ");
        int i = 0;
        for (; i < registrationCount; i++){
            printf("%s %s, ", customers[i].fname, customers[i].lname);
        }

        printf("\n\nSBD: Buying beer, collecting income.\n");
        int beer = customerCount * 5;
        int discountPrice = 0.85f * price;
        int income = registrationCount * price + (customerCount - registrationCount) * discountPrice;

        printf("\nSBD: Sending financial info to SKB.\n");
        write(pipeCtoP[1], &beer, sizeof(int));
        write(pipeCtoP[1], &income, sizeof(int));

        printf("\nSBD: Starting the tour.\n");

        sleep(rand()%(1 - 3) + 1);

        printf("\nSBD: The tour is over, notifying SKB.\n");
        kill(parent, SIGRTMIN);

        exit(0);
    }
}