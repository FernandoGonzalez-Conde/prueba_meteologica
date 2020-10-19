#include <stdio.h>
#include <stdlib.h>
#include <string.h>


typedef struct{
	char *name;
	char *date;
	double max_Temp;
	double min_Temp;
	double precipitation;
	//int cloudiness;
}city_Attributes;


city_Attributes *city;   // We declare an array of type city_Attributes where each element belongs to the days after the date entered for the desired city.


int count_rows();
int find_and_store(int num_rows, char date[], char name_city[]);
void change(char data[]);
void change2(char data[]);
void remove_space(char data[]);
void change_Gijon(char data []);
void empty(char temp[]);
void copy(char temp[], int l, int j);
char* flip_date(char date[]);
double Cel_to_Fahren(double *temperature);
void store_in_JSON(int num_days ,char name_of_city[]);


int main(){
	
	int num_rows; 
	int num_days ;   // This variable is used to count the days of each city.
	char ext[4];   // This variable is used to make the decision whether or not to exit the program.
	char date[11];   // This variable is used to store the date that the user enters.
	char name_city[30];   // This variable is used to store the city that the user enters.
	
	
	
	num_rows = count_rows();   // This function would work for a document with few rows because the malloc function overflows for this one.

	
	
	
	do{
		change2(ext);
		printf("Enter the name of the city you would like to consult:\n");
		fflush(stdin); // Remove the intro-letter from the buffer.
		fgets(name_city,20,stdin);
		change2(name_city);
		printf("\n\nEnter the date you would like to consult (ej:dd/mm/yyyy):\n");
		fflush(stdin); // Remove the intro-letter from the buffer.
		fgets(date,11,stdin);
		change2(date);
		printf("\n\n\n");
		fflush(stdin);
		num_days = find_and_store(num_rows, date, name_city);
		store_in_JSON( num_days ,name_city);
		
		printf("Do you want to continue obtaining data ('Yes' or 'No')?");
		do{
			fflush(stdin);
			scanf("%s", &ext);
			if((strcmpi(ext, "Yes")) && (strcmpi(ext, "No"))){
				printf("you must enter one of two options: 'Yes' or 'No'.\n");
			}
		}while((strcmpi(ext, "Yes")) && (strcmpi(ext, "No")));
		
	}while(!strcmpi(ext,"Yes"));
	
	free(city);   // We free the reserved memory.
	
	system("pause");
	return 0;
	
}



