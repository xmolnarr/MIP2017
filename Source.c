// zadanie2.c -- Roman Molnár, 7.11.2017 12:07

#include <stdio.h>
#include <string.h>

struct vetva
{
	char meno[100];
	struct vetva *pravy;
	struct vetva *lavy;
	struct vetva *rodic;
	int hodnota;
	int vyskaP;
	int vyskaL;
};

struct koren
{
	char stranka[100];
	struct vetva *koren;
	struct koren *dalsi;
};

struct koren *tabulka[1000] = { NULL }, *pomocna = NULL;

int n = 0 , q = 1;


char *randstring(size_t length) {

	static char charset[] = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789,.-#'?!";
	char *randomString = NULL;

	if (length) {
		randomString = malloc(sizeof(char) * (length + 1));

		if (randomString) {
			for (int n = 0; n < length; n++) {
				int key = rand() % (int)(sizeof(charset) - 1);
				randomString[n] = charset[key];
			}

			randomString[length] = '\0';
		}
	}

	return randomString;
}

int ziskajHodnotu(struct vetva *vetva)
{
	int a, b;
	if (vetva->lavy != NULL) a = vetva->lavy->hodnota;
	else a = 0;
	if (vetva->pravy != NULL) b = vetva->pravy->hodnota;
	else b = 0;
	return (a + b);
}

void rotaciaL(struct vetva* vetva)
{
	struct vetva *pom;
	pom = (struct vetva*)malloc(sizeof(struct vetva));
	pom = NULL;
	if (vetva->pravy != NULL) pom = vetva->pravy->lavy;
	vetva->pravy->lavy = vetva;
	if (vetva->rodic == NULL) vetva->pravy->rodic = NULL;
	else
	{
		vetva->pravy->rodic = vetva->rodic;
		if (vetva->rodic->pravy == vetva) vetva->rodic->pravy = vetva->pravy;
		else vetva->rodic->lavy = vetva->pravy;
	}
	vetva->rodic = vetva->pravy;
	vetva->pravy = pom;
	if (pom != NULL)pom->rodic = vetva;
	
}

void rotaciaP(struct vetva* vetva)
{
	struct vetva *pom;
	pom = (struct vetva*)malloc(sizeof(struct vetva));
	pom = NULL;
	if (vetva->lavy != NULL) pom = vetva->lavy->pravy;
	vetva->lavy->pravy = vetva;
	if (vetva->rodic == NULL) vetva->lavy->rodic = NULL;
	else
	{
		vetva->lavy->rodic = vetva->rodic;
		if (vetva->rodic->lavy == vetva) vetva->rodic->lavy = vetva->lavy;
		else vetva->rodic->pravy = vetva->lavy;
	}
	vetva->rodic = vetva->lavy;
	vetva->lavy = pom;
	if (pom != NULL) pom->rodic = vetva;
}

int max(int a, int b)
{
	if (a > b) return a;
	return b;
}

struct vetva* pridajNahradu(struct vetva *rodic)
{
	struct vetva *pom;
	pom = (struct vetva*)malloc(sizeof(struct vetva));
	pom->lavy = NULL;
	pom->pravy = NULL;
	pom->rodic = rodic;
	strcpy(pom->meno, "\0");
	pom->hodnota = 0;
	pom->vyskaL = -1;
	pom->vyskaP = -1;
	return pom;
}

struct vetva* pridaj(char *meno,struct vetva *rodic)
{
	struct vetva *pom;
	pom = (struct vetva*)malloc(sizeof(struct vetva));
	pom->lavy = NULL;
	pom->pravy = NULL;
	strcpy(pom->meno, meno);
	pom->rodic = rodic;
	pom->hodnota = 1;
	pom->vyskaP = 0;
	pom->vyskaL = 0;
	return pom;
}

struct koren* pridajK(char *stranka)
{
	struct koren *pom;
	pom = (struct koren*)malloc(sizeof(struct koren));
	pom->dalsi = NULL;
	strcpy(pom->stranka, stranka);
	return pom;
}

