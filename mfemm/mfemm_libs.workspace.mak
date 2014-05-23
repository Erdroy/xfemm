#------------------------------------------------------------------------------#
# This makefile was generated by 'cbp2make' tool rev.147                       #
#------------------------------------------------------------------------------#


WRKDIR = `pwd`
MAKE = make

all: pfemm_fmesher_libfmesher pfemm_fsolver_libfsolver pfemm_fpproc_libfpproc

pfemm_fmesher_libfmesher: 
	$(MAKE) -C pfemm/fmesher all -f libfmesher.cbp.mak

pfemm_fsolver_libfsolver: 
	$(MAKE) -C pfemm/fsolver all -f libfsolver.cbp.mak

pfemm_fpproc_libfpproc: 
	$(MAKE) -C pfemm/fpproc all -f libfpproc.cbp.mak

clean: clean_pfemm_fmesher_libfmesher clean_pfemm_fsolver_libfsolver clean_pfemm_fpproc_libfpproc

clean_pfemm_fmesher_libfmesher: 
	$(MAKE) -C pfemm/fmesher clean -f libfmesher.cbp.mak

clean_pfemm_fsolver_libfsolver: 
	$(MAKE) -C pfemm/fsolver clean -f libfsolver.cbp.mak

clean_pfemm_fpproc_libfpproc: 
	$(MAKE) -C pfemm/fpproc clean -f libfpproc.cbp.mak

.PHONY: clean_pfemm_fmesher_libfmesher clean_pfemm_fsolver_libfsolver clean_pfemm_fpproc_libfpproc

