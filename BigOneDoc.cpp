// BigOneDoc.cpp : CBigOneDoc ���ʵ��
//

#include "stdafx.h"
#include "BigOne.h"

#include "BigOneDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CBigOneDoc

IMPLEMENT_DYNCREATE(CBigOneDoc, CDocument)

BEGIN_MESSAGE_MAP(CBigOneDoc, CDocument)
END_MESSAGE_MAP()


// CBigOneDoc ����/����

CBigOneDoc::CBigOneDoc()
{
	// TODO: �ڴ����һ���Թ������

}

CBigOneDoc::~CBigOneDoc()
{
}

BOOL CBigOneDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: �ڴ�������³�ʼ������
	// (SDI �ĵ������ø��ĵ�)

	return TRUE;
}




// CBigOneDoc ���л�

void CBigOneDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: �ڴ���Ӵ洢����
	}
	else
	{
		// TODO: �ڴ���Ӽ��ش���
	}
}


// CBigOneDoc ���

#ifdef _DEBUG
void CBigOneDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CBigOneDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG


// CBigOneDoc ����
