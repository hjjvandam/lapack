/*****************************************************************************
  Copyright (c) 2014, Intel Corp.
  All rights reserved.

  Redistribution and use in source and binary forms, with or without
  modification, are permitted provided that the following conditions are met:

    * Redistributions of source code must retain the above copyright notice,
      this list of conditions and the following disclaimer.
    * Redistributions in binary form must reproduce the above copyright
      notice, this list of conditions and the following disclaimer in the
      documentation and/or other materials provided with the distribution.
    * Neither the name of Intel Corporation nor the names of its contributors
      may be used to endorse or promote products derived from this software
      without specific prior written permission.

  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
  AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
  IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
  ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE
  LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
  CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
  SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
  INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
  CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
  ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF
  THE POSSIBILITY OF SUCH DAMAGE.
*****************************************************************************
* Contents: Native middle-level C interface to LAPACK function dgedmdq
* Author: Intel Corporation
*****************************************************************************/

#include "lapacke_utils.h"

lapack_int LAPACKE_dgedmdq_work( int matrix_layout, char jobs, char jobz,
                                 char jobr, char jobq, char jobt, char jobf,
                                 lapack_int whtsvd, lapack_int m, lapack_int n,
                                 double* f, lapack_int ldf, double* x,
                                 lapack_int ldx, double* y, lapack_int ldy,
                                 lapack_int nrnk, double tol, lapack_int k,
                                 double* reig, double* imeig, double* z,
                                 lapack_int ldz, double* res, double* b,
                                 lapack_int ldb, double* v, lapack_int ldv,
                                 double* s, lapack_int lds, double* work,
                                 lapack_int lwork, double* iwork,
                                 lapack_int liwork )
{
    lapack_int info = 0;
    if( matrix_layout == LAPACK_COL_MAJOR ) {
        /* Call LAPACK function and adjust info */
        LAPACK_dgedmdq( &jobs, &jobz, &jobr, &jobq, &jobt, &jobf, &whtsvd, &m,
                        &n, f, &ldf, x, &ldx, y, &ldy, &nrnk, tol, &k, reig,
                        imeig, z, &ldz, res, b, &ldb, v, &ldv, s, &lds,
                        work, &lwork, iwork, &liwork, &info );
        if( info < 0 ) {
            info = info - 1;
        }
    } else if( matrix_layout == LAPACK_ROW_MAJOR ) {
        lapack_int ldf_t = MAX(1,m);
        lapack_int ldx_t = MAX(1,m);
        lapack_int ldy_t = MAX(1,m);
        lapack_int ldz_t = MAX(1,m);
        lapack_int ldb_t = MAX(1,m);
        lapack_int ldv_t = MAX(1,m);
        lapack_int lds_t = MAX(1,m);
        double* f_t = NULL;
        double* x_t = NULL;
        double* y_t = NULL;
        double* z_t = NULL;
        double* b_t = NULL;
        double* v_t = NULL;
        double* s_t = NULL;
        /* Check leading dimension(s) */
        if( ldx < n ) {
            info = -5;
            LAPACKE_xerbla( "LAPACKE_dgedmdq_work", info );
            return info;
        }
        if( ldy < n ) {
            info = -5;
            LAPACKE_xerbla( "LAPACKE_dgedmdq_work", info );
            return info;
        }
        if( ldz < n ) {
            info = -5;
            LAPACKE_xerbla( "LAPACKE_dgedmdq_work", info );
            return info;
        }
        if( ldb < n ) {
            info = -5;
            LAPACKE_xerbla( "LAPACKE_dgedmdq_work", info );
            return info;
        }
        if( ldv < n ) {
            info = -5;
            LAPACKE_xerbla( "LAPACKE_dgedmdq_work", info );
            return info;
        }
        if( lds < n ) {
            info = -5;
            LAPACKE_xerbla( "LAPACKE_dgedmdq_work", info );
            return info;
        }
        /* Query optimal working array(s) size if requested */
        if( lwork == -1 || liwork == -1 ) {
            LAPACK_dgedmdq( &jobs, &jobz, &jobr, &jobq, &jobt, &jobf, &whtsvd, &m,
                            &n, f, &ldf, x, &ldx, y, &ldy, &nrnk, tol, &k, reig,
                            imeig, z, &ldz, res, b, &ldb, v, &ldv, s, &lds,
                            work, &lwork, iwork, &liwork, &info );
            return (info < 0) ? (info - 1) : info;
        }
        /* Allocate memory for temporary array(s) */
        f_t = (double*)LAPACKE_malloc( sizeof(double) * ldf_t * MAX(1,n) );
        if( f_t == NULL ) {
            info = LAPACK_TRANSPOSE_MEMORY_ERROR;
            goto exit_level_0;
        }
        x_t = (double*)LAPACKE_malloc( sizeof(double) * ldx_t * MAX(1,n) );
        if( x_t == NULL ) {
            info = LAPACK_TRANSPOSE_MEMORY_ERROR;
            goto exit_level_0;
        }
        y_t = (double*)LAPACKE_malloc( sizeof(double) * ldy_t * MAX(1,n) );
        if( y_t == NULL ) {
            info = LAPACK_TRANSPOSE_MEMORY_ERROR;
            goto exit_level_0;
        }
        z_t = (double*)LAPACKE_malloc( sizeof(double) * ldz_t * MAX(1,n) );
        if( z_t == NULL ) {
            info = LAPACK_TRANSPOSE_MEMORY_ERROR;
            goto exit_level_0;
        }
        b_t = (double*)LAPACKE_malloc( sizeof(double) * ldb_t * MAX(1,n) );
        if( b_t == NULL ) {
            info = LAPACK_TRANSPOSE_MEMORY_ERROR;
            goto exit_level_0;
        }
        v_t = (double*)LAPACKE_malloc( sizeof(double) * ldv_t * MAX(1,n) );
        if( v_t == NULL ) {
            info = LAPACK_TRANSPOSE_MEMORY_ERROR;
            goto exit_level_0;
        }
        s_t = (double*)LAPACKE_malloc( sizeof(double) * lds_t * MAX(1,n) );
        if( s_t == NULL ) {
            info = LAPACK_TRANSPOSE_MEMORY_ERROR;
            goto exit_level_0;
        }
        /* Transpose input matrices */
        LAPACKE_dge_trans( matrix_layout, m, n, f, ldf, f_t, ldf_t );
        LAPACKE_dge_trans( matrix_layout, m, n, x, ldx, x_t, ldx_t );
        LAPACKE_dge_trans( matrix_layout, m, n, y, ldy, y_t, ldy_t );
        LAPACKE_dge_trans( matrix_layout, m, n, z, ldz, z_t, ldz_t );
        LAPACKE_dge_trans( matrix_layout, m, n, b, ldb, b_t, ldb_t );
        LAPACKE_dge_trans( matrix_layout, m, n, v, ldv, v_t, ldv_t );
        LAPACKE_dge_trans( matrix_layout, m, n, s, lds, s_t, lds_t );
        /* Call LAPACK function and adjust info */
        LAPACK_dgedmdq( &jobs, &jobz, &jobr, &jobq, &jobt, &jobf, &whtsvd, &m,
                        &n, f, &ldf, x, &ldx, y, &ldy, &nrnk, tol, &k, reig,
                        imeig, z, &ldz, res, b, &ldb, v, &ldv, s, &lds,
                        work, &lwork, iwork, &liwork, &info );
        if( info < 0 ) {
            info = info - 1;
        }
        /* Transpose output matrices */
        LAPACKE_dge_trans( LAPACK_COL_MAJOR, m, n, f_t, ldf_t, f, ldf );
        LAPACKE_dge_trans( LAPACK_COL_MAJOR, m, n, x_t, ldx_t, x, ldx );
        LAPACKE_dge_trans( LAPACK_COL_MAJOR, m, n, y_t, ldy_t, y, ldy );
        LAPACKE_dge_trans( LAPACK_COL_MAJOR, m, n, z_t, ldz_t, z, ldz );
        LAPACKE_dge_trans( LAPACK_COL_MAJOR, m, n, b_t, ldb_t, b, ldb );
        LAPACKE_dge_trans( LAPACK_COL_MAJOR, m, n, v_t, ldv_t, v, ldv );
        LAPACKE_dge_trans( LAPACK_COL_MAJOR, m, n, s_t, lds_t, s, lds );
        /* Release memory and exit */
exit_level_1:
        LAPACKE_free( f_t );
        LAPACKE_free( x_t );
        LAPACKE_free( y_t );
        LAPACKE_free( z_t );
        LAPACKE_free( b_t );
        LAPACKE_free( v_t );
        LAPACKE_free( s_t );
exit_level_0:
        if( info == LAPACK_TRANSPOSE_MEMORY_ERROR ) {
            LAPACKE_xerbla( "LAPACKE_dgedmdq_work", info );
        }
    } else {
        info = -1;
        LAPACKE_xerbla( "LAPACKE_dgedmdq_work", info );
    }
    return info;
}
