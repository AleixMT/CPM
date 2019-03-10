#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <assert.h>
#include <omp.h>
#include <limits.h>

#define NN 384000000  // 384000000 int * (4 B/ 1 int) * (1 GB / 2^30 B) = 1,43 GB de dades (com a m�xim) carregades a memoria
#define MAX_INT ((int) ((unsigned int) (-1) >> 1) )  // Definim el valor m�xim d'un enter segons la m�quina

//(validaci� 3221142676932982)

int valors[NN + 1];
int valors2[NN + 1];

// Funci� quicksort
// int pointer *val: Punter que apunta a un vector de dades compartit en memoria.
// int ne: Indica la quantitat de dades que podem modificar del nostre vector apuntat per *val.
// per tant l'�ndex m�xim per a accedir al vector *val �s ne - 1
void qs(int *val, int ne)
{
	int i, f;  // i es l'index per l'esquerra, f index per la dreta
	int pivot, vtmp, vfi;  // valor pivot contra el que ordenarem, vtmp valor que estem ordenant, vfi variable auxiliar que cont� el valor a ordenar
	//quan acabem de colocar un valor DESPR�S del pivot, ja que no tenim espais buits i la necessitem.

	pivot = val[0];  // Inicialment escollim arbitrariament el valor de la primera posicio com a pivot
	i = 1;  // �ndex m�nim per a accedir al vector (comencem a i = 1 perqu� a i = 0 ja hi tenim al pivot)
	f = ne - 1;  // �ndex m�xim per a accedir al nostre vector global
	vtmp = val[i];  // emmagatzemmem de manera temporal el valor a la posici� val[1] per a tenir el primer valor a col�locar

	while (i <= f)  // mentre el �ndex inicial no superi a l'�ndex final
	{
		if (vtmp < pivot)  // Si el valor temporal (el que estem ordenant) �s m�s petit que el pivot...
		{
			val[i - 1] = vtmp;  // Movem el valor temporal a la posici� que acabem de deixar lliure (a la primera iteraci� al lloc on estava el pivot)
			i++;  // passem al seg�ent element
			vtmp = val[i];  // Ara que hem colocat el vtmp carreguem el seg�ent valor a ordenar
		}
		else  // si es mes gran
		{
			vfi = val[f];  // guardem un valor a la dreta del pivot per a fer lloc per al nou valor
			val[f] = vtmp;  // guardem el valor que estavem ordenant
			f--;  // passem al seg�ent element
			vtmp = vfi;  // emmagatzemmem el valor temporal com a nou valor a ordenar
		}
	}
	val[i - 1] = pivot;  // Un cop els dos �ndexos es creuen (i > f, i - 1 = f) coloquem el pivot a la posici� i - 1 per a que els dos
	// sub vectors es trobin ordenats

 	// En principi aquests dos ifs s'activen i porten dues trucades recursives que reordenen els dos subvectors
 	// Si no es aixi, vol dir que el pivot escollit era major o menor que la resta d'elements en l'array (es el pitjor cas al escollir un pivot)
	// o b� que el vector es molt curt
 	// Despr�s repartim els dos subvectors en dues crides recursives, recalculant els indexos que passem a la funci�
	if (f > 1) qs(val, f);  // reordena el vector esquerra
	if (i < ne - 1) qs(&val[i], ne - f - 1);  // reordena el vector dret
}

// Funci� merge
// S'encarrega de mesclar dos vectors ordenats i contigus en memoria de la mateixa mida per a obtenir un vector output ordenat,
// situat en un altre espai de memoria.

void merge2(int* val, int n, int *vo)
{
	int i, posi, posj;

	posi = 0;  // posicio inicial en el vector esquerra
	posj = (n / 2);  // posicio inicial en el vector dret //? redundant parenthesis

	for (i = 0; i < n; i++)  // recorrem el conjunt dels dos vectors
		// si hem acabat amb el vector j o no hem acabat el vector i & el valor al vector i es menor o igual que el valor al vector j
		if (((posi < n/2) && (val[posi] <= val[posj])) || (posj >= n))
			vo[i] = val[posi++];
		else
			vo[i] = val[posj++];
}


