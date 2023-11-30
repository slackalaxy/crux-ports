/* SIM - Local similarity program for use with the LAD/LAV interface.
corrected to accept both lower and upper case.
corrected to read other PAM.
treats self-comparison as pairwise comparison

    Compile with : cc -o sim sim204.c -lm

    For the description of the algorithm, see the paper
    "A Time-Efficient, Linear-Space Local Similarity Algorithm"
        Advances in Applied Mathematics, vol. 12 (1991), pp. 337-357.

    SIM finds k best non-intersecting alignments between two sequences or
    within a sequence using dynamic programming techniques. The alignments are
    reported in order of decreasing similarity score and share no aligned pairs.
    SIM requires space proportional to the sum of the input sequence lengths
    and the output alignment lengths, so it accommodates 100,000-base
    sequences on a workstation.

    Users can supply certain combinations of values for the parameters:
	M = cost of a matching aligned pair (default = 1)
	I = cost of a transition (default is -1)
	V = cost of a transversion (default is -1)
	O = gap open penalty (default is 6.0)
	E = gap extension penalty (default is 0.2)
	S = name of file containing substitution scores
    Thus the score for an N-symbol indel is -(O + E*N).  Values are
    specified with a command argument like O=5.5, where the given number
    is rounded by SIM to the nearest tenth.

    The S parameter cannot be specified if either I or V is specified.  A file
    named with the S parameter should have the following form:
#
# PAM 200 substitution matrix (scale = 0.346574)
#
# Number of letters in alphabet = 23
# Lowest score = -6, Highest score = 12
#
  A  R  N  D  C  Q  E  G  H  I  L  K  M  F  P  S  T  W  Y  V  B  Z  X
  2 -2  0  0 -2 -1  0  1 -1 -1 -2 -1 -1 -3  1  1  1 -5 -3  0  1  1  0
 -2  5  0 -1 -3  1 -1 -3  1 -2 -2  2  0 -3  0  0 -1  1 -4 -2  0  1  0
  0  0  2  2 -3  0  1  0  1 -1 -2  1 -2 -2  0  1  0 -4 -1 -2  3  2  0
  0 -1  2  3 -4  1  3  0  0 -2 -3  0 -2 -4 -1  0  0 -6 -3 -2  4  3  0
 -2 -3 -3 -4  8 -4 -4 -3 -3 -2 -5 -4 -4 -4 -2  0 -2 -6  0 -2 -3 -3  0
 -1  1  0  1 -4  4  2 -1  2 -2 -1  0 -1 -4  0 -1 -1 -4 -3 -2  2  4  0
  0 -1  1  3 -4  2  3  0  0 -2 -3  0 -2 -4  0  0  0 -6 -3 -2  3  4  0
  1 -3  0  0 -3 -1  0  4 -2 -2 -3 -2 -3 -3 -1  1  0 -5 -4 -1  1  0  0
 -1  1  1  0 -3  2  0 -2  5 -2 -2  0 -2 -1  0 -1 -1 -3  0 -2  2  2  0
 -1 -2 -1 -2 -2 -2 -2 -2 -2  4  2 -1  2  1 -2 -1  0 -5 -1  3 -1 -1  0
 -2 -2 -2 -3 -5 -1 -3 -3 -2  2  4 -2  3  1 -2 -2 -1 -4 -1  1 -2 -1  0
 -1  2  1  0 -4  0  0 -2  0 -1 -2  4  1 -4 -1  0  0 -3 -4 -2  1  1  0
 -1  0 -2 -2 -4 -1 -2 -3 -2  2  3  1  5  0 -2 -1  0 -4 -2  1 -1  0  0
 -3 -3 -2 -4 -4 -4 -4 -3 -1  1  1 -4  0  7 -4 -2 -2  0  5 -1 -2 -3  0
  1  0  0 -1 -2  0  0 -1  0 -2 -2 -1 -2 -4  5  1  0 -5 -4 -1  0  1  0
  1  0  1  0  0 -1  0  1 -1 -1 -2  0 -1 -2  1  2  1 -2 -2 -1  1  1  0
  1 -1  0  0 -2 -1  0  0 -1  0 -1  0  0 -2  0  1  3 -4 -2  0  1  0  0 
 -5  1 -4 -6 -6 -4 -6 -5 -3 -5 -4 -3 -4  0 -5 -2 -4 12  0 -6 -3 -4  0 
 -3 -4 -1 -3  0 -3 -3 -4  0 -1 -1 -4 -2  5 -4 -2 -2  0  7 -2 -1 -2  0 
  0 -2 -2 -2 -2 -2 -2 -1 -2  3  1 -2  1 -1 -1 -1  0 -6 -2  4 -1 -1  0 
  1  0  3  4 -3  2  3  1  2 -1 -2  1 -1 -2  0  1  1 -3 -1 -1  4  4  0 
  1  1  2  3 -3  4  4  0  2 -1 -1  1  0 -3  1  1  0 -4 -2 -1  4  5  0 
  0  0  0  0  0  0  0  0  0  0  0  0  0  0  0  0  0  0  0  0  0  0  0 

    The command argument S=PAM200, which is recommended for protein sequences,
    is treated specially; the set of weights given above is used, O defaults
    to 12 and E defaults to 4.  No file named "PAM200" will be read.

    For example, to find 7 best non-intersecting alignments of segments from
    two DNA sequences in files A and B, and using the above default values
    except that transistions are scored 0, use the command
	   sim  7  A  B  I=0

    As a second example, suppose the PAM120 weights are in a file named
    PAM120.  Then the command
	   sim  7  A  B  S=PAM120  O=10  E=2
    runs sim on protein sequence files A and B.  Also, the command
	   sim  7  A  B  S=PAM200
    uses the substitution scores given above, the gap-open penalty 12 and
    the gap extension penalty 4.  If the command does not specify any of
    the parameters M, I, V or S, then sim reads the first 100 characters of
    the first sequence; if at least 25 of these differ from A, C, G or T,
    then the PAM200 scores are used.
    

    Acknowledgments:
    The functions diff() and display() are from Gene Myers. We made the
    following two modifications: similarity weights (integer) are used instead
    of distance weights (float), and the aligned pairs already output are not
    permitted in the subsequent alignments.
*/

