#pragma once
#include<afxwin.h>


class CMemoryDC:public CDC
{
private:
	CSize m_size;
public:
	inline int GetWidth()const{return m_size.cx;}//��ȡλͼ�ؿ�
	inline int GetHeight()const{return m_size.cy;}//��ȡλͼ�ĸ�
	/*****************************
	*
	*	����͸��Rgn����
	*	rgn����
	*	cTrans͸��ɫ
	*
	******************************/
	void MakeTrans(CRgn& rgn, COLORREF cTrans)
	{
		rgn.DeleteObject();						//ɾ�����е�����
		rgn.CreateRectRgn(0, 0, 0, 0);			//����rgn����
		CRgn r;
		r.CreateRectRgn(0, 0, 0, 0);			//����r����

		int cx = m_size.cx;
		int cy = m_size.cy;
		for (int i = 0; i < cx; i++)
		{
			for (int j = 0; j < cy; j++)
			{
				if (GetPixel(i, j) != cTrans)		 //GetPixel�Ǽ��ָ��λ�õ�RGB��ɫ
				{
					r.SetRectRgn(i, j, i + 1, j + 1);//��������
					rgn.CombineRgn(&rgn, &r, RGN_OR);//���rgn�����r����Ϊһ������
				}
			}
		}
	}

	/*******************
	*
	*	����һ���հ��ڴ�λͼ
	*	cx λͼ�Ŀ�
	*	cy λͼ�ظ�
	*
	*********************/
	BOOL Create(int cx, int cy, CDC* pDC = NULL)
	{
		CreateCompatibleDC(pDC);//�ڴ�DC
		CBitmap bmp;
		if (pDC)
			bmp.CreateCompatibleBitmap(pDC, cx, cy);//�����ڴ�λͼ
		else
			bmp.CreateCompatibleBitmap(&(CClientDC)NULL, cx, cy);
		m_size.cx = cx;
		m_size.cy = cy;
		SelectObject(bmp);
		return TRUE;
	}
	/****************************************
	*
	*	ͼƬ��͸��ɫ
	*	int nxDest,			//Ŀ�����x
	*	int nyDest,			//Ŀ�����y
	*	int nDWidth,		//Ŀ����
	*	int nDHeight,		//Ŀ��߶�
	*	CDC* pDC,			//Ŀ��DC
	*	int nxSour,			//��Դ���x
	*	int nySour,			//��Դ���y
	*	COLORREF cTrans		//͸��ɫ
	*
	*******************************************/
	void BitTrans(int nxDest, int nyDest, int nDWidth,
		int nDHeight, CDC* pDC, int nxSour, int nySour, COLORREF cTrans)
	{
		CMemoryDC dcImage(nDWidth, nDHeight, pDC);			//��ʱDC
		CBitmap bmMask;
		bmMask.CreateBitmap(nDWidth, nDHeight, 1, 1, NULL); //������ɫλͼ
		CDC dcMask;											//����DC
		dcMask.CreateCompatibleDC(pDC);						//��������DC���ڴ�DC
		dcMask.SelectObject(bmMask);
		//�����ص�λͼ���ڴ�DC���Ƶ���ʱDC��
		dcImage.BitBlt(0, 0, nDWidth, nDHeight, this, nxSour, nySour, SRCCOPY);

		dcImage.SetBkColor(cTrans);							//������ʱDC��͸����ɫ
		//������DC��͸����������Ϊ��ɫ��������Ϊ��ɫ
		dcMask.BitBlt(0, 0, nDWidth, nDHeight, &dcImage, 0, 0, SRCCOPY);
		//����ʱDC��͸������Ϊ��ɫ���������򲻱�
		dcImage.SetBkColor(RGB(0, 0, 0));
		dcImage.SetTextColor(RGB(255, 255, 255));
		dcImage.BitBlt(0, 0, nDWidth, nDHeight, &dcMask, 0, 0, SRCAND);
		//Ŀ��DC��͸������Ϊ��Ļɫ���������򱣳ֲ���
		pDC->SetBkColor(RGB(255, 255, 255));
		pDC->SetTextColor(RGB(0, 0, 0));
		pDC->BitBlt(nxDest, nyDest, nDWidth, nDHeight, &dcMask, 0, 0, SRCAND);
		pDC->BitBlt(nxDest, nyDest, nDWidth, nDHeight, &dcImage, 0, 0, SRCPAINT);
	}
	/*************************************
	*
	*	����ͼƬ��͸��ɫ
	*	int nxDest,			//Ŀ�����x
	*	int nyDest,			//Ŀ�����y
	*	int nDWidth,		//Ŀ����
	*	int nDHeight,		//Ŀ��߶�
	*	CDC* pDC,			//Ŀ��DC
	*	int nxSour,			//��Դ���x
	*	int nySour,			//��Դ���y
	*	COLORREF cTrans		//͸��ɫ
	*
	**************************************/
	void StretchTrans(int nxDest, int nyDest, int nDWidth, int nDHeight,
		CDC* pDC, int nxSour, int nySour, int nSWidth, int nSHeight, COLORREF cTrans)
	{
		CMemoryDC dcImage(nDWidth, nDHeight, pDC);//��ʱDC
		CBitmap bmpMask;
		bmpMask.CreateBitmap(nDWidth, nDHeight, 1, 1, NULL);//������ɫλͼ
		CDC dcMask;							//����DC
		dcMask.CreateCompatibleDC(pDC);
		dcMask.SelectObject(bmpMask);
		//������λͼ������DC����������ʱDC��
		if (nDWidth == nSWidth && nDHeight == nSHeight)
			dcImage.BitBlt(0, 0, nDWidth, nDHeight, this, nxSour, nySour, SRCCOPY);
		else
			dcImage.StretchBlt(0, 0, nDWidth, nDHeight, this,
				nxSour, nySour, nSWidth, nSHeight, SRCCOPY);
		dcImage.SetBkColor(cTrans);							//������ʱDC��͸����ɫ
		//������DC��͸����������Ϊ��ɫ��������Ϊ��ɫ
		dcMask.BitBlt(0, 0, nDWidth, nDHeight, &dcImage, 0, 0, SRCCOPY);
		//����ʱDC��͸������Ϊ��ɫ���������򲻱�
		dcImage.SetBkColor(RGB(0, 0, 0));
		dcImage.SetTextColor(RGB(255, 255, 255));
		dcImage.BitBlt(0, 0, nDWidth, nDHeight, &dcMask, 0, 0, SRCAND);
		//Ŀ��DC��͸������Ϊ��Ļɫ���������򱣳ֲ���
		pDC->SetBkColor(RGB(255, 255, 255));
		pDC->SetTextColor(RGB(0, 0, 0));
		pDC->BitBlt(nxDest, nyDest, nDWidth, nDHeight, &dcMask, 0, 0, SRCAND);
		pDC->BitBlt(nxDest, nyDest, nDWidth, nDHeight, &dcImage, 0, 0, SRCPAINT);
	}
	/*****************************
	*
	*	����Դ��ͼ�м���λͼ
	*	nBitmapID λͼID
	*
	******************************/
	BOOL LoadBitmap(UINT nBitmapID, CDC* pDC = NULL)
	{
		CBitmap bmp;
		bmp.LoadBitmap(nBitmapID);
		BITMAP hbmp;
		bmp.GetBitmap(&hbmp);		//��ȡλͼ��Ϣ
		m_size.cx = hbmp.bmWidth;
		m_size.cy = hbmp.bmHeight;
		CreateCompatibleDC(pDC);	//�ڴ�DC
		SelectObject(bmp);			//�滻����
		return TRUE;
	}
	