int porovnaj(char *meno1, char *meno2)
{
	int i = 0;
	if (strlen(meno2) == 0) return 0;
	for (i = 0; i <= strlen(meno1); i++)
	{
		if (meno1[i] > meno2[i]) return 1;
		if (meno1[i] < meno2[i]) return 0;
	}
	if (strlen(meno1) == strlen(meno2)) return -1;
	return 0;
}

void najdiRot(struct vetva *rotuj)
{
	struct vetva *druhyPodstrom;
	druhyPodstrom = (struct vetva*)malloc(sizeof(struct vetva));
	if (rotuj->rodic->vyskaL > rotuj->rodic->vyskaP && rotuj->vyskaL > rotuj->vyskaP)
	{
		rotaciaP(rotuj->rodic);

		rotuj->pravy->hodnota = ziskajHodnotu(rotuj->pravy) + 1;
		rotuj->hodnota = ziskajHodnotu(rotuj) + 1;

		if (rotuj->pravy->lavy != NULL ) rotuj->pravy->vyskaL = max(rotuj->pravy->lavy->vyskaL, rotuj->pravy->lavy->vyskaP) + 1;
		else rotuj->pravy->vyskaL = 0;
		rotuj->vyskaP = max(rotuj->pravy->vyskaL, rotuj->pravy->vyskaP) + 1;
		return;
	}
	if (rotuj->rodic->vyskaL > rotuj->rodic->vyskaP && rotuj->vyskaL < rotuj->vyskaP)
	{
		rotaciaL(rotuj);

		rotuj->hodnota = ziskajHodnotu(rotuj) + 1;
		rotuj->rodic->hodnota = ziskajHodnotu(rotuj->rodic) + 1;

		rotaciaP(rotuj->rodic->rodic);

		rotuj->rodic->pravy->hodnota = ziskajHodnotu(rotuj->rodic->pravy) + 1;
		rotuj->rodic->hodnota = ziskajHodnotu(rotuj->rodic) + 1;

		if (rotuj->pravy != NULL) rotuj->vyskaP = max(rotuj->pravy->vyskaL, rotuj->pravy->vyskaP) + 1;
		else rotuj->vyskaP = 0;
		if (rotuj->rodic->pravy->lavy != NULL) rotuj->rodic->pravy->vyskaL = max(rotuj->rodic->pravy->lavy->vyskaL, rotuj->rodic->pravy->lavy->vyskaP) + 1;
		else rotuj->rodic->pravy->vyskaL = 0;
		rotuj->rodic->vyskaP = max(rotuj->rodic->pravy->vyskaL, rotuj->rodic->pravy->vyskaP) + 1;
		rotuj->rodic->vyskaL = max(rotuj->rodic->lavy->vyskaL, rotuj->rodic->lavy->vyskaP) + 1;
		
		return;
	}
	if (rotuj->rodic->vyskaL < rotuj->rodic->vyskaP && rotuj->vyskaL < rotuj->vyskaP)
	{
		rotaciaL(rotuj->rodic);

		rotuj->lavy->hodnota = ziskajHodnotu(rotuj->lavy) + 1;
		rotuj->hodnota = ziskajHodnotu(rotuj) + 1;

		if (rotuj->lavy->pravy != NULL) rotuj->lavy->vyskaP = max(rotuj->lavy->pravy->vyskaL, rotuj->lavy->pravy->vyskaP) + 1;
		else rotuj->lavy->vyskaP = 0;
		rotuj->vyskaL = max(rotuj->lavy->vyskaL, rotuj->lavy->vyskaP) + 1;;
		return;
	}
	if (rotuj->rodic->vyskaL < rotuj->rodic->vyskaP && rotuj->vyskaL > rotuj->vyskaP)
	{
		rotaciaP(rotuj);

		rotuj->hodnota = ziskajHodnotu(rotuj) + 1;
		rotuj->rodic->hodnota = ziskajHodnotu(rotuj->rodic) + 1;

		rotaciaL(rotuj->rodic->rodic);

		rotuj->rodic->lavy->hodnota = ziskajHodnotu(rotuj->rodic->lavy) + 1;
		rotuj->rodic->hodnota = ziskajHodnotu(rotuj->rodic) + 1;

		if (rotuj->lavy != NULL) rotuj->vyskaL = max(rotuj->lavy->vyskaL, rotuj->lavy->vyskaP) + 1;
		else rotuj->vyskaL = 0;
		if (rotuj->rodic->lavy->pravy != NULL) rotuj->rodic->lavy->vyskaP = max(rotuj->rodic->lavy->pravy->vyskaL, rotuj->rodic->lavy->pravy->vyskaP) + 1;
		else rotuj->rodic->lavy->vyskaP = 0;
		rotuj->rodic->vyskaP = max(rotuj->rodic->pravy->vyskaL, rotuj->rodic->pravy->vyskaP) + 1;
		rotuj->rodic->vyskaL = max(rotuj->rodic->lavy->vyskaL, rotuj->rodic->lavy->vyskaP) + 1;
	
		return;
	}

	if (rotuj->rodic->pravy == rotuj || (strlen(rotuj->meno)==0 && strlen(rotuj->rodic->lavy->meno) != 0))
	{
		druhyPodstrom = rotuj->rodic->lavy;
		if (druhyPodstrom->rodic->vyskaL > druhyPodstrom->rodic->vyskaP && druhyPodstrom->vyskaL >= druhyPodstrom->vyskaP)
		{
			rotaciaP(druhyPodstrom->rodic);

			druhyPodstrom->pravy->hodnota = ziskajHodnotu(druhyPodstrom->pravy) + 1;
			druhyPodstrom->hodnota = ziskajHodnotu(druhyPodstrom) + 1;

			if (druhyPodstrom->pravy->lavy != NULL) druhyPodstrom->pravy->vyskaL = max(druhyPodstrom->pravy->lavy->vyskaL, druhyPodstrom->pravy->lavy->vyskaP) + 1;
			else druhyPodstrom->pravy->vyskaL = 0;
			druhyPodstrom->vyskaP = max(druhyPodstrom->pravy->vyskaL, druhyPodstrom->pravy->vyskaP) + 1;
			return;
		}

		if (druhyPodstrom->rodic->vyskaL > druhyPodstrom->rodic->vyskaP && druhyPodstrom->vyskaL < druhyPodstrom->vyskaP)
		{
			rotaciaL(druhyPodstrom);

			druhyPodstrom->hodnota = ziskajHodnotu(druhyPodstrom) + 1;
			druhyPodstrom->rodic->hodnota = ziskajHodnotu(druhyPodstrom->rodic) + 1;

			rotaciaP(druhyPodstrom->rodic->rodic);

			druhyPodstrom->rodic->pravy->hodnota = ziskajHodnotu(druhyPodstrom->rodic->pravy) + 1;
			druhyPodstrom->rodic->hodnota = ziskajHodnotu(druhyPodstrom->rodic) + 1;

			if (druhyPodstrom->pravy != NULL) druhyPodstrom->vyskaP = max(druhyPodstrom->pravy->vyskaL, druhyPodstrom->pravy->vyskaP) + 1;
			else druhyPodstrom->vyskaP = 0;
			if (druhyPodstrom->rodic->pravy->lavy != NULL) druhyPodstrom->rodic->pravy->vyskaL = max(druhyPodstrom->rodic->pravy->lavy->vyskaL, druhyPodstrom->rodic->pravy->lavy->vyskaP) + 1;
			else druhyPodstrom->rodic->pravy->vyskaL = 0;
			druhyPodstrom->rodic->vyskaP = max(druhyPodstrom->rodic->pravy->vyskaL, druhyPodstrom->rodic->pravy->vyskaP) + 1;
			druhyPodstrom->rodic->vyskaL = max(druhyPodstrom->rodic->lavy->vyskaL, druhyPodstrom->rodic->lavy->vyskaP) + 1;

			return;
		}


	}
	else if (rotuj->rodic->lavy == rotuj || (strlen(rotuj->meno) == 0 && strlen(rotuj->rodic->pravy) != 0))
	{
		druhyPodstrom = rotuj->rodic->pravy;
		if ((druhyPodstrom->rodic->vyskaL < druhyPodstrom->rodic->vyskaP && druhyPodstrom->vyskaL < druhyPodstrom->vyskaP )|| (druhyPodstrom->rodic->vyskaL < druhyPodstrom->rodic->vyskaP && druhyPodstrom->vyskaL == druhyPodstrom->vyskaP))
		{
			rotaciaL(druhyPodstrom->rodic);

			druhyPodstrom->lavy->hodnota = ziskajHodnotu(druhyPodstrom->lavy) + 1;
			druhyPodstrom->hodnota = ziskajHodnotu(druhyPodstrom) + 1;

			if (druhyPodstrom->lavy->pravy != NULL) druhyPodstrom->lavy->vyskaP = max(druhyPodstrom->lavy->pravy->vyskaL, druhyPodstrom->lavy->pravy->vyskaP) + 1;
			else druhyPodstrom->lavy->vyskaP = 0;
			druhyPodstrom->vyskaL = max(druhyPodstrom->lavy->vyskaL, druhyPodstrom->lavy->vyskaP) + 1;;
			return;
		}
		if (druhyPodstrom->rodic->vyskaL < druhyPodstrom->rodic->vyskaP && druhyPodstrom->vyskaL > druhyPodstrom->vyskaP)
		{
			rotaciaP(druhyPodstrom);

			druhyPodstrom->hodnota = ziskajHodnotu(druhyPodstrom) + 1;
			druhyPodstrom->rodic->hodnota = ziskajHodnotu(druhyPodstrom->rodic) + 1;

			rotaciaL(druhyPodstrom->rodic->rodic);

			druhyPodstrom->rodic->lavy->hodnota = ziskajHodnotu(druhyPodstrom->rodic->lavy) + 1;
			druhyPodstrom->rodic->hodnota = ziskajHodnotu(druhyPodstrom->rodic) + 1;

			if (druhyPodstrom->lavy != NULL) druhyPodstrom->vyskaL = max(druhyPodstrom->lavy->vyskaL, druhyPodstrom->lavy->vyskaP) + 1;
			else druhyPodstrom->vyskaL = 0;
			if (druhyPodstrom->rodic->lavy->pravy != NULL) druhyPodstrom->rodic->lavy->vyskaP = max(druhyPodstrom->rodic->lavy->pravy->vyskaL, druhyPodstrom->rodic->lavy->pravy->vyskaP) + 1;
			else druhyPodstrom->rodic->lavy->vyskaP = 0;
			druhyPodstrom->rodic->vyskaP = max(druhyPodstrom->rodic->pravy->vyskaL, druhyPodstrom->rodic->pravy->vyskaP) + 1;
			druhyPodstrom->rodic->vyskaL = max(druhyPodstrom->rodic->lavy->vyskaL, druhyPodstrom->rodic->lavy->vyskaP) + 1;

			return;
		}

	}

}

