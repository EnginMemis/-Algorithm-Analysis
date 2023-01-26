#include<stdio.h>
#include<stdlib.h>
#include<string.h>


typedef struct number{					//NODE'UN BAGLI OLDUGU NODE'LARI TUTAN STRUCT
	int element;
	struct number *next;
}NUMBER;


typedef struct node{						//NODE YAPISI
	char name[30];
	char surname[30];
	int nodeNumber;
	int inDegree;								//SON HALDEKI IN-DEGREE SAYILARI
	int originalInDegree;						//BASLANGICTAKI IN-DEGREE SAYILARI
	int total;									//TOTAL CONNECTIONS SAYISI
	int flag;									//IN-DEGREE SAYISINA GORE NODE'UN ELENIP ELENMEDIGINI TUTAN FLAG
	struct number *next;
}NODE;


void inDegreeCalculate(NODE **list, int count, int M);
void deleteNodes(NODE **list, int count, int M);

NODE *createNode(){											//NODE OLUSTURAN FONKSIYON
	NODE *temp;
	temp = (NODE *)malloc(sizeof(NODE));
	temp->next = NULL;
	temp->inDegree = 0;
	temp->originalInDegree = 0;
	temp->total = 0;
	temp->flag = 1;
	return temp;
}

NUMBER *createNumber(){											//NODE OLUSTURAN FONKSIYON
	NUMBER *temp;
	temp = (NUMBER *)malloc(sizeof(NUMBER));
	temp->next = NULL;
	return temp;
} 

void addNumber(NODE **list, int index, NUMBER *number){						//NODE'UN BAGLI OLDUGU NODE'LARI TUTAN LINKLI LISTEYE ELEMAN EKLEYEN FONKSIYON
	NUMBER *iter = list[index]->next;
	if(iter == NULL)
		list[index]->next = number;
	else{
		while(iter->next != NULL){
			iter = iter->next;
		}
		iter->next = number;
	}
}

NODE **readFile(NODE **list, int *count){									//DOSYADAN VERILERI OKUYAN FONKSIYON
	FILE *file;
	NODE *temp, *iter;
	NUMBER *number;
	int i, j;
	char string[100];
	char sayi[10];
	char name[50];
	int index, flag;

	
	if((file = fopen("socialNet.txt", "r")) != NULL){
		while(!feof(file)){
			if(list == NULL){
				list = (NODE **)malloc(sizeof(NODE*));										//LIST EN BASTA NULL ISE MALLOC YAPILIYOR
			}
			else{
				list = (NODE **)realloc(list, (*count + 1) * sizeof(NODE*));				//EGER NULL DEGIL ISE REALLOC ILE YENI YER ACILIYOR
			}
			temp = createNode();															//EKLENILECEK NODE OLUSTURULUYOR
			
			fgets(string, 99, file);														//DOSYADAN SATIRI OKUYOR
			j = 0;
			index = 0;												
			for(j = 0; string[j] != ','; ++j){												//, GELENE KADAR CHAR OLARAK DIZIYE ATIP
				sayi[index] = string[j];
				index++;
			}
			temp->nodeNumber = atoi(sayi);													//INTEGER HALE GETIRILIYOR
			strcpy(sayi, "");
			index = 0;
			for(j = j + 1; string[j] != ','; ++j){											//, GELENE KADAR NAME DIZISINE ATIP
				name[index] = string[j];
				index++;
			}
			name[index] = '\0';
			strcpy(temp->name, name);														//TEMP NODE'UNUN NAME ALANINA KAYDEDILIYOR
			strcpy(name, "");
			index = 0;
			for(j = j + 1; string[j] != '\n'; ++j){											//\n KARAKTERINE KADAR OKUNUP
				name[index] = string[j];
				index++;
			}
			name[index] = '\0';
			strcpy(temp->surname, name);													//TEMP NODE'UNUN SURNAME ALANINA KAYDEDILIYOR
			strcpy(name, "");
			index = 0;
			
			
			list[*count] = temp;															//TEMP NODE'U LIST DIZISININ ILGILI YERINE KAYDEDILIYOR
			(*count)++;																		//ELEMAN SAYISI ARTTIRILIYOR
			i = 0;
			fgets(string, 99, file);														//BAGLI OLDUGU NODE SATIRI OKUNUYOR
			if(string[0] != ','){															//EGER SATIRIN ILK ELEMANI , ISE HICBIR NODE'A GITMIYOR DEMEK
				for(j = 0; j < strlen(string); ++j){
					if(string[j] != ',' && string[j] != EOF){								//, YA DA EOF OLANA KADAR SAYIYI OKUYUP
						sayi[i] = string[j];
						i++;	
					}
					else{
						number = createNumber();
						sayi[i] = '\0';
						number->element = atoi(sayi);										//OLUSTURULAN NUMBER NODE'UNA KAYDEDIP
						addNumber(list, (*count) - 1, number);								//LISTEYE EKLENIYOR
						i = 0;
						strcpy(sayi, "");
					}
				}
			}
		}
		fclose(file);
		return list;
	}
	else{
		printf("File Error!");													//DOSYA ACILAMADIYSA VERILEN UYARI
		return NULL;
	}
		
}