	/*******************************
	*
	*	�����ļ������.bmpλͼ
	*	sBitmapPath λͼ·��
	*
	*******************************/
	BOOL LoadBitmap(LPCTSTR sBitmapPath, CDC* pDC = NULL)
	{
		HBITMAP hBmp = (HBITMAP)LoadImage(AfxGetInstanceHandle(), sBitmapPath, 
			IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);//����λͼ
		if (!hBmp)
			return FALSE;
		BITMAP bmp;
		GetObject(hBmp, sizeof(bmp), &bmp);		//��ȡλͼ��Ϣ
		m_size.cx = bmp.bmWidth;
		m_size.cy = bmp.bmHeight;
		CreateCompatibleDC(pDC);
		SelectObject(hBmp);
		return TRUE;
	}

	BOOL DeleteDC()
	{
		if (!GetSafeHdc())
			return TRUE;
		CBitmap* pBmp = GetCurrentBitmap();
		pBmp->DeleteObject();
		return CDC::DeleteDC();
	}

	CMemoryDC(LPCTSTR sBitmapPath, CDC* pDC = NULL)
	{
		LoadBitmap(sBitmapPath, pDC);
	}

	CMemoryDC(UINT nBitmapID, CDC* pDC = NULL)
	{
		LoadBitmap(nBitmapID, pDC);
	}

	CMemoryDC(int cx, int cy, CDC* pDC = NULL)
	{
		Create(cx, cy, pDC);
	}

	CMemoryDC()
	{
		m_size = CSize();
	}

	~CMemoryDC()
	{
		DeleteDC();
	}

};
