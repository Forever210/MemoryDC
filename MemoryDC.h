#pragma once
#include<afxwin.h>


class CMemoryDC:public CDC
{
private:
	CSize m_size;
public:
	inline int GetWidth()const{return m_size.cx;}//获取位图地宽
	inline int GetHeight()const{return m_size.cy;}//获取位图的高
	/*****************************
	*
	*	创建透明Rgn区域
	*	rgn区域
	*	cTrans透明色
	*
	******************************/
	void MakeTrans(CRgn& rgn, COLORREF cTrans)
	{
		rgn.DeleteObject();						//删除已有的区域
		rgn.CreateRectRgn(0, 0, 0, 0);			//创建rgn区域
		CRgn r;
		r.CreateRectRgn(0, 0, 0, 0);			//创建r区域

		int cx = m_size.cx;
		int cy = m_size.cy;
		for (int i = 0; i < cx; i++)
		{
			for (int j = 0; j < cy; j++)
			{
				if (GetPixel(i, j) != cTrans)		 //GetPixel是检测指定位置的RGB颜色
				{
					r.SetRectRgn(i, j, i + 1, j + 1);//设置区域
					rgn.CombineRgn(&rgn, &r, RGN_OR);//组合rgn区域和r区域为一个区域
				}
			}
		}
	}

