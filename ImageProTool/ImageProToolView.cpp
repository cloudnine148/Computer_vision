// ImageProToolView.cpp : implementation of the CImageProToolView class
//

#include "stdafx.h"
#include "ImageProTool.h"
#include "Dib.h"
#include "math.h"
#include "time.h"
#include "ImageProToolDoc.h"
#include "ImageProToolView.h"
#include <vector>
#include <algorithm>
const double PI = acos(-1.0);
const int N=15;


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif



//========================================
/////////////////////////////////////////////////////////////////////////////
// CImageProToolView

IMPLEMENT_DYNCREATE(CImageProToolView, CScrollView)

BEGIN_MESSAGE_MAP(CImageProToolView, CScrollView)
	//{{AFX_MSG_MAP(CImageProToolView)
	ON_COMMAND(ID_FILE_OPEN, OnFileOpen)

	//}}AFX_MSG_MAP
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, CScrollView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, CScrollView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, CScrollView::OnFilePrintPreview)
	ON_COMMAND(ID_RGBTOGRAY, &CImageProToolView::OnRgbtogray)
	ON_COMMAND(ID_CHAPTER2_PIXELADDITION, &CImageProToolView::OnChapter2Pixeladdition)
	ON_COMMAND(ID_OR_OPERATION, &CImageProToolView::OnOR_Operation)
	ON_COMMAND(ID_NOR_OPERATION, &CImageProToolView::OnNOR_Operation)
	ON_COMMAND(ID_XOR_OPERATION, &CImageProToolView::OnXOR_Operation)
	ON_COMMAND(ID_XNOR_OPERATION, &CImageProToolView::OnXNOR_Operation)
	ON_COMMAND(ID_AND_OPERATION, &CImageProToolView::OnAndOperation)
	ON_COMMAND(ID_MEAN_ADAPTIVE_THRESHOLDING, &CImageProToolView::OnMeanAdaptiveThresholding)
	ON_COMMAND(ID_MEDIAN_ADAPTIVE_THRESHOLDING, &CImageProToolView::OnMedianAdaptiveThresholding)
	ON_COMMAND(ID_MIN_MAX_ADAPTIVE_THRESHOLDING, &CImageProToolView::OnMinMaxAdaptiveThresholding)
	ON_COMMAND(ID_STRETCHING, &CImageProToolView::OnStretching)
	ON_COMMAND(ID_EQUALIZATION, &CImageProToolView::OnEqualization)
	ON_COMMAND(ID_CONSERVATIVE_FILTERING, &CImageProToolView::OnConservativeFiltering)
	ON_COMMAND(ID_UNSHARP_FILTERING, &CImageProToolView::OnUnsharpFiltering)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CImageProToolView construction/destruction

CImageProToolView::CImageProToolView()
{
  Is_FileOpen = false;
}

CImageProToolView::~CImageProToolView()
{
}

BOOL CImageProToolView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CScrollView::PreCreateWindow(cs);
}

/////////////////////////////////////////////////////////////////////////////
// CImageProToolView drawing

void CImageProToolView::OnDraw(CDC* pDC)
{
	CImageProToolDoc* pDoc = GetDocument();
  	ASSERT_VALID(pDoc);

	if(Is_FileOpen)
	{
  
    CSize sizeTotal;
		
	sizeTotal.cx =pDoc->m_Width;
	sizeTotal.cy =pDoc->m_Height;
	SetScrollSizes(MM_TEXT, sizeTotal);
    
	GetParentFrame()->RecalcLayout();
	ResizeParentToFit(FALSE);
   	
	CPalette* pOldPalette = pDC->SelectPalette(pDoc->m_pDib->m_pPalDib, FALSE);
	pDC->RealizePalette();
	pDoc->m_pDib->Draw(pDC, 0, 0,pDoc->m_Width, pDoc->m_Height);	

	}
}


void CImageProToolView::OnInitialUpdate()
{
	CScrollView::OnInitialUpdate();
    CImageProToolDoc* pDoc = GetDocument();
	CSize sizeTotal;
	
	if(pDoc->m_Height<=0 || pDoc->m_Width<=0)
	{sizeTotal.cx =100;
	sizeTotal.cy =100;}
	else {
    sizeTotal.cx =pDoc->m_Height;
	sizeTotal.cy =pDoc->m_Width;
	}
	SetScrollSizes(MM_TEXT, sizeTotal);
    
	GetParentFrame()->RecalcLayout();
	ResizeParentToFit(FALSE);
}

/////////////////////////////////////////////////////////////////////////////
// CImageProToolView printing

BOOL CImageProToolView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CImageProToolView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void CImageProToolView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}

/////////////////////////////////////////////////////////////////////////////
// CImageProToolView diagnostics

#ifdef _DEBUG
void CImageProToolView::AssertValid() const
{
	CScrollView::AssertValid();
}

void CImageProToolView::Dump(CDumpContext& dc) const
{
	CScrollView::Dump(dc);
}

CImageProToolDoc* CImageProToolView::GetDocument() // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CImageProToolDoc)));
	return (CImageProToolDoc*)m_pDocument;
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CImageProToolView message handlers

void CImageProToolView::OnFileOpen() 
{   

	CImageProToolDoc* pDoc = (CImageProToolDoc*)GetDocument();
	ASSERT_VALID(pDoc);

	pDoc->OnFileOpen();
	Is_FileOpen = true;
	Invalidate();
}


//-----------------------------------------------------------------------------------------------Seperate and Set RGB
void CImageProToolView::Seperate_RGB(BYTE* Data, RGBptr** ptr)
{
	CImageProToolDoc* pDoc = (CImageProToolDoc*)GetDocument();
	ASSERT_VALID(pDoc);
	int width = pDoc->m_Width;    
	int height = pDoc->m_Height; 

	int i,j,y,x;

	for(i=height-1,y=0;i>=0;i--,y++)
		for(j=0,x=0;j<width;j++,x++)
		{  
			ptr[y][x].b=Data[i*width*3+j*3];
			ptr[y][x].g=Data[i*width*3+j*3+1];
			ptr[y][x].r=Data[i*width*3+j*3+2];
		}
}
void CImageProToolView::SetRGBptr(BYTE* pData, RGBptr** ptr1,int width, int height)
{
	int i,j,y,x;
	for(i=height-1,y=0;i>=0;i--,y++)
		for(j=0,x=0;j<width;j++,x++)
		{
			pData[i*width*3+j*3]=(BYTE)(ptr1[y][x].b);
			pData[i*width*3+j*3+1]=(BYTE)(ptr1[y][x].g);
			pData[i*width*3+j*3+2]=(BYTE)(ptr1[y][x].r);

		}	
}


//-------------------------------------------------------------------------------------------------------------------


void CImageProToolView::OnRgbtogray()
{
	CImageProToolDoc* pDoc = (CImageProToolDoc*)GetDocument();
	ASSERT_VALID(pDoc);

	int width = pDoc->m_Width;
	int height = pDoc->m_Height;	
 	
	BYTE* pData = pDoc->m_pDib->GetBitsAddress();
	RGBptr** ptr1= new RGBptr*[height];
	BYTE** image = new BYTE*[height];
  
	for( int i = 0 ; i < height ; i++ )
	{
	      ptr1[i]= new RGBptr [width];
		  image[i] = new BYTE [width];
	 }		
	
   	Seperate_RGB(pData,ptr1); 

	for(int i=0;i<height;i++)
	{
		for(int j=0;j<width;j++) 	
		{
			image[i][j]=(BYTE)Saturation((ptr1[i][j].r+ptr1[i][j].g+ptr1[i][j].b)/3);
		}
	}

	//화면 출력을 위한 과정
	for(int i=0;i<height;i++)
		for(int j=0;j<width;j++)
			ptr1[i][j].r=ptr1[i][j].g=ptr1[i][j].b=image[i][j];	

	SetRGBptr(pData,ptr1,width,height);

	for(int i = 0 ; i < height ; i++ )
	{
		delete [] image[i];
		delete [] ptr1[i];
	}
	delete [] image;
	delete [] ptr1;	
		
    Invalidate();
}

void CImageProToolView::OnChapter2Pixeladdition()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	CImageProToolDoc* pDoc = (CImageProToolDoc*)GetDocument();
	ASSERT_VALID(pDoc);

	auto width = pDoc->m_Width;
	auto height = pDoc->m_Height;

	BYTE* pData = pDoc->m_pDib->GetBitsAddress();
	RGBptr** ptr1 = new RGBptr*[height];
	BYTE** image = new BYTE*[height];

	for (auto i = 0; i < height; i++)
	{
		ptr1[i] = new RGBptr[width];
		image[i] = new BYTE[width];
	}

	Seperate_RGB(pData, ptr1);
	

	pDoc->OnFileOpen();
	pData = pDoc->m_pDib->GetBitsAddress();
	RGBptr** ptr2 = new RGBptr*[height];
	for(auto i = 0; i < height ; i++)
		ptr2[i] = new RGBptr[width];

	Seperate_RGB(pData, ptr2);
	
	int C = 50;
	auto temp = 0, max = 255;
	for(auto i = 0; i < height; i++)
		for(auto j = 0; j < width; j++)
		{
			/*ptr1[i][j].b = Saturation(ptr1[i][j].b + ptr2[i][j].b);
			ptr1[i][j].g = Saturation(ptr1[i][j].g +  ptr2[i][j].g);
			ptr1[i][j].r = Saturation(ptr1[i][j].r +  ptr2[i][j].r);*/

			ptr1[i][j].b = Wrapping(ptr1[i][j].b + ptr2[i][j].b);
			ptr1[i][j].g = Wrapping(ptr1[i][j].g +  ptr2[i][j].g);
			ptr1[i][j].r = Wrapping(ptr1[i][j].r +  ptr2[i][j].r);
		}
	SetRGBptr(pData, ptr1, width, height);

	for(auto i = 0; i < height; i++)
	{
		delete[] ptr1[i];
		delete[] ptr2[i];
	}
	delete[] ptr1;
	delete[] ptr2;

	Invalidate();
} 


