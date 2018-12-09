// BigOneView.cpp : CBigOneView ���ʵ��
//

#include "stdafx.h"
#include "BigOne.h"

#include "BigOneDoc.h"
#include "BigOneView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CBigOneView

IMPLEMENT_DYNCREATE(CBigOneView, CView)

BEGIN_MESSAGE_MAP(CBigOneView, CView)
	// ��׼��ӡ����
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CView::OnFilePrintPreview)
END_MESSAGE_MAP()

// CBigOneView ����/����

CBigOneView::CBigOneView()
{
	// TODO: �ڴ˴���ӹ������

}

CBigOneView::~CBigOneView()
{
}

BOOL CBigOneView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: �ڴ˴�ͨ���޸�
	//  CREATESTRUCT cs ���޸Ĵ��������ʽ

	return CView::PreCreateWindow(cs);
}

// CBigOneView ����

void CBigOneView::OnDraw(CDC* /*pDC*/)
{
	CBigOneDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: �ڴ˴�Ϊ����������ӻ��ƴ���
}


// CBigOneView ��ӡ

BOOL CBigOneView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// Ĭ��׼��
	return DoPreparePrinting(pInfo);
}

void CBigOneView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: ��Ӷ���Ĵ�ӡǰ���еĳ�ʼ������
}

void CBigOneView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: ��Ӵ�ӡ����е��������
}


// CBigOneView ���

#ifdef _DEBUG
void CBigOneView::AssertValid() const
{
	CView::AssertValid();
}

void CBigOneView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CBigOneDoc* CBigOneView::GetDocument() const // �ǵ��԰汾��������
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CBigOneDoc)));
	return (CBigOneDoc*)m_pDocument;
}
#endif //_DEBUG


// CBigOneView ��Ϣ�������
