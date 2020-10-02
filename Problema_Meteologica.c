#include <stdio.h>
#include <stdlib.h>
#include <string.h>


typedef struct{
	char *name;
	char *date;
	double max_Temp;
	double min_Temp;
	double precipitation;
	int cloudiness;
}city_Attributes;


typedef struct{
	city_Attributes *Madrid;
	city_Attributes *Sevilla;
	city_Attributes *Gijon;
	city_Attributes *Valencia;
}cities;

cities city;

void count_days(int num_of_cities, int *number_days, int *number_rows);
void reserv(int num_of_days, city_Attributes *Madrid);
void reserv2(int num_of_days, city_Attributes *Sevilla);
void reserv3(int num_of_days, city_Attributes *Gijon);
void reserv4(int num_of_days, city_Attributes *Valencia);
void change(char data[]);
void change2(char data[]);
void empty(char temp[]);
void copy(char temp[], int i, int j);
char* flip_date(char date[]);
void srch_str_data(char name_of_city[], char date[], int number_of_days);
void store_in_JSON(int position_of_day, int number_of_days ,char name_of_city[]);

int main(){
	
	int num_columns = 6; 
	int num_rows;  // We initialize with the number of columns in the file.
	int num_cities = 4;   // We initialize the variable with the number of cities in the file.
	int num_days;   // This variable is used to count the days of each city.
	int i, j, k;
	char temp[200];   // We use a variable temporarily to store the data character by character.
	char aux;
	char name_city[20];
	char date[11];
	//city_Attributes Madrid;
	
	
	count_days(num_cities, &num_days, &num_rows);
	reserv(num_days, city.Madrid);
	reserv2(num_days, city.Sevilla);
	reserv3(num_days, city.Gijon);
	reserv4(num_days, city.Valencia);
	
	// The file that has CSV format opens:
	FILE *f;
	f = fopen("Meteologica_vacante_ProgramadorC_20200901_datos.CSV","r");
	fgets(temp, 200, f);   // We discard the first row that contains the description of the data with the empty function.
	
	// This loop goes through Excel, saving the values of the variables within each city: 
	for(i=0; i < (num_days*num_cities); i++){
		empty(temp);   // We discard the content of temp that contains the firts row.
		
		// The following loop makes it possible to move through the columns of the file:
		for(j = 0; j < num_columns; j++){
			aux = '\0';
			
			// With the following loop we can save the data of each column character by character in the variable 'temp':
			for(k = 0; (aux !=';') && (aux != '\n'); k++){
				aux = fgetc(f);
				if ((aux != ';')&&(aux != '\n')){
					temp[k] = aux;
					copy(temp, i, j);   // Copy the data into the appropiate variable of 'city_Attributes'.
				}
			}
			empty(temp);   // We clean the variable temp so as not to overwrite the data in the next iteration.
		}
		
		
		/*// Take on screen:
		if((i/31)==0){
			printf("Date: %s   City: %s   Max_Temp= %f   Min_Temp= %f   Precipitation= %f   Cloudiness= %i \n", city.Madrid[i%31].date, city.Madrid[i].name,  city.Madrid[i].max_Temp, city.Madrid[i].min_Temp, city.Madrid[i].precipitation, city.Madrid[i%31].cloudiness);
		}else if((i/31)==1){
			printf("Date: %s   City: %s   Max_Temp= %f   Min_Temp= %f   Precipitation= %f   Cloudiness= %i \n", city.Sevilla[i%31].date, city.Sevilla[i%31].name,  city.Sevilla[i%31].max_Temp, city.Sevilla[i%31].min_Temp, city.Sevilla[i%31].precipitation, city.Sevilla[i%31].cloudiness);
		}else if((i/31)==2){
			printf("Date: %s   City: %s   Max_Temp= %f   Min_Temp= %f   Precipitation= %f   Cloudiness= %i \n", city.Gijon[i%31].date, city.Gijon[i%31].name,  city.Gijon[i%31].max_Temp, city.Gijon[i%31].min_Temp, city.Gijon[i%31].precipitation, city.Gijon[i%31].cloudiness);
		}else if((i/31)==3){
			printf("Date: %s   City: %s   Max_Temp= %f   Min_Temp= %f   Precipitation= %f   Cloudiness= %i \n", city.Valencia[i%31].date, city.Valencia[i%31].name,  city.Valencia[i%31].max_Temp, city.Valencia[i%31].min_Temp, city.Valencia[i%31].precipitation, city.Valencia[i%31].cloudiness);
		}*/	
	}
	
	// The file that has CSV format closes
	fclose(f);
	
	
	do{
		printf("Enter the name of the city you would like to consult (Madrid, Sevilla, Gijon, Valencia):\n");
		fflush(stdin); // Remove the intro-letter from the buffer.
		scanf("%s", &name_city);
		if(strcmpi(name_city, "Madrid") && strcmpi(name_city, "Sevilla") && strcmpi(name_city, "Gijon") && strcmpi(name_city, "Valencia")){
			printf("\nEnter a name that is on the list. ");
		}
	}while (strcmpi(name_city, "Madrid") && strcmpi(name_city, "Sevilla") && strcmpi(name_city, "Gijon") && strcmpi(name_city, "Valencia"));
	
	printf("\nEnter the date you would like to consult (ej:dd/mm/yyyy):\n");
	fflush(stdin); // Remove the intro-letter from the buffer.
	scanf("%s", &date);
	
	srch_str_data(name_city, date, num_days);
	
	
	system("pause");
	return 0;
	
}