void CImageProToolView::OnOR_Operation()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	CImageProToolDoc* pDoc = (CImageProToolDoc*)GetDocument();
	ASSERT_VALID(pDoc);

	auto width = pDoc->m_Width;
	auto height = pDoc->m_Height;

	BYTE* pData = pDoc->m_pDib->GetBitsAddress();
	RGBptr** ptr1 = new RGBptr*[height];
	BYTE** image = new BYTE*[height];

	for (auto i = 0; i < height; i++)
	{
		ptr1[i] = new RGBptr[width];
		image[i] = new BYTE[width];
	}

	Seperate_RGB(pData, ptr1);


	pDoc->OnFileOpen();
	pData = pDoc->m_pDib->GetBitsAddress();
	RGBptr** ptr2 = new RGBptr*[height];
	for (auto i = 0; i < height; i++)
		ptr2[i] = new RGBptr[width];

	Seperate_RGB(pData, ptr2);
	int C = 50;
	auto temp = 0, max = 255;
	for (auto i = 0; i < height; i++)
		for (auto j = 0; j < width; j++)
		{
			int a = 3 | 5;
			ptr1[i][j].b = Saturation(ptr1[i][j].b | ptr2[i][j].b);
			ptr1[i][j].g = Saturation(ptr1[i][j].g |  ptr2[i][j].g);
			ptr1[i][j].r = Saturation(ptr1[i][j].r |  ptr2[i][j].r);

		}
	SetRGBptr(pData, ptr1, width, height);

	for (auto i = 0; i < height; i++)
	{
		delete[] ptr1[i];
		delete[] ptr2[i];
	}
	delete[] ptr1;
	delete[] ptr2;

	Invalidate();
}


void CImageProToolView::OnNOR_Operation()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	CImageProToolDoc* pDoc = (CImageProToolDoc*)GetDocument();
	ASSERT_VALID(pDoc);

	auto width = pDoc->m_Width;
	auto height = pDoc->m_Height;

	BYTE* pData = pDoc->m_pDib->GetBitsAddress();
	RGBptr** ptr1 = new RGBptr*[height];
	BYTE** image = new BYTE*[height];

	for (auto i = 0; i < height; i++)
	{
		ptr1[i] = new RGBptr[width];
		image[i] = new BYTE[width];
	}

	Seperate_RGB(pData, ptr1);


	pDoc->OnFileOpen();
	pData = pDoc->m_pDib->GetBitsAddress();
	RGBptr** ptr2 = new RGBptr*[height];
	for (auto i = 0; i < height; i++)
		ptr2[i] = new RGBptr[width];

	Seperate_RGB(pData, ptr2);
	int C = 50;
	auto temp = 0, max = 255;
	for (auto i = 0; i < height; i++)
		for (auto j = 0; j < width; j++)
		{
			ptr1[i][j].b = ~(Saturation(ptr1[i][j].b | ptr2[i][j].b));
			ptr1[i][j].g = ~(Saturation(ptr1[i][j].g | ptr2[i][j].g));
			ptr1[i][j].r = ~(Saturation(ptr1[i][j].r | ptr2[i][j].r));
		}
	SetRGBptr(pData, ptr1, width, height);

	for (auto i = 0; i < height; i++)
	{
		delete[] ptr1[i];
		delete[] ptr2[i];
	}
	delete[] ptr1;
	delete[] ptr2;

	Invalidate();
}


