/* stdio.h library for large systems - small embedded systems use clibrary.c instead */
#ifndef BUILTIN_MINI_STDLIB

#include <errno.h>
#include "../interpreter.h"

#define MAX_FORMAT 80
#define MAX_SCANF_ARGS 10

static int Stdio_ZeroValue = 0;
static int EOFValue = EOF;
static int SEEK_SETValue = SEEK_SET;
static int SEEK_CURValue = SEEK_CUR;
static int SEEK_ENDValue = SEEK_END;
static int BUFSIZValue = BUFSIZ;
static int FILENAME_MAXValue = FILENAME_MAX;
static int _IOFBFValue = _IOFBF;
static int _IOLBFValue = _IOLBF;
static int _IONBFValue = _IONBF;
static int L_tmpnamValue = L_tmpnam;
static int GETS_MAXValue = 255;     /* arbitrary maximum size of a gets() file */

static FILE *stdinValue;
static FILE *stdoutValue;
static FILE *stderrValue;


/* our own internal output stream which can output to FILE * or strings */
typedef struct StdOutStreamStruct
{
    FILE *FilePtr;
    char *StrOutPtr;
    int StrOutLen;
    int CharCount;
    
} StdOutStream;

/* our representation of varargs within picoc */
struct StdVararg
{
    struct Value **Param;
    int NumArgs;
};

/* initialises the I/O system so error reporting works */
void BasicIOInit(Picoc *pc)
{
    pc->CStdOut = stdout;
    stdinValue = stdin;
    stdoutValue = stdout;
    stderrValue = stderr;
}

/* output a single character to either a FILE * or a string */
void StdioOutPutc(int OutCh, StdOutStream *Stream)
{
    if (Stream->FilePtr != NULL)
    {
        /* output to stdio stream */
        putc(OutCh, Stream->FilePtr);
        Stream->CharCount++;
    }
    else if (Stream->StrOutLen < 0 || Stream->StrOutLen > 1)
    {
        /* output to a string */
        *Stream->StrOutPtr = OutCh;
        Stream->StrOutPtr++;
        
        if (Stream->StrOutLen > 1)
            Stream->StrOutLen--;

        Stream->CharCount++;
    }
}

/* output a string to either a FILE * or a string */
void StdioOutPuts(const char *Str, StdOutStream *Stream)
{
    if (Stream->FilePtr != NULL)
    {
        /* output to stdio stream */
        fputs(Str, Stream->FilePtr);
    }
    else
    {
        /* output to a string */
        while (*Str != '\0')
        {
            if (Stream->StrOutLen < 0 || Stream->StrOutLen > 1)
            {
                /* output to a string */
                *Stream->StrOutPtr = *Str;
                Str++;
                Stream->StrOutPtr++;
                
                if (Stream->StrOutLen > 1)
                    Stream->StrOutLen--;
        
                Stream->CharCount++;
            }            
        }
    }
}

/* printf-style format of an int or other word-sized object */
void StdioFprintfWord(StdOutStream *Stream, const char *Format, unsigned long Value)
{
    if (Stream->FilePtr != NULL)
        Stream->CharCount += fprintf(Stream->FilePtr, Format, Value);
    
    else if (Stream->StrOutLen >= 0)
    {
#ifndef WIN32
		int CCount = snprintf(Stream->StrOutPtr, Stream->StrOutLen, Format, Value);
#else
		int CCount = _snprintf(Stream->StrOutPtr, Stream->StrOutLen, Format, Value);
#endif
		Stream->StrOutPtr += CCount;
        Stream->StrOutLen -= CCount;
        Stream->CharCount += CCount;
    }
    else
    {
        int CCount = sprintf(Stream->StrOutPtr, Format, Value);
        Stream->CharCount += CCount;
        Stream->StrOutPtr += CCount;
    }
}

/* printf-style format of a floating point number */
void StdioFprintfFP(StdOutStream *Stream, const char *Format, double Value)
{
    if (Stream->FilePtr != NULL)
        Stream->CharCount += fprintf(Stream->FilePtr, Format, Value);
    
    else if (Stream->StrOutLen >= 0)
    {
#ifndef WIN32
        int CCount = snprintf(Stream->StrOutPtr, Stream->StrOutLen, Format, Value);
#else
        int CCount = _snprintf(Stream->StrOutPtr, Stream->StrOutLen, Format, Value);
#endif
		Stream->StrOutPtr += CCount;
        Stream->StrOutLen -= CCount;
        Stream->CharCount += CCount;
    }
    else
    {
        int CCount = sprintf(Stream->StrOutPtr, Format, Value);
        Stream->CharCount += CCount;
        Stream->StrOutPtr += CCount;
    }
}

