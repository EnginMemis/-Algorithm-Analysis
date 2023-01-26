#include<stdio.h>
#include<stdlib.h>
#include<string.h>

int **initializeBoard(int **board, int N){								//MATRISI OLUSTURAN FONKSIYON
	int i;
	board = (int **)malloc(sizeof(int *) * N);
	for(i = 0; i < N; ++i){
		board[i] = (int *)malloc(sizeof(int) * N);
	}
	return board;
}


char **initializeColorIndex(char **colorIndex, int N){					//RENKLERI TUTACAK OLAN DIZIYI OLUSTURAN FONKSIYON
	int i, j;
	colorIndex = (char **)malloc(sizeof(char *) * N);
	for(i = 0; i < N; ++i){
		colorIndex[i] = (char *)malloc(sizeof(char) * 20);
	}
	return colorIndex;
}

int checkColor(char **colorIndex, int **board, int x, int y, int count, int N, char *color){					//GIRILEN RENKLERI KONTROL EDEN FONKSIYON
	int i = 0;
	while(i < count && strcmp(colorIndex[i], color) != 0){				//GIRILEN RENGI ONCEDEN GIRILEN RENKLER ILE KARSILASTIRIYOR
		i++;
	}
	if(i < N){															//DIZIDEKI RENK SAYISI OLMASI GEREKEN RENK SAYISINDAN KUCUK ISE
		if(i == count){													//YENI GIRILEN RENK DIZIDE YOK ISE DIZIYE GIRILEN RENK KOYULUYOR
			strcpy(colorIndex[i], color);
		}
		board[x][y] = i;												//RENK DIZIDE VAR ISE INTEGER MATRISE O RENGIN INDISI YAZILIYOR
		return 1;
	}
	else{
		printf("Hatali Renk Girisi!\n");								//EGER DIZIDE OLMASI GEREKEN KADAR RENK VAR ISE FARKLI RENK GIRDIGI ICIN HATA MESAJI GONDERILIYOR
		return 0;
	}
		
}

int checkBoard(int **board, int N, int index){							//VERILEN SATIR INDEXINDEN YUKARISINA BAKARAK MATRISIN YERLESIMININ UYGUN OLUP OLMADIGINA BAKAN FONKSIYON
	int i, j, k, flag; 
	int *checkMatrix;
	
	checkMatrix = (int *)calloc(N, sizeof(int));						//KONTROL KOLAY OLABILMESI ICIN DIZI ACILIYOR
	
	i = 0;
	flag = 1;
	while(i < N && flag){
		for(k = 0; k < N; ++k){
			checkMatrix[k] = 0;
		}
		
		j = index;
		while(j >= 0  && flag){											//EGER VERILEN SATIRIDAN YUKARIDA DAHA ONCEDEN ISARETLENMIS BIR ELEMAN VAR ISE DIZI UYGUN OLMADIGI ICIN FLAG 0 YAPILIYOR
			if(checkMatrix[ board[j][i] ] == 1){
				flag = 0;
			}
			else{														//EGER DAHA ONCEDEN ISARETLENMEMIS BIR ELEMAN VAR ISE O ELEMANI DA ISARETLEYIP BIR UST SATIRA GECIYOR
				checkMatrix[ board[j][i] ] = 1;
				j--;	
			}
		}
		i++;
	}
	if(flag == 0)														//EGER MATRIS UYGUN DEGILSE 0 RETURN EDILIYOR
		return 0;
	return 1;															//UYGUNSA 1 RETURN EDILIYOR
}

void rotate(int **board, int N, int index){								//MATRISTE VERILEN SATIRI ROTATE EDEN FONKSIYON
	int i;
	int temp;
	
	temp = board[index][N - 1];											//SON ELEMAN TEMP DEGISKENINE ALINIYOR
	for(i = N - 1; i > 0; --i){
		board[index][i] = board[index][i - 1];							//HER INDISE BIR ONCEKI INDIS DEGERI KOYULUYOR
	}
	board[index][0] = temp;												//0. INDISE TEMP DEGISKENI ATILIYOR
}

