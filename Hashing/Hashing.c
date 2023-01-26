#include<stdio.h>
#include<string.h>
#include<stdlib.h>

#define fileName "Sample.txt"
#define K 31

typedef struct block{
	
	char word[50];							//KELIME VE LINKLERI TUTAN YAPI
	char link[100];
	
}BLOCK;


typedef struct links{
	char link[100];							//VE, VEYA ISLEMLERI ICIN LINKLERI TUTAN YAPI
	int flag;
}LINKS;


typedef struct hashTable{
	BLOCK block;
	int isNull;								//HASH TABLE'IN OLUSTURULACAGI YAPI
	int key;
	int turn;
}HASH_TABLE;


int hash(char *string, int hashSize){						//GELEN STRING DEGERINE GORE HORNER METODU ILE KEY DEGERININ HESAPLANMASI
	int key = 0;
	int length = strlen(string);
	int i, temp;
	
	for(i = 0; i < length; ++i){
		temp = string[i];
		key = K * key + temp;
		key = key % hashSize;								//GELEN STRINGLER UZUN OLDUGUNDA INTEGER KAPASITESINI ASMAMASI ICIN HER ADIMDA MOD ALMA ISLEMI UYGULANIYOR
	}
	return key % hashSize;
}

void lowerCase(char *word){
	int i;
	for(i = 0; i < strlen(word); ++i){							//BUYUK KUCUK HARFLERIN HEPSINI KABUL ETMESI ICIN GIRILEN HER STRINGI KUCUK HARFE CEVIREN FONKSIYON
		if(word[i] < 91 && word[i] > 64){
			word[i] = word[i] + 32;
		}
	}
}

BLOCK* readFile(BLOCK *blocks, LINKS **links, int *wordCount, int *linkCount){
	FILE *file;
	char word[100];
	char link[100];
	
	if((file = fopen(fileName, "r")) == NULL)									//DOSYANIN ACILMASI
		return NULL;
		
	
	while(!feof(file)){																					//DOSYANIN SONUNA KADAR DONEN DONGU
		fscanf(file, "%s", word);
		
		lowerCase(word);																				//OKUNAN STRINGI KUCUK HARFLERE DONUSTURME
		
		if(strstr(word, "http") != NULL){																	//OKUNAN DEGER ICERISINDE http IFADESI GECIYORSA BU DEGERIN LINK OLARAK ALGILANMASININ KONTROLU
			strcpy(link, word);
			strcpy((*links)[*linkCount].link, link);
			(*links)[*linkCount].flag = 0;
			(*linkCount)++;
			(*links) = (LINKS *)realloc((*links), ((*linkCount) + 1) * sizeof(LINKS));								//LINKS DIZISINDE YENI YER ACILIP ICINE LINKIN YERLESTIRILMESI
		}
		else{																								//OKUNAN STRING LINK DEGIL KELIME ISE
			strcpy(blocks[*wordCount].word, word);
			strcpy(blocks[*wordCount].link, link);
			(*wordCount)++;
			blocks = (BLOCK *)realloc(blocks, ((*wordCount) + 1) * sizeof(BLOCK));							//KELIMELERI VE LINKLERI TUTAN DIZIDE YENI YER ACILMASI VE KELIMENIN YERLESTIRILMESI
		}
	}
	fclose(file);
	return blocks;
}

void initializeLinks(LINKS *links, int linkCount){															//VE, VEYA ISLEMLERINI YAPAN LINK DIZISININ HER BIR SORGU ICIN DOGRU CALISABILMESI ICIN DEGERININ RESETLENMESI
	int i;
	for(i = 0; i < linkCount; ++i){
		links[i].flag = 0;
	}
}

void insertKey(BLOCK *blocks, HASH_TABLE *table, int hashSize, int count){									//KELIMELERI HASH TABLE'A YERLESTIREN FONKSIYON
	int i;
	int j = 0;
	int key, turn, adr;
	
	for(i = 0; i < count; ++i){																				//KELIME SAYISI KADAR DONEN DONGU
		turn = 0;
		adr = hash(blocks[i].word, hashSize);														//KELIMEYE KARSILIK GELEN KEY DEGERININ HESAPLANMASI
		key = adr;
		while(table[adr].isNull == 0 && j < hashSize){												//HASH TABLE'DA DOGRU YERE GELENE KADAR DONEN DONGU
			adr = (adr + 1) % hashSize;
			turn++;
		}
		if(j != hashSize){																		//EGER KELIME BOS BIR YER BULDUYSA
			table[adr].block = blocks[i];														//KELIMEYI YERLESTIRME
			table[adr].isNull = 0;																//ARTIK YER NULL OLMADIGI ICIN DEGISKENIN ONA GORE DEGISMESI
			table[adr].key = key;
			table[adr].turn = turn;
			j++;
		}
		else{
			printf("For %s HashTable is Full!\n",blocks[i].word);
			getch();
		}
	}
}