// This function searches the CSV for the indicated date and city, saving in variables and showing the results.
int find_and_store(int num_rows, char date[], char name_city[]){

	int i, j, k;
	int l;
	int count = 0;
	int exits_city = 0;   // Serves to indicate if the entered city exists.
	int exits_day = 0;   // Serves to indicate if the entered date exists.
	char aux='\0';   // It is the variable where the selected character of the CSV is stored in each iteration.
	char date_temp[10];   // It is the variable where the dates are stored while searching for the corresponding row.
	char name_city_temp[50];   // It is the variable where the names of cities are stored while searching for the corresponding row.
	char temp[200];   // We use a variable temporarily to store the data character by character.
	char first_day[11];   // Used to store and then display the first day.
	char last_day[11];   // // Used to store and then display the last day.
	
	FILE *f;
	
	
	// This loop requires that you enter a date and city that are in the document.
	while((exits_city == 0) || (exits_day == 0)){
		l = 0;
		exits_city = 0;
		exits_day = 0;  
		aux='\0';
		count = 0;

		
		f = fopen("Meteologica_vacante_ProgramadorC_20200901_datos.CSV","r");
		if(f == NULL){
			printf("The file could not be opened.\n");
			exit(1);
		}
		
		fgets(temp, 200, f);   // We discard the first row that contains the description of the data with the empty function.
		
		// This loop goes through Excel, saving the values of the searched variables: 
		for(i=0; i < num_rows; i++){
			
			empty(temp);   // We discard the content of temp that contains the firts row.
			aux = '\0';   // Needed when aux is '\n'.
			for(j = 0; aux != '\n'; j++){
				
				aux = '\0';   // Needed when aux is ';'.
				for(k = 0; (aux !=';') && (aux != '\n'); k++){
					
					aux = fgetc(f);   // Extract data from file character by character
					if ((aux != ';')&&(aux != '\n')){
						temp[k] = aux;   // As long as we don't have a';' or a '\n' we fill temp character by character.
					}
				}
				// Act on the first column (dates). We temporarily copy the date and save the first dy and the last day.
				if(j == 0){
					remove_space(temp);
					if(temp[4] == '/'){
					strncpy(date_temp,flip_date(temp),10);
					}else{
						strcpy(date_temp,temp);
					}
					if(i == 0){
						strcpy(first_day,temp);   // Save the first day.
					}else if(i == (num_rows-1)){
						strcpy(last_day, temp);   // save the last day
					}
					if(!strcmpi(date_temp, date)){
						exits_day = 1;   // Informs that the date entered is correct
					}
					empty(temp);   // Once the temp value is saved, we need to initialize the variable so there is no bad data.
				
				// Act on the second column (dates). We temporarily copy the city and change the letters of Gijon.
				}else if(j == 1){  
					remove_space(temp);
					// We do it inside this else if and not the change_Gijon function, so as not to have to call the function constantly:
					if((temp[0]=='G'||temp[0]=='g')&&(temp[1]=='I'||temp[1]=='i')&&(temp[2]=='J'||temp[2]=='j')){ 
						change_Gijon(temp);
						empty(name_city_temp);
					}
					strcpy(name_city_temp,temp);   // Save the name of the city of the row.
					if(!strcmpi(name_city_temp, name_city)){
						exits_city = 1;   // It is used to inform the user of an error in the city he has entered.
					}
					empty(temp);   // Once the temp value is saved, we need to initialize the variable so there is no bad data.
					
				}else if((j==2) && (!strcmpi(date, date_temp) && (!strcmpi(name_city, name_city_temp)))){
					// We allocate a memory space only when the first city variable of type city_Attributes is saved.
					if (l == 0){
						// Memory is reserved for the vector where we are going to store the data
						city = (city_Attributes*)malloc(sizeof(city_Attributes));
						if(city == NULL){
							printf("Memory could not be reserved. \n");
							exit(1);
						}
					}
					if(count == 0){
						copy(temp, l, j); // This function copies in the memory assigned for the vectors of the cities(Madrid, Sevilla..), the temperature.
					}
					count = 1;   // Indicates that the date and city entered exist.
					empty(temp);   // Once the temp value is saved, we need to initialize the variable so there is no bad data.
					
				}else if((count != 0) && (!strcmpi(name_city, name_city_temp)) && (j < 5)){
					copy(date_temp, l, 0);   // Copy the data into the variable of 'city[l].date'.
					copy(name_city_temp, l, 1);   // Copy the data into the variable of 'city[l].name'.
					copy(temp, l, j);   // Copy the data into the appropiate variable of 'city_Attributes'.
					empty(temp);   // Once the temp value is saved, we need to initialize the variable so there is no bad data.
					// The vector 'city' is dimensioned each time a different day is saved.
					city = (city_Attributes*)realloc(city,(l+2)*sizeof(city_Attributes));
					
				// his 'else if' is used to stop searching and save data when changing from the city searched to the next in the list
				}else if((strcmpi(name_city, name_city_temp)) && (count != 0)){ 
					// with this branch we improve efficiency by escaping from the function at the moment we change from our city to another not wanted.
					fclose(f);
					return l;
				}	
				empty(temp);
			}
			if(count == 1){
				// Take on screen
				printf("Date: %s   City: %s   Max_Temp= %f   Min_Temp= %f   Precipitation= %f\n", city[l].date, city[l].name,  city[l].max_Temp, city[l].min_Temp, city[l].precipitation);
				l++;
			}
		}
		// The file that has CSV format closes
		fclose(f);
		if((count == 0) && (exits_city == 0) && (exits_day == 1)){
			printf("\n\nThe city entered is not in the record.You must enter a city that exits.\n");
			fflush(stdin); // Remove the intro-letter from the buffer.
			fgets(name_city,20,stdin);
			change2(name_city);
			
		}else if((count == 0) && (exits_day == 0) && (exits_city == 1)){
			printf("\n\nThe date entered is not in the record.\n");
			printf("You must enter a date between the days: %s  -  %s.\n",first_day ,last_day);
			fflush(stdin); // Remove the intro-letter from the buffer.
			fgets(date,11,stdin);
			change2(date);
			
		}else if((exits_city != 1) && (exits_day != 1)){
			printf("\n\nThe city entered is not in the record.You must enter a city that exits.\n");
			fflush(stdin); // Remove the intro-letter from the buffer.
			fgets(name_city,20,stdin);
			change2(name_city);
			printf("\n\nThe date entered is not in the record.\n");
			printf("You must enter a date between the days: %s  -  %s.\n",first_day ,last_day);
			fflush(stdin); // Remove the intro-letter from the buffer.
			fgets(date,11,stdin);
			change2(date);
			
		}else if(count == 1){
			return l;
		}
	}
	free(city);   // We free the reserved memory to save resources.
}



