/*
 *
 *   z88dk Application Generator (appmake)
 *
 *   This file contains the driver and routines used by multiple
 *   modules
 * 
 *   $Id: appmake.c,v 1.50 2016-08-26 05:35:33 aralbrec Exp $
 */



#define MAIN_C
#include "appmake.h"
#include <stdlib.h>
#include <stdint.h>
#include <time.h>
#include "../config.h"


#if (__GNUC__ || _BSD_SOURCE || _SVID_SOURCE || _XOPEN_SOURCE >= 500)
#include <unistd.h>
#define mktempfile(a) close(mkstemp(a))
#else
#define mktempfile(a) mktemp(a)
#endif


char                c_install_dir[FILENAME_MAX + 1];

static char         chain_temp_filename[FILENAME_MAX+1];
static char        *program_name = NULL;

static void         main_usage(void);
static void         execute_command(char *target, int argc, char **argv, int chainmode);
static void         set_option_by_type(option_t *options, type_t type, char *value);
static int          option_parse(int argc, char *argv[], option_t *options);
static int          option_set(int pos, int max, char *argv[], option_t *opt);
static void         option_print(char *execname, char *ident, char *copyright, char *desc, char *longdesc, option_t *opts);
static void         get_temporary_filename(char *filen);
static void         cleanup_temporary_files(void);

static int          num_temp_files = 0;
static char       **temp_files = NULL;
static char         tmpnambuf[] = "apmXXXX";


int main(int argc, char *argv[])
{
    int     i;
    int     ac;
    char  **av;
    char   *ptr;
    char   *target = NULL;

#ifdef WIN32
    /* Randomize temporary filenames for windows */
    snprintf(tmpnambuf, sizeof(tmpnambuf), "apm%04X", ((unsigned int)time(NULL)) & 0xffff);
#endif

    if ((ptr = getenv("ZCCCFG")) != NULL) {
#ifdef WIN32
        snprintf(c_install_dir, sizeof(c_install_dir), "%s\\..\\..\\", ptr);
#else
        snprintf(c_install_dir, sizeof(c_install_dir), "%s/../../", ptr);
#endif
    }

    chain_temp_filename[0] = 0;

    /* If we're called by the name of one of the subprograms, that's the one we are */
    i = 0;
    while (machines[i].execname ) {
        if ( ( ptr = strstr(argv[0],machines[i].execname )  ) &&
             ( *(ptr + strlen(machines[i].execname) ) == '.' || *(ptr + strlen(machines[i].execname) ) == '\0' ) ) {
            target = machines[i].ident;  
            execute_command(target, argc, argv, 0);
            exit(0);
        }
        i++;
    }

    atexit(cleanup_temporary_files);

    /* So, let's check for + style execution */
    av = calloc(argc,sizeof(char*));
    ac = 0;
    
    for ( i = 0; i < argc; i++ ) {
        if ( argv[i][0] == '+' ) {
            if ( target != NULL ) {
                execute_command(target, ac, av, 1);
                ac = 0;
                memset(av, 0, argc * sizeof(char*));
            }
            target = &argv[i][1];
        } else {
            av[ac] = argv[i];
            ac++;
        }
    }
    

    if ( target == NULL ) {
        main_usage();
    }
    execute_command(target, ac, av, 2);
    exit(0);
}


static void execute_command(char *target, int argc, char **argv, int chainmode)
{
    int   i = 0;
    char  output_file[FILENAME_MAX+1];


    while (machines[i].ident ) {
        if ( strcmp(target,machines[i].ident) == 0 ) {
            option_parse(argc,argv,machines[i].options);
            program_name = target;
            if ( chainmode ) {
                /* Chained command, if we had a previous output, that's our input file for this stage */
                if ( strlen(chain_temp_filename) ) {
                    set_option_by_type(machines[i].options, OPT_STR|OPT_INPUT, chain_temp_filename);
                }

                /* Now, let's create a new temporary output file (but not if last in chain) */
                if ( chainmode == 1 ) {
                    get_temporary_filename(output_file);
                    set_option_by_type(machines[i].options, OPT_STR|OPT_OUTPUT, output_file);
                }
            }
            
            if ( machines[i].exec(target, machines[i].ident) == -1 ) {
                option_print(machines[i].execname, machines[i].ident,machines[i].copyright, machines[i].desc, machines[i].longdesc, machines[i].options);
                exit(1);
            }
            if ( chainmode == 1 ) {
                strcpy(chain_temp_filename, output_file);
            }

            program_name = NULL;
            return;            
        }
        i++;
    }

    fprintf(stderr,"Unknown machine target \"%s\"\n\n",target);
    main_usage();  /* Never returns */
}



/* Useful functions used by many targets */
void exit_log(int code, char *fmt, ...)
{
    va_list  ap;

    va_start(ap, fmt);
    if ( fmt != NULL ) {
        if ( program_name ) {
            fprintf(stderr, "%s: ", program_name);
        }

        vfprintf(stderr, fmt, ap);
    }

    va_end(ap);
    exit(code);
}


