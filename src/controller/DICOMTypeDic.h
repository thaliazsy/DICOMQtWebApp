
using namespace std;

#	define	LITTLE_ENDIAN	1
#	define	BIG_ENDIAN		2
#	define	NATIVE_ENDIAN  LITTLE_ENDIAN

struct	DicElement
    {
    unsigned int  IntGE;
    char	VRType[3];
    char	VM[5];
    short   shortVM;
    char  	Description[128];
    };

extern  vector<DicElement> TagDictionary;
//extern vector<DicElement> TagDictionary;
unsigned int strGEtoInt(char *Gstr , char * Estr);

void LoadDictionary(char *filename);
void SaveDictionary(char *filename);
string GetVRType(char *GE);
//bool  FindDicElement(unsigned short g, unsigned short e, DicElement * ret );
int FindDicElement(unsigned short g, unsigned short e );

double ReverseDouble(double inDouble );
float ReverseFloat(float inFloat );
unsigned short ReverseUnsignedShort(unsigned short inShort );
unsigned int ReverseUnsignedInt(unsigned int inInt );
int ReverseInt(int inInt );
short ReverseShort( short inShort );