void aktualizujS(struct vetva *konec)
{
	
	while (konec != NULL)
	{
		
		if (konec->rodic == NULL) break;
		
		konec->rodic->hodnota = ziskajHodnotu(konec->rodic) + 1;

		if ((strlen(konec->meno) == 0 && konec->rodic->pravy != NULL && strlen(konec->rodic->pravy->meno) != 0) || (strlen(konec->meno) == 0 && konec->rodic->pravy == NULL))
		{
			konec->rodic->vyskaL = 0;
		}
		else 
			if ((strlen(konec->meno) == 0 && konec->rodic->lavy != NULL && strlen(konec->rodic->lavy->meno) != 0) || (strlen(konec->meno) == 0 && konec->rodic->lavy == NULL))
			{
				konec->rodic->vyskaP = 0;
			}
			else
			{
				if (porovnaj(konec->rodic->meno, konec->meno))
					konec->rodic->vyskaL = max(konec->vyskaL, konec->vyskaP) + 1;
				else
					konec->rodic->vyskaP = max(konec->vyskaL, konec->vyskaP) + 1;
			}

		if (abs(konec->rodic->vyskaP - konec->rodic->vyskaL) > 1)
		{
			najdiRot(konec);
		}
		if (konec->rodic != NULL ) konec->rodic->hodnota = ziskajHodnotu(konec->rodic) + 1;

		konec = konec->rodic;
	}
}

