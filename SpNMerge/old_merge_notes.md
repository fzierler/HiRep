# Old merge notes
These notes were written after merging the Swansea SPN
work into the "trunk" version.


## Converter/archive_eolexi.c

Some differences only in the read functions. In the write functions, 
everything should be 'covariant'. It is suboptimal that in the write functions,
MPI communications take actually double than the amount of data needed. 
This is, though, the same that happens for SON. 
Since this is not a critical region of the code, we have not changed that. 

## HMC/hmc.c
In the SPN version, 'fenv.h' was included (possibly for testing).
All changes will be discarded, except the insertions at lines 205-206.

## HMC/hmc_forces.c
A Trivial lptinf change.

## LLR_HB/llr_hb.c
This file was not present in the non-spn version.

## LLR_HMC/llr_hmc.c
This file was not present in the non-spn version.


## LibHR/Random/random_suNg.c
* The function 'random_suNg_unit_vector' is declared in Include/random.h,
  but defined in LibHR/Random/random_suNg.c only in the SPN version.
  Since it seems that it is not needed anywhere (even in the SPN version),
  I will not add it to the 'trunk' version.
* the function random_suNg_epsilon is not present in the 'trunk'. 
  In the SPN version, is only used in the update_constrained function, which
  is used in LLR stuff. Not added to it.
Added the other SPN-related changes, but removed the lines that were 
commented out.

## LibHR/Update/cabmar.c
* Copied as is from SPN version, then added 'static inline' modifier to wtos
  function declaration.

## LibHR/Update/random_momenta.c
* Conditional for the number of generators added. Function gaussian_scalar_momenta
  is missing in SPN version.

## LibHR/Update/representation.c
* In the SPN version, _group_represent2 is 'plugged' to _group_represent.
* The SPN version lacks some things the the trunk version has.

## LibHR/Utils/TMPL/suN_exp.c.tmpl
* Copied as is from SPN version.

## LibHR/Utils/det_suNg.c
* Copied as is from the SPN version.

## LibHR/Utils/inv_suNg.c
* Copied as is from the SPN version.

## LibHR/Utils/suN_utils.c
* Copied a large ifdef. The trunk version has modifications that the SPN version
  has not.

## Make/Utils/autosun/adjoint.h
* Copied as is from the SPN version.

## Make/Utils/autosun/antisymmetric.h
* Copied as is from the SPN version.

## Make/Utils/autosun/fundamental.h
* Copied as is from the SPN version.

## Make/Utils/autosun/representation.h
* Copied as is from the SPN version.

## Make/Utils/autosun/sun.h
* Copied as is from the SPN version.

## Make/Utils/autosun/symmetric.h
* Copied as is from the SPN version.

## RenormalizationFactors/measure_Z_mom.c
* Copied as is from the SPN version - only lprintf modifications.

## Spectrum/measure_formfactor.c
* Copied as is from the SPN version - only lprintf modifications.

## Spectrum/measure_spectrum.c
* Lots of work in the trunk version not present in the SPN version.
  Added lprintf modification.

## Spectrum/mk_mesons_with_z2semwall_new.c
* Copied as is from the SPN version - only lprintf modifications.

## TestProgram/Deflate/check_deflate.c
* Copied as is from the SPN version - only lprintf modifications.

## TestProgram/Propagator/check_propagator.c
* Copied as is from the SPN version - only lprintf modifications.

## WilsonFlow/WF_measure.c
* Copied as is from the SPN version - only lprintf modifications.

## WilsonFlow/WF_measure_adaptative.c
* Copied as is from the SPN version - only lprintf modifications.

# SECOND ROUND                                
Comparison between the current state of the code and the initial state of  
the repo (just taken from the SVN repo)                                   
**Changes are intended new minus old.**

./RenormalizationFactors/measure_Z_mom.c
  2 lines added, #elif GAUGE_SPN and a lprintf.

./Include/global.h
 For debug and testing, added a global var *suN_momenta_backup

./Include/complex.h
 Added new macros for SPN

./Include/memory.h
  Added some logic to differentiate between the SPN FUNDAMENTAL case and the 
  others, for clover fermions.

./Include/spinor_field.h
 Declaring a new struct, suNffull_field.

./Include/update.h
 Declared a update_ghmc_stripped function for testing.

./Include/utils.h
 Only a blank line removed.

./Make/Utils/autosun/fundamental.h
 Header guards added, an ifdef for the GAUGE_SPN case added.
 'RET +=' changed to 'RET =' (is an empty string before this assignment).

./Make/Utils/autosun/polynomial.h
 Header guards and proper includes added.

./Make/Utils/autosun/sparse.h
 Header guards and proper includes added.
 Added cerr.

./Make/Utils/autosun/sun.h
 Header guards and proper includes added.
 refactored init function, added overloaded version, #ifdefs for the gauge 
 group, replaced an ifdef for gauge group with a switch.
 The SPN portion of the code in group::init has been added between the SON and 
 the SUN case.
 Another exponential for the spn case has been implemented (luscher and taylor 
 methods).
 This part of the code has been tested extensively.
 
./Make/Utils/autosun/matrix.h
 Header guards and proper includes added.
 New class - spmatrix - added, new method for class smatrix created.
 
./Make/Utils/autosun/complex.h
 Header guards and proper includes added.
 Removed CR where it was added.

./Make/Utils/autosun/adjoint.h
 Header guards and proper includes added. some #ifdefs for the gauge group.
 Use of spmatrix in lieu of cmatrix for the SPN case.

