
*
* HBF1AL: changes the internals of a 1D HBOOK histogram acccording
* to the arguments.
*
* O.Couet Nov 2001
*
* Arguments: ID    - histogram ID
*            X     - array of bin contents
*            E     - array of bin errors
*            IOPT  - If .ne. 1 the error array is ignored.
*            NOENT - number of entries
*            SW    - sum of weight
*            SWW   - sum of weight^2
*            SWX   - sum of weight*value
*            SWXX  - sum of weight*value^2
*
* The last arguments are used by HBOOK to compute mean, RMS and 
* number of equivalent events on-the-fly.
*

      SUBROUTINE HBF1AL(ID,X,NOENT,SW,SWW,SWX,SWXX,E,IOPT)

      DOUBLE PRECISION SWD(2)
      DIMENSION X(1),E(1)

*     Some of the HBOOK common blocks
      INTEGER     NWPAW,IXPAWC,IHDIV,IXHIGZ,IXKU,        LMAIN
      REAL                                       FENC   ,      HCV
      COMMON/PAWC/NWPAW,IXPAWC,IHDIV,IXHIGZ,IXKU,FENC(5),LMAIN,HCV(9989)
      INTEGER   IQ        ,LQ
      REAL            Q
      DIMENSION IQ(2),Q(2),LQ(8000)
      EQUIVALENCE (LQ(1),LMAIN),(IQ(1),LQ(9)),(Q(1),IQ(1))
      INTEGER       HVERSN,IHWORK,LHBOOK,LHPLOT,LGTIT,LHWORK,
     +LCDIR,LSDIR,LIDS,LTAB,LCID,LCONT,LSCAT,LPROX,LPROY,LSLIX,
     +LSLIY,LBANX,LBANY,LPRX,LPRY,LFIX,LLID,LR1,LR2,LNAME,LCHAR,LINT,
     +LREAL,LBLOK,LLBLK,LBUFM,LBUF,LTMPM,LTMP,LTMP1,LHPLIP,LHDUM,
     +LHFIT,LFUNC,LHFCO,LHFNA,LCIDN
      COMMON/HCBOOK/HVERSN,IHWORK,LHBOOK,LHPLOT,LGTIT,LHWORK,
     +LCDIR,LSDIR,LIDS,LTAB,LCID,LCONT,LSCAT,LPROX,LPROY,LSLIX,
     +LSLIY,LBANX,LBANY,LPRX,LPRY,LFIX,LLID,LR1,LR2,LNAME,LCHAR,LINT,
     +LREAL,LBLOK,LLBLK,LBUFM,LBUF,LTMPM,LTMP,LTMP1,LHPLIP,LHDUM(9),
     +LHFIT,LFUNC,LHFCO,LHFNA,LCIDN
      INTEGER   KNCX   ,KXMIN  ,KXMAX  ,KMIN1  ,KMAX1 ,KNORM  , KTIT1,
     +          KNCY   ,KYMIN  ,KYMAX  ,KMIN2  ,KMAX2 ,KSCAL2 , KTIT2,
     +          KNBIT  ,KNOENT ,KSTAT1 ,KNSDIR  ,KNRH ,
     +          KCON1  ,KCON2  ,KBITS  ,KNTOT
      PARAMETER(KNCX=3,KXMIN=4,KXMAX=5,KMIN1=7,KMAX1=8,KNORM=9,KTIT1=10,
     +          KNCY=7,KYMIN=8,KYMAX=9,KMIN2=6,KMAX2=10,KSCAL2=11,
     +          KTIT2=12,KNBIT=1,KNOENT=2,KSTAT1=3,KNSDIR=5,KNRH=6,
     +          KCON1=9,KCON2=3,KBITS=1,KNTOT=2)

*     Pack contents and errors
      CALL HPAK(ID,X)
      IF (IOPT.EQ.1) CALL HPAKE(ID,E)

*     Number of entries and two of the sums
      IQ(LCONT+KNOENT) = NOENT
      Q(LCONT+KSTAT1)  = SW
      Q(LCONT+KSTAT1+1)= SWW
*     The other two sums (double precision)
      SWD(1) = SWX
      SWD(2) = SWXX 
      CALL UCOPY(SWD,Q(LCONT+KSTAT1+2),4)

      END
