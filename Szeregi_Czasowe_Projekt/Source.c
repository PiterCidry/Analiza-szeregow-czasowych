#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <ctype.h>
#include <string.h>

#define _ROZ1_ 15 /*rozmiar dla ludnosci ogolem*/
#define _ROZWYN_ 5 /* rozmiar tablicy wynikow*/
#define _ROZ2_ 11 /*rozmiar dla emigracji*/
#define _ROZ3_ 11 /*rozmiar dla temperatury*/
#define _ROZ4_ 12 /*rozmiar dla kursu zlota*/

float srednia(float suma, int ilosc) /*funkcja liczaca srednia*/
{
	float wynik;

	wynik = suma / ilosc;
	return wynik;

}

float mediana(int ilosc, float t[]) /*funkcja liczaca mediane*/
{
	float wynik;

	int i, j;
	float temp;

	for (i = 0; i < ilosc; i++)
	{
		for (j = 0; j < (ilosc - 1); j++)
		{
			if (t[j] > t[j + 1])
			{
				temp = t[j + 1];
				t[j + 1] = t[j];
				t[j] = temp;
			}
		}
	}

	if (ilosc % 2 == 0)
	{
		wynik = ((t[(ilosc / 2) - 1] + t[(ilosc / 2)]) / 2);

		return wynik;
	}
	else
	{
		wynik = (t[((ilosc - 1) / 2)]);
		return wynik;
	}

}

float odchylenie_standardowe(int ilosc, float t[], float srednia) /*funkcja liczaca odchylenie standardowe*/
{
	float suma = 0;
	int i;
	float temp, wynik;
	for (i = 0; i < ilosc; i++)
	{
		temp = (t[i] - srednia) * (t[i] - srednia);
		suma += temp;
	}

	wynik = (float)sqrt(suma / ilosc);

	return wynik;
}

float skosnosc(int ilosc, float t[], float srednia, float odchylenie_standardowe) /*funkcja liczaca skosnosc*/
{
	float suma = 0;
	int i;
	float temp, wynik;
	for (i = 0; i < ilosc; i++)
	{
		temp = (t[i] - srednia)*(t[i] - srednia)*(t[i] - srednia);
		suma = suma + temp;
	}

	wynik = (suma / ilosc);
	wynik = (wynik / ((odchylenie_standardowe)*(odchylenie_standardowe)*(odchylenie_standardowe)));


	return wynik;
}

float kurtoza(int ilosc, float t[], float srednia, float odchylenie_standardowe) /*funkcja liczaca kurtoze*/
{
	float suma = 0;
	int i;
	float temp, wynik;
	for (i = 0; i < ilosc; i++)
	{
		temp = (t[i] - srednia)*(t[i] - srednia)*(t[i] - srednia)*(t[i] - srednia);
		suma += temp;
	}
	wynik = (suma / ilosc);
	wynik = (wynik / ((odchylenie_standardowe)*(odchylenie_standardowe)*(odchylenie_standardowe)*(odchylenie_standardowe)));

	return wynik;
}

float korelacja_danych(int ilosc, float t[], float t2[], float srednia, float srednia2) /*funkcja liczaca korelacje danych*/
{
	int i;
	float licznik, mianownik, suma1, suma2, sumamnoz, wynik;
	for (i = 0, licznik = 0; i < ilosc; i++)
	{
		licznik += (t[i] - srednia)*(t2[i] - srednia2);
	}
	for (i = 0, suma1 = 0; i < ilosc; i++)
	{
		suma1 += (t[i] - srednia)*(t[i] - srednia);
	}
	for (i = 0, suma2 = 0; i < ilosc; i++)
	{
		suma2 += (t2[i] - srednia2)*(t2[i] - srednia2);
	}
	sumamnoz = suma1*suma2;
	mianownik = (float)sqrt(sumamnoz);
	wynik = licznik / mianownik;
	return wynik;
}

const char* skosint(float skosnosc) /*funkcja interpretujaca wspolczynnik skosnosci*/
{
	const char *tekst;
	if (skosnosc > 0)
	{
		tekst = "Prawostronny rozklad danych";
	}
	else if (skosnosc == 0)
	{
		tekst = "Symetryczny rozklad danych";
	}
	else
	{
		tekst = "Lewostronny rozklad danych";
	}
	return tekst;
}

const char* kurtint(float kurtoza) /*funkcja interpretujaca wspolczynnik kurtozy*/
{
	const char *tekst;
	if (kurtoza < 3)
	{
		tekst = "Mala koncentracja danych wokol sredniej";
	}
	else if (kurtoza == 3)
	{
		tekst = "Rozklad rowny rozkladowi normalnemu";
	}
	else
	{
		tekst = "Duza koncentracja danych wokol sredniej";
	}
	return tekst;
}