void displayBoard(int **board, char **color, int N){									//MATRISI EKRANA YAZDIRAN FONKSIYON
	int i, j;
	printf("\n");
	for(i = 0; i < N; ++i){
		for(j = 0; j < N; ++j){
			printf("%10s", color[board[i][j]]);							//COLOR DIZISINDE UYGUN INDISTEKI RENGI YAZDIRIYOR
		}
		printf("\n");
	}
	printf("\n\n");
}

int recursive(int **board, char **color, int N, int index, int mode){					//BACKTRACKING KULLANARAK SONUCU BULAN FONKSIYON
	int i;
	int flag;
	
	if(index == N){														//EGER DIZININ EN SON SATIRINI DA GECTIYSE MATRIS UYGUN HALDEDIR
		return 1;
	}
	flag = 0;
	i = 0;
	while(i < N && flag == 0){											//HER SATIRI SATIRDAKI ELEMAN SAYISI KADAR ROTATE EDECEK DONGU
		if(checkBoard(board, N, index) == 1){							//EGER MATRIS UYGUNSA
			flag = recursive(board, color, N, index + 1, mode);				//BIR ALT SATIR ICIN RECURSIVE CAGRILIYOR
		}
		if(flag == 0){													//EGER ALT SATIRDAN 0 RETURN EDILIRSE
			rotate(board, N, index);									//BU SATIR ROTATE EDILIYOR
			if(mode == 2)												//SECILEN MODA GORE EKRANA YAZDIRMA YA DA YAZDIRMAMA KONTROLU
				displayBoard(board, color, N);
		}
		i++;
	}
	if(flag == 0)														//EGER SATIRDAKI BUTUN ROTATELER BITTIKTEN SONRA FLAG 0 ISE UYGUN DIZILIM YOKTUR
		return 0;
	return 1;															//FLAG 1 ISE UYGUN DIZILIM VARDIR
}

int main(){
	
	int N;
	int **board;
	char **colorIndex;
	int i, j, count;
	char color[20];
	int flag;
	int isAvailable;
	int mode;
	
	printf("N*N Tablonun N Degerini Giriniz: ");
	scanf("%d",&N);
	
	board = initializeBoard(board, N);											
	colorIndex = initializeColorIndex(colorIndex, N);
	
	count = 0;
	for(i = 0; i < N; ++i){
		for(j = 0; j < N; ++j){
			do{
				printf("[%d][%d] Renk Giriniz: ", i, j);									//KULLANICIDAN RENKLER ALINIYOR
				scanf("%s", color);
				flag = checkColor(colorIndex, board, i, j, count, N, color);				//ALINAN RENKLER UYGUN MU DIYE KONTROL EDILIYOR
			}while(flag == 0);
			count++;
		}
	}
	
	do{
		printf("\n1. Normal Mode\n2. Detailed Mode\nMod Seciniz: ");						//MODE SECIMI ISTENIYOR
		scanf("%d", &mode);
	}while(mode != 1 && mode != 2);
	
	printf("\n\n***********Baslangic Durumu***********\n");
	displayBoard(board, colorIndex, N);
	printf("***********Adimlar***********\n\n");
	isAvailable = recursive(board, colorIndex, N, 0, mode);												//BACKTRACKING FONKSIYONU CAGRILIYOR
	if(isAvailable == 1){																	//EGER UYGUN MATRIS VAR ISE MATRIS EKRANA YAZDIRILIYOR
		printf("***********Dogru Yerlesim Bulundu!***********\n\n");
		displayBoard(board, colorIndex, N);
	}
	else
		printf("***********Dogru Yerlesim Bulunamadi!***********");							//UYGUN MATRIS YOKSA MESAJ EKRANA YAZDIRILIYOR
		
	
	return 0;
}
