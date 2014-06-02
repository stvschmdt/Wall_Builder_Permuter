/*Steve Schmidt - Coding Challenge 
 *This program is designed to take in 2 parameters (contains a default setting) width and height (depth) of 
 *a 'wall' whereby two foot and three foot bricks are used to construct a crack free wall*/

#include "wallHeader.h"

/*Arrays of struct type Rows (see wallHeader.h) of static size
 * ideally these would be dynamicaaly allocated and freed through the course of the program
 * however each struct is of dynamically malloc'd size*/
Row permutes[LIN_COMBO];
Row matrix[MAX_LENGTH];

/*scoreboard array to hold the 2D matrix of compatibility*/
int scoreboard[MAX_LENGTH][MAX_LENGTH];
/*global counts during each step of the process*/
int p_count=0;
int u_count=0;
int m_count = 0;
int count = 0;
int width = 9;
int depth = 3;
long tot_counter = 0;


int main (int argc, char *argv[]){
    int i;

    /*init vars from command line - if no arguments standard W(9,3) is used*/
    if(argc == 3){
	width = atoi(argv[1]);
	depth = atoi(argv[2]);
    }

    /*generate an array of possible linear combinations of 2's 3's in binary representation 1==2 0==3*/
    get_solutions(width);
    
    /*for each binary representation, generate all permuatations (w duplication) of these i.e. input 0111->1101..1011..1110 and so on
      store these in an array - this is not efficient, would like to no add duplicates to array- could have been done with qsort at least*/
    for(i=0;i<p_count;i++){
	new_permute_row(permutes[i].row, 0, permutes[i].size);
    }
    
    /*build a 2D matrix with a dynamic programming flavor where M(i,j) is 0 if compatible below, 1 if not compatible below*/
    fill_scoreboard(atoi(argv[1]));
    /*free up some memory!*/ 
    for(i=0;i<u_count;i++){
	free(matrix[i].row);
    }
    
    /*recursively count (with a function shout out to karel the robot who loves to count) the number of various build strategies*/
    for(i=0;i<u_count;i++){
	count_karel(depth-2, matrix[i], i);
    }
    
    /*print result*/
    printf("%ld\n",tot_counter);
    
    /*free again*/
    for(i=0;i<p_count;i++){
	free(permutes[i].row);
    }

    return 0;
}

/*this function only returns at the last row of bricks, it called recursively each instance returns one count of a variation*/
void count_karel(int d, Row counter, int c){
    int j;    
    for(j=0;j<u_count;j++){
	if(d == 0 && scoreboard[c][j] == 1){
	    tot_counter +=1;
	}
	else if(scoreboard[c][j] == 1){
	    count_karel(d-1, matrix[j], j);
	}
    }

}	

/*this function fills out the 2D matrix of compatibility with a 1 iff compatible below 0 otherwise*/
void fill_scoreboard(int b){
    int i, j, m, n;
    int sum_upper = 0;
    int sum_lower = 0;
    int add = 1;
    for(i=0;i<u_count;i++){
	for(j=0;j<u_count;j++){

	    scoreboard[i][j] = 1;
	    m = 0;
	    n = 0;
	    sum_upper = 0;
	    sum_lower = 0;
	    add = 1;
	    for(m = 0;m<matrix[j].size-1;m++){
		if(add == 1){
		    if(matrix[i].row[n]==1){
			sum_upper+=2;
		    }
		    else{
			sum_upper+=3;
		    }
		}

		if(matrix[j].row[m]==1){
		    sum_lower += 2;
		}
		else{
		    sum_lower+=3;
		}
		if(sum_lower == sum_upper ){
		    scoreboard[i][j] = 0;
		    break;
		}
		if(sum_lower > sum_upper){
		    n++;
		    if(matrix[i].row[n]==1){
			sum_upper+=2;
		    }
		    else{
			sum_upper+=3;
		    }
		    if(sum_lower == sum_upper){
			scoreboard[i][j] = 0;
			break;
		    }
		}
		add = 0;
	    }

	}
    }
}

/*calculates the linear combinations of 2x+3y=width and sends each combination to get permuted*/
void get_solutions(int b){
    int i, j;
    for(i = 0;i<b;i++){
	for(j=0;j<b;j++){
	    if(2*i+3*j == b){
		make_row(permutes, i,j);
		count++;
	    }
	}
    }

}

/*converts linear combination to binary i.e. x=3 y=1-->1110*/
int * make_row(Row permutes[],int x, int y){
    int i, j;
    permutes[p_count].row = malloc(sizeof(int)*(x+y));
    permutes[p_count].size = x+y;
    for(i=0; i< x; i++){
	permutes[p_count].row[i] = 1;
	permutes[p_count].id += pow(2,i);
    }
    for(j=x;j<y;j++){
	permutes[p_count].row[j] = 0;
    }
    p_count++;
    return permutes[p_count].row;

}

/*for each possible binary representation, store in a global array all the permuations of this
 *i.e. 1110-->1101->1011->0111 with duplication which is expensive on time complexity and memory
 *the goal was to short circuit and remove duplicate on the fly which is still a bit expensive without sorting*/
void new_permute_row(int *array,int i,int width) { 
    if (width == i){
	int p,j;
	int x,c=0;
	/*setup a new permutation and check if it is unique*/
	matrix[u_count].row = malloc(sizeof(int)*width);
	matrix[u_count].size = width;

	for(p=0;p<width;p++){
	    matrix[u_count].row[p] = array[p];
	}
	matrix[u_count].id =0; 
        /*assign a unique id*/
	for(j=0;j<matrix[u_count].size;j++){
	    if(matrix[u_count].row[j] == 1){

		matrix[u_count].id += pow(2,j); 
	    }
	}
	//assign_one_id(u_count);
	for(x=0;x<u_count;x++){
	    if(matrix[u_count].size == matrix[x].size && matrix[u_count].id == matrix[x].id){
		c = 1;
	    }
	}

	/*overwrite the value next time if its a duplicate to save memory*/
	if(c==0){
	    u_count++;

	}
	return;
    }

    /*borrowed from my C Class Assignment*/
    int j = i;
    for (j = i; j < width; j++) { 
	swap( array+i,array+j);
	new_permute_row(array,i+1,width);
	swap( array+i,array+j);

    }
    return;
}

/*canonical swap function for permutations*/
void swap( int * row, int * array ){
    int temp = *row;
    *row = *array;
    *array = temp;
}