void CImageProToolView::OnXOR_Operation()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	CImageProToolDoc* pDoc = (CImageProToolDoc*)GetDocument();
	ASSERT_VALID(pDoc);

	auto width = pDoc->m_Width;
	auto height = pDoc->m_Height;

	BYTE* pData = pDoc->m_pDib->GetBitsAddress();
	RGBptr** ptr1 = new RGBptr*[height];
	BYTE** image = new BYTE*[height];

	for (auto i = 0; i < height; i++)
	{
		ptr1[i] = new RGBptr[width];
		image[i] = new BYTE[width];
	}

	Seperate_RGB(pData, ptr1);


	pDoc->OnFileOpen();
	pData = pDoc->m_pDib->GetBitsAddress();
	RGBptr** ptr2 = new RGBptr*[height];
	for (auto i = 0; i < height; i++)
		ptr2[i] = new RGBptr[width];

	Seperate_RGB(pData, ptr2);
	int C = 50;
	auto temp = 0, max = 255;
	for (auto i = 0; i < height; i++)
		for (auto j = 0; j < width; j++)
		{
			ptr1[i][j].b = Saturation(ptr1[i][j].b ^ ptr2[i][j].b);
			ptr1[i][j].g = Saturation(ptr1[i][j].g ^ ptr2[i][j].g);
			ptr1[i][j].r = Saturation(ptr1[i][j].r ^ ptr2[i][j].r);
		}
	SetRGBptr(pData, ptr1, width, height);

	for (auto i = 0; i < height; i++)
	{
		delete[] ptr1[i];
		delete[] ptr2[i];
	}
	delete[] ptr1;
	delete[] ptr2;

	Invalidate();
}


void CImageProToolView::OnXNOR_Operation()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	CImageProToolDoc* pDoc = (CImageProToolDoc*)GetDocument();
	ASSERT_VALID(pDoc);

	auto width = pDoc->m_Width;
	auto height = pDoc->m_Height;

	BYTE* pData = pDoc->m_pDib->GetBitsAddress();
	RGBptr** ptr1 = new RGBptr*[height];
	BYTE** image = new BYTE*[height];

	for (auto i = 0; i < height; i++)
	{
		ptr1[i] = new RGBptr[width];
		image[i] = new BYTE[width];
	}

	Seperate_RGB(pData, ptr1);


	pDoc->OnFileOpen();
	pData = pDoc->m_pDib->GetBitsAddress();
	RGBptr** ptr2 = new RGBptr*[height];
	for (auto i = 0; i < height; i++)
		ptr2[i] = new RGBptr[width];

	Seperate_RGB(pData, ptr2);
	int C = 50;
	auto temp = 0, max = 255;
	for (auto i = 0; i < height; i++)
		for (auto j = 0; j < width; j++)
		{
			ptr1[i][j].b = ~(Saturation(ptr1[i][j].b ^ ptr2[i][j].b));
			ptr1[i][j].g = ~(Saturation(ptr1[i][j].g ^ ptr2[i][j].g));
			ptr1[i][j].r = ~(Saturation(ptr1[i][j].r ^ ptr2[i][j].r));
		}
	SetRGBptr(pData, ptr1, width, height);

	for (auto i = 0; i < height; i++)
	{
		delete[] ptr1[i];
		delete[] ptr2[i];
	}
	delete[] ptr1;
	delete[] ptr2;

	Invalidate();
}


void CImageProToolView::OnAndOperation()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	CImageProToolDoc* pDoc = (CImageProToolDoc*)GetDocument();
	ASSERT_VALID(pDoc);

	auto width = pDoc->m_Width;
	auto height = pDoc->m_Height;

	BYTE* pData = pDoc->m_pDib->GetBitsAddress();
	RGBptr** ptr1 = new RGBptr*[height];
	BYTE** image = new BYTE*[height];

	for (auto i = 0; i < height; i++)
	{
		ptr1[i] = new RGBptr[width];
		image[i] = new BYTE[width];
	}

	Seperate_RGB(pData, ptr1);


	pDoc->OnFileOpen();
	pData = pDoc->m_pDib->GetBitsAddress();
	RGBptr** ptr2 = new RGBptr*[height];
	for (auto i = 0; i < height; i++)
		ptr2[i] = new RGBptr[width];

	Seperate_RGB(pData, ptr2);
	int C = 50;
	auto temp = 0, max = 255;
	for (auto i = 0; i < height; i++)
		for (auto j = 0; j < width; j++)
		{
			ptr1[i][j].b = ~(Saturation(ptr1[i][j].b & ptr2[i][j].b));
			ptr1[i][j].g = ~(Saturation(ptr1[i][j].g & ptr2[i][j].g));
			ptr1[i][j].r = ~(Saturation(ptr1[i][j].r & ptr2[i][j].r));
		}
	SetRGBptr(pData, ptr1, width, height);

	for (auto i = 0; i < height; i++)
	{
		delete[] ptr1[i];
		delete[] ptr2[i];
	}
	delete[] ptr1;
	delete[] ptr2;

	Invalidate();
}