// function that outputs the number of days and the number of rows of the file:
void count_days(int num_of_cities, int *number_days, int *number_rows){ 
	
	// we declare variables.
	FILE *f;
	char temp[200];

	// the file pointer is assigned.
	f = fopen("Meteologica_vacante_ProgramadorC_20200901_datos.CSV","r");
	if(f == NULL){
		printf("The file could not be opened.\n");
		exit(1);
	}
	
	// count the number of days.
	*number_rows = 0;
	while(!feof(f)){
		fgets(temp, 200, f);
		*number_rows = *number_rows + 1;
	}
	
	// The file that has csv format closes:
	fclose(f);
	*number_rows = *number_rows - 1;   // because the loop counts the first row and the last line break
	*number_days = *number_rows / num_of_cities;
}



// Function that reserves memory for each day of vector Madrid:
void reserv(int num_of_days, city_Attributes *Madrid){
	city.Madrid = (city_Attributes*)malloc(num_of_days*sizeof(city_Attributes));
	
	if(city.Madrid == NULL){
		printf("Memory could not be reserved. \n");
		exit(1);
	}
}



// Function that reserves memory for each day of vector Sevilla.
void reserv2(int num_of_days, city_Attributes *Sevilla){
	city.Sevilla = (city_Attributes*)malloc((num_of_days)*sizeof(city_Attributes));
	//	printf("city_Attributes ocupa: %d.\n", (1)*sizeof(city_Attributes));
	//printf("numero de dias: %i.\n", num_of_days);
	if(city.Sevilla == NULL){
		printf("Memory could not be reserved. \n");
		exit(1);
	}
}



// Function that reserves memory for each day of vector Gijon.
void reserv3(int num_of_days, city_Attributes *Gijon){
	city.Gijon = (city_Attributes*)malloc((num_of_days)*sizeof(city_Attributes));
	//	printf("city_Attributes ocupa: %d.\n", (1)*sizeof(city_Attributes));
	//printf("numero de dias: %i.\n", num_of_days);
	if(city.Gijon == NULL){
		printf("Memory could not be reserved. \n");
		exit(1);
	}
}