void display(NODE **list, int count, int mode){										//SECILEN MODE'A GORE BILGILERIN EKRANA YAZDIRILMASINI SAGLAYAN FONKSIYON
	int i;
	NUMBER *iter;
	if(mode == 1){
		printf("\t\t\tNODE ELENDIKTEN SONRA\n\n");
		for(i = 0; i < count; ++i){
			if(list[i]->flag == 1){
				printf("Node:%d   Flag: %d   In-Degree: %d  Toplam: %d  -> ", list[i]->nodeNumber, list[i]->flag, list[i]->inDegree, list[i]->total);
				iter = list[i]->next;
				while(iter != NULL){								//LINKLI LISTENIN SONUNA KADAR ELEMANLARIN YAZDIRILMASI
					printf("%d ", iter->element);
					iter = iter->next;
				}
				printf("\n");
			}
		}	
	}
		else{
			printf("\t\t\tBASLANGIC DURUMU\n\n");
			for(i = 0; i < count; ++i){
				printf("Node:%d   Flag: %d   Originial In-Degree: %d  Toplam: %d  -> ", list[i]->nodeNumber, list[i]->flag, list[i]->originalInDegree, list[i]->total);
				iter = list[i]->next;
				while(iter != NULL){
					printf("%d ", iter->element);
					iter = iter->next;
				}
				printf("\n");
			}
		}
	printf("\n\n\n");
}

void originalInDegree(NODE **list, int count){										//HICBIR ELEME OLMADAN ONCE EN BASTA ORIGINAL IN-DEGREE DEGERLERINI HESAPLAYAN FONKSIYON
	int i;
	NUMBER *iter;

	for(i = 0; i < count; ++i){
		iter = list[i]->next;
		while(iter != NULL){
			list[iter->element - 1]->originalInDegree++;	
			iter = iter->next;
		}
	}	
}

void inDegreeCalculate(NODE **list, int count, int M){										//ELEME ISLEMLERI OLACAK SEKILDE IN-DEGREE DEGERLERINI HESAPLAYAN FONKSIYON
	int i;
	NUMBER *iter;
	
	for(i = 0; i < count; ++i){
		list[i]->inDegree = 0;
	}
	
	for(i = 0; i < count; ++i){
		iter = list[i]->next;
		while(iter != NULL){
			if(	list[i]->flag == 1){
				list[iter->element - 1]->inDegree++;								//NODE'LARIN IN-DEGREE DEGERLERI HESAPLANIYOR
			}	
			iter = iter->next;
		}
	}	
	deleteNodes(list, count, M);														//IN-DEGREE DEGERINE GORE NODE FLAGLARINI DEGISTECEK FONKSIYON CAGRILIYOR
}

void deleteNodes(NODE **list, int count, int M){											//IN-DEGREE DEGERINE GORE FLAG DEGERLERINI DEGISTIREN FONKSIYON
	int i;
	int flag = 0;
	for(i = 0; i < count; ++i){
		if(list[i]->inDegree < M && list[i]->flag == 1){							//VERILEN M DEGERINE GORE ELENEN BIR NODE VAR ISE TUTULAN FLAG DEGERI 1 YAPILIYOR
			list[i]->flag = 0;
			flag = 1;
		}
	}
	if(flag == 1)																	//EGER BIR NODE ELENDIYSE YENI IN-DEGREE DEGERLERININ HESAPLANMASI ICIN inDegreeCalculate FONKSIYONU TEKRAR CAGRILIYOR
		inDegreeCalculate(list, count, M);
}