void CImageProToolView::OnMeanAdaptiveThresholding()
{
	CImageProToolDoc* pDoc = (CImageProToolDoc*)GetDocument();
	ASSERT_VALID(pDoc);

	int width = pDoc->m_Width;
	int height = pDoc->m_Height;

	BYTE* pData = pDoc->m_pDib->GetBitsAddress();
	RGBptr** ptr1 = new RGBptr*[height];
	BYTE** image = new BYTE*[height];
	BYTE** image2 = new BYTE*[height];

	for (int i = 0; i < height; i++)
	{
		ptr1[i] = new RGBptr[width];
		image[i] = new BYTE[width];
		image2[i] = new BYTE[width];
	}

	Seperate_RGB(pData, ptr1);

	for (int i = 0; i < height; i++)
	{
		for (int j = 0; j < width; j++)
		{
			image[i][j] = (BYTE)Saturation((ptr1[i][j].r + ptr1[i][j].g + ptr1[i][j].b) / 3);
		}
	}


	
	double pixel_sum_value = 0;
	double mean_value;
	for (auto j = 1; j < height - 1; j++)
		for (auto i = 1; i < width - 1; i++)
		{
			pixel_sum_value = image[j - 1][i - 1] + image[j - 1][i] + image[j - 1][i + 1] + image[j][i - 1] +
				image[j][i] + image[j][i + 1] + image[j + 1][i - 1] + image[j + 1][i] + image[j + 1][i + 1];
			mean_value = (int)Saturation(pixel_sum_value / 9. + 0.5);

			if (image[j][i] >= mean_value)
				image2[j][i] = 255;
			
			else
				image2[j][i] = 0;
		}

	//원본 이미지에 색상값 할당
	for (int j = 0; j < height; j++)
		for (int i = 0; i < width; i++)
			ptr1[j][i].r = ptr1[j][i].g = ptr1[j][i].b = image2[j][i];

	SetRGBptr(pData, ptr1, width, height);

	for (auto i = 0; i < height; i++)
	{
		delete[] ptr1[i];
		delete[] image[i];
	}
	delete[] ptr1;
	delete[] image;

	Invalidate();
}


void CImageProToolView::OnMedianAdaptiveThresholding()
{
	CImageProToolDoc* pDoc = (CImageProToolDoc*)GetDocument();
	ASSERT_VALID(pDoc);

	int width = pDoc->m_Width;
	int height = pDoc->m_Height;

	BYTE* pData = pDoc->m_pDib->GetBitsAddress();
	RGBptr** ptr1 = new RGBptr*[height];
	BYTE** image = new BYTE*[height];
	BYTE** image2 = new BYTE*[height];
	for (int i = 0; i < height; i++)
	{
		ptr1[i] = new RGBptr[width];
		image[i] = new BYTE[width];
		image2[i] = new BYTE[width];
	}

	Seperate_RGB(pData, ptr1);

	for (int i = 0; i < height; i++)
	{
		for (int j = 0; j < width; j++)
		{
			image[i][j] = (BYTE)Saturation((ptr1[i][j].r + ptr1[i][j].g + ptr1[i][j].b) / 3);
		}
	}

	std::vector<int> pix_value;
	int median_value;
	for (auto j = 1; j < height - 1; j++)
		for (auto i = 1; i < width - 1; i++)
		{
			pix_value.clear();
			pix_value.push_back(image[j - 1][i - 1]);	pix_value.push_back(image[j - 1][i]);
			pix_value.push_back(image[j - 1][i + 1]);	pix_value.push_back(image[j][i - 1]);
			pix_value.push_back(image[j][i]);			pix_value.push_back(image[j][i+1]);
			pix_value.push_back(image[j + 1][i - 1]); 	pix_value.push_back(image[j + 1][i]);
			pix_value.push_back(image[j + 1][i + 1]);	sort(pix_value.begin(), pix_value.end());
			median_value = pix_value[pix_value.size() / 2];

			if (image[j][i] > median_value)
				image2[j][i] = 255;
			else
				image2[j][i] = 0;
		}

	//원본 이미지에 색상값 할당
	for (int j = 0; j < height; j++)
		for (int i = 0; i < width; i++)
			ptr1[j][i].r = ptr1[j][i].g = ptr1[j][i].b = image2[j][i];

	SetRGBptr(pData, ptr1, width, height);

	for (auto i = 0; i < height; i++)
	{
		delete[] ptr1[i];
		delete[] image[i];
	}
	delete[] ptr1;
	delete[] image;

	Invalidate();
}