/* Search through debris from z80asm for some important parameters */
long parameter_search(char *filen, char *ext,char *target)
{
    char    name[FILENAME_MAX+1];
    char    buffer[LINEMAX+1];
    long    val=-1;
    FILE    *fp;

    if (filen == NULL)
        return(-1);

    /* Create the filename very quickly */
    strcpy(name,filen);
    strcat(name,ext);
    if ( (fp=fopen(name,"r"))==NULL) {
        sprintf(name,"Cannot open %s%s\n",filen,ext);
        myexit(name,1);
    }

     /* Successfully opened the file so search through it.. */
    while ( fgets(buffer,LINEMAX,fp) != NULL ) {
        if (strncmp(buffer,target,strlen(target)) == 0 ) {
            sscanf(buffer,"%*s%*s%*[ $]%lx",&val);
            break;
        }
    }
    fclose(fp);
    return(val);
}


long get_org_addr(char *crtfile)
{
    long    pos;

    if ( ( pos = parameter_search(crtfile,".sym","__crt_org_code") ) == -1 )
        pos = parameter_search(crtfile,".sym","CRT_ORG_CODE");

    return(pos);
}


/* Check for new c lib build and construct output binary
   Otherwise, try to open the "normal" file.
*/

FILE *fopen_bin(char *fname, char *crtfile)
{
    FILE   *fcode, *fdata, *fin;
    char    name[FILENAME_MAX+1];
    char    tname[FILENAME_MAX+1];
    char    cmdline[FILENAME_MAX*2 + 128];
    struct  stat st_file1;
    struct  stat st_file2;
    int     crt_model;
    int     c;

    if (strlen(fname) == 0) return (NULL);

    // Warn if aligned sections are not aligned

    for (c = 0x100; c >= 0x2; c >>= 1)
    {
        int start;

        // data sections

        snprintf(cmdline, sizeof(cmdline), "__data_align_%d_size", c);
        if (parameter_search(crtfile, ".map", cmdline) > 0)
        {
            snprintf(cmdline, sizeof(cmdline), "__data_align_%d_head", c);
            if (((start = parameter_search(crtfile, ".map", cmdline)) & (c - 1)) != 0)
                fprintf(stderr, "Warning: SECTION %*s is not aligned with start address %#x\n", (int)strlen(cmdline) - 5, cmdline, start);
        }

        // bss sections

        snprintf(cmdline, sizeof(cmdline), "__bss_align_%d_size", c);
        if (parameter_search(crtfile, ".map", cmdline) > 0)
        {
            snprintf(cmdline, sizeof(cmdline), "__bss_align_%d_head", c);
            if (((start = parameter_search(crtfile, ".map", cmdline)) & (c - 1)) != 0)
                fprintf(stderr, "Warning: SECTION %*s is not aligned with start address %#x\n", (int)strlen(cmdline) - 5, cmdline, start);
        }
    }

    // Sort out the binary

    strcpy(name, fname);
    suffix_change(name, "_CODE.bin");

    if (stat(name, &st_file2) < 0)
    {
        strcpy(name, fname);
        suffix_change(name, "_COMMON0.bin");
    }

    stat(fname, &st_file1);
  
    if ((stat(name, &st_file2) < 0) || (st_file2.st_mtime < st_file1.st_mtime)) {
        /* classic library */
        fcode = fopen(fname, "rb");

        /* We may have a data section */
        strcpy(name, fname);
        suffix_change(name, "_DATA.bin");
        if ( stat(name, &st_file2) < 0 ) {
             /* Nope, everything was all in one file */
             return fcode;
        }
    } else {
        // new c lib compile

        if (st_file1.st_size > 0)
            fprintf(stderr, "WARNING: some code or data may not be assigned to sections.\n");

        fcode = fopen(name, "rb");
    }

    if ((crtfile == NULL) || ((crt_model = parameter_search(crtfile,".sym", "__crt_model")) == -1))
        crt_model = 0;


    // 0: ram model, complete binary is "*_CODE.bin"

    if (crt_model == 0) return fcode;

    // form complete binary

    get_temporary_filename(tname);

    if ((fin = fopen(tname, "w+b")) == NULL)
        exit_log(1, "ERROR: Unable to create temporary file in fopen_bin()\n");

    // code portion is copied into complete binary

    while ((c = fgetc(fcode)) != EOF)
        fputc(c, fin);
    fclose(fcode);

    // data portion is appended to complete binary

    strcpy(name, fname);
    suffix_change(name, "_DATA.bin");

    if (crt_model == 1) {
        
        // 1: rom model, complete binary is "*_CODE.bin" + "*_DATA.bin"

        if ((fdata = fopen(name, "rb")) == NULL)
            exit_log(1, "ERROR: File %s not found for a rom model compile\n", name);

    } else {

        // 2: compressed rom model, complete binary is "*_CODE.bin" + zx7("*_DATA.bin")

        get_temporary_filename(tname);
        snprintf(cmdline, FILENAME_MAX*2 + 127, "%szx7 -f %s %s", EXEC_PREFIX,name, tname);

        if (system(cmdline) != 0)
            exit_log(1, "ERROR: Unable to compress %s\n", name);

        if ((fdata = fopen(tname, "rb")) == NULL)
            exit_log(1, "ERROR: Unable to open compressed data file %s\n", tname);

    }

    while ((c = fgetc(fdata)) != EOF)
        fputc(c, fin);
    fclose(fdata);

    // return complete binary file

    rewind(fin);
    return fin;
}


