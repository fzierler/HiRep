/******************************************************************************
*
* Test of modules
*
******************************************************************************/

#define MAIN_PROGRAM

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include "io.h"
#include "random.h"
#include "error.h"
#include "geometry.h"
#include "memory.h"
#include "statistics.h"
#include "update.h"
#include "global.h"
#include "observables.h"
#include "suN.h"
#include "suN_types.h"
#include "dirac.h"
#include "linear_algebra.h"
#include "inverters.h"
#include "representation.h"
#include "utils.h"
#include "logger.h"
#include "communications.h"

int nhb,nor,nit,nth,nms,level,seed;
double beta;

static double hmass=1.2;


void D(spinor_field *out, spinor_field *in){
   Dphi(hmass,out,in);
}

void H(spinor_field *out, spinor_field *in){
   g5Dphi(hmass,out,in);
}

static spinor_field *tmp;
static spinor_field_flt *tmp_flt;

void M(spinor_field *out, spinor_field *in){
  g5Dphi_eopre_sq(-hmass,out,in);
}

void M_flt(spinor_field_flt *out, spinor_field_flt *in){
  g5Dphi_eopre_sq_flt(-hmass,out,in);
}


int main(int argc,char *argv[])
{
   char pame[256];
   int i;
   double tau;
   spinor_field *s1, *s2;
   spinor_field *res, *res2;
   struct timeval start, end, etime;

   mshift_par par;

   int cgiters;

   /* setup process id and communications */
   setup_process(&argc,&argv);
   
   /* logger setup */

  logger_setlevel(0,100); /* log all */
  if (PID!=0) { 
    logger_disable();}
  else{
    sprintf(pame,">out_%d",PID); logger_stdout(pame);
    sprintf(pame,"err_%d",PID); freopen(pame,"w",stderr);
  }
     
   lprintf("MAIN",0,"PId =  %d [world_size: %d]\n\n",PID,WORLD_SIZE); 
   
   /* read input file */
   read_input(glb_var.read,"test_input");
   rlxd_init(glb_var.rlxd_level,glb_var.rlxd_seed);
   
   
   /* setup communication geometry */
   if (geometry_init() == 1) {
     finalize_process();
     return 0;
   }
   
   lprintf("MAIN",0,"Gauge group: SU(%d)\n",NG);
   lprintf("MAIN",0,"Fermion representation: " REPR_NAME " [dim=%d]\n",NF);
   lprintf("MAIN",0,"global size is %dx%dx%dx%d\n",GLB_T,GLB_X,GLB_Y,GLB_Z);
   lprintf("MAIN",0,"proc grid is %dx%dx%dx%d\n",NP_T,NP_X,NP_Y,NP_Z);
   
   /* setup lattice geometry */
   geometry_mpi_eo();
   /* test_geometry_mpi_eo(); */
   MPI_Barrier(GLB_COMM);
   u_gauge=alloc_gfield(&glattice);
#ifndef REPR_FUNDAMENTAL
   u_gauge_f=alloc_gfield_f(&glattice);
   u_gauge_f_flt=alloc_gfield_f_flt(&glattice);
#endif
   represent_gauge_field();
   
   lprintf("MAIN",0,"Generating a random gauge field... ");
   fflush(stdout);
   random_u(u_gauge);
   lprintf("MAIN",0,"done.\n");
   represent_gauge_field();
   assign_ud2u_f();
   
   par.n = 6;
   par.shift=(double*)malloc(sizeof(double)*(par.n));
   par.err2=1.e-20;
   par.max_iter=0;
   res=alloc_spinor_field_f(2*par.n+3,&glat_even);
   s1=res+par.n;
   s2=s1+1;
   tmp=s2+1;
   res2=tmp+1;
   tmp_flt=alloc_spinor_field_f_flt(1,&glat_even);

   par.shift[0]=-0.0;
   par.shift[1]=-0.1;
   par.shift[2]=-0.2;
   par.shift[3]=+0.1;
   par.shift[4]=+0.2;
   par.shift[5]=+0.3;

   gaussian_spinor_field(s1);
   
   
   /* TEST CG_M */

   lprintf("CG TEST",0,"Testing CG multishift FLOAT\n");
   lprintf("CG TEST",0,"---------------------\n");

   gettimeofday(&start,0);
   cgiters = cg_mshift_flt(&par, &M, &M_flt, s1, res);
   gettimeofday(&end,0);
   timeval_subtract(&etime,&end,&start);
   lprintf("CG TEST",0,"Inversion took: %ld sec %ld usec\n",etime.tv_sec,etime.tv_usec);
   lprintf("CG TEST",0,"Converged in %d iterations\n",cgiters);
   for(i=0;i<par.n;++i){
     M(s2,&res[i]);
     spinor_field_mul_add_assign_f(s2,-par.shift[i],&res[i]);
     spinor_field_sub_assign_f(s2,s1);
     tau=spinor_field_sqnorm_f(s2)/spinor_field_sqnorm_f(s1);
     lprintf("CG TEST",0,"test cg[%d] = %e (req. %e)\n",i,tau,par.err2);
   }

   lprintf("cg test",0,"testing CG multishift DOUBLE\n");
   lprintf("cg test",0,"---------------------\n");

   gettimeofday(&start,0);
   cgiters = cg_mshift(&par, &M, s1, res2);
   gettimeofday(&end,0);
   timeval_subtract(&etime,&end,&start);
   lprintf("CG TEST",0,"Inversion took: %ld sec %ld usec\n",etime.tv_sec,etime.tv_usec);
   lprintf("CG TEST",0,"Converged in %d iterations\n",cgiters);
   for(i=0;i<par.n;++i){
     M(s2,&res2[i]);
     spinor_field_mul_add_assign_f(s2,-par.shift[i],&res2[i]);
     spinor_field_sub_assign_f(s2,s1);
     tau=spinor_field_sqnorm_f(s2)/spinor_field_sqnorm_f(s1);
     lprintf("CG TEST",0,"test cg[%d] = %e (req. %e)\n",i,tau,par.err2);
   }

   lprintf("CG TEST",0,"Consistency FLOAT <-> DOUBLE\n");
   lprintf("CG TEST",0,"---------------------\n");

   for(i=0;i<par.n;++i){
     spinor_field_sub_assign_f(res+i,res2+i);
     M(s2,res+i);
     spinor_field_mul_add_assign_f(s2,-par.shift[i],&res[i]);
     tau=spinor_field_sqnorm_f(s2)/spinor_field_sqnorm_f(s1);
     lprintf("CG TEST",0,"test double-single[%d] = %e (req. %e)\n",i,tau,par.err2);
   }


   free_spinor_field(res);
   free_spinor_field_flt(tmp_flt);
   free(par.shift);

   finalize_process();

   return 0;
}
