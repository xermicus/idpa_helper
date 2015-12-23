#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

char subst_table[256];

int eval_args(int argc, char *argv[], int *dflag, int *eflag, char **ival, char **oval, int *pflag, char **tval, int *vflag)
{
	
        int c;
        int index;
        opterr = 0;

	while ((c = getopt (argc, argv, "dei:o:pt:v")) != -1)
                switch (c)
                        {
                        case 'd':
                                *dflag = 1;
                                break;
                        case 'e':
                                *eflag = 1;
                                break;
                        case 'i':
                                *ival = optarg;
				break;
                        case 'o':
                                *oval = optarg;
                                break;
                        case 't':
                              	*tval = optarg;
                                break;
                        case 'p':
                                *pflag = 1;
                                break;
                        case 'v':
                                *vflag = 1;
                                break;


                        case '?':
                                if (optopt == 'i')
                                        fprintf (stderr, "Option -%c requires an argument.\n", optopt);
                                else if (optopt == 'o')
                                        fprintf (stderr, "Option -%c requires an argument.\n", optopt);
                                else if (optopt == 't')
                                        fprintf (stderr, "Option -%c requires an argument.\n", optopt);
                                else if (isprint (optopt))
                                        fprintf (stderr, "Unknown option `-%c'.\n", optopt);
                                else
                                        fprintf (stderr, "Unknown option character `\\x%x'.\n",  optopt);
                                return 1;
                        default:
                                abort ();
                        }

	
        for (index = optind; index < argc; index++)
                printf ("Non-option argument %s\n", argv[index]);

	return 0;

}


char get_input(){
	return (char) getchar();
}

int gen_rand(unsigned int *seed, int min, int max)
{
	int value = rand_r(seed);
        int length = max - min + 1;
	return min + (value % length);
}

void init_subst_table(void) {
	int i;
	unsigned int t;
	int c_min = 'a';
	int c_max = 'z';

	// Initialize empty default table without encryption
	for (i = 0; i < sizeof(subst_table) - 1; i++) {
		subst_table[i] = i;
	}
//	srand((unsigned) time(&t));

	for (i = c_min; i < c_max; i++) {
		int sub_val = 0;
		int hit = 0;
		int j = 0;
		do {
			sub_val = gen_rand(&t, c_min, c_max);
			hit = 0;
			for (j = c_min; j < c_max; j++) {
				if (sub_val == subst_table[j]) {
					hit = 1;
				}
			}
		} while(hit);

		subst_table[i] = gen_rand(&t, c_min, c_max);//subst_table[i] = sub_val;
	}
}

char crypt(char c) {
	return subst_table[(int) c];
}

int main(int argc, char *argv[])
{
        int dflag = 0;
        int eflag = 0;
        char *ival = NULL;
        char *oval = NULL;
        int pflag = 0;
        char *tval = NULL;
        int vflag = 0;	
	eval_args(argc, argv, &dflag, &eflag, &ival, &oval, &pflag, &tval, &vflag);

//        printf ("dflag = %d\neflag = %d\nivalue = %s\novalue = %s\npflag = %d\ntvalue = %s\nvflag = %    d\n", dflag, eflag, ival, oval, pflag, tval, vflag);

	init_subst_table(); // Init substitution table

	char i = 0;
	char c;
	while (i != '\n')
	{
		i = get_input();
		c = crypt(i);
		putchar	(c);
	}

	return 0;
} 