#define DEFAULT_M 1.0
#define DEFAULT_I -1.0
#define DEFAULT_V -1.0
#define DEFAULT_O 6.0
#define DEFAULT_E 0.2
#define DEFAULT_PAM_O 12.0
#define DEFAULT_PAM_E 4.0

#include   <stdio.h>
#include   <ctype.h>

char *name1, *name2;		/* names of sequence files    */

char
	achars[] = "ARNDCQEGHILKMFPSTWYVBZX";	/* amino acid names */

int
	alpha = 23;			/* alphabet size */

int wgts[23][23] = {	/* the PAM200 matrix */
 { 2,-2, 0, 0,-2,-1, 0, 1,-1,-1,-2,-1,-1,-3, 1, 1, 1,-5,-3, 0, 1, 1, 0},
 {-2, 5, 0,-1,-3, 1,-1,-3, 1,-2,-2, 2, 0,-3, 0, 0,-1, 1,-4,-2, 0, 1, 0},
 { 0, 0, 2, 2,-3, 0, 1, 0, 1,-1,-2, 1,-2,-2, 0, 1, 0,-4,-1,-2, 3, 2, 0},
 { 0,-1, 2, 3,-4, 1, 3, 0, 0,-2,-3, 0,-2,-4,-1, 0, 0,-6,-3,-2, 4, 3, 0},
 {-2,-3,-3,-4, 8,-4,-4,-3,-3,-2,-5,-4,-4,-4,-2, 0,-2,-6, 0,-2,-3,-3, 0},
 {-1, 1, 0, 1,-4, 4, 2,-1, 2,-2,-1, 0,-1,-4, 0,-1,-1,-4,-3,-2, 2, 4, 0},
 { 0,-1, 1, 3,-4, 2, 3, 0, 0,-2,-3, 0,-2,-4, 0, 0, 0,-6,-3,-2, 3, 4, 0},
 { 1,-3, 0, 0,-3,-1, 0, 4,-2,-2,-3,-2,-3,-3,-1, 1, 0,-5,-4,-1, 1, 0, 0},
 {-1, 1, 1, 0,-3, 2, 0,-2, 5,-2,-2, 0,-2,-1, 0,-1,-1,-3, 0,-2, 2, 2, 0},
 {-1,-2,-1,-2,-2,-2,-2,-2,-2, 4, 2,-1, 2, 1,-2,-1, 0,-5,-1, 3,-1,-1, 0},
 {-2,-2,-2,-3,-5,-1,-3,-3,-2, 2, 4,-2, 3, 1,-2,-2,-1,-4,-1, 1,-2,-1, 0},
 {-1, 2, 1, 0,-4, 0, 0,-2, 0,-1,-2, 4, 1,-4,-1, 0, 0,-3,-4,-2, 1, 1, 0},
 {-1, 0,-2,-2,-4,-1,-2,-3,-2, 2, 3, 1, 5, 0,-2,-1, 0,-4,-2, 1,-1, 0, 0},
 {-3,-3,-2,-4,-4,-4,-4,-3,-1, 1, 1,-4, 0, 7,-4,-2,-2, 0, 5,-1,-2,-3, 0},
 { 1, 0, 0,-1,-2, 0, 0,-1, 0,-2,-2,-1,-2,-4, 5, 1, 0,-5,-4,-1, 0, 1, 0},
 { 1, 0, 1, 0, 0,-1, 0, 1,-1,-1,-2, 0,-1,-2, 1, 2, 1,-2,-2,-1, 1, 1, 0},
 { 1,-1, 0, 0,-2,-1, 0, 0,-1, 0,-1, 0, 0,-2, 0, 1, 3,-4,-2, 0, 1, 0, 0},
 {-5, 1,-4,-6,-6,-4,-6,-5,-3,-5,-4,-3,-4, 0,-5,-2,-4,12, 0,-6,-3,-4, 0},
 {-3,-4,-1,-3, 0,-3,-3,-4, 0,-1,-1,-4,-2, 5,-4,-2,-2, 0, 7,-2,-1,-2, 0},
 { 0,-2,-2,-2,-2,-2,-2,-1,-2, 3, 1,-2, 1,-1,-1,-1, 0,-6,-2, 4,-1,-1, 0},
 { 1, 0, 3, 4,-3, 2, 3, 1, 2,-1,-2, 1,-1,-2, 0, 1, 1,-3,-1,-1, 4, 4, 0},
 { 1, 1, 2, 3,-3, 4, 4, 0, 2,-1,-1, 1, 0,-3, 1, 1, 0,-4,-2,-1, 4, 5, 0},
 { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
};


main(argc, argv) int argc; char *argv[];
{ long  M, N, K;		/* Sequence lengths and k     */
  char  *A,  *B;		/* The two sequences      */
  int i, ii, j, gave_M, gave_I, gave_V, gave_O, gave_E, gave_S, gave_PAM200,
		non_DNA;
  long V[128][128], Q,R;		/* Converted integer weights  */
  float parm_M, parm_I, parm_V, parm_O, parm_E, v;
  double atof();
  char *strchr(), *strcpy(), filename[1000];

	if ( argc < 4) {
		fprintf(stderr, "SIM version 204\n\nUsage:\n");
		fatal("SIM k file1 file2 [M=] [I=] [V=] [O=] [E=] [S=]");
		}

        /* read k: the desired number of local alignments */
	sscanf(argv[1],"%d", &K);
	if (K == 0)
		fatal("specified 0 alignments");

	name1 = argv[2];
	name2 = argv[3];

	M = get_seq(argv[2], &A);

	for(ii=0; ii < M; ii++) A[ii] = toupper(A[ii]);
	--A;	/* subscripts start with 1 */

	/* if (strcmp(argv[2],argv[3])) {	// sequences are different //
		N = get_seq(argv[3], &B);
		--B;
	} */

	N = get_seq(argv[3], &B);
	for(ii=0; ii < N; ii++) B[ii] = toupper(B[ii]);
	--B;


	parm_M = DEFAULT_M;
	parm_I = DEFAULT_I;
	parm_V = DEFAULT_V;
	parm_O = DEFAULT_O;
	parm_E = DEFAULT_E;
	gave_M = gave_I = gave_V = gave_O = gave_E = gave_S = gave_PAM200 = 0;
	while (--argc > 3) {
		if (argv[argc][1] != '=')
			fatalf("argument %d has improper form", argc);
		if (argv[argc][1] != 'S')
			v = atof(argv[argc]+2);
		switch (argv[argc][0]) {
			case 'M': parm_M = v; gave_M = 1; break;
			case 'I': parm_I = v; gave_I = 1; break;
			case 'V': parm_V = v; gave_V = 1; break;
			case 'O': parm_O = v; gave_O = 1; break;
			case 'E': parm_E = v; gave_E = 1; break;
			case 'S':
				if (strcmp(argv[argc]+2, "PAM200") == 0)
					gave_PAM200 = 1;
				else {
					strcpy(filename,argv[argc]+2); gave_S=1;
				}
				break;
			default: fatal("options are M, I, V, O, E and S.");
		}
	}
	if (gave_S && (gave_I || gave_V))
		fatal("Cannot give S parameter with I or V.");
	if (gave_PAM200 && (gave_I || gave_V || gave_S))
		fatal("Cannot use PAM200 with parameters, I, V, or S."); 
	if (!gave_S && !gave_PAM200 && !gave_M && !gave_I && !gave_V) {
		/* Does first sequence look like a protein? */
		j = (M < 100)? M : 100;
		for (non_DNA = i = 0; i < j; ++i)
			if (strchr("ACGT", A[i]) == NULL) 
				++non_DNA;
		if (non_DNA >= j/4)
			gave_PAM200 = 1;
	}
	if (gave_PAM200) {
		if (!gave_O)
			parm_O = DEFAULT_PAM_O;
		if (!gave_E)
			parm_E = DEFAULT_PAM_E;
	}

	printf("#:lav\n\n");
	printf("d {\n  \"SIM output with parameters:\n");
	if (gave_PAM200)
		printf("  Used PAM200 matrix\n");
	else if (gave_S)
		printf("  substitution scores in %s\n", filename);
	else
		printf("  M = %g, I = %g, V = %g\n", parm_M, parm_I, parm_V);
	printf("  O = %g, E = %g\"\n}\n", parm_O, parm_E);

	/* set up scoring matrix */
	if (gave_PAM200) {
		/* set up scoring matrix */
		for (i = 0; i < alpha; ++i)
			for (j = 0; j < alpha; ++j) {
				V[achars[i]][achars[j]] = 10*wgts[i][j];
				/*printf("%c %c %d\n", achars[i], achars[j], V[achars[i]][achars[j]]);*/
				}
	} else if (gave_S) {
		FILE *ckopen(), *fp;
		char buf[200], *p, *fgets(), alph[128];
		int alph_size;
		float weight;
	
		fp = ckopen(filename, "r");
		while (fgets(buf, 200, fp))
			if (buf[0] == ' ' || isupper(buf[0]))
				break;
		for (alph_size = 0, p = buf; *p != '\0'; ++p)
			if (isupper(*p))
				alph[alph_size++] = *p;
		for (i = 0; i < alph_size; ++i)
			for (j = 0; j < alph_size; ++j) {
				fscanf(fp, "%f", &weight);
				V[alph[i]][alph[j]] = 10*weight;
				/*printf("%c %c %d\n", alph[i], alph[j], V[alph[i]][alph[j]]);*/
			}
		fclose(fp);
	} else {
		parm_M += (parm_M > 0) ? 0.05 : -0.05;
		V['A']['A'] = V['C']['C'] = V['G']['G'] = V['T']['T']
			= 10*parm_M;
		parm_I += (parm_I > 0) ? 0.05 : -0.05;
		V['A']['G'] = V['G']['A'] = V['C']['T'] = V['T']['C']
			= 10*parm_I;
		parm_V += (parm_V > 0) ? 0.05 : -0.05;
		V['A']['C'] = V['A']['T'] = V['C']['A'] = V['C']['G'] =
	   	V['G']['C'] = V['G']['T'] = V['T']['A'] = V['T']['G']
			= 10*parm_V;
	}
	parm_O += (parm_O > 0) ? 0.05 : -0.05;
	Q = 10 * parm_O;
	parm_E += (parm_E > 0) ? 0.05 : -0.05;
	R = 10 * parm_E;


	SIM(A,B,M,N,K,V,Q,R,2L);

	/* Remove this test : self-comparison is treated as non-self-comparison
	if (strcmp(argv[2], argv[3]))
		SIM(A,B,M,N,K,V,Q,R,2L);
	else
		SIM(A,A,M,M,K,V,Q,R,1L);
	if (strcmp(argv[2], argv[3]) == 0) {
		// self-comparison; insert trivial diagonal //
		printf("a {\n  s 0.0\n  b 1 1\n  e %d %d\n", M, M);
		printf("  l 1 1 %d %d 0.0\n}\n", M, M);
	}
	 */

	exit(0);
}

/* get_seq - read a sequence */
int get_seq(file_name, seqptr)
char *file_name, **seqptr;
{
/* corrected to take both upper & lower case */
	FILE *qp, *ckopen();
	char *p, *fgets(), *strchr(), *ckalloc();
	int c;
	long n;
		
	qp = ckopen(file_name, "r");
	for (n = 0; (c = getc(qp)) != EOF; )
		if (c != '\n')
			++n;
	*seqptr = ckalloc((n+1)*sizeof(char));
	rewind(qp);
	for (p = *seqptr; ; ) {
		if (fgets(p, (int)n, qp) == NULL) {
			fclose(qp);
			*p = '\0';
			break;
		}
		if (p == *seqptr && !isalpha(*p))
			continue;
		while (isalpha(*p))
			++p;
		if (*p != '\n' && *p != '\t' && *p != '\0' && *p != '*')
			fatalf("Illegal character %c in query sequence.", *p);
	}
	return p - *seqptr;
}

static long (*v)[128];			/* substitution scores */
static long q, r;			/* gap penalties */
static long qr;				/* qr = q + r */

typedef struct ONE { long COL ;  struct ONE  *NEXT ;} pair, *pairptr;
pairptr *row, z; 			/* for saving used aligned pairs */
static short tt;

typedef struct NODE
	{ long  SCORE;
	  long  STARI;
	  long  STARJ;
	  long  ENDI;
	  long  ENDJ;
	  long  TOP;
	  long  BOT;
	  long  LEFT;
	  long  RIGHT; }  vertex, *vertexptr;
		
vertexptr  *LIST;			/* an array for saving k best scores */
vertexptr  low = 0;			/* lowest score node in LIST */
vertexptr  most = 0;			/* latestly accessed node in LIST */
static long numnode;			/* the number of nodes in LIST */

static long *CC, *DD;			/* saving matrix scores */
static long *RR, *SS, *EE, *FF; 	/* saving start-points */
static long *HH, *WW;		 	/* saving matrix scores */
static long *II, *JJ, *XX, *YY; 	/* saving start-points */
static long  m1, mm, n1, nn;		/* boundaries of recomputed area */
static long  rl, cl;			/* left and top boundaries */
static long  min;			/* minimum score in LIST */
static short flag;			/* indicate if recomputation necessary*/

/* DIAG() assigns value to x if (ii,jj) is never used before */
#define DIAG(ii, jj, x, value)				\
{ for ( tt = 1, z = row[(ii)]; z != 0; z = z->NEXT )	\
    if ( z->COL == (jj) )				\
      { tt = 0; break; }				\
  if ( tt )						\
    x = ( value );					\
}

/* replace (ss1, xx1, yy1) by (ss2, xx2, yy2) if the latter is large */
#define ORDER(ss1, xx1, yy1, ss2, xx2, yy2)		\
{ if ( ss1 < ss2 )					\
    { ss1 = ss2; xx1 = xx2; yy1 = yy2; }		\
  else							\
    if ( ss1 == ss2 )					\
      { if ( xx1 < xx2 )				\
	  { xx1 = xx2; yy1 = yy2; }			\
	else						\
	  if ( xx1 == xx2 && yy1 < yy2 )		\
	    yy1 = yy2;					\
      }							\
}

/* The following definitions are for function diff() */

long  diff(), display();
static long  zero = 0;				/* long type zero        */

#define gap(k)  ((k) <= 0 ? 0 : q+r*(k))	/* k-symbol indel score */

static long *sapp;				/* Current script append ptr */
static long  last;				/* Last script op appended */

static long I, J;				/* current positions of A ,B */
static long no_mat; 				/* number of matches */ 
static long no_mis; 				/* number of mismatches */ 
static long al_len; 				/* length of alignment */
						/* Append "Delete k" op */
#define DEL(k)				\
{ I += k;				\
  al_len += k;				\
  if (last < 0)				\
    last = sapp[-1] -= (k);		\
  else					\
    last = *sapp++ = -(k);		\
}
						/* Append "Insert k" op */
#define INS(k)				\
{ J += k;				\
  al_len += k;				\
  if (last < 0)				\
    { sapp[-1] = (k); *sapp++ = last; }	\
  else					\
    last = *sapp++ = (k);		\
}

						/* Append "Replace" op */
#define REP 				\
{ last = *sapp++ = 0; 			\
  al_len += 1;				\
}

/* SIM(A,B,M,N,K,V,Q,R) reports K best non-intersecting alignments of
   the segments of A and B in order of similarity scores, where
   V[a][b] is the score of aligning a and b, and -(Q+R*i) is the score
   of an i-symbol indel.  						*/

SIM(A,B,M,N,K,V,Q,R,nseq) char A[],B[]; long M,N,K,V[][128],Q,R,nseq;
{ long endi, endj, stari, starj;	/* endpoint and startpoint */ 
  long  score;   			/* the max score in LIST */
  long count;				/* maximum size of list */	
  register  long  i, j;			/* row and column indices */
  char *ckalloc();			/* space-allocating function */
  long  *S;				/* saving operations for diff */
  vertexptr cur; 			/* temporary pointer */
  vertexptr findmax();	 		/* return the largest score node */
	
	/* allocate space for all vectors */
	j = (N + 1) * sizeof(long);
	CC = ( long * ) ckalloc(j);
	DD = ( long * ) ckalloc(j);
	RR = ( long * ) ckalloc(j);
	SS = ( long * ) ckalloc(j);
	EE = ( long * ) ckalloc(j);
	FF = ( long * ) ckalloc(j);
	i = (M + 1) * sizeof(long);
	HH = ( long * ) ckalloc(i);
	WW = ( long * ) ckalloc(i);
	II = ( long * ) ckalloc(i);
	JJ = ( long * ) ckalloc(i);
	XX = ( long * ) ckalloc(i);
	YY = ( long * ) ckalloc(i);
	S = ( long * ) ckalloc(i + j);
	row = ( pairptr * ) ckalloc( (M + 1) * sizeof(pairptr));

	/* set up list for each row */
	for ( i = 1; i <= M; i++ )
	  if ( nseq == 2 )
	     row[i] = 0;
	  else
	    { row[i] = z = ( pairptr ) ckalloc( (long) sizeof(pair));
              z->COL = i;			
              z->NEXT = 0;
	    }

	v = V;
	q = Q;
	r = R;
	qr = q + r;

	LIST = ( vertexptr * ) ckalloc( K * sizeof(vertexptr));
	for ( i = 0; i < K ; i++ )
	   LIST[i] = ( vertexptr ) ckalloc( (long) sizeof(vertex));

	printf("s {\n  \"%s\" 1 %d\n  \"%s\" 1 %d\n}\n", name1, M, name2, N);
	printf("k {\n  \"%% match\"\n}\n");

	numnode = min = 0;
	big_pass(A,B,M,N,K,nseq);

        /* Report the K best alignments one by one. After each alignment is
           output, recompute part of the matrix. First determine the size
	   of the area to be recomputed, then do the recomputation         */

	for ( count = K - 1; count >= 0 ; count-- )
	  { if ( numnode == 0 )
	      fatal("The number of alignments computed is too large");
            cur = findmax();	/* Return a pointer to a node with max score*/
            score = cur->SCORE;
      	    stari = ++cur->STARI;
            starj = ++cur->STARJ;
            endi = cur->ENDI;
            endj = cur->ENDJ;
            m1 = cur->TOP;
            mm = cur->BOT;
            n1 = cur->LEFT;
            nn = cur->RIGHT;
            rl = endi - stari + 1;
            cl = endj - starj + 1;
            I = stari - 1;
            J = starj - 1;
            sapp = S;
            last = 0;
            al_len = 0;
            no_mat = 0;
	    no_mis = 0;
            diff(&A[stari]-1, &B[starj]-1,rl,cl,q,q);
            /* Output the best alignment */
            printf("a {\n  s %1.1lf\n  b %d %d\n  e %d %d\n",
                score/10.0, stari, starj, endi, endj);
	
            display(&A[stari]-1,&B[starj]-1,rl,cl,S,stari,starj);
            printf("}\n");
	    fflush(stdout);

            if ( count )
	      { flag = 0;
                locate(A,B,nseq);
                if ( flag )
		   small_pass(A,B,count,nseq);
              }
	  }
}

/* A big pass to compute K best classes */

big_pass(A,B,M,N,K,nseq) char A[],B[]; long M,N,K,nseq;
{ register  long  i, j;			/* row and column indices */
  register  long  c;			/* best score at current point */
  register  long  f;			/* best score ending with insertion */
  register  long  d;			/* best score ending with deletion */
  register  long  p;			/* best score at (i-1, j-1) */
  register  long  ci, cj;		/* end-point associated with c */ 
  register  long  di, dj;		/* end-point associated with d */
  register  long  fi, fj;		/* end-point associated with f */
  register  long  pi, pj;		/* end-point associated with p */
  long  *va;				/* pointer to v(A[i], B[j]) */
  long   addnode();			/* function for inserting a node */

	
	/* Compute the matrix and save the top K best scores in LIST
	   CC : the scores of the current row
	   RR and EE : the starting point that leads to score CC
	   DD : the scores of the current row, ending with deletion
	   SS and FF : the starting point that leads to score DD        */
 	/* Initialize the 0 th row */
	for ( j = 1; j <= N ; j++ )
	  {  CC[j] = 0;
	     RR[j] = 0;
	     EE[j] = j;
	     DD[j] = - (q);
	     SS[j] = 0;
	     FF[j] = j;
	  }
	for ( i = 1; i <= M; i++) 
	  {  c = 0;				/* Initialize column 0 */
	     f = - (q);
	     ci = fi = i;
	     va = v[A[i]];
	     if ( nseq == 2 )
	       { p = 0;
	         pi = i - 1;
	         cj = fj = pj = 0;
	       }
	     else
	       { p = CC[i];
		 pi = RR[i];
		 pj = EE[i];
	         cj = fj = i;
	       }
	     for ( j = (nseq == 2 ? 1 : (i+1)) ; j <= N ; j++ )  
	       {  f = f - r;
		  c = c - qr;
		  ORDER(f, fi, fj, c, ci, cj)
		  c = CC[j] - qr; 
		  ci = RR[j];
		  cj = EE[j];
		  d = DD[j] - r;
		  di = SS[j];
		  dj = FF[j];
		  ORDER(d, di, dj, c, ci, cj)
		  c = 0;
		  DIAG(i, j, c, p+va[B[j]])		/* diagonal */
		  if ( c <= 0 )
		    { c = 0; ci = i; cj = j; }
		  else
		    { ci = pi; cj = pj; }
		  ORDER(c, ci, cj, d, di, dj)
		  ORDER(c, ci, cj, f, fi, fj)
		  p = CC[j];
		  CC[j] = c;
		  pi = RR[j];
		  pj = EE[j];
		  RR[j] = ci;
		  EE[j] = cj;
		  DD[j] = d;
		  SS[j] = di;
		  FF[j] = dj;
		  if ( c > min )	/* add the score into list */
		    min = addnode(c, ci, cj, i, j, K, min);
	        }
	  }
}

/* Determine the left and top boundaries of the recomputed area */

locate(A,B,nseq) char A[],B[]; long nseq;
{ register  long  i, j;			/* row and column indices */
  register  long  c;			/* best score at current point */
  register  long  f;			/* best score ending with insertion */
  register  long  d;			/* best score ending with deletion */
  register  long  p;			/* best score at (i-1, j-1) */
  register  long  ci, cj;		/* end-point associated with c */ 
  register  long  di, dj;		/* end-point associated with d */
  register  long  fi, fj;		/* end-point associated with f */
  register  long  pi, pj;		/* end-point associated with p */
  short  cflag, rflag;			/* for recomputation */
  long  *va;				/* pointer to v(A[i], B[j]) */
  long   addnode();			/* function for inserting a node */
  long  limit;				/* the bound on j */

	/* Reverse pass
	   rows
	   CC : the scores on the current row
	   RR and EE : the endpoints that lead to CC
	   DD : the deletion scores 
	   SS and FF : the endpoints that lead to DD

	   columns
	   HH : the scores on the current columns
	   II and JJ : the endpoints that lead to HH
	   WW : the deletion scores
	   XX and YY : the endpoints that lead to WW
	*/
	for ( j = nn; j >= n1 ; j-- )
          {  CC[j] = 0;
	     EE[j] = j;
	     DD[j] = - (q);
	     FF[j] = j;
	     if ( nseq == 2 || j > mm )
                RR[j] = SS[j] = mm + 1;
	     else
                RR[j] = SS[j] = j;
	  }

        for ( i = mm; i >= m1; i-- )
	  {  c = p = 0;
	     f = - (q);
	     ci = fi = i;
	     pi = i + 1;
	     cj = fj = pj = nn + 1;
	     va = v[A[i]];
	     if ( nseq == 2 || n1 > i )
		limit = n1;
	     else
		limit = i + 1;
	     for ( j = nn; j >= limit ; j-- )  
	       {  f = f - r;
		  c = c - qr;
		  ORDER(f, fi, fj, c, ci, cj)
		  c = CC[j] - qr; 
		  ci = RR[j];
		  cj = EE[j];
		  d = DD[j] - r;
		  di = SS[j];
		  dj = FF[j];
		  ORDER(d, di, dj, c, ci, cj)
		  c = 0;
		  DIAG(i, j, c, p+va[B[j]])		/* diagonal */
		  if ( c <= 0 )
		    { c = 0; ci = i; cj = j; }
		  else
		    { ci = pi; cj = pj; }
		  ORDER(c, ci, cj, d, di, dj)
		  ORDER(c, ci, cj, f, fi, fj)
		  p = CC[j];
		  CC[j] = c;
		  pi = RR[j];
		  pj = EE[j];
		  RR[j] = ci;
		  EE[j] = cj;
		  DD[j] = d;
		  SS[j] = di;
		  FF[j] = dj;
		  if ( c > min )
		    flag = 1;
	        }
	     if ( nseq == 2 || i < n1 )
	       { HH[i] = CC[n1];
	         II[i] = RR[n1];
	         JJ[i] = EE[n1];
	         WW[i] = f;
	         XX[i] = fi;
	         YY[i] = fj;
	       }
	  }
      
  for ( rl = m1, cl = n1; ; )
    { for ( rflag = cflag = 1; ( rflag && m1 > 1 ) || ( cflag && n1 > 1 ) ;  )
        { if ( rflag && m1 > 1 )	/* Compute one row */
            { rflag = 0;
	      m1--;
      	      c = p = 0;
	      f = - (q);
	      ci = fi = m1;
	      pi = m1 + 1;
	      cj = fj = pj = nn + 1;
	      va = v[A[m1]];
	      for ( j = nn; j >= n1 ; j-- )  
	        { f = f - r;
		  c = c - qr;
		  ORDER(f, fi, fj, c, ci, cj)
		  c = CC[j] - qr; 
		  ci = RR[j];
		  cj = EE[j];
		  d = DD[j] - r;
		  di = SS[j];
		  dj = FF[j];
		  ORDER(d, di, dj, c, ci, cj)
		  c = 0;
		  DIAG(m1, j, c, p+va[B[j]])		/* diagonal */
		  if ( c <= 0 )
		    { c = 0; ci = m1; cj = j; }
		  else
		    { ci = pi; cj = pj; }
		  ORDER(c, ci, cj, d, di, dj)
		  ORDER(c, ci, cj, f, fi, fj)
		  p = CC[j];
		  CC[j] = c;
		  pi = RR[j];
		  pj = EE[j];
		  RR[j] = ci;
		  EE[j] = cj;
		  DD[j] = d;
		  SS[j] = di;
		  FF[j] = dj;
		  if ( c > min )
		     flag = 1;
		  if ( ! rflag && ( ci > rl && cj > cl || di > rl && dj > cl
	 		                            || fi > rl && fj > cl ) )
		      rflag = 1;
	        }
	      HH[m1] = CC[n1];
	      II[m1] = RR[n1];
	      JJ[m1] = EE[n1];
	      WW[m1] = f;
	      XX[m1] = fi;
	      YY[m1] = fj;
	      if ( ! cflag && ( ci > rl && cj > cl || di > rl && dj > cl
			     || fi > rl && fj > cl ) )
	         cflag = 1;
	    }

	  if ( nseq == 1 && n1 == (m1 + 1) && ! rflag )
	     cflag = 0;
	  if ( cflag && n1 > 1 )	/* Compute one column */
	    { cflag = 0;
	      n1--;
	      c = 0;
	      f = - (q);
	      cj = fj = n1;
	      va = v[B[n1]];
	      if ( nseq == 2 || mm < n1 )
		{ p = 0;
	          ci = fi = pi = mm + 1;
	          pj = n1 + 1;
		  limit = mm;
		}
	      else
		{ p = HH[n1];
		  pi = II[n1];
		  pj = JJ[n1];
	          ci = fi = n1;
		  limit = n1 - 1;
		}
	      for ( i = limit; i >= m1 ; i-- )  
	        { f = f - r;
		  c = c - qr;
		  ORDER(f, fi, fj, c, ci, cj)
		  c = HH[i] - qr; 
		  ci = II[i];
		  cj = JJ[i];
		  d = WW[i] - r;
		  di = XX[i];
		  dj = YY[i];
		  ORDER(d, di, dj, c, ci, cj)
		  c = 0;
	          DIAG(i, n1, c, p+va[A[i]])
		  if ( c <= 0 )
		    { c = 0; ci = i; cj = n1; }
		  else
		    { ci = pi; cj = pj; }
		  ORDER(c, ci, cj, d, di, dj)
		  ORDER(c, ci, cj, f, fi, fj)
		  p = HH[i];
		  HH[i] = c;
		  pi = II[i];
		  pj = JJ[i];
		  II[i] = ci;
		  JJ[i] = cj;
		  WW[i] = d;
		  XX[i] = di;
		  YY[i] = dj;
		  if ( c > min )
		     flag = 1;
	          if ( ! cflag && ( ci > rl && cj > cl || di > rl && dj > cl
		               || fi > rl && fj > cl ) )
		     cflag = 1;
	        }
	      CC[n1] = HH[m1];
	      RR[n1] = II[m1];
	      EE[n1] = JJ[m1];
	      DD[n1] = f;
	      SS[n1] = fi;
	      FF[n1] = fj;
	      if ( ! rflag && ( ci > rl && cj > cl || di > rl && dj > cl
		                                 || fi > rl && fj > cl ) )
	         rflag = 1;
	    }
	}
      if ( m1 == 1 && n1 == 1 || no_cross() )
	 break;
   }
  m1--;
  n1--;
}

/* recompute the area on forward pass */
small_pass(A,B,count,nseq) char A[], B[]; long count, nseq;
{ register  long  i, j;			/* row and column indices */
  register  long  c;			/* best score at current point */
  register  long  f;			/* best score ending with insertion */
  register  long  d;			/* best score ending with deletion */
  register  long  p;			/* best score at (i-1, j-1) */
  register  long  ci, cj;		/* end-point associated with c */ 
  register  long  di, dj;		/* end-point associated with d */
  register  long  fi, fj;		/* end-point associated with f */
  register  long  pi, pj;		/* end-point associated with p */
  long  *va;				/* pointer to v(A[i], B[j]) */
  long   addnode();			/* function for inserting a node */
  long  limit;				/* lower bound on j */

	for ( j = n1 + 1; j <= nn ; j++ )
	  {  CC[j] = 0;
	     RR[j] = m1;
	     EE[j] = j;
	     DD[j] = - (q);
	     SS[j] = m1;
	     FF[j] = j;
	  }
	for ( i = m1 + 1; i <= mm; i++) 
	  {  c = 0;				/* Initialize column 0 */
	     f = - (q);
	     ci = fi = i;
	     va = v[A[i]];
	     if ( nseq == 2 || i <= n1 )
	       { p = 0;
	         pi = i - 1;
	         cj = fj = pj = n1;
		 limit = n1 + 1;
	       }
	     else
	       { p = CC[i];
		 pi = RR[i];
		 pj = EE[i];
	         cj = fj = i;
		 limit = i + 1;
	       }
	     for ( j = limit ; j <= nn ; j++ )  
	       {  f = f - r;
		  c = c - qr;
		  ORDER(f, fi, fj, c, ci, cj)
		  c = CC[j] - qr; 
		  ci = RR[j];
		  cj = EE[j];
		  d = DD[j] - r;
		  di = SS[j];
		  dj = FF[j];
		  ORDER(d, di, dj, c, ci, cj)
		  c = 0;
		  DIAG(i, j, c, p+va[B[j]])		/* diagonal */
		  if ( c <= 0 )
		    { c = 0; ci = i; cj = j; }
		  else
		    { ci = pi; cj = pj; }
		  ORDER(c, ci, cj, d, di, dj)
		  ORDER(c, ci, cj, f, fi, fj)
		  p = CC[j];
		  CC[j] = c;
		  pi = RR[j];
		  pj = EE[j];
		  RR[j] = ci;
		  EE[j] = cj;
		  DD[j] = d;
		  SS[j] = di;
		  FF[j] = dj;
		  if ( c > min )	/* add the score into list */
		    min = addnode(c, ci, cj, i, j, count, min);
	        }
	  }
}

/* Add a new node into list.  */

long addnode(c, ci, cj, i, j, K, cost)  long c, ci, cj, i, j, K, cost;
{ short found;				/* 1 if the node is in LIST */
  register long d;

  found = 0;
  if ( most != 0 && most->STARI == ci && most->STARJ == cj )
    found = 1;
  else
     for ( d = 0; d < numnode ; d++ )
	{ most = LIST[d];
	  if ( most->STARI == ci && most->STARJ == cj )
	    { found = 1;
	      break;
	    }
        }
  if ( found )
    { if ( most->SCORE < c )
        { most->SCORE = c;
          most->ENDI = i;
          most->ENDJ = j;
        }
      if ( most->TOP > i ) most->TOP = i;
      if ( most->BOT < i ) most->BOT = i;
      if ( most->LEFT > j ) most->LEFT = j;
      if ( most->RIGHT < j ) most->RIGHT = j;
    }
  else
    { if ( numnode == K )	/* list full */
	 most = low;
      else
         most = LIST[numnode++];
      most->SCORE = c;
      most->STARI = ci;
      most->STARJ = cj;
      most->ENDI = i;
      most->ENDJ = j;
      most->TOP = most->BOT = i;
      most->LEFT = most->RIGHT = j;
    }
  if ( numnode == K )
    { if ( low == most || ! low ) 
        { for ( low = LIST[0], d = 1; d < numnode ; d++ )
            if ( LIST[d]->SCORE < low->SCORE )
              low = LIST[d];
	}
      return ( low->SCORE ) ;
    }
  else
    return cost;
}

/* Find and remove the largest score in list */

vertexptr findmax()
{ vertexptr  cur;
  register long i, j;

  for ( j = 0, i = 1; i < numnode ; i++ )
    if ( LIST[i]->SCORE > LIST[j]->SCORE )
       j = i;
  cur = LIST[j];
  if ( j != --numnode )
    { LIST[j] = LIST[numnode];
      LIST[numnode] =  cur;
    }
  most = LIST[0];
  if ( low == cur ) low = LIST[0];
  return ( cur );
}

/* return 1 if no node in LIST share vertices with the area */

no_cross()
{ vertexptr  cur;
  register long i;

      for ( i = 0; i < numnode; i++ )
	{ cur = LIST[i];
	  if ( cur->STARI <= mm && cur->STARJ <= nn && cur->BOT >= m1-1 && 
	       cur->RIGHT >= n1-1 && ( cur->STARI < rl || cur->STARJ < cl ))
	     { if ( cur->STARI < rl ) rl = cur->STARI;
	       if ( cur->STARJ < cl ) cl = cur->STARJ;
	       flag = 1;
	       break;
	     }
	}
      if ( i == numnode )
	return 1;
      else
	return 0;
}

/* diff(A,B,M,N,tb,te) returns the score of an optimum conversion between
   A[1..M] and B[1..N] that begins(ends) with a delete if tb(te) is zero
   and appends such a conversion to the current script.                   */

long diff(A,B,M,N,tb,te) char *A, *B; long M, N; long tb, te;

{ long   midi, midj, type;	/* Midpoint, type, and cost */
  long midc;

{ register long   i, j;
  register long c, e, d, s;
           long t, *va;
  	   char  *ckalloc();

/* Boundary cases: M <= 1 or N == 0 */

  if (N <= 0)
    { if (M > 0) DEL(M)
      return - gap(M);
    }
  if (M <= 1)
    { if (M <= 0)
        { INS(N);
          return - gap(N);
        }
      if (tb > te) tb = te;
      midc = - (tb + r + gap(N) );
      midj = 0;
      va = v[A[1]];
      for (j = 1; j <= N; j++)
        {  for ( tt = 1, z = row[I+1]; z != 0; z = z->NEXT )	
              if ( z->COL == j+J )			
	         { tt = 0; break; }		
           if ( tt )			
            { c = va[B[j]] - ( gap(j-1) + gap(N-j) );
              if (c > midc)
               { midc = c;
                 midj = j;
               }
	    }
	}
      if (midj == 0)
        { INS(N) DEL(1) }
      else
        { if (midj > 1) INS(midj-1)
          REP
	  if ( A[1] == B[midj] )
	     no_mat += 1;
	  else
	     no_mis += 1;
	  /* mark (A[I],B[J]) as used: put J into list row[I] */	
          I++; J++;
	  z = ( pairptr ) ckalloc( (long) sizeof(pair));
          z->COL = J;			
          z->NEXT = row[I];				
	  row[I] = z;
          if (midj < N) INS(N-midj)
        }
      return midc;
    }

/* Divide: Find optimum midpoint (midi,midj) of cost midc */

  midi = M/2;			/* Forward phase:                          */
  CC[0] = 0;			/*   Compute C(M/2,k) & D(M/2,k) for all k */
  t = -q;
  for (j = 1; j <= N; j++)
    { CC[j] = t = t-r;
      DD[j] = t-q;
    }
  t = -tb;
  for (i = 1; i <= midi; i++)
    { s = CC[0];
      CC[0] = c = t = t-r;
      e = t-q;
      va = v[A[i]];
      for (j = 1; j <= N; j++)
        { if ((c = c - qr) > (e = e - r)) e = c;
          if ((c = CC[j] - qr) > (d = DD[j] - r)) d = c;
	  DIAG(i+I, j+J, c, s+va[B[j]])
          if (c < d) c = d;
          if (c < e) c = e;
          s = CC[j];
          CC[j] = c;
          DD[j] = d;
        }
    }
  DD[0] = CC[0];

  RR[N] = 0;			/* Reverse phase:                          */
  t = -q;			/*   Compute R(M/2,k) & S(M/2,k) for all k */
  for (j = N-1; j >= 0; j--)
    { RR[j] = t = t-r;
      SS[j] = t-q;
    }
  t = -te;
  for (i = M-1; i >= midi; i--)
    { s = RR[N];
      RR[N] = c = t = t-r;
      e = t-q;
      va = v[A[i+1]];
      for (j = N-1; j >= 0; j--)
        { if ((c = c - qr) > (e = e - r)) e = c;
          if ((c = RR[j] - qr) > (d = SS[j] - r)) d = c;
	  DIAG(i+1+I, j+1+J, c, s+va[B[j+1]])
          if (c < d) c = d;
          if (c < e) c = e;
          s = RR[j];
          RR[j] = c;
          SS[j] = d;
        }
    }
  SS[N] = RR[N];

  midc = CC[0]+RR[0];		/* Find optimal midpoint */
  midj = 0;
  type = 1;
  for (j = 0; j <= N; j++)
    if ((c = CC[j] + RR[j]) >= midc)
      if (c > midc || CC[j] != DD[j] && RR[j] == SS[j])
        { midc = c;
          midj = j;
        }
  for (j = N; j >= 0; j--)
    if ((c = DD[j] + SS[j] + q) > midc)
      { midc = c;
        midj = j;
        type = 2;
      }
}

/* Conquer: recursively around midpoint */

  if (type == 1)
    { diff(A,B,midi,midj,tb,q);
      diff(A+midi,B+midj,M-midi,N-midj,q,te);
    }
  else
    { diff(A,B,midi-1,midj,tb,zero);
      DEL(2);
      diff(A+midi+1,B+midj,M-midi-1,N-midj,zero,te);
    }
  return midc;
}

/* Alignment display routine */

long display(A,B,M,N,S,AP,BP) char A[], B[]; long M, N; long S[], AP, BP;
{ long   i, j, op, start_i, start_j, match, mis_match;

  for (i = j = 0; i < M || j < N; ) {
	start_i = i;
	start_j = j;
	match = mis_match = 0;
	while (i < M && j < N && *S == 0) {
		++i;
		++j;
		if (A[i] == B[j])
			++match;
		else
			++mis_match;
		S++;
	}
	printf("  l %d %d %d %d %1.1f\n", AP+start_i, BP+start_j, AP+i-1,
		BP+j-1, (float)(100*match)/(float)(match+mis_match));
	if (i < M || j < N)
		if ((op = *S++) > 0)
			j += op;
		else
			i -= op;
    }
}

/* lib.c - library of C procedures. */

/* fatal - print message and die */
fatal(msg)
char *msg;
{
	fprintf(stderr, "%s\n", msg);
	exit(1);
}

/* fatalf - format message, print it, and die */
fatalf(msg, val)
char *msg, *val;
{
	fprintf(stderr, msg, val);
	putc('\n', stderr);
	exit(1);
}
	
/* ckopen - open file; check for success */
FILE *ckopen(name, mode)
char *name, *mode;
{
	FILE *fopen(), *fp;

	if ((fp = fopen(name, mode)) == NULL)
		fatalf("Cannot open %s.", name);
	return(fp);
}

/* ckalloc - allocate space; check for success */
char *ckalloc(amount)
long amount;
{
	char *malloc(), *p;

	if ((p = malloc( (unsigned) amount)) == NULL)
		fatal("Ran out of memory.");
	return(p);
}