/* Generic change suffix routine - make sure name is long enough to hold the suffix */
void suffix_change(char *name, char *suffix)
{
    int j = strlen(name)-1;

    while ( j && name[j-1] != '.' ) 
        j--;

    if ( j)
        name[j-1]='\0';
    strcat(name,suffix);
}

/* Variant for the generic change suffix routine */
void any_suffix_change(char *name, char *suffix, char suffix_delimiter)
{
    int j = strlen(name)-1;

    while ( j && name[j-1] != suffix_delimiter ) 
        j--;

    if ( j)
        name[j-1]='\0';
    strcat(name,suffix);
}

void *must_malloc(size_t sz)
{
    void *p;

    if ((p = malloc(sz)) == NULL)
        exit_log(1, "Error: Out of memory\n");

    return p;
}

void *must_realloc(void *p, size_t sz)
{
    void *r;

    if ((r = realloc(p, sz)) == NULL)
        exit_log(1, "Error: Out of memory\n");

    return r;
}

void *must_strdup(char *p)
{
    void *r;

    if ((r = strdup(p)) == NULL)
        exit_log(1, "Error: Out of memory\n");

    return r;
}

/* Print the overall usage information */
void main_usage(void)
{
    int   i;

    fprintf(stderr,"appmake [+target] [options]\n\n");
    fprintf(stderr,"The z88dk application generator\n\n");
    fprintf(stderr,
            "This program is used to produce files which are suitable for use in\n"
            "emulators or on the real hardware. ");
    fprintf(stderr,"Supported targets are:\n\n");
    i = 0;
    while (machines[i].ident ) {
        fprintf(stderr,"+%-12s (%-8s) - %s\n",machines[i].ident,machines[i].execname,machines[i].copyright);
        i++;
    }
    fprintf(stderr,"\nFor more usage information use +[target] with no options\n");

    exit(1);
}


static void set_option_by_type(option_t *options, type_t type, char *value)
{
    char  *arr[2];

    arr[0] = NULL;
    arr[1] = value;
    do {
        if ( options->type == type ) {
            option_set(0, 2, arr, options);
            return;
        }
        options++;
    } while ( options->type != OPT_NONE );

    exit_log(1,"Cannot set option of type %s for chained command\n",type & OPT_INPUT ? "input" : "output");
}



/* Parse the options - NB. by this stage all options beginning with +
 *  have been parsed out
 */
int option_parse(int argc, char *argv[], option_t *options)
{
    int       i;
    char     *equal;
    option_t *opt;

    for ( i = 0 ; i < argc; i++ )
    {
        if ( argv[i][1] && argv[i][0] == '-' )
        {
            opt = options;
            do
            {
                if ( opt->sopt && argv[i][2] == 0 && argv[i][1] == opt->sopt )
                {
                    i = option_set(i,argc,argv,opt);
                    break;
                }
                else if (opt->lopt && (argv[i][0] == '-') && (argv[i][1] == '-'))
                {
                    equal = strchr(&argv[i][2], '=');
                    if ((equal && (strncmp(&argv[i][2], opt->lopt, strlen(opt->lopt)) == 0)) || (!equal && (strcmp(&argv[i][2], opt->lopt) == 0)))
                    {
                        i = option_set(i,argc,argv,opt);
                        break;   
                    }
                }                
                opt++;
            } while ( opt->type != OPT_NONE );
        }
    }    
    return 0;
}




static int option_set(int pos, int max, char *argv[], option_t *option)
{
    char   *param;
    int     ret;

    ret = pos;
    param = (argv[pos] == NULL) ? NULL : strchr(argv[pos], '=');

    if ((param == NULL) && ((pos + 1) < max))
    {
        param = argv[pos+1];
        ret = pos + 1;
    }
    else
    {
        ++param;
        ret = pos;
    }

    switch ( option->type & OPT_BASE_MASK)
    {
        case OPT_BOOL:
            *(char *)(option->dest) = TRUE;
            ret = pos;
            break;

        case OPT_INT:
            if (param)
                *(int *)(option->dest) = (int)strtol(param, NULL, 0);
            break;

        case OPT_STR:
            if (param)
                *(char **)(option->dest) = strdup(param);
            break;

        case OPT_NONE:
            ret = pos;
            break;
    }

    return ret;
}



