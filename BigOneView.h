// BigOneView.h : CBigOneView ��Ľӿ�
//


#pragma once


class CBigOneView : public CView
{
protected: // �������л�����
	CBigOneView();
	DECLARE_DYNCREATE(CBigOneView)

// ����
public:
	CBigOneDoc* GetDocument() const;

// ����
public:

// ��д
public:
	virtual void OnDraw(CDC* pDC);  // ��д�Ի��Ƹ���ͼ
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

// ʵ��
public:
	virtual ~CBigOneView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// ���ɵ���Ϣӳ�亯��
protected:
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // BigOneView.cpp �еĵ��԰汾
inline CBigOneDoc* CBigOneView::GetDocument() const
   { return reinterpret_cast<CBigOneDoc*>(m_pDocument); }
#endif