/* printf-style format of a pointer */
void StdioFprintfPointer(StdOutStream *Stream, const char *Format, void *Value)
{
    if (Stream->FilePtr != NULL)
        Stream->CharCount += fprintf(Stream->FilePtr, Format, Value);
    
    else if (Stream->StrOutLen >= 0)
    {
#ifndef WIN32
        int CCount = snprintf(Stream->StrOutPtr, Stream->StrOutLen, Format, Value);
#else
		int CCount = _snprintf(Stream->StrOutPtr, Stream->StrOutLen, Format, Value);
#endif
        Stream->StrOutPtr += CCount;
        Stream->StrOutLen -= CCount;
        Stream->CharCount += CCount;
    }
    else
    {
        int CCount = sprintf(Stream->StrOutPtr, Format, Value);
        Stream->CharCount += CCount;
        Stream->StrOutPtr += CCount;
    }
}

/* internal do-anything v[s][n]printf() formatting system with output to strings or FILE * */
int StdioBasePrintf(struct ParseState *Parser, FILE *Stream, char *StrOut, int StrOutLen, char *Format, struct StdVararg *Args)
{
    struct Value *ThisArg = Args->Param[0];
    int ArgCount = 0;
    char *FPos;
    char OneFormatBuf[MAX_FORMAT+1];
    int OneFormatCount;
    struct ValueType *ShowType;
    StdOutStream SOStream;
    Picoc *pc = Parser->pc;
    
    if (Format == NULL)
        Format = "[null format]\n";
    
    FPos = Format;    
    SOStream.FilePtr = Stream;
    SOStream.StrOutPtr = StrOut;
    SOStream.StrOutLen = StrOutLen;
    SOStream.CharCount = 0;
    
    while (*FPos != '\0')
    {
        if (*FPos == '%')
        {
            /* work out what type we're printing */
            FPos++;
            ShowType = NULL;
            OneFormatBuf[0] = '%';
            OneFormatCount = 1;
            
            do
            {
                switch (*FPos)
                {
                    case 'd': case 'i':     ShowType = &pc->IntType; break;     /* integer decimal */
                    case 'o': case 'u': case 'x': case 'X': ShowType = &pc->IntType; break; /* integer base conversions */
#ifndef NO_FP
                    case 'e': case 'E':     ShowType = &pc->FPType; break;      /* double, exponent form */
                    case 'f': case 'F':     ShowType = &pc->FPType; break;      /* double, fixed-point */
                    case 'g': case 'G':     ShowType = &pc->FPType; break;      /* double, flexible format */
#endif
                    case 'a': case 'A':     ShowType = &pc->IntType; break;     /* hexadecimal, 0x- format */
                    case 'c':               ShowType = &pc->IntType; break;     /* character */
                    case 's':               ShowType = pc->CharPtrType; break;  /* string */
                    case 'p':               ShowType = pc->VoidPtrType; break;  /* pointer */
                    case 'n':               ShowType = &pc->VoidType; break;    /* number of characters written */
                    case 'm':               ShowType = &pc->VoidType; break;    /* strerror(errno) */
                    case '%':               ShowType = &pc->VoidType; break;    /* just a '%' character */
                    case '\0':              ShowType = &pc->VoidType; break;    /* end of format string */
                }
                
                /* copy one character of format across to the OneFormatBuf */
                OneFormatBuf[OneFormatCount] = *FPos;
                OneFormatCount++;

                /* do special actions depending on the conversion type */
                if (ShowType == &pc->VoidType)
                {
                    switch (*FPos)
                    {
                        case 'm':   StdioOutPuts(strerror(errno), &SOStream); break;
                        case '%':   StdioOutPutc(*FPos, &SOStream); break;
                        case '\0':  OneFormatBuf[OneFormatCount] = '\0'; StdioOutPutc(*FPos, &SOStream); break;
                        case 'n':   
                            ThisArg = (struct Value *)((char *)ThisArg + MEM_ALIGN(sizeof(struct Value) + TypeStackSizeValue(ThisArg)));
                            if (ThisArg->Typ->Base == TypeArray && ThisArg->Typ->FromType->Base == TypeInt)
                                *(int *)ThisArg->Val->Pointer = SOStream.CharCount;
                            break;
                    }
                }
                
                FPos++;
                
            } while (ShowType == NULL && OneFormatCount < MAX_FORMAT);
            
            if (ShowType != &pc->VoidType)
            {
                if (ArgCount >= Args->NumArgs)
                    StdioOutPuts("XXX", &SOStream);
                else
                {
                    /* null-terminate the buffer */
                    OneFormatBuf[OneFormatCount] = '\0';
    
                    /* print this argument */
                    ThisArg = (struct Value *)((char *)ThisArg + MEM_ALIGN(sizeof(struct Value) + TypeStackSizeValue(ThisArg)));
                    if (ShowType == &pc->IntType)
                    {
                        /* show a signed integer */
                        if (IS_NUMERIC_COERCIBLE(ThisArg))
                            StdioFprintfWord(&SOStream, OneFormatBuf, ExpressionCoerceUnsignedInteger(ThisArg));
                        else
                            StdioOutPuts("XXX", &SOStream);
                    }
#ifndef NO_FP
                    else if (ShowType == &pc->FPType)
                    {
                        /* show a floating point number */
                        if (IS_NUMERIC_COERCIBLE(ThisArg))
                            StdioFprintfFP(&SOStream, OneFormatBuf, ExpressionCoerceFP(ThisArg));
                        else
                            StdioOutPuts("XXX", &SOStream);
                    }                    
#endif
                    else if (ShowType == pc->CharPtrType)
                    {
                        if (ThisArg->Typ->Base == TypePointer)
                            StdioFprintfPointer(&SOStream, OneFormatBuf, ThisArg->Val->Pointer);
                            
                        else if (ThisArg->Typ->Base == TypeArray && ThisArg->Typ->FromType->Base == TypeChar)
                            StdioFprintfPointer(&SOStream, OneFormatBuf, &ThisArg->Val->ArrayMem[0]);
                            
                        else
                            StdioOutPuts("XXX", &SOStream);
                    }
                    else if (ShowType == pc->VoidPtrType)
                    {
                        if (ThisArg->Typ->Base == TypePointer)
                            StdioFprintfPointer(&SOStream, OneFormatBuf, ThisArg->Val->Pointer);
                            
                        else if (ThisArg->Typ->Base == TypeArray)
                            StdioFprintfPointer(&SOStream, OneFormatBuf, &ThisArg->Val->ArrayMem[0]);
                            
                        else
                            StdioOutPuts("XXX", &SOStream);
                    }
                    
                    ArgCount++;
                }
            }
        }
        else
        {
            /* just output a normal character */
            StdioOutPutc(*FPos, &SOStream);
            FPos++;
        }
    }
    
    /* null-terminate */
    if (SOStream.StrOutPtr != NULL && SOStream.StrOutLen > 0)
        *SOStream.StrOutPtr = '\0';      
    
    return SOStream.CharCount;
}