static void option_print(char *execname, char *ident, char *copyright, char *desc, char *longdesc, option_t *opts)
{
    option_t *opt = opts;
    char      optstr[4];

    fprintf(stderr,"appmake +%s (%s)\n\n%s\n",ident,execname,copyright);

    if ( desc && strlen(desc) )
        fprintf(stderr,"\n%s\n",desc);

    if ( longdesc && strlen(longdesc) )
        fprintf(stderr,"\n%s",longdesc);

    fprintf(stderr,"\nOptions:\n\n");
    while ( opt->type != OPT_NONE ) {
        if ( opt->sopt ) {
            sprintf(optstr,"-%c",opt->sopt);
        } else {
            sprintf(optstr,"  ");
        }

        switch ( opt->type & OPT_BASE_MASK ) {
        case OPT_BOOL:
            fprintf(stderr,"%s   --%-15s (bool)    %s\n",optstr,opt->lopt,opt->desc);
            break;
        case OPT_INT:
            fprintf(stderr,"%s   --%-15s (integer) %s\n",optstr,opt->lopt,opt->desc);
            break;
        case OPT_STR:
            fprintf(stderr,"%s   --%-15s (string)  %s\n",optstr,opt->lopt,opt->desc);
            break;
        case OPT_NONE:
            break;
        }
        opt++;
    }
}


/* Number to BCD */
uint32_t num2bcd(uint32_t num)
{
    int i;
    uint32_t bcd;

    bcd = 0;
    for (i=0; i<32; i+=8)
    {
        bcd += ((num%100)/10*16 + (num%100)%10) << i;
        num /= 100;
    }

    return bcd;
}


/* Writing routines */
void writebyte(unsigned char c, FILE *fp)
{
        fputc(c,fp);
}



void writeword(unsigned int i, FILE *fp)
{
    fputc(i%256,fp);
    fputc(i/256,fp);
}


void writestring(char *mystring, FILE *fp)
{
    size_t c;

    for (c=0; c < strlen(mystring); c++) {
        writebyte(mystring[c],fp);
    }
}


void writelong(unsigned long i, FILE *fp)
{
    writeword(i%65536,fp);
    writeword(i/65536,fp);
}



void writeword_p(unsigned int i, FILE *fp,unsigned char *p)
{
    writebyte_p(i%256,fp,p);
    writebyte_p(i/256,fp,p);
}


/* Parity in kansas-city mode (Sorcerer Exidy) */
void writeword_pk(unsigned int i, FILE *fp,unsigned char *p)
{
    writebyte_pk(i%256,fp,p);
    writebyte_pk(i/256,fp,p);
}



/* Parity in checksum mode (Spectrum, Newbrain..) */
void writebyte_p(unsigned char c, FILE *fp,unsigned char *p)
{
    fputc(c,fp);
    *p^=c;
}


/* Parity in kansas-city mode (Sorcerer Exidy) */
void writebyte_pk(unsigned char c, FILE *fp,unsigned char *p)
{
    fputc(c,fp);
    *p=0xff^(c-*p);
}


void writestring_p(char *mystring, FILE *fp,unsigned char *p)
{
    size_t c;

    for (c=0; c < strlen(mystring); c++) {
        writebyte_p(mystring[c],fp,p);
    }
}



void writestring_pk(char *mystring, FILE *fp,unsigned char *p)
{
    size_t c;

    for (c=0; c < strlen(mystring); c++) {
        writebyte_pk(mystring[c],fp,p);
    }
}



void writebyte_cksum(unsigned char c, FILE *fp, unsigned long *cksum)
{
    *cksum += (unsigned int) c;
    fputc(c,fp);
}



void writeword_cksum(unsigned int i, FILE *fp, unsigned long *cksum)
{
    writebyte_cksum(i%256,fp,cksum);
    writebyte_cksum(i/256,fp,cksum);
}



void writestring_cksum(char *mystring, FILE *fp, unsigned long *cksum)
{
    size_t c;

    for (c=0; c < strlen(mystring); c++) {
        writebyte_cksum(mystring[c],fp,cksum);
    }
}



