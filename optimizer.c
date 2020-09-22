/*******************************************
 * Solutions for the CS:APP Performance Lab
 ********************************************/

#include <stdio.h>
#include <stdlib.h>
#include "defs.h"

/* 
 * Please fill in the following student struct 
 */
student_t student = {
  "Ryan Shirey",     /* Full name */
  "u0889965@utah.edu",  /* Email address */
};

/***************
 * COMPLEX KERNEL
 ***************/

/******************************************************
 * Your different versions of the complex kernel go here
 ******************************************************/

/* 
 * naive_complex - The naive baseline version of complex 
 */
char naive_complex_descr[] = "naive_complex: Naive baseline implementation";
void naive_complex(int dim, pixel *src, pixel *dest)
{
  int i, j;

  for(i = 0; i < dim; i++)
    for(j = 0; j < dim; j++)
    {

      dest[RIDX(dim - j - 1, dim - i - 1, dim)].red = ((int)src[RIDX(i, j, dim)].red +
						      (int)src[RIDX(i, j, dim)].green +
						      (int)src[RIDX(i, j, dim)].blue) / 3;
      
      dest[RIDX(dim - j - 1, dim - i - 1, dim)].green = ((int)src[RIDX(i, j, dim)].red +
							(int)src[RIDX(i, j, dim)].green +
							(int)src[RIDX(i, j, dim)].blue) / 3;
      
      dest[RIDX(dim - j - 1, dim - i - 1, dim)].blue = ((int)src[RIDX(i, j, dim)].red +
						       (int)src[RIDX(i, j, dim)].green +
						       (int)src[RIDX(i, j, dim)].blue) / 3;

    }
}


/* 
 * complex - Your current working version of complex
 * IMPORTANT: This is the version you will be graded on
 */
char complex_descr[] = "complex: Current working version";
void complex(int dim, pixel *src, pixel *dest)
{
  int i, j, w,c,d;
  w = 8;
  int ii, jj;
  for(i=0; i<dim; i+=w){
    for(j=0; j<dim; j+=w){
      for(ii=i; ii<i+w; ii++){
       
	int tempSrcA = RIDX(ii,j,dim);
	int tempDesA = RIDX(dim-1-j,dim-ii-1,dim);
	c=0;
	d=0;
	for(jj=j; jj<j+w; jj++ ){
	  
	  
	  int tempColA = ((int)src[tempSrcA+c].red +
			  (int)src[tempSrcA+c].green+
			  (int)src[tempSrcA+c].blue)/3;
	  dest[tempDesA+d].red = tempColA;
	  dest[tempDesA+d].green = tempColA;
	  dest[tempDesA+d].blue = tempColA;
	  c++;
	  d-=dim;
	}
      }
    }
  }
}

/*********************************************************************
 * register_complex_functions - Register all of your different versions
 *     of the complex kernel with the driver by calling the
 *     add_complex_function() for each test function. When you run the
 *     driver program, it will test and report the performance of each
 *     registered test function.  
 *********************************************************************/

void register_complex_functions() {
  add_complex_function(&complex, complex_descr);
  add_complex_function(&naive_complex, naive_complex_descr);
}


/***************
 * MOTION KERNEL
 **************/

/***************************************************************
 * Various helper functions for the motion kernel
 * You may modify these or add new ones any way you like.
 **************************************************************/


/* 
 * weighted_combo - Returns new pixel value at (i,j) 
 */
__attribute__((always_inline))static pixel weighted_combo(int dim, int i, int j, pixel *src) 
{
  int ii, jj;
  pixel current_pixel;

  int red, green, blue;
  red = green = blue = 0;

  int num_neighbors = 0;
  for(ii=0; ii < 3; ii++)
    for(jj=0; jj < 3; jj++) 
      if ((i + ii < dim) && (j + jj < dim)) 
      {
	num_neighbors++;
	red += (int) src[RIDX(i+ii,j+jj,dim)].red;
	green += (int) src[RIDX(i+ii,j+jj,dim)].green;
	blue += (int) src[RIDX(i+ii,j+jj,dim)].blue;
      }
  
  current_pixel.red = (unsigned short) (red / num_neighbors);
  current_pixel.green = (unsigned short) (green / num_neighbors);
  current_pixel.blue = (unsigned short) (blue / num_neighbors);
  
  return current_pixel;
}



/******************************************************
 * Your different versions of the motion kernel go here
 ******************************************************/


/*
 * naive_motion - The naive baseline version of motion 
 */
char naive_motion_descr[] = "naive_motion: Naive baseline implementation";
void naive_motion(int dim, pixel *src, pixel *dst) 
{
  int i, j;
    
  for (i = 0; i < dim; i++)
    for (j = 0; j < dim; j++)
      dst[RIDX(i, j, dim)] = weighted_combo(dim, i, j, src);
}


/*
 * motion - Your current working version of motion. 
 * IMPORTANT: This is the version you will be graded on
 */
char motion_descr[] = "motion: Current working version";
void motion(int dim, pixel *src, pixel *dst) 
{
  
  
  int i, j,target;
  int edge = dim-2;
  int red, green, blue;
  pixel current_pixel;
  for(i = 0; i<edge; i++){
    for(j = 0; j<edge; j++){
      red = green = blue = 0;
      target = RIDX(i,j,dim);
      //1a
      red += (int) src[target].red;
      green += (int) src[target].green;
      blue += (int) src[target].blue;
      
      //2a
      red += (int) src[target+1].red;
      green += (int) src[target+1].green;
      blue += (int) src[target+1].blue;
      //3a
      red += (int) src[target+2].red;
      green += (int) src[target+2].green;
      blue += (int) src[target+2].blue;
      //1b
      red += (int) src[target+dim].red;
      green += (int) src[target+dim].green;
      blue += (int) src[target+dim].blue;
      //2b
      red += (int) src[target+dim+1].red;
      green += (int) src[target+dim+1].green;
      blue += (int) src[target+dim+1].blue;
      //3b
      red += (int) src[target+dim+2].red;
      green += (int) src[target+dim+2].green;
      blue += (int) src[target+dim+2].blue;
      //1c
      red += (int) src[target+dim+dim].red;
      green += (int) src[target+dim+dim].green;
      blue += (int) src[target+dim+dim].blue;
      //2c
      red += (int) src[target+dim+dim+1].red;
      green += (int) src[target+dim+dim+1].green;
      blue += (int) src[target+dim+dim+1].blue;
      //3c
      red += (int) src[target+dim+dim+2].red;
      green += (int) src[target+dim+dim+2].green;
      blue += (int) src[target+dim+dim+2].blue;
      
      current_pixel.red = (unsigned short) (red / 9);
      current_pixel.green = (unsigned short) (green / 9);
      current_pixel.blue = (unsigned short) (blue / 9);
      dst[target] = current_pixel;
    }
    
  } 

  for(i=edge;i<dim;i++){
    for(j=0;j<dim;j++){
      dst[RIDX(i, j, dim)] = weighted_combo(dim, i, j, src);
      dst[RIDX(j, i, dim)] = weighted_combo(dim, j, i, src);
      
    }
  }
  
  
  
  //naive_motion(dim,src,dst);
}

/********************************************************************* 
 * register_motion_functions - Register all of your different versions
 *     of the motion kernel with the driver by calling the
 *     add_motion_function() for each test function.  When you run the
 *     driver program, it will test and report the performance of each
 *     registered test function.  
 *********************************************************************/

void register_motion_functions() {
  add_motion_function(&motion, motion_descr);
  add_motion_function(&naive_motion, naive_motion_descr);
}