HASH_TABLE *initializeHashTable(HASH_TABLE *table, int hashSize){								//HASH TABLE'IN OLMASI GEREKEN BOYUTA GORE OLUSTURULMASI
	int i;
	
	table = (HASH_TABLE *)malloc(sizeof(HASH_TABLE) * hashSize);	
	
	for(i = 0; i < hashSize; ++i){
		table[i].isNull = 1;
		strcpy(table[i].block.link, "");
		strcpy(table[i].block.word, "");
		table[i].key = -1;
		table[i].turn = -1;
	}
	return table;
}


int calculateHashSize(float load_factor, int count){											//KULLANICIDAN ALINAN LOAD_FACTOR DEGERINE VE KELIME SAYISINA GORE HASH TABLE BOYUTUNUN HESAPLANMASI
	int prime;
	prime = count / load_factor;
	
	while(isPrime(prime) == 0){
		prime++;
	}
	return prime;
}

int isPrime(int number){																		//HASH TABLE BOYUTUNUN ASAL SAYI OLMASI ICIN ASAL SAYI KONTROLU YAPAN FONKSIYON
	int i;
	int flag;
	
	flag = 1;
	for(i = 2; i * i <= number; ++i){
		if(number % i == 0)
			return 0;
	}
	return 1;
}

void displayHashTable(HASH_TABLE *table, int hashSize){												//DETAIL MODE ICIN HASH TABLE'I EKRANA YAZDIRAN FONKSIYON
	int i;
	
	printf("\n\nIndex isNull    Key    Turn           Word 				Link\n");
	for(i = 0; i < hashSize; ++i){
		printf("%3d | %5d | %5d | %5d | %20s | %40s\n", i, table[i].isNull, table[i].key, table[i].turn, table[i].block.word, table[i].block.link);
	}
}

int filterQuery(char words[3][50], char query[100]){								//GIRILEN SORGUDA ISTENILEN KELIMELERI VE YAPMAK ISTEDIGI SECIMI(VE, VEYA) FILTRELEME ISLEMINI YAPAN FONKSIYON
	int i, j, k;
	i = 0;
	j = 0;
	
	lowerCase(query);
	
	while(query[i] != '\0'){
		k = 0;
		while(query[i] != '\0' && query[i] != ' '){
			words[j][k] = query[i];
			k++;
			i++;
		}
		words[j][k] = '\0';
		i++;
		j++;
	}
	
	if(strcmp(words[1], "ve") == 0){
		return 1;
	}
	else if(strcmp(words[1], "veya") == 0){
		return 2;
	}
	else{
		return -1;
	}
}

void findLink(HASH_TABLE *table, int hashSize, char word[100], LINKS *links, int linkCount){						//FONKSIYONA GELEN KELIMENIN KARSILIGI OLAN LINKLERIN FLAG DEGERININ 1 ARTTIRILMASI
	int i, j;
	int adr;
	
	j = 0;
	adr = hash(word, hashSize);
	while(j < hashSize && table[adr].isNull == 0){															//HASH TABLE'DA KELIMEYI BULUP O KELIMENIN KARSILIK GELEN LINKLERIN BULUNMASI
		if(strcmp(table[adr].block.word, word) == 0){
			for(i = 0; i < linkCount; ++i){
				if(strcmp(links[i].link, table[adr].block.link) == 0){
					links[i].flag++;
				}
			}
		}
		adr = (adr + 1) % hashSize;
		j++;
	}
}

