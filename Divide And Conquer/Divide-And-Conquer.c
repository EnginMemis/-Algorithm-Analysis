#include<stdio.h>
#include<stdlib.h>
#include<limits.h>

typedef struct block{					//OLUSACAK MAX PARCANIN DEGERINI VE INDISLERINI TUTAN STRUCT YAPISI
	int value;
	int left;
	int right;
}BLOCK;


BLOCK findMaxSum(int *array, BLOCK *maxArray, int count){												
	
	BLOCK max;
	BLOCK dMax;
	int sum, midSum;
	int i;
	

	max.value = maxArray[1].value;												//MAX ELEMANI ONCELIKLE GELEN DIZIDEKI MID ELEMANINA ESITLIYORUZ(OLABILECEK EN BUYUK ELEMAN SADECE MID ELEMANI DA OLABILIR)
	max.left = maxArray[1].left;
	max.right = maxArray[1].right;
	
	//bMax
	if(maxArray[0].left != -1 && maxArray[0].right != -1){						//EGER DIZININ SOL TARAFINDA ELEMANLAR VAR ISE SOL TARAFINDAKI INDISTEN MID ELEMANININ INDISINE KADAR OLAN KISIMDAKI MAX ELEMAN BULMA ISLEMI
		sum = 0;
		for(i = maxArray[0].left; i < maxArray[1].right; ++i){
			sum += array[i];
			
			if(sum > max.value){
				max.value = sum;
				max.left = maxArray[0].left;
				max.right = i;
			}	
		}
	}
	
	
	//cMax
	
	if(maxArray[2].left != -1 && maxArray[2].right != -1){						//EGER DIZININ SAG TARAFINDA ELEMANLAR VAR ISE MID ELEMANINDAN  SAG TARAFTAKI ELEMANIN INDISINE KADAR OLAN KISIMDAKI MAX ELEMAN BULMA ISLEMI
		sum = 0;
		for(i = maxArray[1].left + 1; i <= maxArray[2].right; ++i){
			sum += array[i];
			if(sum > max.value){
				max.value = sum;
				max.left = maxArray[1].left + 1;
				max.right = i;
			}	
		}
	}
	
	
	//dMax
	
	dMax.value = INT_MIN;														//SOL, SAG VE MID DEGERLERINI BIRLESTIREREK OLUSABILECEK MAX PARCAYI TUTAN DEGERIN OLUSTURULMASI
	dMax.left = -1;
	dMax.right = -1;
																				//DIZININ SOL, SAG VE MID ELEMANINI KATARAK OLUSABILECEK EN BUYUK PARCAYI BULAN FONKSIYONU ASLINDA RECURSIVE OLARAK DEGIL DE TUM DIZIYE 1 KERE UYGULASAK
																				//EN BUYUK PARCAYI BULABILIYOR ANCAN BIZDEN DIVIDE AND CONQUER ISTENDIGI ICIN COZUM BOYLE YAPILDI
	sum = 0;	
	if(maxArray[0].left != -1 && maxArray[0].right != -1){						//DIZININ SOL TARAFINDA ELEMAN VAR ISE
		for(i = maxArray[0].left; i <= maxArray[2].right; ++i){					//ONCELIKLE SOL TARAFTAN SAG TARAFA KADAR DEGERLERI TOPLAYARAK GIDIP OLUSABILECEK EN BUYUK PARCANIN SAG INDEXINI BULUYOR
			sum += array[i];
			if(sum > dMax.value){
				dMax.value = sum;
				dMax.left = maxArray[0].left;
				dMax.right = i;
			}	
		}
		
		sum = dMax.value;														//ARDINDAN YUKARDA BULDUGU MAX DEGERI SUM DEGISKENINE ATIP YINE SOLDAN SAG TARAFA GIDECEK DONGUDE BU SEFER OLUSABILECEK EN BUYUK PARCANIN
		for(i = maxArray[0].left; i <= dMax.right; ++i){						//SAG INDEXINI BULMAK ICIN SOL TARAFTAN DEGERLERI CIKARTARAK GELIYOR EGER MAXTAN BUYUK BIR ELEMAN ELDE EDERSE SOL INDEXI DEGISTIRIYOR
			sum = sum - array[i];
			if(sum > dMax.value){
				dMax.value = sum;
				dMax.left = i + 1;
			}	
		}
	}
	else{																		//BURDAKI KONTROL ISE DIZININ SOLUNDA MAX ELEMAN YOK ISE SOLDAN SAGA GITMEK YERINE MIDDEN SAG TARAFA KADAR GIDIYOR
		for(i = maxArray[1].left; i <= maxArray[2].right; ++i){
			sum += array[i];
			if(sum > dMax.value){
				dMax.value = sum;
				dMax.right = i;
			}	
		}
		
		sum = dMax.value;
		for(i = maxArray[1].left; i <= dMax.right; ++i){
			sum = sum - array[i];
			if(sum > dMax.value){
				dMax.value = sum;
				dMax.left = maxArray[1].left;
				dMax.left = i + 1;
			}	
		}
	}
	
	if(dMax.value > max.value){													//OLUSAN DEGERLER ARASINDA EN BUYUK ELEMANI BULMASI
		max.value = dMax.value;
		if(dMax.left != -1)
			max.left = dMax.left;
		if(dMax.right != -1)
			max.right = dMax.right;
	}
	
	return max;																	//MAX ELEMANI DONDURUYOR
}




