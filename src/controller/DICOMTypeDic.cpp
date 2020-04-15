# include	"dicomheader.h"
vector<DicElement> TagDictionary;

// http://stackoverflow.com/questions/2782725/converting-float-values-from-big-endian-to-little-endian
double ReverseDouble(double inDouble ){
    //FD : Floating Point Double. Not test yet.
    double retVal;
    char *DataToConvert = ( char* ) &inDouble;
    char *returnData = ( char* ) &retVal;
    returnData[0] = DataToConvert[7];
    returnData[1] = DataToConvert[6];
    returnData[2] = DataToConvert[5];
    returnData[3] = DataToConvert[4];
    returnData[4] = DataToConvert[3];
    returnData[5] = DataToConvert[2];
    returnData[6] = DataToConvert[1];
    returnData[7] = DataToConvert[0];
    return retVal;
}

float ReverseFloat(float inFloat ){
    float retVal;
    char *DataToConvert = ( char* ) &inFloat;
    char *returnData = ( char* ) &retVal;
    returnData[0] = DataToConvert[3];
    returnData[1] = DataToConvert[2];
    returnData[2] = DataToConvert[1];
    returnData[3] = DataToConvert[0];
    return retVal;
}
unsigned int ReverseUnsignedInt(unsigned int inInt ){
    unsigned int retVal;
    char *DataToConvert = ( char* ) &inInt;
    char *returnData = ( char* ) &retVal;
    returnData[0] = DataToConvert[3];
    returnData[1] = DataToConvert[2];
    returnData[2] = DataToConvert[1];
    returnData[3] = DataToConvert[0];
    return retVal;
}
unsigned short ReverseUnsignedShort(unsigned short inShort ){
    unsigned short retVal;
    char *DataToConvert = ( char* ) &inShort;
    char *returnData = ( char* ) &retVal;
    returnData[0] = DataToConvert[1];
    returnData[1] = DataToConvert[0];
    return retVal;
}

int ReverseInt(int inInt ){
    int retVal;
    char *DataToConvert = ( char* ) &inInt;
    char *returnData = ( char* ) &retVal;
    returnData[0] = DataToConvert[3];
    returnData[1] = DataToConvert[2];
    returnData[2] = DataToConvert[1];
    returnData[3] = DataToConvert[0];
    return retVal;
}
short ReverseShort( short inShort ){
    short retVal;
    char *DataToConvert = ( char* ) &inShort;
    char *returnData = ( char* ) &retVal;
    returnData[0] = DataToConvert[1];
    returnData[1] = DataToConvert[0];
    return retVal;
}

// char: 0-9 a,b,c,d,e,f, A, B, C, D, E, F to int number 0-15
static	unsigned int HexToNnmber(char	ch)
{
    if((ch>='a')&&(ch<='f'))
        return(ch-'a'+10);
    if((ch>='A')&&(ch<='F'))
        return(ch-'A'+10);
    return(ch-'0');
}


/*  Testing data
     unsigned int ret;
     printf("Ret %u", ret);   // %u unsigned int
     ret = strGEtoInt("F000","0000"); // 4094115840
    ret = strGEtoInt("0000","0010"); // 16
     */
unsigned int strGEtoInt(char *Gstr , char * Estr)
{unsigned int  intGE ,intGEret,intNumber;
    intGE =0;
    char	*cPtr;
    //Group
    cPtr = Gstr;
    intNumber = HexToNnmber(*cPtr);   cPtr++;
    intGE = (intGE<< 4) + intNumber;
    intNumber = HexToNnmber(*cPtr);   cPtr++;
    intGE = (intGE<< 4) + intNumber;
    intNumber = HexToNnmber(*cPtr);   cPtr++;
    intGE = (intGE<< 4) + intNumber;
    intNumber = HexToNnmber(*cPtr);   cPtr++;
    intGE = (intGE<< 4) + intNumber;

    cPtr = Estr;
    intNumber = HexToNnmber(*cPtr);   cPtr++;
    intGE = (intGE<< 4) + intNumber;
    intNumber = HexToNnmber(*cPtr);   cPtr++;
    intGE = (intGE<< 4) + intNumber;
    intNumber = HexToNnmber(*cPtr);   cPtr++;
    intGE = (intGE<< 4) + intNumber;
    intNumber = HexToNnmber(*cPtr);   cPtr++;
    intGE = (intGE<< 4) + intNumber;
    return intGE;

}