/* Add the WAV header to a 44100 Khz RAW sound file */
void raw2wav(char *wavfile)
{
    char    rawfilename[FILENAME_MAX+1];
    FILE    *fpin, *fpout;
    int		c;
    long	i, len;

    strcpy(rawfilename,wavfile);

    if ( (fpin=fopen(wavfile,"rb") ) == NULL ) {
        fprintf(stderr,"Can't open file %s for wave conversion\n",wavfile);
        myexit(NULL,1);
    }

    if (fseek(fpin,0,SEEK_END)) {
       fclose(fpin);
       myexit("Couldn't determine size of file\n",1);
    }

    len=ftell(fpin);
    fseek(fpin,0L,SEEK_SET);
    suffix_change(wavfile,".wav");

    if ( (fpout=fopen(wavfile,"wb") ) == NULL ) {
        fprintf(stderr,"Can't open output raw audio file %s\n",wavfile);
        myexit(NULL,1);
    }

    /* Now let's think at the WAV file */
    writestring("RIFF",fpout);
    writelong(len+63,fpout);
    writestring("WAVEfmt ",fpout);
    writelong(0x10,fpout);
    writeword(1,fpout);
    writeword(1,fpout);
    writelong(44100,fpout);
    writelong(44100,fpout);
    writeword(1,fpout);
    writeword(8,fpout);
    writestring("data",fpout);
    writelong(len,fpout);

    for (i=0; i<63;i++) {
      fputc(0x20,fpout);
    }

    /*
    //writestring(wav_table,fpout);
    for (i=0; i<28;i++) {
      fputc(0x20,fpout);
    }
    */

    for (i=0; i<len;i++) {
      c=getc(fpin);
      fputc(c,fpout);
    }

    fclose(fpin);
    fclose(fpout);
    remove (rawfilename);
}


/* Pilot lenght in standard mode is about 2000 */
void zx_pilot(int pilot_len, FILE *fpout)
{
  int i,j;

  /* First a short gap.. */
  for (i=0; i < 200; i++)
    fputc (0x80,fpout);

  /* Then the beeeep */
  for (j=0; j<pilot_len; j++) {
    for (i=0; i < 27; i++)
      fputc (0x20,fpout);

    for (i=0; i < 27; i++)
      fputc (0xe0,fpout);
  }


  /* Sync */
  for (i=0; i < 8; i++)
    fputc (0x20,fpout);

  for (i=0; i < 8; i++)
    fputc (0xe0,fpout);
}


void zx_rawbit(FILE *fpout, int period)
{
  int i;

  for (i=0; i < period; i++)
    fputc (0x20,fpout);

  for (i=0; i < period; i++)
    fputc (0xe0,fpout);
}



void zx_rawout (FILE *fpout, unsigned char b, char fast)
{
  static unsigned char c[8] = { 0x80, 0x40, 0x20, 0x10, 0x08, 0x04, 0x02, 0x01 };
  int i,period;

  for (i=0; i < 8; i++)
  {
    if (b & c[i])
      /* Experimental MIN limit is 17 */
      if ( fast ) period = 19; else period = 22;
      //period = 22;
    else
      /* Experimental MIN limit is 7 */
      if ( fast ) period = 9; else period = 11;
      //period = 11;
    zx_rawbit(fpout, period);
  }
}



int hexdigit(char digit) 
{
    if (digit >= 'A' && digit <= 'F') {
        return digit - 'A' + 10;
    } else if (digit >= 'a' && digit <= 'f') {
        return digit - 'a' + 10;
    } else if (digit >= '0' && digit <= '9') {
        return digit - '0';
    }

    fprintf(stderr,"\nError in patch string\n");
    myexit(NULL,1);
    return 0; /* Not reached */
}





/* iHEX format file conversion (+hex and others) */
static int checksum(const unsigned char *data, int size)
{
    int sum = 0;

    while ( size-- ) {
        sum += *data++;
        sum &= 0xff;
    }

    if (sum > 0) {
        sum = 0x100 - sum;
    }

    return(sum);    
}


// if eofrec == 0 do not write end-of-file record
int bin2hex(FILE *input, FILE *output, int address, int recsize, int eofrec)
{
    unsigned char *outbuf;
    unsigned char *inbuf;
    int byte;
    int size;   
    int i;

    if (recsize < 1)
        recsize = 16;
    else if (recsize > 255)
        recsize = 255;

    if ((outbuf = malloc((recsize + 6) * sizeof(*outbuf))) == NULL)
        exit_log(1, "Error bin2hex: out of memory\n");

    inbuf = outbuf + 5;
    outbuf[0] = ':';

    do
    {    
        size = 0;
        while (size < recsize)
        {
            byte = fgetc(input);

            if ( byte == EOF )
                break;

            inbuf[size++] = byte;
        }

        if (size == 0 && !eofrec)
            break;

        outbuf[1] = size;
        if (size > 0)
        {
            outbuf[2] = address >> 8;
            outbuf[3] = address & 0xff;
            outbuf[4] = 0;
        }
        else
        {
            outbuf[2] = 0;
            outbuf[3] = 0;
            outbuf[4] = 1;
        }
        outbuf[5 + size] = checksum(outbuf + 1, size + 4);
        fputc(outbuf[0], output);

        for (i=1; i<(size+6); i++)
            fprintf(output, "%02X", outbuf[i]);

        fprintf(output, "\n");
        address += size;        
    } while (!feof(input) || (size > 0));

    free(outbuf);
    return 0;
}