void search(HASH_TABLE *table, int hashSize, LINKS *links, int linkCount, char query[100]){							//GIRILEN SORGUDAKI KELIMELERE GORE LINKLERIN YAZDIRILMASI ISLEMINI YAPAN FONKSIYON
	int i, j;
	int flag;
	char words[3][50];
	int firstQueryCount, secondQueryCount;
	int isAvailable;
	
	flag = filterQuery(words, query);																			//GIRILEN SORGUYA GORE VE MI VEYA MI ISLEMININ ALINMASI
	
	findLink(table, hashSize, words[0], links, linkCount);
	
	if(flag != -1){																								//EGER GIRILEN SORGU 1 ELEMANLI DEGIL ISE 2. KELIME ICIN DE LINKLERIN DEGERININ ARTTIRILMA ISLEMI
		findLink(table, hashSize, words[2], links, linkCount);
	}
	
	isAvailable = 0;
	printf("\nLINKS:\n\n");
	
	if(flag == 1){																								//EGER YAPILACAK ISLEM 'VE' ISLEMI ISE
		for(i = 0; i < linkCount; ++i){
			if(links[i].flag == 2){
				printf("%s \n",links[i].link);						//LINKLERIN DEGERI 2 ISE YANI 2 KELIME ICIN DE O LINK BULUNDUYSA LINKIN YAZDIRILMA ISLEMI
				isAvailable = 1;
			}																				
				
		}
	}
	else{
		for(i = 0; i < linkCount; ++i){
			if(links[i].flag != 0){																				//EGER ISLEM 'VEYA' YA DA TEK KELIME ISE O KELIME YA DA KELIMELERIN KARSILIK GELDIGI LINKLERIN EKRANA YAZDIRILMASI
				printf("%s \n",links[i].link);
				isAvailable = 1;
			}
		}
	}
	if(isAvailable == 0 && flag == 1){
		printf("Bu iki kelimenin birlikte gectigi dokuman bulunmamaktadir\n");
	}
	else if(isAvailable == 0){
		printf("Bu kelimenin gectigi dokuman bulunmamaktadir. \n");
	}
	initializeLinks(links, linkCount);															//YENI SORGU ISLEMININ DUZGUN CALISABILMESI ICIN LINKLERIN FLAG DEGERLERININ RESETLENMESI
	
}

void query(int mode, HASH_TABLE *table, int hashSize, LINKS *links, int linkCount){
	
	char userQuery[100];
	int choice = 1;
	
	while(choice != 3){																				//MENU ISLEMLERI
		system("cls");
		if(mode == 1)
			printf("\nMode: Normal Mode\n\n1. New Query\n2. Change Mode\n3. Exit");
		else
			printf("\nMode: Detail Mode\n\n1. New Query\n2. Change Mode\n3. Exit",mode);
		do{
			printf("\n\nYour Choice: ");
			scanf("%d",&choice);
		}while(choice != 1 && choice != 2 && choice != 3);
		
		
		if(choice == 1){
			if(mode == 2){
				displayHashTable(table, hashSize);
			}	
			printf("\n\nQUERY:\n");
			printf("Please Enter the Query: ");
			scanf(" %[^\n]", userQuery);
			search(table, hashSize, links, linkCount, userQuery);									//GIRILEN SORGUYA GORE YAPILACAK ISLEMLERIN CAGRILMASI
			printf("\n\nPress Any Key!");
			getch();
		}
		
		else if(choice == 2){
			system("cls");
			printf("1. Normal Mode\n2. Detail Mode");
			do{
				printf("\n\nYour Choice: ");
				scanf("%d",&mode);
			}while(mode != 1 && mode != 2);
			
		}
	}
	
}

int main(){
	
	BLOCK *blocks;
	LINKS *links;
	HASH_TABLE *hashTable;
	
	int wordCount, linkCount;
	float load_factor;
	int hashSize;
	int i;
	
	links = (LINKS *)malloc(sizeof(LINKS));
	blocks = (BLOCK *)malloc(sizeof(BLOCK));
	
	wordCount = 0;
	linkCount = 0;
	
	blocks = readFile(blocks, &links, &wordCount, &linkCount);												//DOSYANIN OKUNMASI
	
	if(blocks != NULL){
		printf("Load Factor: ");
		scanf("%f",&load_factor);
		
		hashSize = calculateHashSize(load_factor, wordCount);												//GEREKLI FONKSIYONLARIN CAGRILMASI
		hashTable = initializeHashTable(hashTable, hashSize);
		insertKey(blocks, hashTable, hashSize, wordCount);
		query(1, hashTable, hashSize, links, linkCount);
		free(links);
		free(blocks);
		free(hashTable);
	}
	else{
		printf("ERROR!");
	}

	return 0;
}