// Function that reserves memory for each day of vector Valencia.
void reserv4(int num_of_days, city_Attributes *Valencia){
	city.Valencia = (city_Attributes*)malloc((num_of_days)*sizeof(city_Attributes));
	if(city.Valencia == NULL){
		printf("Memory could not be reserved. \n");
		exit(1);
	}
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
	
	// This loop changes ',' to a '.'
	for( i = 0; i < 11 ; i++){
		if(data[i] == '\n'){
			data[i] = '\0';
		}
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
void copy(char temp[], int i, int j){
	int N = strlen(temp) ;
	
	if((i/31)==0){
		// With this switch we store the values in the variables with their corresponding type:
		switch(j){
				case 0 :{
					city.Madrid[i%31].date = (char*)malloc(N*sizeof(char));
					if(city.Madrid[i%31].date == NULL){
						printf("Memory could not be reserved.\n");
						exit(1);
					}
					// We correct the dates saved as: yyyy / mm / dd, and we save the data.
					if(temp[4] == '/'){
						strcpy( city.Madrid[i%31].date, flip_date(temp));
					}else{
						strcpy(	city.Madrid[i%31].date, temp);
					}
					
					break;
				}
				
				case 1 :{
					city.Madrid[i%31].name = (char*)malloc(N*sizeof(char));
					if(city.Madrid[i%31].name == NULL){
						printf("Memory could not be reserved.\n");
						exit(1);
					}
					strcpy(	city.Madrid[i%31].name, temp);
					break;
				}
				
				case 2 :{
					change(temp);
					city.Madrid[i%31].max_Temp = atof(temp);
					break;
				}
				
				case 3 :{
					change(temp);
					city.Madrid[i%31].min_Temp = atof(temp);
					break;
				}
				
				case 4 :{
					change(temp);
					city.Madrid[i%31].precipitation = atof(temp);
					break;
				}
				
				case 5 :{
					change(temp);
					city.Madrid[i%31].cloudiness = atoi(temp);
					break;
				}
		}
	}else if((i/31) == 1){
		// With this switch we store the values in the variables with their corresponding type:
		switch(j){
				case 0 :{
					city.Sevilla[i%31].date = (char*)malloc(N*sizeof(char));
					if(city.Sevilla[i%31].date == NULL){
						printf("Memory could not be reserved.\n");
						exit(1);
					}
					// We correct the dates saved as: yyyy / mm / dd, and we save the data.
					if(temp[4] == '/'){
						strcpy( city.Sevilla[i%31].date, flip_date(temp));
					}else{
						strcpy(	city.Sevilla[i%31].date, temp);
					}
					break;
				}
				
				case 1 :{
					city.Sevilla[i%31].name = (char*)malloc(N*sizeof(char));
					if(city.Sevilla[i%31].name == NULL){
						printf("Memory could not be reserved.\n");
						exit(1);
					}
					strcpy(	city.Sevilla[i%31].name, temp);
					break;
				}
				
				case 2 :{
					change(temp);
					city.Sevilla[i%31].max_Temp = atof(temp);
					break;
				}
				
				case 3 :{
					change(temp);
					city.Sevilla[i%31].min_Temp = atof(temp);
					break;
				}
				
				case 4 :{
					change(temp);
					city.Sevilla[i%31].precipitation = atof(temp);
					break;
				}
				
				case 5 :{
					change(temp);
					city.Sevilla[i%31].cloudiness = atoi(temp);
					break;
				}
		}
	}else if((i/31) == 2){
		// With this switch we store the values in the variables with their corresponding type:
		switch(j){
				case 0 :{
					city.Gijon[i%31].date = (char*)malloc(N*sizeof(char));
					if(city.Gijon[i%31].date == NULL){
						printf("Memory could not be reserved.\n");
						exit(1);
					}
					// We correct the dates saved as: yyyy / mm / dd, and we save the data.
					if(temp[4] == '/'){
						strcpy( city.Gijon[i%31].date, flip_date(temp));
					}else{
						strcpy(	city.Gijon[i%31].date, temp);
					}
					break;
				}
				
				case 1 :{
					city.Gijon[i%31].name = (char*)malloc(N*sizeof(char));
					if(city.Gijon[i%31].name == NULL){
						printf("Memory could not be reserved.\n");
						exit(1);
					}
					strcpy(	city.Gijon[i%31].name, "Gijon");
					break;
				}
				
				case 2 :{
					change(temp);
					city.Gijon[i%31].max_Temp = atof(temp);
					break;
				}
				
				case 3 :{
					change(temp);
					city.Gijon[i%31].min_Temp = atof(temp);
					break;
				}
				
				case 4 :{
					change(temp);
					city.Gijon[i%31].precipitation = atof(temp);
					break;
				}
				
				case 5 :{
					change(temp);
					city.Gijon[i%31].cloudiness = atoi(temp);
					break;
				}
		}
	}else if((i/31) == 3){
		// With this switch we store the values in the variables with their corresponding type:
		switch(j){
				case 0 :{
					city.Valencia[i%31].date = (char*)malloc(N*sizeof(char));
					if(city.Valencia[i%31].date == NULL){
						printf("Memory could not be reserved.\n");
						exit(1);
					}
					// We correct the dates saved as: yyyy / mm / dd, and we save the data.
					if(temp[4] == '/'){
						strcpy( city.Valencia[i%31].date, flip_date(temp));
					}else{
						strcpy(	city.Valencia[i%31].date, temp);
					}
					break;
				}
				
				case 1 :{
					city.Valencia[i%31].name = (char*)malloc(N*sizeof(char));
					if(city.Valencia[i%31].name == NULL){
						printf("Memory could not be reserved.\n");
						exit(1);
					}
					strcpy(	city.Valencia[i%31].name, temp);
					break;
				}
				
				case 2 :{
					change(temp);
					city.Valencia[i%31].max_Temp = atof(temp);
					break;
				}
				
				case 3 :{
					change(temp);
					city.Valencia[i%31].min_Temp = atof(temp);
					break;
				}
				
				case 4 :{
					change(temp);
					city.Valencia[i%31].precipitation = atof(temp);
					break;
				}
				
				case 5 :{
					change(temp);
					city.Valencia[i%31].cloudiness = atoi(temp);
					break;
				}
		}
	}
}



// This function changes the order of the file dates that are saved as: yyyy / mm / dd:
char* flip_date(char date[]){
	int i;
	char* aux=malloc(10);   // Memory is reserved for the variable.
	
	// Algorithm for date change:
	for(i = 0; i < 10; i++){
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



// This function searches for the requested day and saves the information for the following days:
void srch_str_data(char name_of_city[], char date[], int number_of_days){
	int i;
	int aux = 0;   // 'aux' is an escape route
	char answer[10];   // It is where the answer to 'exit' or enter a 'correct date' is saved.
	char degrees[11];
	
	if(!(strcmpi(name_of_city, "Madrid"))){
		// With this do-while loop we go through the stored dates and see if any match, otherwise it is suggested to introduce a true one or exit.
		do{
			i = 0;
			while((i < number_of_days) && (aux == 0)){
				if(!strcmpi(city.Madrid[i].date, date)){
					aux = 1;
				}
				i++;
			}
			
			if(aux == 0){
				printf("The date entered is not in the record.\n");
				printf("You must enter a date between the days: %s  -  %s. Or exit by typing 'exit'\n",city.Madrid[0].date ,city.Madrid[i-1].date);
				fflush(stdin); // Remove the intro-letter from the buffer.
				fgets(date,11,stdin);
				change2(date);
			}
		}while(strcmpi(date, "exit") && (aux==0));
		if(!strcmpi(date, "exit")){
			exit(-1);
		}
		store_in_JSON(i, number_of_days,name_of_city);
		
	}else if(!strcmpi(name_of_city, "Sevilla")){
		// With this do-while loop we go through the stored dates and see if any match, otherwise it is suggested to introduce a true one or exit.
		do{
			i = 0;
			while((i < number_of_days) && (aux == 0)){
				if(!strcmpi(city.Sevilla[i].date, date)){
					aux = 1;
				}
				i++;
			}
			
			if(aux == 0){
				printf("The date entered is not in the record.\n");
				printf("You must enter a date between the days: %s  -  %s. Or exit by typing 'exit'\n",city.Sevilla[0].date ,city.Sevilla[i-1].date);
				fflush(stdin); // Remove the intro-letter from the buffer.
				fgets(date,11,stdin);
				change2(date);
			}
		}while(strcmpi(date, "exit") && (aux==0));
		store_in_JSON(i, number_of_days,name_of_city);
		
	}else if(!strcmpi(name_of_city, "Gijon")){
		// With this do-while loop we go through the stored dates and see if any match, otherwise it is suggested to introduce a true one or exit.
		do{
			i = 0;
			while((i < number_of_days) && (aux == 0)){
				if(!strcmpi(city.Gijon[i].date, date)){
					aux = 1;
				}
				i++;
			}
			
			if(aux == 0){
				printf("The date entered is not in the record.\n");
				printf("You must enter a date between the days: %s  -  %s. Or exit by typing 'exit'\n",city.Gijon[0].date ,city.Gijon[i-1].date);
				fflush(stdin); // Remove the intro-letter from the buffer.
				fgets(date,11,stdin);
				change2(date);
			}
		}while(strcmpi(date, "exit") && (aux==0));
		store_in_JSON(i, number_of_days,name_of_city);
		
	}else if(!strcmpi(name_of_city, "Valencia")){
		// With this do-while loop we go through the stored dates and see if any match, otherwise it is suggested to introduce a true one or exit.
		do{
			i = 0;
			while((i < number_of_days) && (aux == 0)){
				if(!strcmpi(city.Valencia[i].date, date)){
					aux = 1;
				}
				i++;
			}
			
			if(aux == 0){
				printf("The date entered is not in the record.\n");
				printf("You must enter a date between the days: %s  -  %s. Or exit by typing 'exit'\n",city.Valencia[0].date ,city.Valencia[i-1].date);
				fflush(stdin); // Remove the intro-letter from the buffer.
				fgets(date,11,stdin);
				change2(date);
			}
		}while(strcmpi(date, "exit") && (aux==0));
		store_in_JSON(i, number_of_days,name_of_city);
	}
}



// This function stores the required information in JSON format:
void store_in_JSON(int position_of_day, int number_of_days ,char name_of_city[]){
	
	
	FILE* JSON_file;
    char d_quotes = '"';
    char d_points = ':';
	int temperature;
	int i = position_of_day;
	char degrees[11];
	
	printf("enter in which units you want the temperature: Fahrenheit or Celsius.\n");
	fflush(stdin); // Remove the intro-letter from the buffer.
	do{
		
		scanf("%s", &degrees);
		if((strcmpi(degrees, "Celsius"))&&(strcmpi(degrees, "Fahrenheit"))){
			printf("you must enter one of two options: Fahrenheit or Celsius.\n");
		}
	}while((strcmpi(degrees, "Celsius"))&&(strcmpi(degrees, "Fahrenheit")));
	
		
	
	if(!(strcmpi(name_of_city, "Madrid"))){
		JSON_file = fopen("Madrid.JSON", "w+t");
		fputs("{\n", JSON_file);
		fprintf(JSON_file,"  %c%s%c:[\n", d_quotes, city.Madrid[i].name, d_quotes);
		for(i = position_of_day; i < number_of_days; i++){
		    fputs("   {\n", JSON_file);
			fprintf(JSON_file,"    %cDate%c: %c%s%c,\n", d_quotes, d_quotes, d_quotes, city.Madrid[i].date, d_quotes);
		    if(!strcmpi(degrees,"Celsius")){
		    	fprintf(JSON_file,"    %cmax_temp%c: %f,\n", d_quotes, d_quotes, city.Madrid[i].max_Temp);
				fprintf(JSON_file,"    %cmin_temp%c: %f,\n", d_quotes, d_quotes, city.Madrid[i].min_Temp);
			}else{
				fprintf(JSON_file,"    %cmax_temp%c: %f,\n", d_quotes, d_quotes, city.Madrid[i].max_Temp*1.8+32);
				fprintf(JSON_file,"    %cmax_temp%c: %f,\n", d_quotes, d_quotes, city.Madrid[i].min_Temp*1.8+32);
			}
		    fprintf(JSON_file,"    %cprecipitation%c: %f,\n", d_quotes, d_quotes, city.Madrid[i].precipitation);
		    fprintf(JSON_file,"    %ccloudiness%c: %i\n", d_quotes, d_quotes, city.Madrid[i].cloudiness);
		    if(i<(number_of_days-1)){
		    	fputs("   },\n", JSON_file);
			}else{
				fputs("   }\n", JSON_file);
			}
		}
		fputs("  ]\n", JSON_file);
		fputs("}\n", JSON_file);
	    fclose(JSON_file);

	}else if(!(strcmpi(name_of_city, "Sevilla"))){
		JSON_file = fopen("Sevilla.JSON", "w+t");
		fputs("{\n", JSON_file);
		fprintf(JSON_file,"  %c%s%c:[\n", d_quotes, city.Sevilla[i].name, d_quotes);
		for(i = position_of_day; i < number_of_days; i++){
		    fputs("   {\n", JSON_file);
			fprintf(JSON_file,"    %cDate%c: %c%s%c,\n", d_quotes, d_quotes, d_quotes, city.Sevilla[i].date, d_quotes);
		    if(!strcmpi(degrees,"Celsius")){
		    	fprintf(JSON_file,"    %cmax_temp%c: %f,\n", d_quotes, d_quotes, city.Sevilla[i].max_Temp);
				fprintf(JSON_file,"    %cmin_temp%c: %f,\n", d_quotes, d_quotes, city.Sevilla[i].min_Temp);
			}else{
				fprintf(JSON_file,"    %cmax_temp%c: %f,\n", d_quotes, d_quotes, city.Sevilla[i].max_Temp*1.8+32);
				fprintf(JSON_file,"    %cmax_temp%c: %f,\n", d_quotes, d_quotes, city.Sevilla[i].min_Temp*1.8+32);
			}
		    fprintf(JSON_file,"    %cprecipitation%c: %f,\n", d_quotes, d_quotes, city.Sevilla[i].precipitation);
		    fprintf(JSON_file,"    %ccloudiness%c: %i\n", d_quotes, d_quotes, city.Sevilla[i].cloudiness);
		    if(i<(number_of_days-1)){
		    	fputs("   },\n", JSON_file);
			}else{
				fputs("   }\n", JSON_file);
			}
		}
		fputs("  ]\n", JSON_file);
		fputs("}\n", JSON_file);
	    fclose(JSON_file);
	}else if(!(strcmpi(name_of_city, "Gijon"))){
		JSON_file = fopen("Gijon.JSON", "w+t");
		fputs("{\n", JSON_file);
		fprintf(JSON_file,"  %c%s%c:[\n", d_quotes, city.Gijon[i].name, d_quotes);
		for(i = position_of_day; i < number_of_days; i++){
		    fputs("   {\n", JSON_file);
			fprintf(JSON_file,"    %cDate%c: %c%s%c,\n", d_quotes, d_quotes, d_quotes, city.Gijon[i].date, d_quotes);
		    if(!strcmpi(degrees,"Celsius")){
		    	fprintf(JSON_file,"    %cmax_temp%c: %f,\n", d_quotes, d_quotes, city.Gijon[i].max_Temp);
				fprintf(JSON_file,"    %cmin_temp%c: %f,\n", d_quotes, d_quotes, city.Gijon[i].min_Temp);
			}else{
				fprintf(JSON_file,"    %cmax_temp%c: %f,\n", d_quotes, d_quotes, city.Gijon[i].max_Temp*1.8+32);
				fprintf(JSON_file,"    %cmax_temp%c: %f,\n", d_quotes, d_quotes, city.Gijon[i].min_Temp*1.8+32);
			}
		    fprintf(JSON_file,"    %cprecipitation%c: %f,\n", d_quotes, d_quotes, city.Gijon[i].precipitation);
		    fprintf(JSON_file,"    %ccloudiness%c: %i\n", d_quotes, d_quotes, city.Gijon[i].cloudiness);
		    if(i<(number_of_days-1)){
		    	fputs("   },\n", JSON_file);
			}else{
				fputs("   }\n", JSON_file);
			}
		}
		fputs("  ]\n", JSON_file);
		fputs("}\n", JSON_file);
	    fclose(JSON_file);
	}else if(!(strcmpi(name_of_city, "Valencia"))){
		JSON_file = fopen("Valencia.JSON", "w+t");
		fputs("{\n", JSON_file);
		fprintf(JSON_file,"  %c%s%c:[\n", d_quotes, city.Valencia[i].name, d_quotes);
		for(i = position_of_day; i < number_of_days; i++){
		    fputs("   {\n", JSON_file);
			fprintf(JSON_file,"    %cDate%c: %c%s%c,\n", d_quotes, d_quotes, d_quotes, city.Valencia[i].date, d_quotes);
		    if(!strcmpi(degrees,"Celsius")){
		    	fprintf(JSON_file,"    %cmax_temp%c: %f,\n", d_quotes, d_quotes, city.Valencia[i].max_Temp);
				fprintf(JSON_file,"    %cmin_temp%c: %f,\n", d_quotes, d_quotes, city.Valencia[i].min_Temp);
			}else{
				fprintf(JSON_file,"    %cmax_temp%c: %f,\n", d_quotes, d_quotes, city.Valencia[i].max_Temp*1.8+32);
				fprintf(JSON_file,"    %cmax_temp%c: %f,\n", d_quotes, d_quotes, city.Valencia[i].min_Temp*1.8+32);
			}
		    fprintf(JSON_file,"    %cprecipitation%c: %f,\n", d_quotes, d_quotes, city.Valencia[i].precipitation);
		    fprintf(JSON_file,"    %ccloudiness%c: %i\n", d_quotes, d_quotes, city.Valencia[i].cloudiness);
		    if(i<(number_of_days-1)){
		    	fputs("   },\n", JSON_file);
			}else{
				fputs("   }\n", JSON_file);
			}
		}
		fputs("  ]\n", JSON_file);
		fputs("}\n", JSON_file);
	    fclose(JSON_file);
	}
}