static void get_temporary_filename(char *filen)
{
#ifdef _WIN32
    char   *ptr;

    if ((ptr = _tempnam(".\\", tmpnambuf)) == NULL) {
        fprintf(stderr, "Failed to create temporary filename\n");
        exit(1);
    }
    strcpy(filen, ptr);
    free(ptr);
#elif defined(__MSDOS__) && defined(__TURBOC__)
    /* Both predefined by Borland's Turbo C/C++ and Borland C/C++ */
    if (ptr = getenv("TEMP")) {    /* From MS-DOS 5, C:\TEMP, C:\DOS,
                     * C:\WINDOWS\TEMP or whatever or
                     * nothing */
        strcpy(filen, ptr);    /* Directory is not guaranteed to
                     * exist */
        strcat(filen, "\\");
        tmpnam(filen + strlen(filen));    /* Adds strings like
                         * TMP1.$$$, TMP2.$$$ */
    }

     /* Allways starts at TMP1.$$$. Does not */ 
    else            /* check if file already exists. So is  */
        tmpnam(filen);    /* not suitable for executing zcc more  */
    if (ptr = strrchr(filen, '.'))    /* than once without cleaning out
                     * files. */
        *ptr = 0;    /* Don't want to risk too long filenames */
#else
    strcpy(filen, "/tmp/appmakechainXXXXXXXX");
    mktempfile(filen);
#endif
    num_temp_files++;
    temp_files = realloc(temp_files, num_temp_files * sizeof(temp_files[0]));
    temp_files[num_temp_files-1] = strdup(filen);
}



static void  cleanup_temporary_files(void)
{
    int   i;

    for ( i = 0; i < num_temp_files; i++ ) {
         remove(temp_files[i]);
    }
}


/* memory banks */

#define MBLINEMAX 1024

void mb_create_bankspace(struct banked_memory *memory, char *bank_id)
{
    memory->num++;
    memory->bankspace = must_realloc(memory->bankspace, memory->num * sizeof(*memory->bankspace));

    memset(&memory->bankspace[memory->num - 1], 0, sizeof(*memory->bankspace));
    memory->bankspace[memory->num - 1].bank_id = must_strdup(bank_id);
}

enum
{
    TYPE_HEAD = 1,
    TYPE_SIZE
};

void mb_enumerate_banks(FILE *fmap, char *binname, struct banked_memory *memory, struct aligned_data *aligned)
{
    char buffer[MBLINEMAX];
    char symbol_name[MBLINEMAX];
    long symbol_value;
    char section_name[MBLINEMAX];
    char bfilename[MBLINEMAX];
    int  c;
    struct stat st;

    // organize output binaries into banks

    while (fgets(buffer, MBLINEMAX, fmap) != NULL)
    {
        // have one line of the map file
        // make sure the entire line is consumed

        if (buffer[strlen(buffer) - 1] != '\n')
            while (((c = fgetc(fmap)) != EOF) && (c != '\n'));

        // get symbol name and value

        if (sscanf(buffer, "%s = $%lx", symbol_name, &symbol_value) == 2)
        {
            // find out if symbol corresponds to a section name

            int len = strlen(symbol_name);
            int type = 0;

            if ((len >= 6) && (symbol_name[0] == '_') && (symbol_name[1] == '_'))
            {
                if (strcmp(symbol_name + len - 5, "_head") == 0)
                    type = TYPE_HEAD;
                else if (strcmp(symbol_name + len - 5, "_size") == 0)
                    type = TYPE_SIZE;

                if (type)
                {
                    char *p;

                    // section found
                    // extract section name and form binary filename

                    if (len == 6)
                    {
                        // section name is empty
                        // classic can generate main bank binaries with empty section names

                        section_name[0] = 0;
                        snprintf(bfilename, sizeof(bfilename), "%s.bin", binname);
                        if ((stat(bfilename, &st) < 0) || (S_IFREG != (st.st_mode & S_IFMT)))
                            suffix_change(bfilename, "");
                    }
                    else
                    {
                        // section name is present

                        snprintf(section_name, sizeof(section_name), "%s", &symbol_name[2]);
                        section_name[len - 7] = 0;
                        snprintf(bfilename, sizeof(bfilename), "%s_%s.bin", binname, section_name);
                    }

                    // if section head
                    // check if a corresponding binary output file exists

                    if ((type == TYPE_HEAD) && (stat(bfilename, &st) >= 0) && (S_IFREG == (st.st_mode & S_IFMT)) && (st.st_size > 0))
                    {
                        // found a section binary
                        // find out which memory bank it belongs to

                        struct memory_bank *mb = &memory->mainbank;

                        for (int i = 0; i < memory->num; ++i)
                        {
                            if (p = strstr(section_name, memory->bankspace[i].bank_id))
                            {
                                int banknum;

                                if (sscanf(p + strlen(memory->bankspace[i].bank_id), "_%d", &banknum) == 1)
                                {
                                    if ((banknum >= 0) && (banknum < MAXBANKS))
                                    {
                                        mb = &memory->bankspace[i].membank[banknum];
                                        break;
                                    }
                                    else
                                    {
                                        fprintf(stderr, "Warning: Bank number in %s is out of range\n(will likely end up in main bank)\n", section_name);
                                    }
                                }
                            }
                        }

                        // mb is the memory bank the section belongs to
                        // section duplicates cannot occur so simply add to list

                        mb->num++;
                        mb->secbin = must_realloc(mb->secbin, mb->num * sizeof(*mb->secbin));

                        // add new section information

                        struct section_bin *sb = &mb->secbin[mb->num - 1];

                        sb->filename = must_strdup(bfilename);
                        sb->section_name = must_strdup(section_name);
                        sb->org = symbol_value;
                        sb->size = st.st_size;
                    }

                    // record an aligned section

                    int alignment;

                    if ((p = strstr(section_name, "_align_")) && (sscanf(p, "_align_%d", &alignment) == 1))
                    {
                        // look for an existing entry for the section

                        int index;

                        for (index = 0; index < aligned->num; ++index)
                            if (strcmp(section_name, aligned->array[index].section_name) == 0)
                                break;

                        if (index >= aligned->num)
                        {
                            // existing entry not found so make new entry

                            aligned->num++;
                            aligned->array = must_realloc(aligned->array, aligned->num * sizeof(*aligned->array));

                            memset(&aligned->array[index], 0, sizeof(aligned->array[index]));
                            aligned->array[index].section_name = must_strdup(section_name);
                            aligned->array[index].alignment = alignment;
                        }

                        // collect section information

                        if (type == TYPE_HEAD)
                            aligned->array[index].org = (int)symbol_value;
                        else
                            aligned->array[index].size = (int)symbol_value;
                    }
                }
            }
        }
        else
        {
            // map file line is wrong format
            // flag error if the line is not blank

            char *p;

            for (p = buffer; isspace(*p); ++p) ;

            if (*p)
                fprintf(stderr, "Warning: Unable to parse line from map file\n\t%s\n", buffer);
        }
    }
}