// function that outputs the number of days and the number of rows of the file:
int count_rows(){ 
	
	// we declare variables.
	FILE *f;
	char temp[200];
	int number_rows;
	// the file pointer is assigned.
	f = fopen("Meteologica_vacante_ProgramadorC_20200901_datos.CSV","r");
	if(f == NULL){
		printf("The file could not be opened.\n");
		exit(1);
	}
	
	// count the number of rows.
	number_rows = 0;
	while(!feof(f)){
		fgets(temp, 200, f);
		number_rows++;
	}
	
	// The file that has csv format closes:
	fclose(f);
	number_rows = number_rows - 2;   // because the loop counts the first row and the last line break
	return number_rows;
}




// This function allows you to change the ',' to the '.' in numeric data, to be able to convert it later from a string to a double:
void change(char data[]){
	int i, aux = 0;
	int N = strlen(data) ;   // Stores the length of the string so as not to do iterations in vain.
	
	// This loop changes ',' to a '.'
	for( i = 0; i < N && aux == 0; i++){
		if(data[i] == ','){
			data[i] = '.';
			aux = 1;   // Makes the loop finish executing when we reach the comma to improve efficiency.
		}
	}
}



// This function allows you to change the '\n' to the '\0' in numeric data, for data coming from a fgets:
void change2(char data[]){
	int i;
	
	// This loop changes '\n' to a '\0'
	for( i = 0; i < 11 ; i++){
		if(data[i] == '\n'){
			data[i] = '\0';
		}
	}
}



// This function removes the white space from the data beginning with this:
void remove_space(char data[]){
	int i;
	int N = strlen(data);
	
	// Algorithm for date change:
	while(data[0] == ' '){
		for(i = 0; i < N; i++){
				
			data[i] = data[i+1];
		}
	}
}




void change_Gijon(char data[]){
	int i;
	int N = strlen(data);

	// Algorithm for date change:
	for(i = 0; i < N; i++){
		empty(data);
		data[0]='G';
		data[1]='i';
		data[2]='j';
		data[3]='o';
		data[4]='n';
	}
}




// This function clears the character string 'temp':
void empty(char temp[]){
	int i;
	
	for(i = 0; i < 50; i++){
		temp[i] = '\0';
	}
}



