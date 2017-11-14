include(__link__.m4)

#ifndef __SYSVAR_H__
#define __SYSVAR_H__

#include <arch.h>

// Values Expected by Basic

#define SYS_IY   __SYS_IY
#define SYS_HLP  __SYS_HLP

// System Variables - Absolute Addresses
// See http://www.worldofspectrum.org/ZXSpectrum128+3Manual/chapter8pt25.html

extern unsigned char SYSVAR_SWAP[16];
extern unsigned char SYSVAR_STOO[17];
extern unsigned char SYSVAR_YOUNGER[9];
extern unsigned char SYSVAR_REGNUOY[16];
extern unsigned char SYSVAR_ONERR[24];
extern unsigned int SYSVAR_OLDHL;
extern unsigned int SYSVAR_OLDBC;
extern unsigned int SYSVAR_OLDAF;
extern unsigned int SYSVAR_TARGET;
extern unsigned int SYSVAR_RETADDR;
extern unsigned char SYSVAR_BANKM;
extern unsigned char SYSVAR_RAMRST;
extern unsigned char SYSVAR_RAMERR;
extern unsigned int SYSVAR_BAUD;
extern unsigned char SYSVAR_SERFL[2];
extern unsigned char SYSVAR_COL;
extern unsigned char SYSVAR_WIDTH;
extern unsigned char SYSVAR_TVPARS;
extern unsigned char SYSVAR_FLAGS3;
extern unsigned char SYSVAR_BANK678;
extern unsigned char SYSVAR_XLOC;
extern unsigned char SYSVAR_YLOC;
extern unsigned int SYSVAR_OLDSP;
extern unsigned int SYSVAR_SYNRET;
extern unsigned char SYSVAR_LASTV[5];
extern unsigned int SYSVAR_RCLINE;
extern unsigned int SYSVAR_RCSTART;
extern unsigned int SYSVAR_RCSTEP;
extern unsigned char SYSVAR_LODDRV;
extern unsigned char SYSVAR_SAVDRV;
extern unsigned char SYSVAR_DUMPLF;
extern unsigned char SYSVAR_STRIP1[8];
extern unsigned char SYSVAR_STRIP2[8];
extern unsigned char SYSVAR_TSTACK[];

extern unsigned char SYSVAR_KSTATE[8];
extern unsigned char SYSVAR_LASTK;
extern unsigned char SYSVAR_REPDEL;
extern unsigned char SYSVAR_REPPER;
extern unsigned int SYSVAR_DEFADD;
extern unsigned char SYSVAR_K_DATA;
extern unsigned char SYSVAR_TVDATA;
extern unsigned char SYSVAR_STRMS[38];
extern unsigned int SYSVAR_CHARS;
extern unsigned char SYSVAR_RASP;
extern unsigned char SYSVAR_PIP;
extern unsigned char SYSVAR_ERRNR;
extern unsigned char SYSVAR_FLAGS;
extern unsigned char SYSVAR_TVFLAG;
extern unsigned int SYSVAR_ERRSP;
extern unsigned int SYSVAR_LISTSP;
extern unsigned char SYSVAR_MODE;
extern unsigned int SYSVAR_NEWPPC;
extern unsigned char SYSVAR_NSPPC;
extern unsigned int SYSVAR_PPC;
extern unsigned char SYSVAR_SUBPPC;
extern unsigned char SYSVAR_BORDCR;
extern unsigned int SYSVAR_E_PPC;
extern unsigned int SYSVAR_VARS;
extern unsigned int SYSVAR_DEST;
extern unsigned int SYSVAR_CHANS;
extern unsigned int SYSVAR_CURCHL;
extern unsigned int SYSVAR_PROG;
extern unsigned int SYSVAR_NXTLIN;
extern unsigned int SYSVAR_DATADD;
extern unsigned int SYSVAR_E_LINE;
extern unsigned int SYSVAR_K_CUR;
extern unsigned int SYSVAR_CH_ADD;
extern unsigned int SYSVAR_X_PTR;
extern unsigned int SYSVAR_WORKSP;
extern unsigned int SYSVAR_STKBOT;
extern unsigned int SYSVAR_STKEND;
extern unsigned char SYSVAR_BREG;
extern unsigned int SYSVAR_MEM;
extern unsigned char SYSVAR_FLAGS2;
extern unsigned char SYSVAR_DF_SZ;
extern unsigned int SYSVAR_S_TOP;
extern unsigned int SYSVAR_OLDPPC;
extern unsigned char SYSVAR_OSPPC;
extern unsigned char SYSVAR_FLAGX;
extern unsigned int SYSVAR_STRLEN;
extern unsigned int SYSVAR_T_ADDR;
extern unsigned int SYSVAR_SEED;
extern unsigned char SYSVAR_FRAMES[3];
extern unsigned int SYSVAR_UDG;
extern unsigned char SYSVAR_COORDS[2];
extern unsigned char SYSVAR_P_POSN;
extern unsigned int SYSVAR_PRCC;
extern unsigned char SYSVAR_ECHO_E[2];
extern unsigned int SYSVAR_DF_CC;
extern unsigned int SYSVAR_DF_CCL;
extern unsigned char SYSVAR_S_POSN[2];
extern unsigned char SYSVAR_SPOSNL[2];
extern unsigned char SYSVAR_SCR_CT;
extern unsigned char SYSVAR_ATTR_P;
extern unsigned char SYSVAR_MASK_P;
extern unsigned char SYSVAR_ATTR_T;
extern unsigned char SYSVAR_MASK_T;
extern unsigned char SYSVAR_P_FLAG;
extern unsigned char SYSVAR_MEMBOT[30];
extern unsigned int SYSVAR_NMIADD;
extern unsigned int SYSVAR_RAMTOP;
extern unsigned int SYSVAR_P_RAMT;

#endif