./Make/Utils/autosun/symmetric.h
 Header guards and proper includes added. some #ifdefs for the gauge group.
 Use of spmatrix in lieu of cmatrix for the SPN case. (as above)



./Make/Utils/autosun/representation.h
 Header guards and proper includes added. some #ifdefs for the gauge group.
 Proper includes also depend on the group of choice.
 
./Make/Utils/autosun/antisymmetric.h
 Header guards and proper includes added. some #ifdefs for the gauge group.
 Antisymmetric representation for SPN is constructed in a different way from
 the symmetric one.
 Use of spmatrix in lieu of cmatrix for the SPN case. (as above)

./Make/Utils/autosun/list.h
 Header guards and proper includes added. 

./Converter/archive_eolexi.c
 Preprocessor directives for switching between SPN and SUN

./HMC/hmc_forces.c
 Preprocessor directives for switching between SPN and SUN, affecting only lprintf.

./HMC/hmc.c
 Preprocessor directives for switching between SPN and SUN, affecting only lprintf.
 (as above).

./WilsonFlow/WF_measure.c
 Preprocessor directives for switching between SPN and SUN, affecting only lprintf.
 (as above, 2).

./WilsonFlow/WF_measure_adaptative.c
 Preprocessor directives for switching between SPN and SUN, affecting only lprintf.
 (as above, 3).

./LibHR/Memory/amalloc.c
 Added a debug/printing function.

./LibHR/Memory/field_alloc.c
 Declared memory-related functions for the new type suNffull_field, for SPN.
 Wrapped in Macro logic.

./LibHR/Update/random_momenta.c
 ngen depends now on the gauge group (defined as the ratio
 sizeof(suNg_algebra_vector)/sizeof(double)).

./LibHR/Update/representation.c
 For spn _group_represent2 is only a wrapper to _group_represent.
 Wrapped in Macro logic.

./LibHR/Update/update_mt.c
 Created function update_ghmc_stripped, which is like update_ghmc but without 
 initialization and metropolis tests.

./LibHR/Update/luscherweisz.c
 A big amount of code originally commented out has been revived. Changes are 
 only in the revived, originally-commented out portion of the code.

./LibHR/Update/cabmar.c
 Machinery for SPN added. Note: not all su2 subgroup in SPN are taken care of.
 Note: in our version the random_su2 call on line 103 was commented out.
       de-commented it.

./LibHR/Update/clover_tools.c
 #ifdef for the SPN, FUNDAMENTAL case, to use suNffull matrix.

./LibHR/Utils/inv_suNg.c
 #ifdef for the SPN, FUNDAMENTAL case, to use suNffull matrix.

./LibHR/Utils/det_suNg.c
 #ifdef for the SPN, FUNDAMENTAL case, to use suNffull matrix.

./LibHR/Utils/suN_utils.c
 #ifdef for the SPN, adapted the projection algorithm (modified
   stabilized Gram-Schmidt)

./LibHR/Utils/HYP_smearing.c
 Project_cooling_to_suNg is not suitable for spn. Modified preprocessor logic
 to take care of this.

./LibHR/Utils/boundary_conditions.c
 Added some #ifdefs 

./LibHR/IO/logger.c
 Some initializations to NULL added

./LibHR/IO/archive_su2quat.c
 Added preprocessor directives to choose between SPN and SUN

./LibHR/Random/random_suNg.c
 Added preprocessor directives to choose between SPN and SUN.
 Implemented SPN case.
  
./Spectrum/measure_spectrum.c
 Preprocessor directives for switching between SPN and SUN, affecting only lprintf.
./Spectrum/measure_formfactor.c
 Preprocessor directives for switching between SPN and SUN, affecting only lprintf.
./Spectrum/mk_mesons_with_z2semwall_new.c
 Preprocessor directives for switching between SPN and SUN, affecting only lprintf.

# TEST PROGRAMS (NOT CRUCIAL)
./TestProgram/Deflate/check_deflate.c
  2 lines added, #elif GAUGE_SPN and a lprintf (as above).

./TestProgram/DiracOperator/check_diracoperator_1.c
  Assertion added, commented a wrong function out, glat_even used in place of
  glattice.

./TestProgram/Propagator/check_propagator.c
  2 lines added, #elif GAUGE_SPN and a lprintf (as above).

./TestProgram/Update/check_update_1.c
  A lot of changes. Now test works as intended. 
  No need to comment further since modification on this file have no 
  consequences elsewhere.

./TestProgram/Algebra/check_algebra_1.c
  Added some print functions, modifications specific for the SPN case
  No need to comment further since modification on this file have no 
  consequences elsewhere.


./TestProgram/Algebra/check_algebra_2.c
  Added some assertions, a function, refactored macro.
  No need to comment further since modification on this file have no 
  consequences elsewhere.


./TestProgram/PureGauge/check_puregauge_3.c
  2 lines added, #elif GAUGE_SPN and a lprintf (as above).
  No need to comment further since modification on this file have no 
  consequences elsewhere.


./TestProgram/PureGauge/check_puregauge_2.c
  2 lines added, #elif GAUGE_SPN and a lprintf (as above), 
  #error added (test does not work for SPN).
  No need to comment further since modification on this file have no 
  consequences elsewhere.


./TestProgram/PureGauge/check_puregauge_1.c
  assertions and a error check function added.
  2 lines added, #elif GAUGE_SPN and a lprintf (as above), 
  No need to comment further since modification on this file have no 
  consequences elsewhere.