void LoadDictionary(char	*filename)
{
    FILE		*fp;
    //	unsigned int		Index, DIndex;

    DicElement	DElement;

    char		s[1024];
    //char		s1[1024];
    if(filename){
        fp = fopen ( filename, "r" );
        if ( fp )
        {
            while ( ! feof ( fp ) )
            {
                fgets ( s, 1024 , fp );
                if ( s[0] != '(' )  // not data line
                {
                    fgets( s, 1024, fp);
                    continue;
                }

                DElement.IntGE =0;
                strcpy(DElement.VRType ,"UN");
                strcpy(DElement.Description, "Unknown");
                unsigned	char	Digit;

                DElement.IntGE = strGEtoInt(&s[1],&s[6]); // s[1]-s[4] Gstr, s[6]-s[9]

                // Now scan for 'VR=" and Keyword="
                char *p,	vType[3];
                p = strstr(s, "VR=");
                if(p)
                {DElement.VRType[0] =p[4];
                    DElement.VRType[1] =p[5];
                    DElement.VRType[2]='\0';
                }
                //	p = strstr(s, "Keyword=\"");

                p = strstr(s, "VM=");
                if(p)
                {for (int i=0; i<5; i++)
                    {if( p[i+4] !='\"')
                            DElement.VM[i]= p[i+4];
                        else { DElement.VM[i] ='\0'; i=5; }
                        // DElement.Description[24]='\0';

                    }
                }
                // dynanic VM length, eg. 1-n, 2-N, or VM >9   may further consider furthor VM eg. 1-n, 2-N
                DElement.shortVM = 1;
                if(strcmp( DElement.VM, "2") ==0 ) DElement.shortVM =2;
                if(strcmp( DElement.VM, "3") ==0 ) DElement.shortVM =3;
                if(strcmp( DElement.VM, "4") ==0 ) DElement.shortVM =4;
                if(strcmp( DElement.VM, "5") ==0 ) DElement.shortVM =5;
                if(strcmp( DElement.VM, "6") ==0 ) DElement.shortVM =6;
                if(strcmp( DElement.VM, "7") ==0 ) DElement.shortVM =7;
                if(strcmp( DElement.VM, "8") ==0 ) DElement.shortVM =8;
                if(strcmp( DElement.VM, "9") ==0 ) DElement.shortVM =9;
                p = strstr(s, "Keyword=");
                if(p)
                {for (int i=0; i<128; i++)
                    {if( p[i+9] !='\"')
                            DElement.Description[i]= p[i+9];
                        else { DElement.Description[i] ='\0'; i=128; }
                        // DElement.Description[24]='\0';

                    }
                }
                // Vector push_back function :  create a element in vector and copy DElement to the element
                TagDictionary.push_back(DElement);

            }

            fclose(fp);
        }
        else {
            printf(" Dictionary not found");
        }

    }
}

void SaveDictionary(char	*filename)
{
    FILE * pFile;
    pFile = fopen (filename,"w+");

    unsigned int DicSize;
    int GEindex;
    int	 i;
    char * TypeCodeTemp;
    char * DescriptionTemp;
    DicSize = TagDictionary.size();


    unsigned	char	Digit;

    fprintf(pFile,"Total Dictory size: %d  \n",DicSize);
    for(i=0;i < DicSize;i++)
    {
        fprintf(pFile,"Group Eelement: %u, VR data type: %s , VM %s , Description: %s \n", TagDictionary[i].IntGE,TagDictionary[i].VRType,TagDictionary[i].VM, TagDictionary[i].Description);
    }
    fclose (pFile);
}

int BinarySearch(unsigned int key)
{
    int left = 0, right = TagDictionary.size(), middle;
    while (left <= right){
        middle = (left + right) / 2;
        if (key < TagDictionary[middle].IntGE)
            right = middle - 1;
        else if (key > TagDictionary[middle].IntGE)
            left = middle + 1;
        else  return middle;
    }
    return -1;

}

int FindDicElement(unsigned short g, unsigned short e )
{
    unsigned int DicSize;
    unsigned int Key, GEindex;
    Key =(g <<16) + e;
    int	 i;
    char * TypeCodeTemp;
    char * DescriptionTemp;
    DicSize = TagDictionary.size();
    // printf(" Size %d ", TagDictionary.size());
    GEindex=BinarySearch(Key);
    // printf("Group Eelement: %u, VR data type: %s , VM %s , Description: %s \n", TagDictionary[GEindex].IntGE,TagDictionary[GEindex].TypeCode, TagDictionary[GEindex].VM, TagDictionary[GEindex].Description );
    return GEindex;


}