int hash(char *stranka)
{
	int hash = 0;
	for (int i = 0; i < strlen(stranka); i++)
	{
		hash += (stranka[i] - '0');
	}
	return hash%1000;
}

struct vetva* ziskajvPoradi(struct vetva* vetva, int k)
{
	int poradie;
	if (vetva->lavy != NULL)
		poradie = vetva->lavy->hodnota + 1;
	else
		poradie = 1;
	if (k == poradie) return vetva;
	else if (k < poradie)
		return ziskajvPoradi(vetva->lavy, k);
	else
		return ziskajvPoradi(vetva->pravy, k - poradie);
}


struct vetva* najdiS(struct vetva* vetva)
{
	struct vetva *hladaj,*rodic;
	hladaj = (struct vetva*)malloc(sizeof(struct vetva));
	rodic = (struct vetva*)malloc(sizeof(struct vetva));
	rodic = NULL;
	hladaj = vetva;
	hladaj = hladaj->pravy;
	while (hladaj->lavy != NULL)
	{
		rodic = hladaj;
		hladaj = hladaj->lavy;
	}
	
	if (rodic != NULL)
		rodic->lavy = NULL;
	
	return hladaj;
}

void like(char *page, char *user)
{
	struct vetva *akt;
	struct koren *koren,*pred;
	int pocitadlo = 0, k = 0;
	akt = (struct vetva*)malloc(sizeof(struct vetva));
	koren = (struct koren*)malloc(sizeof(struct koren));
	pred = (struct koren*)malloc(sizeof(struct koren));
	koren = tabulka[hash(page)];
	pred = NULL;
	if (tabulka[hash(page)] == NULL)
	{
		tabulka[hash(page)] = pridajK(page);
		tabulka[hash(page)]->koren = pridaj(user, NULL);
	}
	else
	{
		while (koren != NULL)
		{
			if (porovnaj(koren->stranka, page) == -1)
			{
				k = 1;
				break;
			}
			pred = koren;
			koren = koren->dalsi;
		}
		if (k == 0 || koren->koren == NULL)
		{
			if (k) koren->koren = pridaj(user, NULL);
			else
			{
				pred->dalsi = pridajK(page);
				pred->dalsi->koren = pridaj(user, NULL);
			}
		}
		else
		{
			akt = koren->koren;
			while (pocitadlo != 1)
			{
				if (porovnaj(akt->meno, user) && akt->lavy == NULL)
				{
					akt->lavy = pridaj(user, akt);
					akt->vyskaL += 1;
					akt->hodnota = ziskajHodnotu(akt) + 1;
					pocitadlo = 1;
					break;
				}
				if (porovnaj(akt->meno, user) && akt->lavy != NULL)
					akt = akt->lavy;

				if (porovnaj(akt->meno, user) == 0 && akt->pravy == NULL)
				{
					akt->pravy = pridaj(user, akt);
					akt->vyskaP += 1;
					akt->hodnota = ziskajHodnotu(akt) + 1;
					pocitadlo = 1;
					break;
				}

				if (porovnaj(akt->meno, user) == 0 && akt->pravy != NULL)
					akt = akt->pravy;
			}

			aktualizujS(akt);
			if (koren->koren->rodic != NULL) koren->koren = koren->koren->rodic;
		}
	}
}

