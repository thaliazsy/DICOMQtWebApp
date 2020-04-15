using namespace std;
#include <vector>
#include <string.h>
//vector<string> vi;
/*
class   DataElement
    {
    public:
        unsigned short			ParentDICOMObjectID;
        unsigned short			Group;
        unsigned short		Element;
        unsigned  int	Length;
        char  VRType[3];
        char	VM[5];
        char  	Description[40];
        unsigned  int  Offset;
        void		*Data;
        char  strData[256];
    //	DataElement * PreviousElement, *NextElement;
    };
class	DICOMDataObject
    {   //  ParseRawDataIntoVRElement(UINT	Mode);
        //FileStream DataStream; //
        //bool ParseDCMFileIntoDataElement();
        unsigned short			DICOMObjectID;
        bool DataToString(void * Data,int DataLength, char * VRType, char * ReturnData);

        int Mode;



        public:
      //  vector<DICOMDataObject> SQDICOMObject;

        DataElement *FirstElement, * LastElement;
        int DataElementCount;
            //vector <DataElement> DataElementsArray;
         //使用 MyElements.push_back(DataElement) 方法將 DataElement 加入陣列


    };
*/
struct S {
  char strData[130];
};
struct   DataElement
    {
    public:
        int			ParentDICOMObjectID;
        int			ElementID;
        bool IsBeginOfObject;
        bool IsEndOfObject;
        vector <int> SQEndings;
        //unsigned short EdgeCondiction;   // 0: normal, 1:first of SQ and Object,  2:first of Object, 3: end of object, 4: end of SQ and object
        unsigned short			Group;
        unsigned short		Element;
        unsigned int	Length;
        char  VRType[3];
        char	VM[5];
        short shortVM;
        char  	Description[128];
        unsigned  int  Offset;
        void		*Data;
        vector <S> StrDataV;
        //char  strData[256];
    //	DataElement * PreviousElement, *NextElement;
    };


struct	DICOMObjectIndex
    { 	public:
        bool isKnowLength;
        bool IsBeginOfSequence;
        bool IsEndOfSeqence;
        //unsigned short EdgeCondiction;   // 0: normal, 1:first of SQ and Object,  2:first of Object, 3: end of object, 4: end of SQ and object
        int  ItemNumber;
        int	 ParentDataElementID;
        int	 DICOMObjectID;
        unsigned  int     Length;
        unsigned  int Offset;
    };


class	DICOMDataObject
{
public:
unsigned int DICOMObjectCount;
vector<DataElement> AllElement;
vector<DICOMObjectIndex> AllObject;
char	DICOMFileName[512];
bool IsExplicitVR;
bool IsLittleEanian;

unsigned int EndOfP10HeaderOffset;
//unsigned int		Offset;
FILE	*fp;
unsigned int		fileSize;

DICOMDataObject();

~DICOMDataObject(); //需 release  DataStream 之 Menory
//bool  DataToString(void * Data,int DataLength, char * VRType, char * ReturnData);
bool  DataToString(DataElement * DataElementP);
//Initial read DICOM file functions
int ReadDICOMPart10File(char * FileName);
int ReadDICOMFileObject(char * FileName, unsigned int DDOOffset, bool inIsExplicitVR,bool inIsLittleEanian);
int DecodeDICOMObject(DICOMObjectIndex MyObjectIndex);
//bool DecodeDICOMObject(unsigned int ParentDataElementID, unsigned int DDOOffset);
int ParseSQElement(DataElement ParentElement);

bool SetElementProperities( DataElement *CurElement);
FILE	*Retfp;
void DecodeRetToXML(char * FileName);
bool DecodeSRToSVG(char * FileName);
//int DecodeSQToXML(int ElementIndex);
//int DecodeSubObjectToXML(int ElementIndex);
//bool LoadDICOMFile(char * FileName);
};
