#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <math.h>
#include <stdbool.h>
#include <string.h>
#include <time.h>

// maximum size of the (square) im
#define S 4096
// default margin
#define M 10
// number of hits of the border before we stop
#define H 1
// real size of im, margin and hits of the border
int s, m1, m2, h;

// tmpres 
#define TMPRES 1000

// max number of particles
#define MAXP 6000000

// resulting lichtenberg im
bool im[S][S];

bool indisk(int x, int y, int r, int m) {
    return (pow(x-r,2)+pow(y-r,2)<pow(r-m-2,2));
}

bool nearcell(int x, int y) {
    return(im[x-1][y] || im[x+1][y] || im[x][y-1] || im[x][y+1] || im[x-1][y-1] || im[x+1][y+1] || im[x-1][y+1] || im[x+1][y-1]);
}

void savepgm(int s, char *filename, char *DATE, char *extra) {
    // output the im in PGM format
    int i, j, dot;
    int l;
    FILE *output;
    // 5 is for - and .pgm
    l=strlen(DATE)+strlen(filename)+strlen(extra)+5;
    char namefile[l+1];
    strcpy(namefile,filename);
    strcat(namefile,DATE);
    strcat(namefile,"-");
    strcat(namefile,extra);
    strcat(namefile,".pgm");
    printf("filename generated: %s\n", namefile);
    output = fopen(namefile,"w");
    fprintf(output,"P2\n");
    fprintf(output,"%d\n",s);
    fprintf(output,"%d\n",s);
    fprintf(output,"1\n");
    for (i=0; i<s; i++) {
        for (j=0; j<s; j++) {
	    if (im[i][j]) dot = 2;
	    else dot = 1;
            fprintf(output,"%d ",dot%2);
	}
    }
    fclose(output);
}

int main(int argc, char *argv[]) {

    int i = 0, j;
    int x, y;
    int nbp = 1, nbh = 0;
    double t;
    bool done = false;
    char *extra = '\0';

    // default values
    s=1024;
    m1=M;
    m2=0;
    h=H;
    if (argc == 6) {
        s=atoi(argv[1]);
        m1=atoi(argv[2]);
        m2=atoi(argv[3]);
        h=atoi(argv[4]);
        extra=argv[5];
    }

    char DATE[14];
    time_t now = time(NULL);
    struct tm *time = localtime(&now);
    strftime(DATE, sizeof(DATE)-1, "-%Y%m%d%H%M", time);
    DATE[13] = 0;

    printf("DLA program %s launched with s=%d, m1=%d, m2=%d, h=%d at date %s\n", argv[0], s, m1, m2, h, DATE);

    // radius of total figure
    int r = s/2;

    // new seed at each run
    srand(getpid());

    // init image to only central point
    for (i=0; i<s; i++)
        for (j=0; j<s; j++)
            im[i][j] = false;
    // first attractor: a particle is stuck in the middle of the im
    im[r][r] = true;

    while (!done) {
    	i++;

	// save temporary image each TMPRES particles
	if (nbp % TMPRES == 0) {
	    // printf(".");
	    fflush(stdout);
	    // savepgm(s,"temp",DATE,extra);
	}
	// initial position of the particle: launch on a circle at m from center
	t = ((double) rand() / (RAND_MAX));
        x = (int)((r-m1) * cos(2*M_PI*t)+r);
        y = (int)((r-m1) * sin(2*M_PI*t)+r);

        // random walk move the particle until it reaches a border or close to existing stucked particle
        while (indisk(x,y,r,0)) {
	    // random walk
	    t = ((double) rand() / (RAND_MAX)); // 0<=r<1
	    if (t < 0.25) x--; // left
	    else if (t < 0.50) x++; // right
	    else if (t < 0.75) y++; // up
	    else y--; // down
	    // if particle gets near a stuck one we freeze it and launch a new particle
	    if (nearcell(x,y)) {
	    	im[x][y]=true;
		nbp++;
                printf("fixpt: part #%d, x=%d, y=%d, nbhit=%d\n",nbp,x,y,nbh);
		// stop after MAXP drawn or if more than h hits of particle grows outside the disk delimited by launch circle (r-m)
		// we use extra margin m2 when there are multiple h hits required to avoid circles shapes to allow random parts
		if (! indisk(x,y,r,m1+m2)) nbh++;
		if ((nbp>MAXP) || (nbh>=h)) done = true;
		break;
	    }
        }
    }
    savepgm(s,"lichtenberg",DATE,extra);
    printf("DLA program %s launched with s=%d, m1=%d, m2=%d, h=%d at date %s: number of parts %d and fractal dimension %f\n",
	   argv[0], s, m1, m2, h, DATE, nbp, -log2((double)nbp)/log2(1/(double)(s-m1-m2)));
    return 0;
}