void unlike(char *page, char *user)
{
	struct vetva *hladaj, *succ, *docasna;
	struct koren *koren = NULL;
	int kon = -2, smer = -2, kvocient = 0;
	koren = (struct koren*)malloc(sizeof(struct koren));
	hladaj = (struct vetva*)malloc(sizeof(struct vetva));
	succ = (struct vetva*)malloc(sizeof(struct vetva));
	docasna = (struct vetva*)malloc(sizeof(struct vetva));
	koren = tabulka[hash(page)];
	while (koren != NULL)
	{
		if (porovnaj(koren->stranka, page) == -1)
			break;
		koren = koren->dalsi;
	}
	if (porovnaj(koren->koren->meno, user) == -1)
	{
		hladaj = koren->koren;
		if (koren->koren->pravy != NULL)
		{
			succ = najdiS(hladaj);

			if (succ->rodic != hladaj)
			{
				if (succ->pravy != NULL)
				{
					succ->rodic->lavy = succ->pravy;
					succ->pravy->rodic = succ->rodic;
					succ->pravy->pravy = pridajNahradu(succ->pravy);
					docasna = pridajNahradu(succ->pravy);
					kvocient = 2;
				}
				else
				{
					succ->rodic->lavy = pridajNahradu(succ->rodic);
					docasna = pridajNahradu(succ->rodic);
				}
			}
			else
			{
				if (succ->pravy != NULL)
				{
					succ->pravy->pravy = pridajNahradu(succ->pravy);
					docasna = pridajNahradu(succ->pravy);
					kvocient = 1;
				}
				else
				{
					succ->pravy = pridajNahradu(succ);
					docasna = pridajNahradu(succ);
				}
			}
			koren->koren = succ;
			succ->lavy = hladaj->lavy;
			if (succ->lavy != NULL) succ->lavy->rodic = succ;
			succ->vyskaL = hladaj->vyskaL;
			succ->rodic = NULL;

			if (succ != hladaj->pravy)
			{
				succ->pravy = hladaj->pravy;
				hladaj->pravy->rodic = succ;
			}

			aktualizujS(docasna);
			if (kvocient == 2) docasna->rodic->pravy = NULL;
			else if (docasna->rodic == succ) succ->pravy = NULL;
			else if (kvocient == 1) succ->pravy->pravy = NULL;
			else docasna->rodic->lavy = NULL;

			koren->koren = succ;

		}
		else
			if (hladaj->lavy != NULL)
			{
				koren->koren = hladaj->lavy;
				hladaj->lavy->rodic = NULL;
			}
			else
				koren->koren = NULL;
	}
	else
	{
		hladaj = koren->koren;
		while ((kon = (porovnaj(hladaj->meno, user))) != -1)
		{
			smer = kon;
			if (kon)
				hladaj = hladaj->lavy;

			else
				hladaj = hladaj->pravy;
		}
		if (hladaj->pravy != NULL)
		{
			succ = najdiS(hladaj);
			if (succ->rodic != hladaj)
			{
				if (succ->pravy != NULL)
				{
					succ->rodic->lavy = succ->pravy;
					succ->pravy->rodic = succ->rodic;
					succ->pravy->pravy = pridajNahradu(succ->pravy);
					docasna = pridajNahradu(succ->pravy);
					kvocient = 2;
				}
				else
				{
					succ->rodic->lavy = pridajNahradu(succ->rodic);
					docasna = pridajNahradu(succ->rodic);
				}
			}
			else
			{
				if (succ->pravy != NULL)
				{
					succ->pravy->pravy = pridajNahradu(succ->pravy);
					docasna = pridajNahradu(succ->pravy);
					kvocient = 1;
				}
				else
				{
					succ->pravy = pridajNahradu(succ);
					docasna = pridajNahradu(succ);
				}
			}
			if (smer) hladaj->rodic->lavy = succ;
			else hladaj->rodic->pravy = succ;

			succ->rodic = hladaj->rodic;
			succ->lavy = hladaj->lavy;

			if (succ->lavy != NULL) succ->lavy->rodic = succ;
			succ->vyskaL = hladaj->vyskaL;

			if (succ != hladaj->pravy)
			{
				succ->pravy = hladaj->pravy;
				hladaj->pravy->rodic = succ;
			}

			aktualizujS(docasna);
			if (kvocient == 2) docasna->rodic->pravy = NULL;
			else
				if (docasna->rodic == succ) succ->pravy = NULL;
				else if (kvocient == 1) succ->pravy->pravy = NULL;
				else docasna->rodic->lavy = NULL;
				while (succ->rodic != NULL) succ = succ->rodic;

				koren->koren = succ;
		}
		else
			if (hladaj->lavy != NULL)
			{
				if (smer) hladaj->rodic->lavy = hladaj->lavy;
				else hladaj->rodic->pravy = hladaj->lavy;

				hladaj->lavy->rodic = hladaj->rodic;
				aktualizujS(hladaj->lavy);

				hladaj = hladaj->hodnota;
				while (hladaj->rodic != NULL) hladaj = hladaj->rodic;
				koren->koren = hladaj;
			}
			else
				if (smer)
				{
					hladaj->rodic->lavy = pridajNahradu(hladaj->rodic);
					docasna = pridajNahradu(hladaj->rodic);
					aktualizujS(docasna);
					hladaj->rodic->lavy = NULL;
					while (hladaj->rodic != NULL) hladaj = hladaj->rodic;
					koren->koren = hladaj;
				}
				else
				{
					hladaj->rodic->pravy = pridajNahradu(hladaj->rodic);
					docasna = pridajNahradu(hladaj->rodic);
					aktualizujS(docasna);
					hladaj->rodic->pravy = NULL;
					while (hladaj->rodic != NULL) hladaj = hladaj->rodic;
					koren->koren = hladaj;
				}
	}
}