void totalCalculate(NODE **list, int nodeNumber, int visited[]){					//NODE'LARIN TOPLAM CONNECTIONS SAYILARINI DFS KULLANARAK HESAPLAYAN FONKSIYON
	NUMBER *iter = list[nodeNumber - 1]->next;
	if(list[nodeNumber - 1]->flag == 1){											//EGER NODE ELENMEDIYSE
		while(iter != NULL){
			if(list[iter->element - 1]->flag == 1){									//VE BAGLI OLDUGU NODE ELENMEMISSE
				if(visited[iter->element] == 0){									//BAGLI OLDUGU NODE'A DAHA ONCE GIDILMEMISSE 
					list[iter->element - 1]->total++;								//TOTAL DEGERI ARTTIRILIP
					visited[iter->element] = 1;										//NODE GIDILMIS OLARAK DEGISTIRILIYOR
					totalCalculate(list, iter->element, visited);					//RECURSIVE OLARAK FONKSIYON YENI NODE ILE TEKRAR CAGRILIYOR
				}
			}
			iter = iter->next;
		}
	}
}

void findInfluencer(NODE **list, int count, int mode, int X, int Y){								//VERILEN X VE Y DEGERINE GORE INFLUENCER OLANLARI EKRANA YAZDIRAN FONKSIYON
	int i;
	printf("\t\t\tINFLUENCERS\n");
	for(i = 0; i < count; ++i){
		if(list[i]->inDegree >= X && list[i]->total >= Y){
			if(mode == 1)
				printf("Node: %3d  Name: %15s  Surname: %15s   Flag: %2d   In-Degree: %3d  Toplam: %3d\n", list[i]->nodeNumber, list[i]->name, list[i]->surname, list[i]->flag, list[i]->inDegree, list[i]->total);
			else
				printf("Name: %15s  Surname: %15s\n", list[i]->name, list[i]->surname);
		}
	}
}

void resetVisited(int *visited, int count){											//BUTUN NODE'LAR ICIN TOTAL CONNECTIONS HESAPLANACAGI ICIN HER NODE KONTROLUNDEN SONRA VISITED DIZISINI SIFIRLAYAN FONKSIYON
	int i;
	for(i = 0; i < count; ++i){
		visited[i] = 0;
	}
}

int menu(){																			//EKRANA MENU YAZDIRAN FONKSIYON
	int choice;
	do{
		printf("1. Read File\n2. Normal Mode\n3. Detailed Mode\n4. Exit\nChoice: ");
		scanf("%d",&choice);
	}while(choice != 1 && choice != 2 && choice != 3 && choice != 4);
	return choice;
}

int main(){
	
	NODE **list = NULL;
	int count = 0;
	int i;
	int choice;
	int *visited;
	int M, X, Y;
	
	printf("M Degerini Giriniz: ");												//M, X VE Y DEGERLERININ KULLANICIDAN ALINMASI
	scanf("%d", &M);
	printf("X Degerini Giriniz: ");
	scanf("%d", &X);
	printf("Y Degerini Giriniz: ");
	scanf("%d", &Y);	
	
	do{
		system("cls");
		choice = menu();														//MENUNUN CAGRILMASI
		system("cls");
		switch(choice){
			case 1:
				list = readFile(list, &count);									//DOSYANIN OKUNMASI
				inDegreeCalculate(list, count, M);									//IN-DEGREE DEGERLERININ HESAPLANMASI
				originalInDegree(list, count);									//ORIGINAL IN-DEGREE DEGERLERININ HESAPLANMASI
				
				visited = (int *)malloc((count + 1) * sizeof(int));				//ELEMAN SAYISINA GORE VISITED DIZISININ ACILMASI
				for(i = 0; i < count; ++i){
					if(list[i]->flag == 1){										//EGER NODE ELENMEMISSE
						resetVisited(visited, count + 1);						//VISITED DIZISI HER NODE ICIN RESETLENIYOR
						visited[i + 1] = 1;										//CAGRILAN NODE VISITED OLARAK AYARLANIP
						totalCalculate(list, i + 1, visited);					//DFS KULLANARAK TOTAL CONNECTIONS BULAN FONKSIYON CAGRILIYOR
					}			
				}
				printf("File Read Successfully\n\nPress Any Key!");
				getch();
				break;
			case 2:
				findInfluencer(list, count, 0, X, Y);									//MODE'A UYGUN SEKILDE INFLUENCER'LARI EKRANA YAZDIRAN FONKSIYON CAGRILIYOR
				printf("\n\nPress Any Key!");
				getch();
				break;
			case 3:
				display(list, count, 0);										//MODE'A UYGUN SEKILDE EKRANA YAZDIRAN FONKSIYONLAR CAGRILIYOR
				display(list, count, 1);
				findInfluencer(list, count, 1, X, Y);
				printf("\n\nPress Any Key!");
				getch();
				break;
		}
	}while(choice != 4);

	free(list);
	
	return 0;
}