/* internal do-anything v[s][n]scanf() formatting system with input from strings or FILE * */
int StdioBaseScanf(struct ParseState *Parser, FILE *Stream, char *StrIn, char *Format, struct StdVararg *Args)
{
    struct Value *ThisArg = Args->Param[0];
    int ArgCount = 0;
    void *ScanfArg[MAX_SCANF_ARGS];
    
    if (Args->NumArgs > MAX_SCANF_ARGS)
        ProgramFail(Parser, "too many arguments to scanf() - %d max", MAX_SCANF_ARGS);
    
    for (ArgCount = 0; ArgCount < Args->NumArgs; ArgCount++)
    {
        ThisArg = (struct Value *)((char *)ThisArg + MEM_ALIGN(sizeof(struct Value) + TypeStackSizeValue(ThisArg)));
        
        if (ThisArg->Typ->Base == TypePointer) 
            ScanfArg[ArgCount] = ThisArg->Val->Pointer;
        
        else if (ThisArg->Typ->Base == TypeArray)
            ScanfArg[ArgCount] = &ThisArg->Val->ArrayMem[0];
        
        else
            ProgramFail(Parser, "non-pointer argument to scanf() - argument %d after format", ArgCount+1);
    }
    
    if (Stream != NULL)
        return fscanf(Stream, Format, ScanfArg[0], ScanfArg[1], ScanfArg[2], ScanfArg[3], ScanfArg[4], ScanfArg[5], ScanfArg[6], ScanfArg[7], ScanfArg[8], ScanfArg[9]);
    else
        return sscanf(StrIn, Format, ScanfArg[0], ScanfArg[1], ScanfArg[2], ScanfArg[3], ScanfArg[4], ScanfArg[5], ScanfArg[6], ScanfArg[7], ScanfArg[8], ScanfArg[9]);
}

/* stdio calls */