char *getuser(char *page, int k)
{
	struct vetva* docasna = NULL;
	struct koren *hladaj = NULL;
	n = 0;

	pomocna = NULL;

	hladaj = tabulka[hash(page)];

	while (hladaj != NULL)
	{
		if (porovnaj(hladaj->stranka, page) == -1)
		{
			break;
		}
		hladaj = hladaj->dalsi;
	}
	if (k <= hladaj->koren->hodnota)
	docasna = ziskajvPoradi(hladaj->koren, k);

	if (docasna == NULL) return NULL;

	return (docasna->meno);
}

// Vlastna funkcia main() je pre vase osobne testovanie. Dolezite: pri testovacich scenaroch sa nebude spustat!
int main()
{
	char user[20][50];
	for (int i = 0; i < 20; i++)
	{
		
		strcpy(user[i], randstring(40));
		if (i<10) like("asd", user[i]);
		else like("dsa", user[i]);
	}
	printf("----------------------------\n------------------------------\n");
	for (int i = 1; i <= 12; i++)
	{
		printf("%s\n",getuser("asd", i));
	}
	printf("----------------------------\n------------------------------\n");
	for (int i = 1; i <= 12; i++)
	{
		printf("%s\n",getuser("dsa", i));
	}
	printf("----------------------------\n------------------------------\n");
	for (int i = 3; i < 7; i++)
	{
		unlike("asd", user[i]);
		unlike("dsa", user[i + 8]);
	}

	printf("----------------------------\n------------------------------\n");
	for (int i = 1; i <= 12; i++)
	{
		printf("%s\n",getuser("asd", i));
	}
	printf("----------------------------\n------------------------------\n");
	for (int i = 1; i <= 12; i++)
	{
		printf("%s\n",getuser("dsa", i));
	}

		return 0;
}