int main(int nargs, char* args[])
{
	int ndades, i, m, parts, porcio;
	int *vin, *vout, *vtmp;
	long long sum = 0;

	assert(nargs == 3);

	ndades = atoi(args[1]);
	assert(ndades <= NN);

	parts = atoi(args[2]);

	if (parts < 2)
		assert("Han d'haver dues parts com a minim" == 0);

	if (ndades % parts)
		assert("N ha de ser divisible per parts" == 0);

	porcio = ndades / parts;

	for (i = 0; i < ndades; i++)
		valors[i] = rand();
		//valors[i] = rand_r(&seed);

//////////////////////////
	//unsigned int max_ui = ~0;
	//max_ui = UINT_MAX;
	int nums[32][3] = {
	{1804289383, 846930886, 1681692777},
	{910375920, 1345461578, 1896181529},
	{1221069815, 771523977, 1636361493},
	{1617739725, 1455033559, 522227855},
	{1330809454, 704191590, 301995565},
	{1516306338, 154833854, 1656844268},
	{2118383419, 559927362, 1870697007},
	{541647651, 419723294, 302649230},
	{572146091, 1818107435, 567288879},
	{1589564037, 1362466805, 1275346964},
	{1345396254, 1951582429, 657760915},
	{130564568, 2123025505, 148382276},
	{2128036368, 295810087, 571797961},
	{1799264005, 1734174567, 514150428},
	{1377995364, 1359940662, 914556769},
	{791921621, 2079843371, 1655431707},
	{320471078, 2040977409, 1524058057},
	{1712480662, 118419781, 1455920076},
	{682185433, 888623812, 321684413},
	{1579718082, 892744789, 1923477653},
	{1255584569, 711491752, 1421496539},
	{1301173762, 1385232134, 2054821568},
	{1432017735, 591339226, 655812167},
	{2130863259, 1794850474, 1481703773},
	{53799339, 1751150258, 1060228832},
	{236173651, 1378816047, 853126510},
	{434667338, 191011618, 1060566167},
	{286647845, 395698231, 1666153870},
	{1921715949, 285742864, 56636122},
	{2076920936, 754081009, 148149595},
	{1929690634, 2115227139, 731032562},
	{104351210, 1249467362, 853451224}
	};
	int j;
	unsigned int k, uintmax = UINT_MAX;
	int rand_tmp[3];
	FILE *file = NULL;
	file = fopen("results.txt", "a");

	//#pragma omp parallel for schedule(static)
	for (k = 0; k <= uintmax; k++)
	{
		if (!(k % 10000000))
			printf("\nLlavor %u de %u comprobada. %u llavors restants", k, uintmax, uintmax - k);

		srand(k);
		for (j = 0; j < 3; j++)
			rand_tmp[j] = rand();
		for (j = 0; j < 32; j++)
		{
			if (nums[j][0] == rand_tmp[0] && nums[j][1] == rand_tmp[1] && nums[j][2] == rand_tmp[2])
			{
				printf("\n%i, %i, %i", rand_tmp[0], rand_tmp[1], rand_tmp[2]);
				printf("\nla sequencia de nombres random %i, %i, %i corresponent al thread %i tenen la llavor %i", nums[j][0], nums[j][1], nums[j][2], j, k);
				/*fprintf(file, "\nla sequencia de nombres random %i, %i, %i corresponent al thread %i tenen la llavor %i", nums[j][0], nums[j][1], nums[j][2], j, i);
				putw(nums[j][0], file);
				putw(nums[j][1], file);
				putw(nums[j][2], file);
				putw(j, file);
				putw(i, file);
				fputc('\n', file);

				//fprintf(file, "\nla sequencia de nombres random %i, %i, %i corresponent al thread %i tenen la llavor %i", nums[j][0], nums[j][1], nums[j][2], j, i);
				getchar();**/
				srand(k);
				printf("\nComprovacio: %i, ", rand());
				printf("%i, ", rand());
				printf("%i, ", rand());
			}
		}

	}
	fclose(file);

	//exit(0);
///////////////////////
	//printf("Ya termine con los randoms misenyor");
	// Quicksort a parts
	for (i = 0; i < parts; i++)
		qs(&valors[i * porcio], porcio);

	// Merge en arbre
	vin = valors;
	vout = valors2;
	//#pragma omp parallel for ordered
	for (m = 2 * porcio; m <= ndades; m *= 2)
	{
		for (i = 0; i < ndades; i += m)
			merge2(&vin[i], m, &vout[i]);
		vtmp = vin;
		vin = vout;
    	vout = vtmp;
  	}

	// Validacio
	bool correct = true;  // Transform validation into acummulation

	for (i = 1; i < ndades; i++)
		correct &= vin[i - 1] <= vin[i];

	assert(correct);

	for (i = 0; i < ndades; i += 100)
		sum += vin[i];

	printf("validacio %lli \n", sum);
	exit(0);
}