int mb_remove_section(struct banked_memory *memory, char *section_name)
{
    // remove a particular section from the bank enumeration

    // first find out which memory bank the section belongs to

    char  *p;
    struct memory_bank *mb = &memory->mainbank;

    for (int i = 0; i < memory->num; ++i)
    {
        if (p = strstr(section_name, memory->bankspace[i].bank_id))
        {
            int banknum;

            if (sscanf(p + strlen(memory->bankspace[i].bank_id), "_%d", &banknum) == 1)
            {
                if ((banknum >= 0) && (banknum < MAXBANKS))
                {
                    mb = &memory->bankspace[i].membank[banknum];
                    break;
                }
            }
        }
    }

    // remove the section from the memory bank

    for (int i = 0; i < mb->num; ++i)
    {
        if (strcmp(mb->secbin[i].section_name, section_name) == 0)
        {
            // section has been found
            // free allocated memory, remove it from the section array

            free(mb->secbin[i].filename);
            free(mb->secbin[i].section_name);

            memcpy(&mb->secbin[i], &mb->secbin[i + 1], (mb->num - i - 1) * sizeof(*mb->secbin));

            if (--mb->num > 0)
                mb->secbin = must_realloc(mb->secbin, mb->num * sizeof(*mb->secbin));
            else
            {
                free(mb->secbin);
                mb->secbin = NULL;
            }

            return 1;
        }
    }

    return 0;
}

int mb_compare_aligned(const struct section_aligned *a, const struct section_aligned *b)
{
    return strcmp(a->section_name, b->section_name);
}

int mb_check_alignment(struct aligned_data *aligned)
{
    int errors = 0;

    qsort(aligned->array, aligned->num, sizeof(*aligned->array), mb_compare_aligned);

    for (int i = 0; i < aligned->num; ++i)
    {
        if ((aligned->array[i].size > 0) && (aligned->array[i].org & (aligned->array[i].alignment - 1)))
        {
            errors++;
            fprintf(stderr, "Warning: Section %s at address 0x%04x is not properly aligned\n", aligned->array[i].section_name, aligned->array[i].org);
        }
    }

    return errors;
}

int mb_compare_banks(const struct section_bin *a, const struct section_bin *b)
{
    return a->org - b->org;
}