void CImageProToolView::OnMinMaxAdaptiveThresholding()
{
	CImageProToolDoc* pDoc = (CImageProToolDoc*)GetDocument();
	ASSERT_VALID(pDoc);

	int width = pDoc->m_Width;
	int height = pDoc->m_Height;

	BYTE* pData = pDoc->m_pDib->GetBitsAddress();
	RGBptr** ptr1 = new RGBptr*[height];
	BYTE** image = new BYTE*[height];
	BYTE** image2 = new BYTE*[height];
	for (int i = 0; i < height; i++)
	{
		ptr1[i] = new RGBptr[width];
		image[i] = new BYTE[width];
		image2[i] = new BYTE[width];
	}

	Seperate_RGB(pData, ptr1);

	for (int i = 0; i < height; i++)
	{
		for (int j = 0; j < width; j++)
		{
			image[i][j] = (BYTE)Saturation((ptr1[i][j].r + ptr1[i][j].g + ptr1[i][j].b) / 3);
		}
	}

	//화면 출력을 위한 과정
	for (int i = 0; i < height; i++)
		for (int j = 0; j < width; j++)
			ptr1[i][j].r = ptr1[i][j].g = ptr1[i][j].b = image[i][j];

	SetRGBptr(pData, ptr1, width, height);

	std::vector<int> pix_value;
	int min_value, max_value;
	double min_max_threshold;
	for (auto j = 1; j < height - 1; j++)
		for (auto i = 1; i < width - 1; i++)
		{
			pix_value.clear();
			pix_value.push_back(image[j - 1][i - 1]);	pix_value.push_back(image[j - 1][i]);
			pix_value.push_back(image[j - 1][i + 1]);	pix_value.push_back(image[j][i - 1]);
			pix_value.push_back(image[j][i]);			pix_value.push_back(image[j][i + 1]);
			pix_value.push_back(image[j + 1][i - 1]);	pix_value.push_back(image[j + 1][i]);
			pix_value.push_back(image[j + 1][i + 1]);	
			min_value = *min_element(pix_value.begin(), pix_value.end());
			max_value = *max_element(pix_value.begin(), pix_value.end());

			min_max_threshold = (min_value + max_value) / 2.;
			if (image[j][i] > min_max_threshold)
				image2[j][i] = 255;
			else
				image2[j][i] = 0;
		}

	//원본 이미지에 색상값 할당
	for (int j = 0; j < height; j++)
		for (int i = 0; i < width; i++)
			ptr1[j][i].r = ptr1[j][i].g = ptr1[j][i].b = image2[j][i];

	SetRGBptr(pData, ptr1, width, height);

	for (auto i = 0; i < height; i++)
	{
		delete[] ptr1[i];
		delete[] image[i];
	}
	delete[] ptr1;
	delete[] image;

	Invalidate();
}


void CImageProToolView::OnStretching()
{
	CImageProToolDoc* pDoc = (CImageProToolDoc*)GetDocument();
	ASSERT_VALID(pDoc);

	int width = pDoc->m_Width;
	int height = pDoc->m_Height;

	BYTE* pData = pDoc->m_pDib->GetBitsAddress();

	RGBptr** ptr1 = new RGBptr*[height];
	BYTE** image_r = new BYTE*[height];
	BYTE** image_g = new BYTE*[height];
	BYTE** image_b = new BYTE*[height];

	for (int i = 0; i < height; i++)
	{
		ptr1[i] = new RGBptr[width];
		image_r[i] = new BYTE[width];
		image_g[i] = new BYTE[width];
		image_b[i] = new BYTE[width];
	}


	Seperate_RGB(pData, ptr1);

	int i, j, index;

	for (i = 0; i < height; i++)
		for (j = 0; j < width; j++)
		{
			image_r[i][j] = ptr1[i][j].r;
			image_g[i][j] = ptr1[i][j].g;
			image_b[i][j] = ptr1[i][j].b;
		}

	float cumulation_r = 0.0;
	float cumulation_g = 0.0;
	float cumulation_b = 0.0;

	int histogram_r[256] = { 0 };
	int histogram_g[256] = { 0 };
	int histogram_b[256] = { 0 };

	for (i = 0; i < height; i++)
		for (j = 0; j < width; j++)
		{
			histogram_r[image_r[i][j]]++;
			histogram_g[image_g[i][j]]++;
			histogram_b[image_b[i][j]]++;
		}
	
	for (index = 0; index < 256; index++)
	{
		cumulation_r += (float)histogram_r[index];
		cumulation_g += (float)histogram_g[index];
		cumulation_b += (float)histogram_b[index];
		histogram_r[index]
			= (int)((cumulation_r / (float)(width*height))*255.0 + 0.5);
		histogram_g[index]
			= (int)((cumulation_g / (float)(width*height))*255.0 + 0.5);
		histogram_b[index]
			= (int)((cumulation_b / (float)(width*height))*255.0 + 0.5);
	}

	for (i = 0; i < height; i++)
		for (j = 0; j < width; j++)
		{
			index = image_r[i][j];
			image_r[i][j] = histogram_r[index];
		}
	for (i = 0; i < height; i++)
		for (j = 0; j < width; j++)
		{
			index = image_g[i][j];
			image_g[i][j] = histogram_g[index];
		}
	for (i = 0; i < height; i++)
		for (j = 0; j < width; j++)
		{
			index = image_b[i][j];
			image_b[i][j] = histogram_b[index];
		}

	for (i = 0; i < height; i++)
		for (j = 0; j < width; j++)
		{
			ptr1[i][j].r = image_r[i][j];
			ptr1[i][j].g = image_g[i][j];
			ptr1[i][j].b = image_b[i][j];
		}


	SetRGBptr(pData, ptr1, width, height);

	for (i = 0; i < height; i++)
	{
		delete[] ptr1[i];
		delete[] image_r[i];
	}
	delete[] ptr1;
	delete[] image_r;

	Invalidate();
}