BLOCK divideAndConquer(int *array, int count, int left, int right){
	
	BLOCK max;
	BLOCK maxArray[3];											// RECURSIVE YAPIDA OLUSAN HER DIZININ SOLUNDAKI SAGINDAKI MAX ELEMANLARI VE MID ELEMANINI TUTAN DIZI (0. indis bMax  1. indis mid   2. indis cMax)
	int mid;
	int i;
	
	mid = (left + right) / 2;									
																//MAX ELEMANI TUTACAK OLAN YAPI OLUSTURULUP ONCELIKLE ICINE MID ELEMANININ DEGERLERINI ATIYORUZ
	max.value = array[mid];										
	max.left = mid;
	max.right = mid;
	
	if(left == right){											//EGER DIZIDE TEK ELEMAN KALDIYSA MAX ELEMANI TUTAN YAPIYI RETURN EDIYORUZ
		return max;
	}
	
	if(left > right){											//LEFT RIGHT'TAN DAHA BUYUK OLDUGU DURUMDA INT_MIN DONDUREREK MAX ELEMANI ARARKEN YAPIYI BOZMAMASINI SAGLIYORUZ
		max.value = INT_MIN;
		max.left = -1;
		max.right = -1;
		return max;
	}	
		
	maxArray[0].value = INT_MIN;								//ONCELIKLE DIZININ SOL VE SAGINDA MAX ELEMAN OLMADIGI ICIN INT_MIN DEGERLERI EKLIYORUZ ANCAK DIZIDE TEK ELEMAN KALSA BILE MID OLDUGU ICIN MID DEGERLERINI DE ATIYORUZ
	maxArray[0].left = -1;
	maxArray[0].right = -1;
	
	maxArray[1].value = array[mid];
	maxArray[1].left = mid;
	maxArray[1].right = mid;
	
	maxArray[2].value = INT_MIN;
	maxArray[2].left = -1;
	maxArray[2].right = -1;
	
	maxArray[0] = divideAndConquer(array, count, left, mid - 1);					//SOL AGACA DALLANDIGIMIZDA SOL AGACTAN GELEN MAX DEGER BU DIZININ SOL TARAFINDAKI MAX ELEMANI TUTAN DIZININ 0. INDISINE ATANIYOR
	maxArray[2] = divideAndConquer(array, count, mid + 1, right);					//SAG AGACA DALLANDIGIMIZDA SAG AGACTAN GELEN MAX DEGER BU DIZININ SAG TARAFINDAKI MAX ELEMANI TUTAN DIZININ 2. INDISINE ATANIYOR
	max = findMaxSum(array, maxArray, count);										//VE BU OLUSAN DEGERLER ARASINDA EN BUYUK ELEMANI BULAN FONKSIYONU CAGIRIYORUZ
	return max;
	
}


int bruteForce(int *array, int *left, int *right, int count){						//ALGORITMAYI BUTUN HER SEYI KONTROL EDEREK COZEN BRUTE FORCE ALGORITMASI
	int i, j;
	int sum, max;
	
	max = INT_MIN;										//MAX ELEMANA INT_MIN DEGERININ ATANMASI
	for(i = 0; i < count; ++i){							//DIZININ TUM ELEMANLARINI 0. INDISTEN BASLAYIP SON INDISE KADAR GIDEREK EN BUYUK PARCAYI ARAMA ISLEMI
		sum = 0;
		for(j = i; j < count; ++j){
			sum += array[j];
			if(sum > max){
				max = sum;
				*left = i;								//BULUNAN INDISLERIN MAINDE DE DEGISMESI ICIN POINTER KULLANILMASI
				*right = j;
			}
		}
	}
	return max;											//MAX DEGERIN DONDURULMESI
}


int main(){
	
	int *array;
	int maxLeft, maxRight;
	int count;
	int sum;
	int i;
	BLOCK max;
	
	printf("Eleman Sayisini Giriniz: ");
	scanf("%d",&count);
	
	array = (int *)malloc(sizeof(int) * count);										//ELEMAN SAYISINA GORE DINAMIK OLARAK OLUSTURULAN DIZI
	
	
	printf("Dizinin Elemanlarini Giriniz:\n");
	for(i = 0; i < count; ++i){
		scanf("%d",&array[i]);														//DIZIDEKI ELEMANLARIN KULLANICIDAN ALINMASI
	}
	sum = bruteForce(array, &maxLeft, &maxRight, count);											//BRUTE FORCE CAGRILMASI
	printf("Brute Force -> %d Index: %d - %d\n", sum, maxLeft, maxRight);
	max = divideAndConquer(array, count, 0, count - 1);												//DIVIDE AND CONQUER CAGRILMASI
	printf("Divide And Conquer -> %d Index:  %d - %d\n",max.value, max.left, max.right);

	free(array);																	//TUTULAN DIZININ FREE EDILMESI
	
	
	return 0;
}