int mb_sort_banks_check(struct memory_bank *mb)
{
    int errors = 0;

    if (mb->num)
    {
        qsort(mb->secbin, mb->num, sizeof(*mb->secbin), mb_compare_banks);

        for (int k = 0; k < mb->num; ++k)
        {
            // check if section exceeds 64k address space

            if ((mb->secbin[k].org < 0x10000) && ((mb->secbin[k].org + mb->secbin[k].size) > 0x10000))
            {
                errors++;
                fprintf(stderr, "Error: Section %s exceeds 64k [0x%04x,0x%04x]\n", mb->secbin[k].section_name, mb->secbin[k].org, mb->secbin[k].org + mb->secbin[k].size - 1);
            }

            // check for section overlap

            if (k > 0)
            {
                if (mb->secbin[k].org < (mb->secbin[k - 1].org + mb->secbin[k - 1].size))
                {
                    errors++;
                    fprintf(stderr, "Error: Section %s overlaps section %s by %d bytes\n", mb->secbin[k - 1].section_name, mb->secbin[k].section_name, mb->secbin[k - 1].org + mb->secbin[k - 1].size - mb->secbin[k].org);
                }
            }
        }
    }

    return errors;
}

int mb_sort_banks(struct banked_memory *memory)
{
    int errors;

    // sort the main bank

    errors = mb_sort_banks_check(&memory->mainbank);

    // sort each bank space

    for (int i = 0; i < memory->num; ++i)
    {
        struct bank_space *bs = &memory->bankspace[i];

        for (int j = 0; j < MAXBANKS; ++j)
        {
            struct memory_bank *mb = &bs->membank[j];
            errors += mb_sort_banks_check(&bs->membank[j]);
        }
    }

    return errors;
}

int mb_generate_output_binary(FILE *fbin, int filler, FILE *fhex, int ipad, int irecsz, struct memory_bank *mb)
{
    // fhex may be NULL to indicate hex file not wanted
    // fbin should be opened in "wb+" mode
    // filler is fill byte for gaps between sections
    // ipad is non-zero if hex file should use filler in gaps
    // irecsz is intel hex record size

    FILE *fin;
    int   c;

    // iterate over all sections in memory bank

    for (int i = 0; i < mb->num; ++i)
    {
        // open section binary file

        if ((fin = fopen(mb->secbin[i].filename, "rb")) == NULL)
        {
            fprintf(stderr, "Error: Cannot read section binary %s\n", mb->secbin[i].filename);
            return 1;
        }

        // pad space between sections in memory bank

        if (i > 0)
        {
            c = mb->secbin[i].org - mb->secbin[i - 1].org - mb->secbin[i - 1].size;
            while (c-- > 0)
                fputc(filler, fbin);
        }

        // add section binary to memory bank

        while ((c = fgetc(fin)) != EOF)
            fputc(c, fbin);

        // generate into ihex file if padding is off

        if ((fhex != NULL) && !ipad)
        {
            rewind(fin);
            bin2hex(fin, fhex, mb->secbin[i].org, irecsz, 0);
        }

        // close section binary file

        fclose(fin);
    }

    // terminate ihex file

    if (fhex != NULL)
    {
        if (ipad)
        {
            // nothing has been written to the hex file yet
            // use the generated binary file as it contains filler bytes

            fflush(fbin);
            rewind(fbin);
            bin2hex(fbin, fhex, mb->secbin[0].org, irecsz, 1);
        }
        else
            fprintf(fhex, ":00000001FF\n");
    }

    return 0;
}

void mb_delete_source_binaries(struct banked_memory *memory)
{
    // remove main bank binaries

    for (int i = 0; i < memory->mainbank.num; ++i)
        remove(memory->mainbank.secbin[i].filename);

    // remove binaries from all bank spaces

    for (int i = 0; i < memory->num; ++i)
    {
        for (int j = 0; j < MAXBANKS; ++j)
        {
            struct memory_bank *mb = &memory->bankspace[i].membank[j];

            for (int k = 0; k < mb->num; ++k)
                remove(mb->secbin[k].filename);
        }
    }
}

void mb_cleanup_memory(struct banked_memory *memory)
{
    // does not free "memory"

    for (int i = 0; i < memory->num; ++i)
    {
        struct bank_space *bs = &memory->bankspace[i];
        
        for (int j = 0; j < MAXBANKS; ++j)
        {
            struct memory_bank *mb = &bs->membank[j];

            for (int k = 0; k < mb->num; ++k)
            {
                struct section_bin *sb = &mb->secbin[k];

                free(sb->filename);
                free(sb->section_name);
            }

            free(mb->secbin);
        }

        free(bs->bank_id);
    }

    free(memory->bankspace);

    memory->num = 0;
    memory->bankspace = NULL;

    free(memory->mainbank.secbin);

    memory->mainbank.num = 0;
    memory->mainbank.secbin = NULL;
}

void mb_cleanup_aligned(struct aligned_data *aligned)
{
    // does not free "aligned"

    for (int i = 0; i < aligned->num; ++i)
    {
        struct section_aligned *sa = &aligned->array[i];

        free(sa->section_name);
    }

    free(aligned->array);

    aligned->num = 0;
    aligned->array = NULL;
}
