/************************************************/
/*                                              */
/*          AIMP Programming Interface          */
/*               v3.60 build 1455               */
/*                                              */
/*                Artem Izmaylov                */
/*                (C) 2006-2015                 */
/*                 www.aimp.ru                  */
/*              ICQ: 345-908-513                */
/*            Mail: support@aimp.ru             */
/*                                              */
/************************************************/

#ifndef apiObjectsH
#define apiObjectsH

#include <windows.h>
#include <unknwn.h>

static const GUID IID_IAIMPConfig = {0x41494D50, 0x436F, 0x6E66, 0x69, 0x67, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
static const GUID IID_IAIMPErrorInfo = {0x41494D50, 0x4572, 0x7249, 0x6E, 0x66, 0x6F, 0x00, 0x00, 0x00, 0x00, 0x00};
static const GUID IID_IAIMPHashCode = {0x41494D50, 0x4861, 0x7368, 0x43, 0x6F, 0x64, 0x65, 0x00, 0x00, 0x00, 0x00};
static const GUID IID_IAIMPFileStream = {0x41494D50, 0x4669, 0x6C65, 0x53, 0x74, 0x72, 0x65, 0x61, 0x6D, 0x00, 0x00};
static const GUID IID_IAIMPImage = {0x41494D50, 0x496D, 0x6167, 0x65, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
static const GUID IID_IAIMPImageContainer = {0x41494D50, 0x496D, 0x6167, 0x65, 0x43, 0x6F, 0x6E, 0x74, 0x6E, 0x72, 0x00};
static const GUID IID_IAIMPMemoryStream = {0x41494D50, 0x4D65, 0x6D53, 0x74, 0x72, 0x65, 0x61, 0x6D, 0x00, 0x00, 0x00};
static const GUID IID_IAIMPObjectList = {0x41494D50, 0x4F62, 0x6A4C, 0x69, 0x73, 0x74, 0x00, 0x00, 0x00, 0x00, 0x00};
static const GUID IID_IAIMPProgressCallback = {0x41494D50, 0x5072, 0x6F67, 0x72, 0x65, 0x73, 0x73, 0x43, 0x42, 0x00, 0x00};
static const GUID IID_IAIMPPropertyList = {0x41494D50, 0x5072, 0x6F70, 0x4C, 0x69, 0x73, 0x74, 0x00, 0x00, 0x00, 0x00};
static const GUID IID_IAIMPStream = {0x41494D50, 0x5374, 0x7265, 0x61, 0x6D, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
static const GUID IID_IAIMPString = {0x41494D50, 0x5374, 0x7269, 0x6E, 0x67, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};

// IAIMPImage and IAIMPImageContainer FormatID
const int AIMP_IMAGE_FORMAT_UNKNOWN = 0;
const int AIMP_IMAGE_FORMAT_BMP     = 1;
const int AIMP_IMAGE_FORMAT_GIF     = 2;
const int AIMP_IMAGE_FORMAT_JPG     = 3;
const int AIMP_IMAGE_FORMAT_PNG     = 4;

// Flags for IAIMPImage.Draw
const int AIMP_IMAGE_DRAW_STRETCHMODE_STRETCH = 0;
const int AIMP_IMAGE_DRAW_STRETCHMODE_FILL    = 1;
const int AIMP_IMAGE_DRAW_STRETCHMODE_FIT     = 2;
const int AIMP_IMAGE_DRAW_STRETCHMODE_TILE    = 4;
const int AIMP_IMAGE_DRAW_QUALITY_DEFAULT     = 0;
const int AIMP_IMAGE_DRAW_QUALITY_LOW         = 8;
const int AIMP_IMAGE_DRAW_QUALITY_HIGH        = 16;
  
// IAIMPPropertyList
const int AIMP_PROPERTYLIST_CUSTOM_PROPID_BASE = 1000;

// Flags for IAIMPString.ChangeCase
const int AIMP_STRING_CASE_LOWER                          = 1;
const int AIMP_STRING_CASE_UPPER                          = 2;
const int AIMP_STRING_CASE_ALL_WORDS_WITH_CAPICAL_LETTER  = 3;
const int AIMP_STRING_CASE_FIRST_WORD_WITH_CAPICAL_LETTER = 4;

// Flags for IAIMPString.Find and IAIMPString.Replace
const int AIMP_STRING_FIND_IGNORECASE = 1;
const int AIMP_STRING_FIND_WHOLEWORD  = 2;

// IAIMPStream.Seet Mode
const int AIMP_STREAM_SEEKMODE_FROM_BEGINNING = 0;
const int AIMP_STREAM_SEEKMODE_FROM_CURRENT   = 1;
const int AIMP_STREAM_SEEKMODE_FROM_END       = 2;

/* IAIMPHashCode */

class IAIMPHashCode: public IUnknown
{
	public:
		virtual int  WINAPI GetHashCode() = 0;
		virtual void WINAPI Recalculate() = 0;
};

/* IAIMPObjectList */

class IAIMPObjectList: public IUnknown
{
	public:
		virtual HRESULT WINAPI Add(IUnknown* Obj) = 0;
		virtual HRESULT WINAPI Clear() = 0;
		virtual HRESULT WINAPI Delete(int Index) = 0;
		virtual HRESULT WINAPI Insert(int Index, IUnknown* Obj) = 0;
		
		virtual int WINAPI GetCount() = 0;
		virtual HRESULT WINAPI GetObject(int Index, REFIID IID, void **Obj) = 0;
		virtual HRESULT WINAPI SetObject(int Index, IUnknown* Obj) = 0;
};

/* IAIMPString */

class IAIMPString: public IUnknown
{
	public:
		virtual HRESULT WINAPI GetChar(int Index, WCHAR *Char) = 0;
		virtual WCHAR* WINAPI GetData() = 0;
		virtual int WINAPI GetLength() = 0;
		virtual int WINAPI GetHashCode() = 0;
		virtual HRESULT WINAPI SetChar(int Index, WCHAR Char) = 0;
		virtual HRESULT WINAPI SetData(WCHAR* Chars, int CharsCount) = 0;

		virtual HRESULT WINAPI Add(IAIMPString* S) = 0;
		virtual HRESULT WINAPI Add2(WCHAR* Chars, int CharsCount) = 0;

		virtual HRESULT WINAPI ChangeCase(int Mode) = 0;
		virtual HRESULT WINAPI Clone(IAIMPString **S) = 0;

		virtual HRESULT WINAPI Compare(IAIMPString* S, int* CompareResult, bool IgnoreCase) = 0;
		virtual HRESULT WINAPI Compare2(WCHAR* Chars, int CharsCount, int* CompareResult, bool IgnoreCase) = 0;

		virtual HRESULT WINAPI Delete(int Index, int Count) = 0;

		virtual HRESULT WINAPI Find(IAIMPString* S, int *Index, int Flags, int StartFromIndex) = 0;
		virtual HRESULT WINAPI Find2(WCHAR *Chars, int CharsCount, int *Index, int Flags, int StartFromIndex) = 0;

		virtual HRESULT WINAPI Insert(int Index, IAIMPString *S) = 0;
		virtual HRESULT WINAPI Insert2(int Index, WCHAR *Chars, int CharsCount) = 0;

		virtual HRESULT WINAPI Replace(IAIMPString *OldPattern, IAIMPString *NewPattern, int Flags) = 0;
		virtual HRESULT WINAPI Replace2(WCHAR *OldPatternChars, int OldPatternCharsCount,
			WCHAR *NewPatternChars, int NewPatternCharsCount, int Flags) = 0;

		virtual HRESULT WINAPI SubString(int Index, int Count, IAIMPString **S) = 0;
};

/* IAIMPStream */

class IAIMPStream: public IUnknown
{
	public:
		virtual INT64 WINAPI GetSize() = 0;
		virtual HRESULT WINAPI SetSize(const INT64 Value) = 0;
		virtual INT64 WINAPI GetPosition() = 0;
		virtual HRESULT WINAPI Seek(const INT64 Offset, int Mode) = 0;
		virtual int WINAPI Read(unsigned char* Buffer, unsigned int Count) = 0;
		virtual HRESULT WINAPI Write(unsigned char* Buffer, unsigned int Count, unsigned int* Written) = 0;
};

/* IAIMPFileStream */

class IAIMPFileStream: public IAIMPStream
{
	public:
		virtual HRESULT WINAPI GetClipping(INT64 *Offset, INT64 *Size) = 0;
		virtual HRESULT WINAPI GetFileName(IAIMPString **S) = 0;
};

/* IAIMPMemoryStream */

class IAIMPMemoryStream: public IAIMPStream
{
	public:
		virtual void* WINAPI GetData() = 0;
};

/* IAIMPErrorInfo */

class IAIMPErrorInfo: public IUnknown
{
	public:
		virtual HRESULT WINAPI GetInfo(int *ErrorCode, IAIMPString **Message, IAIMPString **Details) = 0;
		virtual HRESULT WINAPI GetInfoFormatted(IAIMPString **S) = 0;
		virtual void WINAPI	SetInfo(int ErrorCode, IAIMPString *Message, IAIMPString *Details) = 0;
};

/* IAIMPImage */

class IAIMPImage: public IUnknown
{
	public:
		virtual HRESULT WINAPI LoadFromFile(IAIMPString *FileName) = 0;
		virtual HRESULT WINAPI LoadFromStream(IAIMPStream *Stream) = 0;
		virtual HRESULT WINAPI SaveToFile(IAIMPString *FileName, int FormatID) = 0;
		virtual HRESULT WINAPI SaveToStream(IAIMPStream *Stream, int FormatID) = 0;
		virtual int WINAPI GetFormatID() = 0;
		virtual HRESULT WINAPI GetSize(SIZE *size) = 0;
		virtual HRESULT Clone(IAIMPImage **Image) = 0;
		virtual HRESULT WINAPI Draw(HDC DC, RECT R, DWORD Flags, IUnknown *Attrs) = 0;
		virtual HRESULT WINAPI Resize(int Width, int Height) = 0;
};

/* IAIMPImageContainer */

class IAIMPImageContainer: public IUnknown
{
	public:
		virtual HRESULT WINAPI CreateImage(IAIMPImage **Image) = 0;
		virtual HRESULT WINAPI GetInfo(SIZE *Size, int *FormatID) = 0;
		virtual byte* WINAPI GetData() = 0;
		virtual DWORD WINAPI GetDataSize() = 0;
		virtual HRESULT WINAPI SetDataSize(DWORD Value) = 0;		
};

/* IAIMPProgressCallback */

class IAIMPProgressCallback: public IUnknown
{
	public:
		virtual HRESULT WINAPI Process(float Progress, bool *Canceled) = 0;
};

/* IAIMPPropertyList */

class IAIMPPropertyList: public IUnknown
{
	public:
		virtual void WINAPI BeginUpdate() = 0;
		virtual void WINAPI EndUpdate() = 0;
		virtual HRESULT WINAPI Reset() = 0;
		// Read
		virtual HRESULT WINAPI GetValueAsFloat(int PropertyID, double *Value) = 0;
		virtual HRESULT WINAPI GetValueAsInt32(int PropertyID, int *Value) = 0;
		virtual HRESULT WINAPI GetValueAsInt64(int PropertyID, INT64 *Value) = 0;
		virtual HRESULT WINAPI GetValueAsObject(int PropertyID, REFIID IID, void **Value) = 0;
		// Write
		virtual HRESULT WINAPI SetValueAsFloat(int PropertyID, const double Value) = 0;
		virtual HRESULT WINAPI SetValueAsInt32(int PropertyID, int Value) = 0;
		virtual HRESULT WINAPI SetValueAsInt64(int PropertyID, const INT64 Value) = 0;
		virtual HRESULT WINAPI SetValueAsObject(int PropertyID, IUnknown *Value) = 0;
};

/* IAIMPConfig */

class IAIMPConfig: public IUnknown
{
	public:
		// Delete
		virtual HRESULT WINAPI Delete(IAIMPString *KeyPath) = 0;
		// Read
		virtual HRESULT WINAPI GetValueAsFloat(IAIMPString *KeyPath, double *Value) = 0;
		virtual HRESULT WINAPI GetValueAsInt32(IAIMPString *KeyPath, int *Value) = 0;
		virtual HRESULT WINAPI GetValueAsInt64(IAIMPString *KeyPath, INT64 *Value) = 0;
		virtual HRESULT WINAPI GetValueAsStream(IAIMPString *KeyPath, IAIMPStream **Value) = 0;
		virtual HRESULT WINAPI GetValueAsString(IAIMPString *KeyPath, IAIMPString **Value) = 0;
		// Write
		virtual HRESULT WINAPI SetValueAsFloat(IAIMPString *KeyPath, const double Value) = 0;
		virtual HRESULT WINAPI SetValueAsInt32(IAIMPString *KeyPath, int Value) = 0;
		virtual HRESULT WINAPI SetValueAsInt64(IAIMPString *KeyPath, const INT64 Value) = 0;
		virtual HRESULT WINAPI SetValueAsStream(IAIMPString *KeyPath, IAIMPStream *Value) = 0;
		virtual HRESULT WINAPI SetValueAsString(IAIMPString *KeyPath, IAIMPString *Value) = 0;
};

#endif // !apiObjectsH