void CImageProToolView::OnEqualization()
{
	CImageProToolDoc* pDoc = (CImageProToolDoc*)GetDocument();
	ASSERT_VALID(pDoc);

	int width = pDoc->m_Width;
	int height = pDoc->m_Height;

	BYTE* pData = pDoc->m_pDib->GetBitsAddress();

	RGBptr** ptr1 = new RGBptr*[height];
	BYTE** image_r = new BYTE*[height];
	BYTE** image_g = new BYTE*[height];
	BYTE** image_b = new BYTE*[height];
	for (int i = 0; i < height; i++)
	{
		ptr1[i] = new RGBptr[width];
		image_r[i] = new BYTE[width];
		image_g[i] = new BYTE[width];
		image_b[i] = new BYTE[width];
	}


	Seperate_RGB(pData, ptr1);

	int i, j, index;

	for (i = 0; i < height; i++)
		for (j = 0; j < width; j++)
		{
			image_r[i][j] = ptr1[i][j].r;
			image_g[i][j] = ptr1[i][j].g;
			image_b[i][j] = ptr1[i][j].b;
		}

	float cumulation_r = 0.0;
	float cumulation_g = 0.0;
	float cumulation_b = 0.0;
	int histogram_r[256] = { 0 };
	int histogram_g[256] = { 0 };
	int histogram_b[256] = { 0 };

	for (i = 0; i < height; i++)
		for (j = 0; j < width; j++)
		{
			histogram_r[image_r[i][j]]++;
			histogram_g[image_g[i][j]]++;
			histogram_b[image_b[i][j]]++;
		}

	for (index = 0; index < 256; index++)
	{
		cumulation_r += (float)histogram_r[index];
		cumulation_g += (float)histogram_g[index];
		cumulation_b += (float)histogram_b[index];
		histogram_r[index]
			= (int)((cumulation_r / (float)(width*height))*255.0 + 0.5);
		histogram_g[index]
			= (int)((cumulation_g / (float)(width*height))*255.0 + 0.5);
		histogram_b[index]
			= (int)((cumulation_b / (float)(width*height))*255.0 + 0.5);
	}

	for (i = 0; i < height; i++)
		for (j = 0; j < width; j++)
		{
			index = image_r[i][j];
			image_r[i][j] = histogram_r[index];
		}

	for (i = 0; i < height; i++)
		for (j = 0; j < width; j++)
		{
			index = image_g[i][j];
			image_g[i][j] = histogram_g[index];
		}
	for (i = 0; i < height; i++)
		for (j = 0; j < width; j++)
		{
			index = image_b[i][j];
			image_b[i][j] = histogram_b[index];
		}

	for (i = 0; i < height; i++)
		for (j = 0; j < width; j++)
		{
			ptr1[i][j].r = image_r[i][j];
			ptr1[i][j].g = image_g[i][j];
			ptr1[i][j].b = image_b[i][j];
		}


	SetRGBptr(pData, ptr1, width, height);

	for (i = 0; i < height; i++)
	{
		delete[] ptr1[i];
		delete[] image_r[i];
	}
	delete[] ptr1;
	delete[] image_r;

	Invalidate();
}


