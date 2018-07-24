
#pragma once
#include <Windows.h>
#include "MemoryPool.h"

struct HEADER
{
	char Code;
	short Len;
	unsigned char RandXOR;
	unsigned char CheckSum;
};
<<<<<<< HEAD
enum Errflag
{
	Get_Error,
	Put_Error,
	PutHeader_Error
};
=======
>>>>>>> c535bd7fc73a5367d12c92e9ead468baa9e47f0c

struct ErrorAlloc
{
	int UseDataSize;
	int GetSize;
	int PutSize;
	int UseHeaderSize;

	int Flag;
};
<<<<<<< HEAD
=======
enum ThrowCase
{
	Get_Error = 1,
	Put_Error = 2,
	PutHeader_Error = 3
};
>>>>>>> c535bd7fc73a5367d12c92e9ead468baa9e47f0c

class Packet
{
public:


	enum PACKET
	{
		BUFFER_DEFAULT			= 10000,		// ��Ŷ�� �⺻ ���� ������.
		HEADERSIZE_DEFAULT		= 5
	};
	// ������, �ı���.
			Packet();
			Packet(int iBufferSize);
			Packet(const Packet &SrcPacket);
			Packet (unsigned char PacketCode,char XOR_Code1,char XOR_Code2,int iBufferSize);

	virtual	~Packet();

	// ��Ŷ �ʱ�ȭ.
	void	Initial(int iBufferSize = BUFFER_DEFAULT);

	//RefCnt�� 1 ������Ŵ. 
	void	Add (void);

	// ��Ŷ ����.
	void	Clear(void);


	// ���� ������ ���.
	int		GetBufferSize(void) { return _iBufferSize; }

	// ���� ������� ������ ���.
	int		GetDataSize(void) { return _iDataSize; }



	// ���� ������ ���.
	char	*GetBufferPtr(void) { return HeaderStartPos; }

	// ���� Pos �̵�.
	int		MoveWritePos(int iSize);
	int		MoveReadPos(int iSize);






	// ������ ���۷�����.
	Packet	&operator = (Packet &SrcPacket);

	//////////////////////////////////////////////////////////////////////////
	// �ֱ�.	�� ���� Ÿ�Ը��� ��� ����.
	//////////////////////////////////////////////////////////////////////////
	Packet	&operator << (BYTE byValue);
	Packet	&operator << (char chValue);
	Packet &operator << (WCHAR &chValue);


	Packet	&operator << (short shValue);
	Packet	&operator << (WORD wValue);

	Packet	&operator << (int iValue);
	Packet	&operator << (DWORD dwValue);
	Packet	&operator << (float fValue);

	Packet	&operator << (__int64 iValue);
	Packet	&operator << (double dValue);

	//////////////////////////////////////////////////////////////////////////
	// ����.	�� ���� Ÿ�Ը��� ��� ����.
	//////////////////////////////////////////////////////////////////////////
	Packet	&operator >> (BYTE &byValue);
	Packet	&operator >> (char &chValue);
	Packet &operator >> (WCHAR &chValue);

	Packet	&operator >> (short &shValue);
	Packet	&operator >> (WORD &wValue);

	Packet	&operator >> (int &iValue);
	Packet	&operator >> (DWORD &dwValue);
	Packet	&operator >> (float &fValue);

	Packet	&operator >> (__int64 &iValue);
	Packet	&operator >> (double &dValue);




	// ����Ÿ ���.
	// Return ������ ������.
	int		GetData(char *chpDest, int iSize);

	// ����Ÿ ����.
	// Return ������ ������.
	int		PutData(char *chpSrc, int iSrcSize);


protected:

	// RefCnt�� �ϳ� ������Ű�� REfCnt�� 0�� �Ǹ� �ڱ��ڽ� delete�ϰ� ��������.
	void	Release (void);


	//------------------------------------------------------------
	// ��Ŷ���� / ���� ������.
	//------------------------------------------------------------
	char	BufferDefault[BUFFER_DEFAULT];
	char	*BufferExpansion;

	char	*Buffer;
	int		_iBufferSize;
	//------------------------------------------------------------
	// ��Ŷ���� ���� ��ġ.
	//------------------------------------------------------------
	char	*DataFieldStart;
	char	*DataFieldEnd;
	char	*HeaderStartPos;


	//------------------------------------------------------------
	// ������ ���� ��ġ, ���� ��ġ.
	//------------------------------------------------------------
	char	*ReadPos;
	char	*WritePos;


	//------------------------------------------------------------
	// ���� ���ۿ� ������� ������.
	//------------------------------------------------------------
	int		_iDataSize;
	int		HeaderSize;
	//------------------------------------------------------------
	// ���� Packet�� RefCnt
	//------------------------------------------------------------
	int iRefCnt;
	INT64 DeCodeCount;
	int AllocCount = 0;
private :
	static CMemoryPool<Packet> *PacketPool;

	int	PutHeader (char *chpSrc, int iSrcSize);

<<<<<<< HEAD
	static unsigned char _PacketCode;
	static unsigned char _XORCode1;
	static unsigned char _XORCode2;
	bool EnCodeFlag = false;
=======
	unsigned char _PacketCode;
	char _XORCode1;
	char _XORCode2;
	bool _EnCodeFlag;
>>>>>>> c535bd7fc73a5367d12c92e9ead468baa9e47f0c


	bool EnCode (void);
	bool DeCode (HEADER *SrcHeader = NULL);

	void AcquireLOCK (void)
	{
		AcquireSRWLockExclusive (&_CS);
	}
	void ReleaseLOCK (void)
	{
		ReleaseSRWLockExclusive (&_CS);
	}
	SRWLOCK _CS;

public :
	
	static void Initialize (void)
	{
		if ( PacketPool == NULL )
		{
			PacketPool = new CMemoryPool<Packet> (0);
		}
		return;
	}
	static void NetHeader_Initial (unsigned char PacketCode, unsigned char XOR1, unsigned char XOR2);

	static Packet *Alloc (void)
	{
		PROFILE_BEGIN (L"Alloc");
		Packet *p = PacketPool->Alloc ();
		PROFILE_END (L"Alloc");
		return p;
	}

	static bool Free (Packet *p)
	{
		bool flag;
		if ( InterlockedDecrement (( volatile long * )&p->iRefCnt) == 0 )
		{
			PROFILE_BEGIN (L"Free");
			flag = PacketPool->Free (p);
			PROFILE_END (L"Free");
			return flag;
		}
		return true;
	}
	static INT64 PacketPool_Full ()
	{
		return PacketPool->GetFullCount ();
	}

	static INT64 PacketPool_Alloc ()
	{
		return PacketPool->GetAllocCount ();
	}

	static INT64 PacketPool_Free ()
	{
		return PacketPool->GetFreeCount ();
	}

	friend class CNetServer;
	friend class CLanServer;

};
