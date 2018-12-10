// Magic.h: interface for the CMagic class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MAGIC_H__652BC4ED_5F86_11D3_8424_00105A6B96E7__INCLUDED_)
#define AFX_MAGIC_H__652BC4ED_5F86_11D3_8424_00105A6B96E7__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CLSPResMagic;
class CDDSurface;

#define MAGIC_TYPE_UP		0
#define MAGIC_TYPE_DOWN		1
#define MAGIC_TYPE_POSION   2 // ������ ��ġ�� ��Ÿ���� �Ѵ�.
#define MAGIC_TYPE_LOOP     3 // ������ ���� ����.
#define MAGIC_TYPE_NOTALPHA_UP		4
#define MAGIC_TYPE_NOTALPHA_DOWN	5



typedef struct
{
	CPoint	m_ptPos;	//���ư��� ������ ����� ��ǥ
	int		m_nDir;		//	"                    ����
} MR_CELL;	//Magic Routing cell

typedef struct
{
	int		m_nMagicIndex;	//������ �ε���
	int		m_nCurFrame;	//������ �׷����� ���� ������
	int		m_nFrame;		//���� ���ҽ��� �� ������
	int		m_nMagicType;	//������ Ÿ��
	DWORD	m_dwLastTick;	
	BOOL	m_bShowContinue;//������ �ݺ��ؼ� �ٽ� �׸�������.....
	int		m_nHeight;
	BYTE	m_nType; // 1 = Magic, 2 = Special
} MAGIC_INFO;//dspring_0720	

class CMagic  
{
public:
	//*********************************************
	//�ڽ��� User��ü�� ������ ��� ����ϴ� �Լ���
	//*********************************************
	//������ Frame�� �׸����  True �� return��
	BOOL					DrawMagic(CDDSurface *pSurface);
	BOOL					DrawMagic(CDDSurface *pSurface, int x, int y, int Alpha = 1, BOOL bNext = FALSE);//dspring_0721
//	BOOL					DrawMagic(CDDSurface *pSurface, int x, int y, int Alpha = 1);//soma_0721
	//BOOL					SetMagicRes(int nIndex);//soma_0720

	BOOL					SetSpecialRes(int nIndex, int MagicType, bool bForceNew = false);

	BOOL					SetMagicRes(int nIndex,int MagicType=0);//dspring_0720
	void					SetMagicInfo( int index , int MagicType, BYTE type, bool bForceNew = false );//dspring_0720

	int						GetMagicAttackDesUid(){ return m_nDstUid;}//dspring_0722
	void					SetMagicAttackDesUid( int uid ){ m_nDstUid = uid;}//dspring_0722
	//*******************************************************
	//��ü�� ������ ����Ǵ� ������ �׸� ��� ����ϴ� �Լ���
	//*******************************************************

	//�ٱ׸���� TRUE return
	BOOL					DrawFlyingMagic(CDDSurface *pSurface);
	//Cell�̵��� �Ǹ� TRUE return
	BOOL					DrawFlyingMagic(CDDSurface *pSurface, int nDir, int x, int y); //���� �Լ����� ���
	int						DirStartFrame(int nDir, int nResMaxFrame);

	BOOL					SetSkyMagicRes(int nIndex, CPoint startPos, CPoint EndPos, int &nFirstDir,int nSeries);
	void					SetSkyRoute(CPoint ptStart, CPoint ptEnd, int iDir, int nSeries = 0);

	BOOL					SetFlyMagicRes(int nIndex, CPoint ptStart, CPoint ptEnd, int &nFirstDir,int nSeries);	//����Resource Load�� �ʱ����� ����
	void					RoutePositon(CPoint ptStart, CPoint ptEnd, int nSeries = 0);	//���ư��� ���� ��������(����:����)
	BOOL					IsFlyingMagic() {return m_bTypeFlying;}		

	BOOL					IsDownMagic() {return (BOOL)(m_nType==MAGIC_TYPE_DOWN);}	//�Ʒ� �򸮴� �����ΰ�?
	void					SetMagicType(int nType) {m_nType = nType;}					//���� Type����
	int                     GetMagicType() { return m_nType; }

	void					SetLastBeatEffect(int nDstUid);//���� ������ �´� ȿ�� ������ ��� ����
	CMagic();
	virtual ~CMagic();
	static	DWORD			dwMagicMoveDelay;				//���ư��� �ӵ� Delay

public:
	CPoint                  m_Pos;// 
	int                     m_nsUid;//������ ����� ��� �Ƶ�..
	int                     m_nDir;
	int                     m_nCount;
	int                     m_height;
	CArray<MAGIC_INFO* ,MAGIC_INFO*> m_arrMagicInfo;//dspring_0720���������� ������ �ִ� array

protected:
	int						m_nDstUid;
	int						m_nType;	//������ ���⼺ (�Ʒ� �򸮴� �����ΰ� , ���� ���� �����ΰ�?);
	BOOL					m_bTypeFlying;
	DWORD					m_dwLastMoveTick;	//������ �̵��ӵ��� �����ϴ� �ð�
	int						m_nCurDir; //���ư��� ������ ��� ���ư��� ����
	int						m_nFrame;	//������ Frame��(��ü �� Frame)
	int						m_nCurFrame;	//���� �׸��� ������ Frame
	CLSPResMagic*				m_bpMagicRes;	//���� Resource�� ���� ������(���ҽ��� MagicManager�� ����)
	DWORD					m_dwLastTick;	//������ Frame��ȭ�� ����
	int						m_nResIndex;	//MagicManager������ Resource Index
	
	CList<MR_CELL, MR_CELL>	m_lstMagicRoutingCell;	//������ ������� ��(���ư��� ���� �� ���)
};

#endif // !defined(AFX_MAGIC_H__652BC4ED_5F86_11D3_8424_00105A6B96E7__INCLUDED_)