void StdioPrintf(struct ParseState *Parser, struct Value *ReturnValue, struct Value **Param, int NumArgs){
    struct StdVararg PrintfArgs;
    
    PrintfArgs.Param = Param;
    PrintfArgs.NumArgs = NumArgs-1;
    ReturnValue->Val->Integer = StdioBasePrintf(Parser, stdout, NULL, 0, (char*)Param[0]->Val->Pointer, &PrintfArgs);
}

/* handy structure definitions */
const char StdioDefs[] = "\
typedef struct __va_listStruct va_list; \
typedef struct __FILEStruct FILE;\
";

/* all stdio functions */
struct LibraryFunction StdioFunctions[] ={
    { StdioPrintf,  "int printf(char *, ...);" },
    { NULL,         NULL }
};

/* creates various system-dependent definitions */
void StdioSetupFunc(Picoc *pc)
{
    struct ValueType *StructFileType;
    struct ValueType *FilePtrType;

    /* make a "struct __FILEStruct" which is the same size as a native FILE structure */
    StructFileType = TypeCreateOpaqueStruct(pc, NULL, TableStrRegister(pc, "__FILEStruct"), sizeof(FILE));
    
    /* get a FILE * type */
    FilePtrType = TypeGetMatching(pc, NULL, StructFileType, TypePointer, 0, pc->StrEmpty, TRUE);

    /* make a "struct __va_listStruct" which is the same size as our struct StdVararg */
    //TypeCreateOpaqueStruct(pc, NULL, TableStrRegister(pc, "__va_listStruct"), sizeof(FILE));
    
    /* define EOF equal to the system EOF */
    VariableDefinePlatformVar(pc, NULL, "EOF", &pc->IntType, (union AnyValue *)&EOFValue, FALSE);
    VariableDefinePlatformVar(pc, NULL, "SEEK_SET", &pc->IntType, (union AnyValue *)&SEEK_SETValue, FALSE);
    VariableDefinePlatformVar(pc, NULL, "SEEK_CUR", &pc->IntType, (union AnyValue *)&SEEK_CURValue, FALSE);
    VariableDefinePlatformVar(pc, NULL, "SEEK_END", &pc->IntType, (union AnyValue *)&SEEK_ENDValue, FALSE);
    VariableDefinePlatformVar(pc, NULL, "BUFSIZ", &pc->IntType, (union AnyValue *)&BUFSIZValue, FALSE);
    VariableDefinePlatformVar(pc, NULL, "FILENAME_MAX", &pc->IntType, (union AnyValue *)&FILENAME_MAXValue, FALSE);
    VariableDefinePlatformVar(pc, NULL, "_IOFBF", &pc->IntType, (union AnyValue *)&_IOFBFValue, FALSE);
    VariableDefinePlatformVar(pc, NULL, "_IOLBF", &pc->IntType, (union AnyValue *)&_IOLBFValue, FALSE);
    VariableDefinePlatformVar(pc, NULL, "_IONBF", &pc->IntType, (union AnyValue *)&_IONBFValue, FALSE);
    VariableDefinePlatformVar(pc, NULL, "L_tmpnam", &pc->IntType, (union AnyValue *)&L_tmpnamValue, FALSE);
    VariableDefinePlatformVar(pc, NULL, "GETS_MAX", &pc->IntType, (union AnyValue *)&GETS_MAXValue, FALSE);
    
    /* define stdin, stdout and stderr */
    VariableDefinePlatformVar(pc, NULL, "stdin", FilePtrType, (union AnyValue *)&stdinValue, FALSE);
    VariableDefinePlatformVar(pc, NULL, "stdout", FilePtrType, (union AnyValue *)&stdoutValue, FALSE);
    VariableDefinePlatformVar(pc, NULL, "stderr", FilePtrType, (union AnyValue *)&stderrValue, FALSE);

    /* define NULL, TRUE and FALSE */
    if (!VariableDefined(pc, TableStrRegister(pc, "NULL")))
        VariableDefinePlatformVar(pc, NULL, "NULL", &pc->IntType, (union AnyValue *)&Stdio_ZeroValue, FALSE);
}

/* portability-related I/O calls */
void PrintCh(char OutCh, FILE *Stream)
{
    putc(OutCh, Stream);
}

void PrintSimpleInt(long Num, FILE *Stream)
{
    fprintf(Stream, "%ld", Num);
}

void PrintStr(const char *Str, FILE *Stream)
{
    fputs(Str, Stream);
}

void PrintFP(double Num, FILE *Stream)
{
    fprintf(Stream, "%f", Num);
}

#endif /* !BUILTIN_MINI_STDLIB */