int main(void)
{
	int op;
	op = 1;
	while (op == 1)
	{
		int wybor, wybor2, wyborszer1, wyborszer2, i, j, ilosc;
		float suma, suma1, suma2, WYNIKI[_ROZWYN_], LUDNOSC[_ROZ1_], EMIGRACJA[_ROZ2_], TEMPERATURA[_ROZ3_], ZLOTO[_ROZ4_];
		printf("Jaka analize chcesz przeprowadzic?\n1. Analiza na 1 szeregu\n2. Korelacja 2 szeregow\n3. Analiza w oknie ruchomym\n"); /*wybor co zamierzamy liczyc*/
		scanf_s("%i", &wybor2);

		if (wybor2 == 1) /*Czesc dla analizy 1 szeregu*/
		{
			printf("\nJakie dane chcialbys przeanalizowac?\n1. Liczba ludnosci w Polsce w latach 2000-2014\n2. Liczba emigrantow z Polski w latach 2004-2014\n3. Srednia jesienna temperatura w latach 2004-2014\n4. Kurs zlota w roku 2015\n5. Chce wprowadzic swoje wlasne dane\n"); /*wybór szeregow do liczenia*/
			scanf_s("%i", &wybor);
			printf("\n");

			if (wybor == 1) /*czesc wczytujaca dane dla liczby ludnosci i przeprowadzajaca na nich obliczenia*/
			{
				FILE *fp;
				fp = fopen("ludnosc ogolem.txt", "rt");
				if (fp == NULL) /*jezeli taki plik nie istnieje*/
				{
					printf("Nie mozna wczytac pliku\n");
					system("PAUSE");
					exit(0);
				}
				else
				{
					for (i = 0; i < _ROZ1_; i++) /*wczytywanie*/
					{
						fscanf_s(fp, "%f,", &LUDNOSC[i]);
					}
					for (i = 0, j = 2000; i < _ROZ1_; i++, j++) /*pokazanie danych na ktorych dokonaja sie operacje*/
					{
						printf("%i - %.0f\n", j, LUDNOSC[i]);
					}
					printf("\n");
				}
				fclose(fp); /*zamkniecie pliku z danymi*/

				for (i = 0, suma = 0; i < _ROZ1_; i++) /*zliczanie ludzi*/
				{
					suma += LUDNOSC[i];
				}

				for (i = 0, ilosc = 0; i < _ROZ1_; i++) /*zliczanie ilosci danych*/
				{
					ilosc += 1;
				}

				WYNIKI[0] = srednia(suma, ilosc);
				WYNIKI[1] = mediana(ilosc, LUDNOSC);
				WYNIKI[2] = odchylenie_standardowe(ilosc, LUDNOSC, srednia(suma, ilosc));
				WYNIKI[3] = skosnosc(ilosc, LUDNOSC, srednia(suma, ilosc), odchylenie_standardowe(ilosc, LUDNOSC, srednia(suma, ilosc)));
				WYNIKI[4] = kurtoza(ilosc, LUDNOSC, srednia(suma, ilosc), odchylenie_standardowe(ilosc, LUDNOSC, srednia(suma, ilosc)));
				printf("Srednia liczba ludnosci w Polsce w latach 2000-2014: %.2f\n", WYNIKI[0]); /*srednia	*/
				printf("Mediana liczby ludnosci w Polsce w latach 2000-2014: %.2f\n", WYNIKI[1]); /*mediana*/
				printf("Odchylenie standardowe liczby ludnosci w Polsce w latach 2000-2014: %.2f\n", WYNIKI[2]); /*odchylenie standardowe*/
				printf("Skosnosc liczby ludnosci w Polsce w latach 2000-2014: %.2f\n", WYNIKI[3]); /*skosnosc*/
				printf("Kurtoza liczby ludnosci w Polsce w latach 2000-2014: %.2f\n", WYNIKI[4]); /*kurtoza*/
				printf("Skosnosc: %s\n", skosint(skosnosc(ilosc, LUDNOSC, srednia(suma, ilosc), odchylenie_standardowe(ilosc, LUDNOSC, srednia(suma, ilosc))))); /*interpretacja skosnosci*/
				printf("Kurtoza: %s\n\n", kurtint(kurtoza(ilosc, LUDNOSC, srednia(suma, ilosc), odchylenie_standardowe(ilosc, LUDNOSC, srednia(suma, ilosc))))); /*interpretacja kurtozy*/

				FILE *fp2; /*zapisywanie wynikow do pliku*/
				fp2 = fopen("wyniki.txt", "a");
				if (fp2 == NULL) /*jezeli taki plik nie istnieje*/
				{
					printf("Nie mozna otworzyc pliku do zapisu wynikow\n");
				}
				else /*zapis tablicy wynikow do pliku*/
				{
					fprintf(fp2, "Liczba ludnosci w Polsce w latach 2000-2014:\n");
					fprintf(fp2, "Srednia - %.2f, ", WYNIKI[0]);
					fprintf(fp2, "Mediana - %.2f, ", WYNIKI[1]);
					fprintf(fp2, "Odchylenie standardowe - %.2f, ", WYNIKI[2]);
					fprintf(fp2, "Skosnosc - %.2f, ", WYNIKI[3]);
					fprintf(fp2, "Kurtoza - %.2f\n\n", WYNIKI[4]);
				}
				fclose(fp2); /*zamkniecie pliku zapisu*/
			}

			else if (wybor == 2) /*czesc wczytujaca dane dla emigracji ludzi z Polski i przeprowadzajaca na nich obliczenia*/
			{
				FILE *fp;
				fp = fopen("emigracjapolska.txt", "rt");
				if (fp == NULL) /*jezeli taki plik nie istnieje*/
				{
					printf("Nie mozna wczytac pliku\n");
					system("PAUSE");
					exit(0);
				}
				else
				{
					for (i = 0; i < _ROZ2_; i++) /*wczytywanie*/
					{
						fscanf_s(fp, "%f,", &EMIGRACJA[i]);
					}
					for (i = 0, j = 2004; i < _ROZ2_; i++, j++) /*pokazanie danych na ktorych dokonaja sie operacje*/
					{
						printf("%i - %.0f\n", j, EMIGRACJA[i]);
					}
					printf("\n");
				}
				fclose(fp); /*zamkniecie pliku z danymi*/

				for (i = 0, suma = 0; i < _ROZ2_; i++) /*zliczanie ludzi*/
				{
					suma += EMIGRACJA[i];
				}

				for (i = 0, ilosc = 0; i < _ROZ2_; i++) /*zliczanie ilosci danych*/
				{
					ilosc += 1;
				}

				WYNIKI[0] = srednia(suma, ilosc);
				WYNIKI[1] = mediana(ilosc, EMIGRACJA);
				WYNIKI[2] = odchylenie_standardowe(ilosc, EMIGRACJA, srednia(suma, ilosc));
				WYNIKI[3] = skosnosc(ilosc, EMIGRACJA, srednia(suma, ilosc), odchylenie_standardowe(ilosc, EMIGRACJA, srednia(suma, ilosc)));
				WYNIKI[4] = kurtoza(ilosc, EMIGRACJA, srednia(suma, ilosc), odchylenie_standardowe(ilosc, EMIGRACJA, srednia(suma, ilosc)));
				printf("Srednia liczba emigrantow z Polski w latach 2004-2014: %.2f\n", WYNIKI[0]); /*srednia	*/
				printf("Mediana liczby emigrantow z Polski w latach 2004-2014: %.2f\n", WYNIKI[1]); /*mediana*/
				printf("Odchylenie standardowe liczby emigrantow z Polski w latach 2004-2014: %.2f\n", WYNIKI[2]); /*odchylenie standardowe*/
				printf("Skosnosc liczby emigrantow z Polski w latach 2004-2014: %.2f\n", WYNIKI[3]); /*skosnosc*/
				printf("Kurtoza liczby emigrantow z Polski w latach 2004-2014: %.2f\n", WYNIKI[4]); /*kurtoza*/
				printf("Skosnosc: %s\n", skosint(skosnosc(ilosc, EMIGRACJA, srednia(suma, ilosc), odchylenie_standardowe(ilosc, EMIGRACJA, srednia(suma, ilosc))))); /*interpretacja skosnosci*/
				printf("Kurtoza: %s\n\n", kurtint(kurtoza(ilosc, EMIGRACJA, srednia(suma, ilosc), odchylenie_standardowe(ilosc, EMIGRACJA, srednia(suma, ilosc))))); /*interpretacja kurtozy*/

				FILE *fp2; /*zapisywanie wynikow do pliku*/
				fp2 = fopen("wyniki.txt", "a");
				if (fp2 == NULL) /*jezeli taki plik nie istnieje*/
				{
					printf("Nie mozna otworzyc pliku do zapisu wynikow\n");
				}
				else /*zapis tablicy wynikow do pliku*/
				{
					fprintf(fp2, "Emigracja ludnosci z Polski w latach 2004-2014:\n");
					fprintf(fp2, "Srednia - %.2f, ", WYNIKI[0]);
					fprintf(fp2, "Mediana - %.2f, ", WYNIKI[1]);
					fprintf(fp2, "Odchylenie standardowe - %.2f, ", WYNIKI[2]);
					fprintf(fp2, "Skosnosc - %.2f, ", WYNIKI[3]);
					fprintf(fp2, "Kurtoza - %.2f\n\n", WYNIKI[4]);
				}
				fclose(fp2); /*zamkniecie pliku zapisu*/
			}

			else if (wybor == 3) /*czesc wczytujaca dane dla sredniej jesiennej temperatury 2004-2014 i przeprowadzajaca na nich obliczenia*/
			{
				FILE *fp;
				fp = fopen("temperatura.txt", "rt");
				if (fp == NULL) /*jezeli taki plik nie istnieje*/
				{
					printf("Nie mozna wczytac pliku\n");
					system("PAUSE");
					exit(0);
				}
				else
				{
					for (i = 0; i < _ROZ3_; i++) /*wczytywanie*/
					{
						fscanf_s(fp, "%f,", &TEMPERATURA[i]);
					}
					for (i = 0, j = 2004; i < 11; i++, j++) /*pokazanie danych na ktorych dokonaja sie operacje*/
					{
						printf("%i - %.1f\n", j, TEMPERATURA[i]);
					}
					printf("\n");
				}
				fclose(fp); /*zamkniecie pliku z danymi*/

				for (i = 0, suma = 0; i < _ROZ3_; i++) /*zliczanie temperatur*/
				{
					suma += TEMPERATURA[i];
				}

				for (i = 0, ilosc = 0; i < _ROZ3_; i++) /*zliczanie ilosci danych*/
				{
					ilosc += 1;
				}

				WYNIKI[0] = srednia(suma, ilosc);
				WYNIKI[1] = mediana(ilosc, TEMPERATURA);
				WYNIKI[2] = odchylenie_standardowe(ilosc, TEMPERATURA, srednia(suma, ilosc));
				WYNIKI[3] = skosnosc(ilosc, TEMPERATURA, srednia(suma, ilosc), odchylenie_standardowe(ilosc, TEMPERATURA, srednia(suma, ilosc)));
				WYNIKI[4] = kurtoza(ilosc, TEMPERATURA, srednia(suma, ilosc), odchylenie_standardowe(ilosc, TEMPERATURA, srednia(suma, ilosc)));
				printf("Srednia temperatura jesienia w Polsce w latach 2004-2014: %.2f\n", WYNIKI[0]); /*srednia	*/
				printf("Mediana temperatury w Polsce w latach 2004-2014: %.2f\n", WYNIKI[1]); /*mediana*/
				printf("Odchylenie standardowe temperatury w Polsce w latach 2004-2014: %.2f\n", WYNIKI[2]); /*odchylenie standardowe*/
				printf("Skosnosc temperatury w Polsce w latach 2004-2014: %.2f\n", WYNIKI[3]); /*skosnosc*/
				printf("Kurtoza temperatury w Polsce w latach 2004-2014: %.2f\n", WYNIKI[4]); /*kurtoza*/
				printf("Skosnosc: %s\n", skosint(skosnosc(ilosc, TEMPERATURA, srednia(suma, ilosc), odchylenie_standardowe(ilosc, TEMPERATURA, srednia(suma, ilosc))))); /*interpretacja skosnosci*/
				printf("Kurtoza: %s\n\n", kurtint(kurtoza(ilosc, TEMPERATURA, srednia(suma, ilosc), odchylenie_standardowe(ilosc, TEMPERATURA, srednia(suma, ilosc))))); /*interpretacja kurtozy*/

				FILE *fp2; /*zapisywanie wynikow do pliku*/
				fp2 = fopen("wyniki.txt", "a");
				if (fp2 == NULL) /*jezeli taki plik nie istnieje*/
				{
					printf("Nie mozna otworzyc pliku do zapisu wynikow\n");
				}
				else /*zapis tablicy wynikow do pliku*/
				{
					fprintf(fp2, "Temperatura jesienia w Polsce w latach 2000-2014:\n");
					fprintf(fp2, "Srednia - %.2f, ", WYNIKI[0]);
					fprintf(fp2, "Mediana - %.2f, ", WYNIKI[1]);
					fprintf(fp2, "Odchylenie standardowe - %.2f, ", WYNIKI[2]);
					fprintf(fp2, "Skosnosc - %.2f, ", WYNIKI[3]);
					fprintf(fp2, "Kurtoza - %.2f\n\n", WYNIKI[4]);
				}
				fclose(fp2); /*zamkniecie pliku zapisu*/
			}

			else if (wybor == 4) /*czesc wczytujaca dane dla kursu zlota 2015 i przeprowadzajaca na nich obliczenia*/
			{
				FILE *fp;
				fp = fopen("kurszlota.txt", "rt");
				if (fp == NULL) /*jezeli taki plik nie istnieje*/
				{
					printf("Nie mozna wczytac pliku\n");
					system("PAUSE");
					exit(0);
				}
				else
				{
					for (i = 0; i < _ROZ4_; i++) /*wczytywanie*/
					{
						fscanf_s(fp, "%f,", &ZLOTO[i]);
					}
					for (i = 0, j = 1; i < _ROZ4_; i++, j++) /*pokazanie danych na ktorych dokonaja sie operacje*/
					{
						printf("Numer miesiaca: ");
						printf("%i - %.0f\n", j, ZLOTO[i]);
					}
					printf("\n");
				}
				fclose(fp); /*zamkniecie pliku z danymi*/

				for (i = 0, suma = 0; i < _ROZ4_; i++) /*zliczanie kursu*/
				{
					suma += ZLOTO[i];
				}

				for (i = 0, ilosc = 0; i < _ROZ4_; i++) /*zliczanie ilosci danych*/
				{
					ilosc += 1;
				}

				WYNIKI[0] = srednia(suma, ilosc);
				WYNIKI[1] = mediana(ilosc, ZLOTO);
				WYNIKI[2] = odchylenie_standardowe(ilosc, ZLOTO, srednia(suma, ilosc));
				WYNIKI[3] = skosnosc(ilosc, ZLOTO, srednia(suma, ilosc), odchylenie_standardowe(ilosc, ZLOTO, srednia(suma, ilosc)));
				WYNIKI[4] = kurtoza(ilosc, ZLOTO, srednia(suma, ilosc), odchylenie_standardowe(ilosc, ZLOTO, srednia(suma, ilosc)));
				printf("Sredni kurs zlota w roku 2015: %.2f\n", WYNIKI[0]); /*srednia	*/
				printf("Mediana kursu zlota w roku 2015: %.2f\n", WYNIKI[1]); /*mediana*/
				printf("Odchylenie standardowe kursu zlota w roku 2015: %.2f\n", WYNIKI[2]); /*odchylenie standardowe*/
				printf("Skosnosc kursu zlota w roku 2015: %.2f\n", WYNIKI[3]); /*skosnosc*/
				printf("Kurtoza kursu zlota w roku 2015: %.2f\n", WYNIKI[4]); /*kurtoza*/
				printf("Skosnosc: %s\n", skosint(skosnosc(ilosc, ZLOTO, srednia(suma, ilosc), odchylenie_standardowe(ilosc, ZLOTO, srednia(suma, ilosc))))); /*interpretacja skosnosci*/
				printf("Kurtoza: %s\n\n", kurtint(kurtoza(ilosc, ZLOTO, srednia(suma, ilosc), odchylenie_standardowe(ilosc, ZLOTO, srednia(suma, ilosc))))); /*interpretacja kurtozy*/

				FILE *fp2; /*zapisywanie wynikow do pliku*/
				fp2 = fopen("wyniki.txt", "a");
				if (fp2 == NULL) /*jezeli taki plik nie istnieje*/
				{
					printf("Nie mozna otworzyc pliku do zapisu wynikow\n");
				}
				else /*zapis tablicy wynikow do pliku*/
				{
					fprintf(fp2, "Kurs zlota w roku 2015:\n");
					fprintf(fp2, "Srednia - %.2f, ", WYNIKI[0]);
					fprintf(fp2, "Mediana - %.2f, ", WYNIKI[1]);
					fprintf(fp2, "Odchylenie standardowe - %.2f, ", WYNIKI[2]);
					fprintf(fp2, "Skosnosc - %.2f, ", WYNIKI[3]);
					fprintf(fp2, "Kurtoza - %.2f\n\n", WYNIKI[4]);
				}
				fclose(fp2); /*zamkniecie pliku zapisu*/
			}

			else if (wybor == 5) /*czesc wczytujaca dane od uzytkownika i przeprowadzajaca na nich obliczenia*/
			{
				int ROZUZ, czy_liczba, przec_count = 0;
				printf("Podaj ilosc danych, ktore wprowadzisz: ");
				scanf_s("%i", &ROZUZ);
				printf("\n");
				float *TABUZ;
				TABUZ = (float*)malloc(ROZUZ * sizeof(*TABUZ)); /*deklaracja dynamicznej tablicy o rozmiarze zadeklarowanym przez uzytkownika*/
				char tab[15];  /* tablica na wczytywanie 1 LICZBY*/

				if (ROZUZ <= 0) /*zabezpieczenie przed nieprawidlowa iloscia danych*/
				{
					printf("Musisz wprowadzic co najmniej dwie wartosci\n");
					system("PAUSE");
					exit(0);
			
				}
				else
				{
					for (i = 0, j = 1; i < ROZUZ; i++, j++) /*wczytywanie wartosci*/
					{
						printf("Wprowadz %i wartosc: ", j);
						scanf("%s", &tab);
						unsigned int k;
						for (k = 0; k < strlen(tab); k++)
						{
							czy_liczba = 1;

							if (tab[k] < 48 || tab[k] > 57){
								if (k == 0 && tab[0] == 45){
								}
								else{
									if (tab[k] == 46){
										przec_count++;
									}
									else{
										czy_liczba = 0;
										break;
									}
								}
							}
						}
						if (przec_count>1){
							czy_liczba = 0;
							przec_count = 0;
						}
						else;
						if (czy_liczba == 1)
						{
							TABUZ[i] = (float)atof(tab);
							przec_count = 0;
						}
						else
						{
							printf("Podana wartosc nie jest liczba\nSprobuj jeszcze raz\n");
							i--;
							j--;
							przec_count = 0;
						}
					}
					printf("\n");
					for (i = 0, j = 1; i < ROZUZ; i++, j++) /*wypisywanie wartosci*/
					{
						printf("%i wartosc - %.2f\n", j, TABUZ[i]);
					}
					printf("\n");
				}

				for (i = 0, suma = 0; i < ROZUZ; i++) /*zliczanie sumy wartosci uzytkownika*/
				{
					suma += TABUZ[i];
				}

				for (i = 0, ilosc = 0; i < ROZUZ; i++) /*zliczanie ilosci danych uzytkownika*/
				{
					ilosc += 1;
				}

				WYNIKI[0] = srednia(suma, ilosc);
				WYNIKI[1] = mediana(ilosc, TABUZ);
				WYNIKI[2] = odchylenie_standardowe(ilosc, TABUZ, srednia(suma, ilosc));
				WYNIKI[3] = skosnosc(ilosc, TABUZ, srednia(suma, ilosc), odchylenie_standardowe(ilosc, TABUZ, srednia(suma, ilosc)));
				WYNIKI[4] = kurtoza(ilosc, TABUZ, srednia(suma, ilosc), odchylenie_standardowe(ilosc, TABUZ, srednia(suma, ilosc)));
				printf("Srednia twoich danych: %.2f\n", WYNIKI[0]); /*srednia	*/
				printf("Mediana twoich danych: %.2f\n", WYNIKI[1]); /*mediana*/
				printf("Odchylenie standardowe twoich danych: %.2f\n", WYNIKI[2]); /*odchylenie standardowe*/
				printf("Skosnosc twoich danych: %.2f\n", WYNIKI[3]); /*skosnosc*/
				printf("Kurtoza twoich danych: %.2f\n", WYNIKI[4]); /*kurtoza*/
				printf("Skosnosc: %s\n", skosint(skosnosc(ilosc, TABUZ, srednia(suma, ilosc), odchylenie_standardowe(ilosc, TABUZ, srednia(suma, ilosc))))); /*interpretacja skosnosci*/
				printf("Kurtoza: %s\n\n", kurtint(kurtoza(ilosc, TABUZ, srednia(suma, ilosc), odchylenie_standardowe(ilosc, TABUZ, srednia(suma, ilosc))))); /*interpretacja kurtozy*/

				FILE *fp; /*zapisywanie wynikow do pliku*/
				fp = fopen("wyniki.txt", "a");
				if (fp == NULL) /*jezeli taki plik nie istnieje*/
				{
					printf("Nie mozna otworzyc pliku do zapisu wynikow\n");
				}
				else /*zapis tablicy wynikow do pliku*/
				{
					fprintf(fp, "Obliczenia dla szeregu wprowadzonego przez uzytkownika:\n");
					for (i = 0, j = 1; i < ROZUZ; i++, j++)
					{
						fprintf(fp, "%i wartosc - %.2f\t", j, TABUZ[i]);
					}
					fprintf(fp, "\nSrednia - %.2f, ", WYNIKI[0]);
					fprintf(fp, "Mediana - %.2f, ", WYNIKI[1]);
					fprintf(fp, "Odchylenie standardowe - %.2f, ", WYNIKI[2]);
					fprintf(fp, "Skosnosc - %.2f, ", WYNIKI[3]);
					fprintf(fp, "Kurtoza - %.2f\n\n", WYNIKI[4]);
				}
				fclose(fp); /*zamkniecie pliku*/
				free(TABUZ); /*zwolnienie miejsca w pamieci*/
			}
			else /*kiedy uzytkownik zle wpisze*/
			{
				printf("Zle wybrales sprobuj ponownie\n\n");
			}
		}

		else if (wybor2 == 2) /*Czesc dla analizy korelacji danych*/
		{
			float korelacja;
			printf("\nPomiedzy jakimi szeregami chcialbys policzyc korelacje?\n1. Liczba emigrantow z Polski w latach 2004-2014\n2. Srednia jesienna temperatura w latach 2004-2014\n3. Liczba ludnosci w Polsce w latach 2004-2014\n4. Korelacja dla wlasnych danych(Wpisac 4 w jednym z wybieranych szeregow)\n");
			printf("Wybierz pierwszy szereg: ");
			scanf_s("%i", &wyborszer1);
			printf("Wybierdz drugi szereg: ");
			scanf_s("%i", &wyborszer2);

			if ((wyborszer1 == 1 && wyborszer2 == 2) || (wyborszer1 == 2 && wyborszer2 == 1))
			{
				printf("\n");
				FILE *fp;
				fp = fopen("emigracjapolska.txt", "rt");
				if (fp == NULL)
				{
					printf("Nie mozna wczytac pliku\n");
					system("PAUSE");
					exit(0);
				}
				else
				{
					printf("Liczba emigrantow:\n");
					for (i = 0; i < _ROZ2_; i++) /*wczytywanie*/
					{
						fscanf_s(fp, "%f,", &EMIGRACJA[i]);
					}
					for (i = 0, j = 2004; i < _ROZ2_; i++, j++) /*pokazanie danych na ktorych dokonaja sie operacje*/
					{
						printf("%i - %.0f\n", j, EMIGRACJA[i]);
					}
					printf("\n");
				}
				fclose(fp);

				FILE *fp2;
				fp2 = fopen("temperatura.txt", "rt");
				if (fp2 == NULL)
				{
					printf("Nie mozna wczytac pliku\n");
					system("PAUSE");
					exit(0);
				}
				else
				{
					printf("Srednia jesienna temperatura:\n");
					for (i = 0; i < _ROZ3_; i++) /*wczytywanie*/
					{
						fscanf_s(fp2, "%f,", &TEMPERATURA[i]);
					}
					for (i = 0, j = 2004; i < _ROZ3_; i++, j++) /*pokazanie danych na ktorych dokonaja sie operacje*/
					{
						printf("%i - %.1f\n", j, TEMPERATURA[i]);
					}
					printf("\n");
				}
				fclose(fp);

				for (i = 0, ilosc = 0; i < _ROZ2_; i++)
				{
					ilosc += 1;
				}
				for (i = 0, suma1 = 0; i < _ROZ2_; i++)
				{
					suma1 += EMIGRACJA[i];
				}
				for (i = 0, suma2 = 0; i < _ROZ3_; i++)
				{
					suma2 += TEMPERATURA[i];
				}

				korelacja = korelacja_danych(ilosc, EMIGRACJA, TEMPERATURA, srednia(suma1, ilosc), srednia(suma2, ilosc));
				printf("Wspolczynnik korelacji dla tych 2 szeregow wynosi: %.2f\n\n", korelacja);

				FILE *fp3; /*zapisywanie wynikow do pliku*/
				fp3 = fopen("wyniki.txt", "a");
				if (fp3 == NULL) /*jezeli taki plik nie istnieje*/
				{
					printf("Nie mozna otworzyc pliku do zapisu wynikow\n");
				}
				else /*zapis wyniku korelacji do pliku*/
				{
					fprintf(fp3, "\nWpolczynnik korelacji miedzy liczba emigrantow a srednia jesienna temperatura w latach 2004-2014: %.2f\n\n", korelacja);
				}
				fclose(fp3); /*zamkniecie pliku zapisu*/
			}

			else if ((wyborszer1 == 1 && wyborszer2 == 3) || (wyborszer1 == 3 && wyborszer2 == 1))
			{
				printf("\n");
				FILE *fp;
				fp = fopen("emigracjapolska.txt", "rt");
				if (fp == NULL)
				{
					printf("Nie mozna wczytac pliku\n");
					system("PAUSE");
					exit(0);
				}
				else
				{
					printf("Liczba emigrantow:\n");
					for (i = 0; i < _ROZ2_; i++) /*wczytywanie*/
					{
						fscanf_s(fp, "%f,", &EMIGRACJA[i]);
					}
					for (i = 0, j = 2004; i < _ROZ2_; i++, j++) /*pokazanie danych na ktorych dokonaja sie operacje*/
					{
						printf("%i - %.0f\n", j, EMIGRACJA[i]);
					}
					printf("\n");
				}
				fclose(fp);

				FILE *fp2;
				fp2 = fopen("ludnosc ogolem.txt", "rt");
				if (fp2 == NULL)
				{
					printf("Nie mozna wczytac pliku\n");
					system("PAUSE");
					exit(0);
				}
				else
				{
					printf("Liczba ludnosci:\n");
					for (i = 0; i < _ROZ1_; i++) /*wczytywanie*/
					{
						fscanf_s(fp2, "%f,", &LUDNOSC[i]);
					}
					for (i = 4, j = 2004; i < _ROZ1_; i++, j++) /*pokazanie danych na ktorych dokonaja sie operacje*/
					{
						printf("%i - %.0f\n", j, LUDNOSC[i]);
					}
					printf("\n");
				}
				fclose(fp);

				float LUDNOSCKOR[11];
				for (i = 0, j = 4; i < 11; i++, j++)
				{
					LUDNOSCKOR[i] = LUDNOSC[j];
				}
				for (i = 0, ilosc = 0; i < _ROZ2_; i++)
				{
					ilosc += 1;
				}
				for (i = 0, suma1 = 0; i < _ROZ2_; i++)
				{
					suma1 += EMIGRACJA[i];
				}
				for (i = 0, suma2 = 0; i < 11; i++)
				{
					suma2 += LUDNOSCKOR[i];
				}

				korelacja = korelacja_danych(ilosc, EMIGRACJA, LUDNOSCKOR, srednia(suma1, ilosc), srednia(suma2, ilosc));
				printf("Wspolczynnik korelacji dla tych 2 szeregow wynosi: %.2f\n\n", korelacja);

				FILE *fp3; /*zapisywanie wynikow do pliku*/
				fp3 = fopen("wyniki.txt", "a");
				if (fp3 == NULL) /*jezeli taki plik nie istnieje*/
				{
					printf("Nie mozna otworzyc pliku do zapisu wynikow\n");
				}
				else /*zapis wyniku korelacji do pliku*/
				{
					fprintf(fp3, "\nWpolczynnik korelacji miedzy liczba ludnosci a liczba emigrantow w Polsce w latach 2004-2014: %.2f\n\n", korelacja);
				}
				fclose(fp3); /*zamkniecie pliku zapisu*/
			}

			else if ((wyborszer1 == 2 && wyborszer2 == 3) || (wyborszer1 == 3 && wyborszer2 == 2))
			{
				printf("\n");
				FILE *fp;
				fp = fopen("temperatura.txt", "rt");
				if (fp == NULL)
				{
					printf("Nie mozna wczytac pliku\n");
					system("PAUSE");
					exit(0);
				}
				else
				{
					printf("Srednia temperatura jesienia:\n");
					for (i = 0; i < _ROZ2_; i++) /*wczytywanie*/
					{
						fscanf_s(fp, "%f,", &TEMPERATURA[i]);
					}
					for (i = 0, j = 2004; i < _ROZ2_; i++, j++) /*pokazanie danych na ktorych dokonaja sie operacje*/
					{
						printf("%i - %.0f\n", j, TEMPERATURA[i]);
					}
					printf("\n");
				}
				fclose(fp);

				FILE *fp2;
				fp2 = fopen("ludnosc ogolem.txt", "rt");
				if (fp2 == NULL)
				{
					printf("Nie mozna wczytac pliku\n");
					system("PAUSE");
					exit(0);
				}
				else
				{
					printf("Liczba ludnosci:\n");
					for (i = 0; i < _ROZ1_; i++) /*wczytywanie*/
					{
						fscanf_s(fp2, "%f,", &LUDNOSC[i]);
					}
					for (i = 4, j = 2004; i < _ROZ1_; i++, j++) /*pokazanie danych na ktorych dokonaja sie operacje*/
					{
						printf("%i - %.0f\n", j, LUDNOSC[i]);
					}
					printf("\n");
				}
				fclose(fp);

				float LUDNOSCKOR[11];
				for (i = 0, j = 4; i < 11; i++, j++)
				{
					LUDNOSCKOR[i] = LUDNOSC[j];
				}
				for (i = 0, ilosc = 0; i < _ROZ2_; i++)
				{
					ilosc += 1;
				}
				for (i = 0, suma1 = 0; i < _ROZ2_; i++)
				{
					suma1 += TEMPERATURA[i];
				}
				for (i = 0, suma2 = 0; i < 11; i++)
				{
					suma2 += LUDNOSCKOR[i];
				}

				korelacja = korelacja_danych(ilosc, TEMPERATURA, LUDNOSCKOR, srednia(suma1, ilosc), srednia(suma2, ilosc));
				printf("Wspolczynnik korelacji dla tych 2 szeregow wynosi: %.2f\n\n", korelacja);

				FILE *fp3; /*zapisywanie wynikow do pliku*/
				fp3 = fopen("wyniki.txt", "a");
				if (fp3 == NULL) /*jezeli taki plik nie istnieje*/
				{
					printf("Nie mozna otworzyc pliku do zapisu wynikow\n");
				}
				else /*zapis wyniku korelacji do pliku*/
				{
					fprintf(fp3, "\nWpolczynnik korelacji miedzy liczba ludnosci a srednia temperatura jesienia 2004-2014: %.2f\n\n", korelacja);
				}
				fclose(fp3); /*zamkniecie pliku zapisu*/
			}

			else if (wyborszer1 == 4 || wyborszer2 == 4)
			{
				{
					int ROZUZ, czy_liczba, przec_count = 0;
					printf("\nPodaj ilosc danych, ktore wprowadzisz: ");
					scanf_s("%i", &ROZUZ);
					printf("\n");
					float *TABUZ;
					TABUZ = (float*)malloc(ROZUZ * sizeof(*TABUZ)); /*deklaracja dynamicznej tablicy o rozmiarze zadeklarowanym przez uzytkownika*/
					float *TABUZ2;
					TABUZ2 = (float*)malloc(ROZUZ * sizeof(*TABUZ2)); /*deklaracja dynamicznej drugiej tablicy o rozmiarze zadeklarowanym przez uzytkownika*/
					char tab[15];  /* tablica na wczytywanie 1 LICZBY*/

					if (ROZUZ <= 1) /*zabezpieczenie przed nieprawidlowa iloscia danych*/
					{
						printf("Musisz wprowadzic co najmniej dwie wartosci\n");
						system("PAUSE");
						exit(0);
					}
					else
					{
						for (i = 0, j = 1; i < ROZUZ; i++, j++) /*wczytywanie wartosci*/
						{
							printf("Wprowadz %i wartosc do 1 szeregu: ", j);
							scanf("%s", &tab);
							unsigned int k;
							for (k = 0; k<strlen(tab); k++)
							{
								czy_liczba = 1;

								if (tab[k] < 48 || tab[k] > 57){
									if (k == 0 && tab[0] == 45){
									}
									else{
										if (tab[k] == 46){
											przec_count++;
										}
										else{
											czy_liczba = 0;
											break;
										}
									}
								}
							}
							if (przec_count>1){
								czy_liczba = 0;
								przec_count = 0;
							}
							else;
							if (czy_liczba == 1)
							{
								TABUZ[i] = (float)atof(tab);
								przec_count = 0;
							}
							else
							{
								printf("Podana wartosc nie jest liczba\nSprobuj jeszcze raz\n");
								i--;
								j--;
								przec_count = 0;
							}
						}
						printf("\n");
						for (i = 0, j = 1; i < ROZUZ; i++, j++) /*wczytywanie wartosci*/
						{
							printf("Wprowadz %i wartosc do 2 szeregu: ", j);
							scanf("%s", &tab);
							unsigned int k;
							for (k = 0; k<strlen(tab); k++)
							{
								czy_liczba = 1;

								if (tab[k] < 48 || tab[k] > 57){
									if (k == 0 && tab[0] == 45){
									}
									else{
										if (tab[k] == 46){
											przec_count++;
										}
										else{
											czy_liczba = 0;
											break;
										}
									}
								}
							}
							if (przec_count>1){
								czy_liczba = 0;
								przec_count = 0;
							}
							else;
							if (czy_liczba == 1)
							{
								TABUZ2[i] = (float)atof(tab);
								przec_count = 0;
							}
							else
							{
								printf("Podana wartosc nie jest liczba\nSprobuj jeszcze raz\n");
								i--;
								j--;
								przec_count = 0;
							}
						}
						printf("\n");
						for (i = 0, j = 1; i < ROZUZ; i++, j++) /*wypisywanie wartosci pierwszego szeregu*/
						{
							printf("%i wartosc - %.2f\n", j, TABUZ[i]);
						}
						printf("\n");
						for (i = 0, j = 1; i < ROZUZ; i++, j++) /*wypisywanie wartosci drugiego szeregu*/
						{
							printf("%i wartosc - %.2f\n", j, TABUZ2[i]);
						}
						printf("\n");
					}

					for (i = 0, suma1 = 0; i < ROZUZ; i++) /*zliczanie sumy wartosci uzytkownika*/
					{
						suma1 += TABUZ[i];
					}
					for (i = 0, suma2 = 0; i < ROZUZ; i++) /*zliczanie sumy wartosci uzytkownika*/
					{
						suma2 += TABUZ2[i];
					}
					for (i = 0, ilosc = 0; i < ROZUZ; i++) /*zliczanie ilosci danych uzytkownika*/
					{
						ilosc += 1;
					}

					korelacja = korelacja_danych(ilosc, TABUZ, TABUZ2, srednia(suma1, ilosc), srednia(suma2, ilosc));
					printf("Korelacja danych w twoich szeregach wynosi: %.2f\n\n", korelacja);

					FILE *fp; /*zapisywanie wynikow do pliku*/
					fp = fopen("wyniki.txt", "a");
					if (fp == NULL) /*jezeli taki plik nie istnieje*/
					{
						printf("Nie mozna otworzyc pliku do zapisu wynikow\n");
					}
					else /*zapis tablicy wynikow do pliku*/
					{
						fprintf(fp, "\nObliczenia korelacji dla szeregu wprowadzonego przez uzytkownika:\n");
						for (i = 0, j = 1; i < ROZUZ; i++, j++)
						{
							fprintf(fp, "%i wartosc w 1 szeregu - %.2f\t", j, TABUZ[i]);
						}
						for (i = 0, j = 1; i < ROZUZ; i++, j++)
						{
							fprintf(fp, "%i wartosc w 2 szeregu - %.2f\t", j, TABUZ2[i]);
						}
						fprintf(fp, "\nKorelacja danych w szeregach uzytkownika - %.2f \n\n", korelacja);
					}
					fclose(fp); /*zamkniecie pliku*/
					free(TABUZ); /*zwolnienie miejsca w pamieci*/
					free(TABUZ2); /*zwolnienie miejsca w pamieci*/
				}
			}
			else
			{
				printf("\nZle wybrales szeregi do analizy\n\n");
			}
		}

		else if (wybor2 == 3) /*czesc dla analizy okien ruchomych*/
		{
			printf("\nJakie dane chcialbys przeanalizowac?\n1. Liczba ludnosci w Polsce w latach 2000-2014\n2. Liczba emigrantow z Polski w latach 2004-2014\n3. Srednia jesienna temperatura w latach 2004-2014\n4. Kurs zlota w roku 2015\n5. Chce wprowadzic swoje wlasne dane\n"); /*wybór szeregow do liczenia*/
			scanf_s("%i", &wybor);
			printf("\n");

			if (wybor == 1) /*czesc wczytujaca dane dla liczby ludnosci i przeprowadzajaca na nich obliczenia*/
			{
				FILE *fp;
				fp = fopen("ludnosc ogolem.txt", "rt");
				if (fp == NULL) /*jezeli taki plik nie istnieje*/
				{
					printf("Nie mozna wczytac pliku\n");
					system("PAUSE");
					exit(0);
				}
				else
				{
					for (i = 0; i < _ROZ1_; i++) /*wczytywanie*/
					{
						fscanf_s(fp, "%f,", &LUDNOSC[i]);
					}
					for (i = 0, j = 2000; i < _ROZ1_; i++, j++) /*pokazanie danych na ktorych dokonaja sie operacje*/
					{
						printf("%i - %.0f\n", j, LUDNOSC[i]);
					}
					printf("\n");
				}
				fclose(fp); /*zamkniecie pliku z danymi*/
				
				int p, i, k;
				float suma2 = 0, wynik2;
				const char *wynik3;
				printf("Podaj szerokosc okna: ");
				scanf("%d", &p);
				if (p < 2 || p > _ROZ1_) /*zabezpieczenie przed zla szerokoscia okna*/
				{
					printf("Zla szerokosc okna\n\n");
				}
				else
				{
					float *tempTAB;
					tempTAB = (float*)malloc(p); /*deklaracja tablicy o szerokosci okna*/
					FILE *fp2; /*zapisywanie wynikow do pliku*/
					fp2 = fopen("wyniki.txt", "a");
					if (fp2 == NULL) /*jezeli taki plik nie istnieje*/
					{
						printf("Nie mozna otworzyc pliku do zapisu wynikow\n");
					}
					else
					{
						fprintf(fp2, "\nLiczba ludnosci w Polsce w latach 2000-2014:\n");
						for (i = 0; i + p <= _ROZ1_; i++)
						{
							suma2 = 0;
							for (k = 0; k < p; k++)
							{
								suma2 += LUDNOSC[i + k];
								tempTAB[k] = LUDNOSC[i + k];
							}
							printf("\n~~~~ Wyniki w prezdziale %d - %d~~~~\n", i + 1, i + p);
							fprintf(fp2, "\n~~~~ Wyniki w prezdziale %d - %d~~~~\n", i + 1, i + p);
							wynik2 = srednia(suma2, p);
							printf("Srednia: %.2f\n", wynik2);
							fprintf(fp2, "Srednia - %.2f, ", wynik2);
							wynik2 = mediana(p, tempTAB);
							fprintf(fp2, "Mediana - %.2f, ", wynik2);
							printf("Mediana: %.2f\n", wynik2);
							wynik2 = odchylenie_standardowe(p, tempTAB, srednia(suma2, p));
							printf("Odchylenie standardowe: %.2f\n", wynik2);
							fprintf(fp2, "Odchylenie standardowe - %.2f, ", wynik2);
							if (p == 3)
							{
								wynik2 = skosnosc(p, tempTAB, srednia(suma2, p), odchylenie_standardowe(p, tempTAB, srednia(suma2, p)));
								printf("Skosnosc: %.2f\n", wynik2);
								fprintf(fp2, "Skosnosc - %.2f\n", wynik2);
								wynik3 = skosint(skosnosc(p, tempTAB, srednia(suma2, p), odchylenie_standardowe(p, tempTAB, srednia(suma2, p))));
								printf("Skosnosc: %s\n", wynik3);
							}
							else if (p > 3)
							{
								wynik2 = skosnosc(p, tempTAB, srednia(suma2, p), odchylenie_standardowe(p, tempTAB, srednia(suma2, p)));
								printf("Skosnosc: %.2f\n", wynik2);
								fprintf(fp2, "Skosnosc - %.2f, ", wynik2);
								wynik3 = skosint(skosnosc(p, tempTAB, srednia(suma2, p), odchylenie_standardowe(p, tempTAB, srednia(suma2, p))));
								printf("Skosnosc: %s\n", wynik3);
								wynik2 = kurtoza(p, tempTAB, srednia(suma2, p), odchylenie_standardowe(p, tempTAB, srednia(suma2, p)));
								printf("Kurtoza: %.2f\n", wynik2);
								fprintf(fp2, "Kurtoza - %.2f\n", wynik2);
								wynik3 = kurtint(kurtoza(p, tempTAB, srednia(suma2, p), odchylenie_standardowe(p, tempTAB, srednia(suma2, p))));
								printf("Kurtoza: %s\n", wynik3);
							}
						}
					}
					fclose(fp2); /*zamkniecie pliku zapisu*/
				}
			}

			else if (wybor == 2) /*czesc wczytujaca dane dla emigracji ludzi z Polski i przeprowadzajaca na nich obliczenia*/
			{
				FILE *fp;
				fp = fopen("emigracjapolska.txt", "rt");
				if (fp == NULL) /*jezeli taki plik nie istnieje*/
				{
					printf("Nie mozna wczytac pliku\n");
					system("PAUSE");
					exit(0);
				}
				else
				{
					for (i = 0; i < _ROZ2_; i++) /*wczytywanie*/
					{
						fscanf_s(fp, "%f,", &EMIGRACJA[i]);
					}
					for (i = 0, j = 2004; i < _ROZ2_; i++, j++) /*pokazanie danych na ktorych dokonaja sie operacje*/
					{
						printf("%i - %.0f\n", j, EMIGRACJA[i]);
					}
					printf("\n");
				}
				fclose(fp); /*zamkniecie pliku z danymi*/
				
				int p, i, k;
				float suma2 = 0, wynik2;
				const char *wynik3;
				printf("Podaj szerokosc okna: ");
				scanf("%d", &p);
				if (p < 2 || p > _ROZ2_) /*zabezpieczenie przed zla szerokoscia okna*/
				{
					printf("Zla szerokosc okna\n\n");
				}
				else
				{
					float *tempTAB;
					tempTAB = (float*)malloc(p); /*deklaracja tablicy o szerokosci okna*/
					FILE *fp2; /*zapisywanie wynikow do pliku*/
					fp2 = fopen("wyniki.txt", "a");
					if (fp2 == NULL) /*jezeli taki plik nie istnieje*/
					{
						printf("Nie mozna otworzyc pliku do zapisu wynikow\n");
					}
					else
					{
						fprintf(fp2, "\nEmigracja z Polski:\n");
						for (i = 0; i + p <= _ROZ2_; i++)
						{
							suma2 = 0;
							for (k = 0; k < p; k++)
							{
								suma2 += EMIGRACJA[i + k];
								tempTAB[k] = EMIGRACJA[i + k];
							}
							printf("\n~~~~ Wyniki w prezdziale %d - %d~~~~\n", i + 1, i + p);
							fprintf(fp2, "\n~~~~ Wyniki w prezdziale %d - %d~~~~\n", i + 1, i + p);
							wynik2 = srednia(suma2, p);
							printf("Srednia: %.2f\n", wynik2);
							fprintf(fp2, "Srednia: %.2f, ", wynik2);
							wynik2 = mediana(p, tempTAB);
							fprintf(fp2, "Mediana - %.2f, ", wynik2);
							printf("Mediana: %.2f\n", wynik2);
							wynik2 = odchylenie_standardowe(p, tempTAB, srednia(suma2, p));
							fprintf(fp2, "Odchylenie standardowe - %.2f, ", wynik2);
							printf("Odchylenie standardowe: %.2f\n", wynik2);
							if (p == 3)
							{
								wynik2 = skosnosc(p, tempTAB, srednia(suma2, p), odchylenie_standardowe(p, tempTAB, srednia(suma2, p)));
								printf("Skosnosc: %.2f\n", wynik2);
								fprintf(fp2, "Skosnosc - %.2f\n", wynik2);
								wynik3 = skosint(skosnosc(p, tempTAB, srednia(suma2, p), odchylenie_standardowe(p, tempTAB, srednia(suma2, p))));
								printf("Skosnosc: %s\n", wynik3);
							}
							else if (p > 3)
							{
								wynik2 = skosnosc(p, tempTAB, srednia(suma2, p), odchylenie_standardowe(p, tempTAB, srednia(suma2, p)));
								printf("Skosnosc: %.2f\n", wynik2);
								fprintf(fp2, "Skosnosc - %.2f, ", wynik2);
								wynik3 = skosint(skosnosc(p, tempTAB, srednia(suma2, p), odchylenie_standardowe(p, tempTAB, srednia(suma2, p))));
								printf("Skosnosc: %s\n", wynik3);
								wynik2 = kurtoza(p, tempTAB, srednia(suma2, p), odchylenie_standardowe(p, tempTAB, srednia(suma2, p)));
								printf("Kurtoza: %.2f\n", wynik2);
								fprintf(fp2, "Kurtoza - %.2f\n", wynik2);
								wynik3 = kurtint(kurtoza(p, tempTAB, srednia(suma2, p), odchylenie_standardowe(p, tempTAB, srednia(suma2, p))));
								printf("Kurtoza: %s\n", wynik3);
							}
						}
					}
					fclose(fp2); /*zamkniecie pliku zapisu*/
				}
			}

			else if (wybor == 3) /*czesc wczytujaca dane dla sredniej jesiennej temperatury 2004-2014 i przeprowadzajaca na nich obliczenia*/
			{
				FILE *fp;
				fp = fopen("temperatura.txt", "rt");
				if (fp == NULL) /*jezeli taki plik nie istnieje*/
				{
					printf("Nie mozna wczytac pliku\n");
					system("PAUSE");
					exit(0);
				}
				else
				{
					for (i = 0; i < _ROZ3_; i++) /*wczytywanie*/
					{
						fscanf_s(fp, "%f,", &TEMPERATURA[i]);
					}
					for (i = 0, j = 2004; i < 11; i++, j++) /*pokazanie danych na ktorych dokonaja sie operacje*/
					{
						printf("%i - %.1f\n", j, TEMPERATURA[i]);
					}
					printf("\n");
				}
				fclose(fp); /*zamkniecie pliku z danymi*/

				int p, i, k;
				float suma2 = 0, wynik2;
				const char *wynik3;
				printf("Podaj szerokosc okna: ");
				scanf("%d", &p);
				float *tempTAB;
				tempTAB = (float*)malloc(p); /*deklaracja tablicy o szerokosci okna*/
				if (p < 2 || p > _ROZ3_) /*zabezpieczenie przed zla szerokoscia okna*/
				{
					printf("Zla szerokosc okna\n\n");
				}
				else
				{
					FILE *fp2; /*zapisywanie wynikow do pliku*/
					fp2 = fopen("wyniki.txt", "a");
					if (fp2 == NULL) /*jezeli taki plik nie istnieje*/
					{
						printf("Nie mozna otworzyc pliku do zapisu wynikow\n");
					}
					else
					{
						fprintf(fp2, "\nSrednia jesienna temperatura w Polsce:\n");
						for (i = 0; i + p <= _ROZ3_; i++)
						{
							suma2 = 0;
							for (k = 0; k < p; k++)
							{
								suma2 += TEMPERATURA[i + k];
								tempTAB[k] = TEMPERATURA[i + k];
							}
							printf("\n~~~~ Wyniki w prezdziale %d - %d~~~~\n", i + 1, i + p);
							fprintf(fp2, "\n~~~~ Wyniki w prezdziale %d - %d~~~~\n", i + 1, i + p);
							wynik2 = srednia(suma2, p);
							printf("Srednia: %.2f\n", wynik2);
							fprintf(fp2, "Srednia: %.2f, ", wynik2);
							wynik2 = mediana(p, tempTAB);
							printf("Mediana: %.2f\n", wynik2);
							fprintf(fp2, "Mediana: %.2f, ", wynik2);
							wynik2 = odchylenie_standardowe(p, tempTAB, srednia(suma2, p));
							printf("Odchylenie standardowe: %.2f\n", wynik2);
							fprintf(fp2, "Odchylenie standardowe: %.2f", wynik2);
							if (p == 3)
							{
								wynik2 = skosnosc(p, tempTAB, srednia(suma2, p), odchylenie_standardowe(p, tempTAB, srednia(suma2, p)));
								printf("Skosnosc: %.2f\n", wynik2);
								fprintf(fp2, "Skosnosc: %.2f\n", wynik2);
								wynik3 = skosint(skosnosc(p, tempTAB, srednia(suma2, p), odchylenie_standardowe(p, tempTAB, srednia(suma2, p))));
								printf("Skosnosc: %s\n", wynik3);
							}
							else if (p > 3)
							{
								wynik2 = skosnosc(p, tempTAB, srednia(suma2, p), odchylenie_standardowe(p, tempTAB, srednia(suma2, p)));
								printf("Skosnosc: %.2f\n", wynik2);
								fprintf(fp2, "Skosnosc: %.2f, ", wynik2);
								wynik3 = skosint(skosnosc(p, tempTAB, srednia(suma2, p), odchylenie_standardowe(p, tempTAB, srednia(suma2, p))));
								printf("Skosnosc: %s\n", wynik3);
								wynik2 = kurtoza(p, tempTAB, srednia(suma2, p), odchylenie_standardowe(p, tempTAB, srednia(suma2, p)));
								printf("Kurtoza: %.2f\n", wynik2);
								fprintf(fp2, "Kurtoza: %.2f\n", wynik2);
								wynik3 = kurtint(kurtoza(p, tempTAB, srednia(suma2, p), odchylenie_standardowe(p, tempTAB, srednia(suma2, p))));
								printf("Kurtoza: %s\n", wynik3);
							}
						}
					}
					fclose(fp2);
				}
			}

			else if (wybor == 4) /*czesc wczytujaca dane dla kursu zlota 2015 i przeprowadzajaca na nich obliczenia*/
			{
				FILE *fp;
				fp = fopen("kurszlota.txt", "rt");
				if (fp == NULL) /*jezeli taki plik nie istnieje*/
				{
					printf("Nie mozna wczytac pliku\n");
					system("PAUSE");
					exit(0);
				}
				else
				{
					for (i = 0; i < _ROZ4_; i++) /*wczytywanie*/
					{
						fscanf_s(fp, "%f,", &ZLOTO[i]);
					}
					for (i = 0, j = 1; i < _ROZ4_; i++, j++) /*pokazanie danych na ktorych dokonaja sie operacje*/
					{
						printf("Numer miesiaca: ");
						printf("%i - %.0f\n", j, ZLOTO[i]);
					}
					printf("\n");
				}
				fclose(fp); /*zamkniecie pliku z danymi*/
				
				int p, i, k;
				float suma2 = 0, wynik2;
				const char *wynik3;
				printf("Podaj szerokosc okna: ");
				scanf("%d", &p);
				float *tempTAB;
				tempTAB = (float*)malloc(p); /*deklaracja tablicy o szerokosci okna*/
				if (p < 2 || p > _ROZ4_) /*zabezpieczenie przed zla szerokoscia okna*/
				{
					printf("Zla szerokosc okna\n\n");
				}
				else
				{
					FILE *fp2; /*zapisywanie wynikow do pliku*/
					fp2 = fopen("wyniki.txt", "a");
					if (fp2 == NULL) /*jezeli taki plik nie istnieje*/
					{
						printf("Nie mozna otworzyc pliku do zapisu wynikow\n");
					}
					else
					{
						fprintf(fp2, "\nSredni kurs zlota:\n");
						for (i = 0; i + p <= _ROZ4_; i++)
						{
							suma2 = 0;
							for (k = 0; k < p; k++)
							{
								suma2 += ZLOTO[i + k];
								tempTAB[k] = ZLOTO[i + k];
							}
							printf("\n~~~~ Wyniki w prezdziale %d - %d~~~~\n", i + 1, i + p);
							fprintf(fp2, "\n~~~~ Wyniki w prezdziale %d - %d~~~~\n", i + 1, i + p);
							wynik2 = srednia(suma2, p);
							printf("Srednia: %.2f\n", wynik2);
							fprintf(fp2, "Srednia: %.2f, ", wynik2);
							wynik2 = mediana(p, tempTAB);
							printf("Mediana: %.2f\n", wynik2);
							fprintf(fp2, "Mediana: %.2f, ", wynik2);
							wynik2 = odchylenie_standardowe(p, tempTAB, srednia(suma2, p));
							printf("Odchylenie standardowe: %.2f\n", wynik2);
							fprintf(fp2, "Odchylenie standardowe: %.2f, ", wynik2);
							if (p == 3)
							{
								wynik2 = skosnosc(p, tempTAB, srednia(suma2, p), odchylenie_standardowe(p, tempTAB, srednia(suma2, p)));
								printf("Skosnosc: %.2f\n", wynik2);
								fprintf(fp2, "Skosnosc: %.2f\n", wynik2);
								wynik3 = skosint(skosnosc(p, tempTAB, srednia(suma2, p), odchylenie_standardowe(p, tempTAB, srednia(suma2, p))));
								printf("Skosnosc: %s\n", wynik3);
							}
							else if (p > 3)
							{
								wynik2 = skosnosc(p, tempTAB, srednia(suma2, p), odchylenie_standardowe(p, tempTAB, srednia(suma2, p)));
								printf("Skosnosc: %.2f\n", wynik2);
								fprintf(fp2, "Skosnosc: %.2f, ", wynik2);
								wynik3 = skosint(skosnosc(p, tempTAB, srednia(suma2, p), odchylenie_standardowe(p, tempTAB, srednia(suma2, p))));
								printf("Skosnosc: %s\n", wynik3);
								wynik2 = kurtoza(p, tempTAB, srednia(suma2, p), odchylenie_standardowe(p, tempTAB, srednia(suma2, p)));
								printf("Kurtoza: %.2f\n", wynik2);
								fprintf(fp2, "Kurtoza: %.2f\n", wynik2);
								wynik3 = kurtint(kurtoza(p, tempTAB, srednia(suma2, p), odchylenie_standardowe(p, tempTAB, srednia(suma2, p))));
								printf("Kurtoza: %s\n", wynik3);
							}
						}
					}
					fclose(fp2);
				}
			}

			else if (wybor == 5) /*czesc wczytujaca dane od uzytkownika i przeprowadzajaca na nich obliczenia*/
			{
				int ROZUZ, czy_liczba, przec_count = 0;
				printf("Podaj ilosc danych, ktore wprowadzisz: ");
				scanf_s("%i", &ROZUZ);
				printf("\n");
				float *TABUZ;
				TABUZ = (float*)malloc(ROZUZ * sizeof(*TABUZ)); /*deklaracja dynamicznej tablicy o rozmiarze zadeklarowanym przez uzytkownika*/
				char tab[15];  /* tablica na wczytywanie 1 LICZBY*/

				if (ROZUZ <= 0) /*zabezpieczenie przed nieprawidlowa iloscia danych*/
				{
					printf("Musisz wprowadzic co najmniej dwie wartosci\n");
					system("PAUSE");
					exit(0);
				}
				else
				{
					for (i = 0, j = 1; i < ROZUZ; i++, j++) /*wczytywanie wartosci*/
					{
						printf("Wprowadz %i wartosc: ", j);
						scanf("%s", &tab);
						unsigned int k;
						for (k = 0; k<strlen(tab); k++)
						{
							czy_liczba = 1;

							if (tab[k] < 48 || tab[k] > 57){
								if (k == 0 && tab[0] == 45){
								}
								else{
									if (tab[k] == 46){
										przec_count++;
									}
									else{
										czy_liczba = 0;
										break;
									}
								}
							}
						}
						if (przec_count>1){
							czy_liczba = 0;
							przec_count = 0;
						}
						else;
						if (czy_liczba == 1)
						{
							TABUZ[i] = (float)atof(tab);
							przec_count = 0;
						}
						else
						{
							printf("Podana wartosc nie jest liczba\nSprobuj jeszcze raz\n");
							i--;
							j--;
							przec_count = 0;
						}
					}
					printf("\n");
					for (i = 0, j = 1; i < ROZUZ; i++, j++) /*wypisywanie wartosci*/
					{
						printf("%i wartosc - %.2f\n", j, TABUZ[i]);
					}
					printf("\n");
				}

				int p, i, k;
				float suma2 = 0, wynik2;
				const char *wynik3;
				printf("Podaj szerokosc okna: ");
				scanf("%d", &p);
				float *tempTAB;
				tempTAB = (float*)malloc(p); /*deklaracja tablicy o szerokosci okna*/
				if (p < 2 || p > ROZUZ) /*zabezpieczenie przed zla szerokoscia okna*/
				{
					printf("Zla szerokosc okna\n\n");
				}
				else
				{
					FILE *fp; /*zapisywanie wynikow do pliku*/
					fp = fopen("wyniki.txt", "a");
					if (fp == NULL) /*jezeli taki plik nie istnieje*/
					{
						printf("Nie mozna otworzyc pliku do zapisu wynikow\n");
					}
					else
					{
						fprintf(fp, "\nObliczenia dla danych wprowadzonych przez uzytkownika:\n");
						for (i = 0; i < ROZUZ; i++)
						{
							fprintf(fp, "Wartosc %i - %.2f, ", i+1, TABUZ[i]);
						}
						fprintf(fp, "\n");
						for (i = 0; (i + p) <= ROZUZ; i++)
						{
							suma2 = 0;
							for (k = 0; k < p; k++)
							{
								suma2 += TABUZ[i + k];
								tempTAB[k] = TABUZ[i + k];
							}
							printf("\n~~~~ Wyniki w prezdziale %d - %d~~~~\n", i + 1, i + p);
							fprintf(fp, "\n~~~~ Wyniki w prezdziale %d - %d~~~~\n", i + 1, i + p);
							wynik2 = srednia(suma2, p);
							printf("Srednia: %.2f\n", wynik2);
							fprintf(fp, "Srednia: %.2f, ", wynik2);
							wynik2 = mediana(p, tempTAB);
							printf("Mediana: %.2f\n", wynik2);
							fprintf(fp, "Mediana: %.2f, ", wynik2);
							wynik2 = odchylenie_standardowe(p, tempTAB, srednia(suma2, p));
							printf("Odchylenie standardowe: %.2f\n", wynik2);
							fprintf(fp, "Odchylenie standardowe: %.2f, ", wynik2);
							if (p == 3)
							{
								wynik2 = skosnosc(p, tempTAB, srednia(suma2, p), odchylenie_standardowe(p, tempTAB, srednia(suma2, p)));
								printf("Skosnosc: %.2f\n", wynik2);
								fprintf(fp, "Skosnosc: %.2f\n", wynik2);
								wynik3 = skosint(skosnosc(p, tempTAB, srednia(suma2, p), odchylenie_standardowe(p, tempTAB, srednia(suma2, p))));
								printf("Skosnosc: %s\n", wynik3);
							}
							else if (p > 3)
							{
								wynik2 = skosnosc(p, tempTAB, srednia(suma2, p), odchylenie_standardowe(p, tempTAB, srednia(suma2, p)));
								printf("Skosnosc: %.2f\n", wynik2);
								fprintf(fp, "Skosnosc: %.2f, ", wynik2);
								wynik3 = skosint(skosnosc(p, tempTAB, srednia(suma2, p), odchylenie_standardowe(p, tempTAB, srednia(suma2, p))));
								printf("Skosnosc: %s\n", wynik3);
								wynik2 = kurtoza(p, tempTAB, srednia(suma2, p), odchylenie_standardowe(p, tempTAB, srednia(suma2, p)));
								printf("Kurtoza: %.2f\n", wynik2);
								fprintf(fp, "Kurtoza: %.2f\n", wynik2);
								wynik3 = kurtint(kurtoza(p, tempTAB, srednia(suma2, p), odchylenie_standardowe(p, tempTAB, srednia(suma2, p))));
								printf("Kurtoza: %s\n", wynik3);
							}
						}
					}
					fclose(fp);
				}
				free(TABUZ); /*zwolnienie miejsca w pamieci*/
			}

			else
			{
				printf("\nZle wybrales sprobuj jeszcze raz\n");
			}
		}

		printf("Czy chcesz cos jeszcze policzyc?\n1. TAK\n2. NIE\n");
		scanf_s("%i", &op);
		printf("\n");
	}
	system("PAUSE");
	return 0;
}