	/*******************
	*
	*	创建一个空白内存位图
	*	cx 位图的宽
	*	cy 位图地高
	*
	*********************/
	BOOL Create(int cx, int cy, CDC* pDC = NULL)
	{
		CreateCompatibleDC(pDC);//内存DC
		CBitmap bmp;
		if (pDC)
			bmp.CreateCompatibleBitmap(pDC, cx, cy);//创建内存位图
		else
			bmp.CreateCompatibleBitmap(&(CClientDC)NULL, cx, cy);
		m_size.cx = cx;
		m_size.cy = cy;
		SelectObject(bmp);
		return TRUE;
	}
	/****************************************
	*
	*	图片的透明色
	*	int nxDest,			//目标起点x
	*	int nyDest,			//目标起点y
	*	int nDWidth,		//目标宽度
	*	int nDHeight,		//目标高度
	*	CDC* pDC,			//目标DC
	*	int nxSour,			//来源起点x
	*	int nySour,			//来源起点y
	*	COLORREF cTrans		//透明色
	*
	*******************************************/
	void BitTrans(int nxDest, int nyDest, int nDWidth,
		int nDHeight, CDC* pDC, int nxSour, int nySour, COLORREF cTrans)
	{
		CMemoryDC dcImage(nDWidth, nDHeight, pDC);			//临时DC
		CBitmap bmMask;
		bmMask.CreateBitmap(nDWidth, nDHeight, 1, 1, NULL); //创建单色位图
		CDC dcMask;											//掩码DC
		dcMask.CreateCompatibleDC(pDC);						//创建掩码DC的内存DC
		dcMask.SelectObject(bmMask);
		//将加载的位图的内存DC复制到临时DC中
		dcImage.BitBlt(0, 0, nDWidth, nDHeight, this, nxSour, nySour, SRCCOPY);

		dcImage.SetBkColor(cTrans);							//设置临时DC的透明颜色
		//把掩码DC的透明区域设置为白色其余区域为黑色
		dcMask.BitBlt(0, 0, nDWidth, nDHeight, &dcImage, 0, 0, SRCCOPY);
		//把临时DC的透明区域为黑色，其余区域不变
		dcImage.SetBkColor(RGB(0, 0, 0));
		dcImage.SetTextColor(RGB(255, 255, 255));
		dcImage.BitBlt(0, 0, nDWidth, nDHeight, &dcMask, 0, 0, SRCAND);
		//目标DC的透明区域为屏幕色，其余区域保持不变
		pDC->SetBkColor(RGB(255, 255, 255));
		pDC->SetTextColor(RGB(0, 0, 0));
		pDC->BitBlt(nxDest, nyDest, nDWidth, nDHeight, &dcMask, 0, 0, SRCAND);
		pDC->BitBlt(nxDest, nyDest, nDWidth, nDHeight, &dcImage, 0, 0, SRCPAINT);
	}
	/*************************************
	*
	*	伸缩图片的透明色
	*	int nxDest,			//目标起点x
	*	int nyDest,			//目标起点y
	*	int nDWidth,		//目标宽度
	*	int nDHeight,		//目标高度
	*	CDC* pDC,			//目标DC
	*	int nxSour,			//来源起点x
	*	int nySour,			//来源起点y
	*	COLORREF cTrans		//透明色
	*
	**************************************/
	void StretchTrans(int nxDest, int nyDest, int nDWidth, int nDHeight,
		CDC* pDC, int nxSour, int nySour, int nSWidth, int nSHeight, COLORREF cTrans)
	{
		CMemoryDC dcImage(nDWidth, nDHeight, pDC);//临时DC
		CBitmap bmpMask;
		bmpMask.CreateBitmap(nDWidth, nDHeight, 1, 1, NULL);//创建单色位图
		CDC dcMask;							//掩码DC
		dcMask.CreateCompatibleDC(pDC);
		dcMask.SelectObject(bmpMask);
		//将加载位图的掩码DC，拷贝到临时DC中
		if (nDWidth == nSWidth && nDHeight == nSHeight)
			dcImage.BitBlt(0, 0, nDWidth, nDHeight, this, nxSour, nySour, SRCCOPY);
		else
			dcImage.StretchBlt(0, 0, nDWidth, nDHeight, this,
				nxSour, nySour, nSWidth, nSHeight, SRCCOPY);
		dcImage.SetBkColor(cTrans);							//设置临时DC的透明颜色
		//把掩码DC的透明区域设置为白色其余区域为黑色
		dcMask.BitBlt(0, 0, nDWidth, nDHeight, &dcImage, 0, 0, SRCCOPY);
		//把临时DC的透明区域为黑色，其余区域不变
		dcImage.SetBkColor(RGB(0, 0, 0));
		dcImage.SetTextColor(RGB(255, 255, 255));
		dcImage.BitBlt(0, 0, nDWidth, nDHeight, &dcMask, 0, 0, SRCAND);
		//目标DC的透明区域为屏幕色，其余区域保持不变
		pDC->SetBkColor(RGB(255, 255, 255));
		pDC->SetTextColor(RGB(0, 0, 0));
		pDC->BitBlt(nxDest, nyDest, nDWidth, nDHeight, &dcMask, 0, 0, SRCAND);
		pDC->BitBlt(nxDest, nyDest, nDWidth, nDHeight, &dcImage, 0, 0, SRCPAINT);
	}
	/*****************************
	*
	*	从资源视图中加载位图
	*	nBitmapID 位图ID
	*
	******************************/
	BOOL LoadBitmap(UINT nBitmapID, CDC* pDC = NULL)
	{
		CBitmap bmp;
		bmp.LoadBitmap(nBitmapID);
		BITMAP hbmp;
		bmp.GetBitmap(&hbmp);		//获取位图信息
		m_size.cx = hbmp.bmWidth;
		m_size.cy = hbmp.bmHeight;
		CreateCompatibleDC(pDC);	//内存DC
		SelectObject(bmp);			//替换对象
		return TRUE;
	}
	
	/*******************************
	*
	*	加载文件夹里的.bmp位图
	*	sBitmapPath 位图路径
	*
	*******************************/
	BOOL LoadBitmap(LPCTSTR sBitmapPath, CDC* pDC = NULL)
	{
		HBITMAP hBmp = (HBITMAP)LoadImage(AfxGetInstanceHandle(), sBitmapPath, 
			IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);//加载位图
		if (!hBmp)
			return FALSE;
		BITMAP bmp;
		GetObject(hBmp, sizeof(bmp), &bmp);		//获取位图信息
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
