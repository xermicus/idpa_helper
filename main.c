#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>


// Options
int dflag = 0;
int eflag = 0;
char *ival = NULL;
char *oval = NULL;
int pflag = 0;
char *tval = NULL;
int vflag = 0;

// Global
char subst_table[256];
char count_table[256];
int c_min = 'a';
int c_max = 'z';
FILE *file_table;

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


void init_count_table() {
	int i;
	for (i = 0; i < sizeof(count_table) - 1; i++) {
		 count_table[i] = 0;
	}
}


void clear_subst_table() {
        int i;
	        for (i = 0; i < sizeof(subst_table) - 1; i++) {
	                subst_table[i] = i;
        }
}


void init_subst_table() {
	int i;
	unsigned int t;

	// Initialize empty default table without encryption
	/*for (i = 0; i < sizeof(subst_table) - 1; i++) {
		subst_table[i] = i;
	}*/
	clear_subst_table();


	for (i = c_min; i <= c_max; i++) {
		int sub_val = 0;
		int hit = 0;
		int j = 0;
		do {
			sub_val = gen_rand(&t, c_min, c_max);
			hit = 0;
			for (j = c_min; j < subst_table[i]; j++) {
				if (sub_val == subst_table[j]) {
					hit = 1;
				}
			}
		} while(hit);

		subst_table[i] = sub_val;
	}
}


void read_subst_table() {
	clear_subst_table();

	char c;
	file_table = fopen(tval, "r");
	while (c != EOF) {
		subst_table[(int) fgetc(file_table)] = fgetc(file_table);
		c = fgetc(file_table);
	}
	fclose(file_table);
}


char crypt(char c) {
	return subst_table[(int) c];
}


int count(char c) {
	return count_table[(int) c]++;
}


void process() {
	char i;
	char c;
	if(eflag) {
		while (i != '\n') {
			i = get_input();
			c = crypt(i);
			putchar (c);
			count(i);
        	}	
	}

        putchar('\n');

        if(pflag && eflag) {
		for (i = c_min; i <= c_max; i++) {
	        	printf("%c %i\n", subst_table[(int) i], count_table[(int) i]);
        	}	
	}
	else if (pflag) {
		while (i != '\n') {
			i = get_input();
			count(i);
		}
		for (i = c_min; i <= c_max; i++) {
		        printf("%c %i\n", i, count_table[(int) i]);
		}
	}
}


void init() {
	if (tval) {
                read_subst_table();
	}
	else {
		init_subst_table();
        }

	if (pflag) {
		init_count_table();
	}
}


int main(int argc, char *argv[])
{
	eval_args(argc, argv, &dflag, &eflag, &ival, &oval, &pflag, &tval, &vflag);

	init();

	process();

	return 0;
} 