// This function copies in the memory assigned for the vectors of the cities(Madrid, Sevilla..), the data such as date, city, temperature ...
void copy(char temp[], int l, int k){
	int N = strlen(temp) ;

		// With this switch we store the values in the variables with their corresponding type:
		switch(k){
				case 0 :{
					city[l].date = (char*)malloc(N*sizeof(char));   // Memory is reserved for the variable.
					if(city[l].date == NULL){
						printf("Memory could not be reserved.\n");
						exit(1);
					}
					if(temp[4] == '/'){
						strcpy( city[l].date, flip_date(temp));	 // We correct the dates saved as: yyyy / mm / dd, and we save the data.
					}else{
						strcpy(	city[l].date, temp);
					}
					break;
				}
				
				case 1 :{
					city[l].name = (char*)malloc(N*sizeof(char));
					if(city[l].name == NULL){
						printf("Memory could not be reserved.\n");
						exit(1);
						
					}
					strcpy( city[l].name, temp);   // Save the data.
					break;
				}
				
				case 2 :{
					change(temp);
					city[l].max_Temp = atof(temp);   // The string of numbers is changed by data type float and save the data.
					break;
				}
				
				case 3 :{
					change(temp);
					city[l].min_Temp = atof(temp);   
					break;
				}
				
				case 4 :{
					change(temp);
					city[l].precipitation = atof(temp);  
					break;
				}
				
				case 5 :{
				//	city[l].cloudiness = atoi(temp);   // The string of numbers is changed by data type int and save the data.
					break;
				}
		}
}
	


// This function changes the order of the file dates that are saved as: yyyy / mm / dd:
char* flip_date(char date[]){
	int i;
	int N = strlen(date);
	char* aux=(char*)malloc(N*sizeof(char));   // Memory is reserved for the variable.
	
	// Algorithm for date change:
	for(i = 0; i < N; i++){
		if(i < 2){
			aux[i] = date[i+8];
		}else if((i > 1) && (i < 6)){
			aux[i] = date[i+2];
		}else{
			aux[i] = date[i-6];
		}
	}
	return aux; 
}



double Cel_to_Fahren(double *temperature){
	*temperature = *temperature*1.8+32;
	return *temperature;
}



// This function stores the required information in JSON format:
void store_in_JSON( int num_days ,char name_of_city[]){
	
	FILE* JSON_file;
    char d_quotes = '"';
    char d_points = ':';
    char degrees[50];
	int temperature;
	int i = 0;
	
	
	name_of_city = strcat(name_of_city,".JSON");
	
	printf("\n\nEnter in which units you want the temperature: Fahrenheit or Celsius.\n");
	fflush(stdin); // Remove the intro-letter from the buffer.
	do{
		scanf("%s", &degrees);
		if((strcmpi(degrees, "Celsius"))&&(strcmpi(degrees, "Fahrenheit"))){
			printf("you must enter one of two options: Fahrenheit or Celsius.\n");
		}
	}while((strcmpi(degrees, "Celsius"))&&(strcmpi(degrees, "Fahrenheit")));
	
		
	
		// The following lines of code write to the file in JSON format:
		JSON_file = fopen(name_of_city, "w+t");
		fputs("{\n", JSON_file);
		fprintf(JSON_file,"  %c%s%c:[\n", d_quotes, city[i].name, d_quotes);
		for(i = 1; i < num_days; i++){
		    fputs("   {\n", JSON_file);
			fprintf(JSON_file,"    %cDate%c: %c%s%c,\n", d_quotes, d_quotes, d_quotes, city[i].date, d_quotes);
			
		    if(!strcmpi(degrees,"Celsius")){
		    	fprintf(JSON_file,"    %cmax_temp%c: %f,\n", d_quotes, d_quotes, city[i].max_Temp);
				fprintf(JSON_file,"    %cmin_temp%c: %f,\n", d_quotes, d_quotes, city[i].min_Temp);
				
			}else{
				fprintf(JSON_file,"    %cmax_temp%c: %f,\n", d_quotes, d_quotes, Cel_to_Fahren(&city[i].max_Temp));
				fprintf(JSON_file,"    %cmax_temp%c: %f,\n", d_quotes, d_quotes, Cel_to_Fahren(&city[i].min_Temp));
			}
			
		    fprintf(JSON_file,"    %cprecipitation%c: %f\n", d_quotes, d_quotes, city[i].precipitation);
		    //fprintf(JSON_file,"    %ccloudiness%c: %i\n", d_quotes, d_quotes, city[i].cloudiness);
		    
		    if(i<(num_days-1)){
		    	fputs("   },\n", JSON_file);
		    	
			}else{
				fputs("   }\n", JSON_file);
			}
			
		}
		fputs("  ]\n", JSON_file);
		fputs("}\n", JSON_file);
	    fclose(JSON_file);
}