void CImageProToolView::OnConservativeFiltering()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	CImageProToolDoc* pDoc = (CImageProToolDoc*)GetDocument();
	ASSERT_VALID(pDoc);

	int width = pDoc->m_Width;
	int height = pDoc->m_Height;

	BYTE* pData = pDoc->m_pDib->GetBitsAddress();
	RGBptr** ptr1 = new RGBptr*[height];
	BYTE** image = new BYTE*[height];
	BYTE** image2 = new BYTE*[height];
	for (int i = 0; i < height; i++)
	{
		ptr1[i] = new RGBptr[width];
		image[i] = new BYTE[width];
		image2[i] = new BYTE[width];
	}

	Seperate_RGB(pData, ptr1);

	for (int i = 0; i < height; i++)
	{
		for (int j = 0; j < width; j++)
		{
			image[i][j] = (BYTE)Saturation((ptr1[i][j].r + ptr1[i][j].g + ptr1[i][j].b) / 3);
		}
	}

	//화면 출력을 위한 과정
	for (int i = 0; i < height; i++)
		for (int j = 0; j < width; j++)
			ptr1[i][j].r = ptr1[i][j].g = ptr1[i][j].b = image[i][j];

	SetRGBptr(pData, ptr1, width, height);

	std::vector<int> pix_value;
	int min_value, max_value;
	double min_max_threshold;
	for (auto j = 1; j < height - 1; j++)
		for (auto i = 1; i < width - 1; i++)
		{
			pix_value.clear();
			pix_value.push_back(image[j - 1][i - 1]);	pix_value.push_back(image[j - 1][i]);
			pix_value.push_back(image[j - 1][i + 1]);	pix_value.push_back(image[j][i - 1]);
			//pix_value.push_back(image[j][i]);			
			pix_value.push_back(image[j][i + 1]);
			pix_value.push_back(image[j + 1][i - 1]);	pix_value.push_back(image[j + 1][i]);
			pix_value.push_back(image[j + 1][i + 1]);
			min_value = *min_element(pix_value.begin(), pix_value.end());
			max_value = *max_element(pix_value.begin(), pix_value.end());

			int center = image[j][i];
			if (center > max_value) center = max_value;
			else if (center < min_value) center = min_value;

			image2[j][i] = Saturation(center);
		}
	for (int j = 0; j < height; j++)
		for (int i = 0; i < width; i++)
			ptr1[j][i].r = ptr1[j][i].g = ptr1[j][i].b = image2[j][i];

	SetRGBptr(pData, ptr1, width, height);

	for (auto i = 0; i < height; i++)
	{
		delete[] ptr1[i];
		delete[] image[i];
		delete[] image2[i];
	}
	
	delete[] ptr1;
	delete[] image;
	delete[] image2;

	Invalidate();
}


void CImageProToolView::OnUnsharpFiltering()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	CImageProToolDoc* pDoc = (CImageProToolDoc*)GetDocument();
	ASSERT_VALID(pDoc);

	int width = pDoc->m_Width;
	int height = pDoc->m_Height;

	BYTE* pData = pDoc->m_pDib->GetBitsAddress();
	RGBptr** ptr1 = new RGBptr*[height];
	BYTE** image = new BYTE*[height];
	BYTE** image2 = new BYTE*[height];
	for (int i = 0; i < height; i++)
	{
		ptr1[i] = new RGBptr[width];
		image[i] = new BYTE[width];
		image2[i] = new BYTE[width];
	}

	Seperate_RGB(pData, ptr1);

	for (int i = 0; i < height; i++)
	{
		for (int j = 0; j < width; j++)
		{
			image[i][j] = (BYTE)Saturation((ptr1[i][j].r + ptr1[i][j].g + ptr1[i][j].b) / 3);
		}

	}
	double mean;
	float mean3x3[3][3];

	for (auto k = 0; k < 3; k++)
		for (auto l = 0; l < 3; l++)
			mean3x3[k][l] = 1. / (3 * 3);
	for(auto j = 1; j < height - 1; j++)
		for (auto i = 1; i < width - 1; i++)
		{
			mean = 0;
			for(auto m = -1; m < 1; m++)
				for (auto n = -1; n < 1; n++)
				{
					mean += (int)image[j + m][i + n] * mean3x3[m + 1][n + 1];
				}
			int val = (int)((float)image[j][i] + 0.7 * ((float)image[j][i] - mean));
			image2[j][i] = (BYTE)Saturation(val);
		}

	for (int j = 0; j < height; j++)
		for (int i = 0; i < width; i++)
			ptr1[j][i].r = ptr1[j][i].g = ptr1[j][i].b = image2[j][i];

	SetRGBptr(pData, ptr1, width, height);

	for (auto i = 0; i < height; i++)
	{
		delete[] ptr1[i];
		delete[] image[i];
		delete[] image2[i];
	}

	delete[] ptr1;
	delete[] image;
	delete[] image2;

	Invalidate();
}
