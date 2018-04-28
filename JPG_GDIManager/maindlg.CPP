


#include "stdafx.h"
#include "MainDlg.h"
#include <atldlgs.h>
#include <atltypes.h>
#include <atlstr.h>
#include <math.h>
#include <comutil.h>
//using namespace Gdiplus;
//#pragma comment(lib,"gdiplus.lib")
//����������ֵ
#define MAX_FATOR   1.0
#define MIN_FATOR   0.1


LRESULT CMainDlg::OnBnClickedBtninsert(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{

    DestoryPicture();
    //���ͼƬ·��
    m_wstrImagePath = _T("");
    //�����Ի���
    TCHAR strFileFilters[1024] = _T("JPG Files(*.jpg)\0*.jpg\0\0");
    CFileDialog selImageDialog(TRUE, NULL, NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, strFileFilters, this->m_hWnd);
    TCHAR temp[1024];
    ZeroMemory(temp, sizeof(TCHAR) * 1024);
    selImageDialog.m_ofn.lpstrFile = temp;
    selImageDialog.m_ofn.nMaxFile = 1024;
    m_wstrImagePath = _T("");
    if (IDOK == selImageDialog.DoModal())
    {
        m_wstrImagePath = temp;
    }
    //�ж�ͼƬ·���Ƿ�Ϊ��
    if (!m_wstrImagePath.empty())
    {
    
        ShowPicture(m_wstrImagePath);
        //��ʼ����������
        m_fFactor = 1.0;
        //��ʼ����ת����
        m_fRoate = 0.0;
    }
    else
    {
        ::MessageBox(NULL, _T("��ѡ����Ҫ�����ͼƬ��"), _T("Tips"), MB_OK);
    }
    return 0;
}


LRESULT CMainDlg::OnBnClickedBtndelete(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
    // TODO: Add your control notification handler code here
    DestoryPicture();
    //���ͼƬ·��
    m_wstrImagePath = _T("");
    return 0;
}


LRESULT CMainDlg::OnBnClickedBtnpus(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
    //�ж�ͼƬ·���Ƿ�Ϊ��
    if (!m_wstrImagePath.empty())
    {
        //�Ŵ�
        if (m_fFactor < MAX_FATOR)
        {
            std::wstring wstrTemp = _T("");
            float fTemp = m_fFactor;
            ::GetDlgItemText(this->m_hWnd, IDC_EDIT_FATOR, (LPWSTR)wstrTemp.c_str(), wstrTemp.max_size());
            float nfator = _wtof(wstrTemp.c_str());
            m_fFactor = m_fFactor + nfator;
            if (m_fFactor > MAX_FATOR)
            {
                m_fFactor = fTemp;
                //return 0;
            }
            DestoryPicture();
            ZoomPicture(m_wstrImagePath, m_fFactor);
        }
        else
        {
            ::MessageBox(NULL, _T("�Ե�ǰ�����޷��ڼ����Ŵ�"), _T("Tips"), MB_OK);
            return 0;
        }
    }
    return 0;
}



LRESULT CMainDlg::OnBnClickedBtndec(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
    //�ж�ͼƬ·���Ƿ�Ϊ��
    if (!m_wstrImagePath.empty())
    {
        //��С
        if (m_fFactor > MIN_FATOR)
        {
            std::wstring wstrTemp = _T("");
            float fTemp = m_fFactor;
            ::GetDlgItemText(this->m_hWnd, IDC_EDIT_FATOR, (LPWSTR)wstrTemp.c_str(), wstrTemp.max_size());
            float nfator = _wtof(wstrTemp.c_str());
            m_fFactor = m_fFactor - nfator;
            if (m_fFactor <= MIN_FATOR)
            {
                m_fFactor = fTemp;
                ::MessageBox(NULL, _T("�Ե�ǰ�����޷��ڼ�����С��"), _T("Tips"), MB_OK);
                return 0;
            }
            DestoryPicture();
            ZoomPicture(m_wstrImagePath, m_fFactor);
        }
        else
        {
            return 0;
        }
    }
    return 0;
}


LRESULT CMainDlg::OnBnClickedBtnnagtive(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
    if (!m_wstrImagePath.empty())
    {
        //��ʱ��
        Gdiplus::Image *image = new Gdiplus::Image(m_wstrImagePath.c_str());
        //Gdiplus::Image image1;
        if (NULL != image)
        {
            std::wstring wstrTemp = _T("");
            ::GetDlgItemText(this->m_hWnd, IDC_EDIT_ROTATE, (LPWSTR)wstrTemp.c_str(), wstrTemp.max_size());
            int nRotar = _wtoi(wstrTemp.c_str());
            m_fRoate = m_fRoate - nRotar;
            //
            Gdiplus::Image *roteimage = RotateImage(image, m_fRoate);
            //���
            DestoryPicture();  
            CRect rect;
            CRect roterect;
            //�õ�ͼƬ�߶� 
            int height = roteimage->GetHeight();
            int width = roteimage->GetWidth();
            CWindow wnd = this->GetDlgItem(IDC_PIC);
            //���pictrue�ؼ����ڵľ�������
            wnd.GetClientRect(&rect);
            CDC pDc(wnd.GetDC());
            Gdiplus::Graphics graphics(pDc.m_hDC);
            //��ֹʧ��
            SetStretchBltMode(pDc.m_hDC, STRETCH_HALFTONE);

            //���ͼƬ�ĳߴ�С��ͼƬ�ؼ��ߴ磬��������ʾ  
            if (width <= rect.Width() && height <= rect.Width())
            {
                roterect = CRect(rect.TopLeft(), CSize(width, height));
                //��ͼƬ����Picture�ؼ���ʾ�ľ�������  
                graphics.DrawImage(roteimage, roterect.top, roterect.left, roterect.Width(), roterect.Height());
            }
            //���ͼƬ�ĳߴ����ͼƬ�ؼ��ĳߴ� 
            else
            {
                //��Y������������� 
                float xScale = (float)rect.Width() / (float)width;    
                float yScale = (float)rect.Height() / (float)height;
                float ScaleIndex = (xScale <= yScale ? xScale : yScale);
                roterect = CRect(rect.TopLeft(), CSize((int)width*ScaleIndex, (int)height*ScaleIndex));
                //��ͼƬ����Picture�ؼ���ʾ�ľ�������  
                graphics.DrawImage(roteimage, roterect.top, roterect.left, roterect.Width(), roterect.Height());
                ReleaseDC(pDc);
            }
        }
        delete image;
        image = nullptr;
    }
    return 0;
}


LRESULT CMainDlg::OnBnClickedBtnpostive(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
    if (!m_wstrImagePath.empty())
    {
        //��ʱ��
        Gdiplus::Image *image = new Gdiplus::Image(m_wstrImagePath.c_str());
        //Gdiplus::Image image1;
        if (NULL != image)
        {
            std::wstring wstrTemp = _T("");
            ::GetDlgItemText(this->m_hWnd, IDC_EDIT_ROTATE, (LPWSTR)wstrTemp.c_str(), wstrTemp.max_size());
            int nRotar = _wtoi(wstrTemp.c_str());
            m_fRoate = m_fRoate + nRotar;
            Gdiplus::Image *roteimage = RotateImage(image, m_fRoate);
            //���
            DestoryPicture();
            CRect rect;
            CRect roterect;
            //�õ�ͼƬ�߶� 
            int height = roteimage->GetHeight();
            //�õ�ͼƬ���� 
            int width = roteimage->GetWidth();

            CWindow wnd = this->GetDlgItem(IDC_PIC);
            //���pictrue�ؼ����ڵľ�������
            wnd.GetClientRect(&rect);
            CDC pDc(wnd.GetDC());
            Gdiplus::Graphics graphics(pDc.m_hDC);
            //��ֹʧ��
            SetStretchBltMode(pDc.m_hDC, STRETCH_HALFTONE);
            //���ͼƬ�ĳߴ�С��ͼƬ�ؼ��ߴ磬��������ʾ  
            if (width <= rect.Width() && height <= rect.Width())
            {
                roterect = CRect(rect.TopLeft(), CSize(width, height));
                //��ͼƬ����Picture�ؼ���ʾ�ľ�������  
                graphics.DrawImage(roteimage, roterect.top, roterect.left, roterect.Width(), roterect.Height());
          
            }
            //���ͼƬ�ĳߴ����ͼƬ�ؼ��ĳߴ� 
            else
            {
                //��Y�������������
                float xScale = (float)rect.Width() / (float)width;
                float yScale = (float)rect.Height() / (float)height;
                float ScaleIndex = (xScale <= yScale ? xScale : yScale);
                roterect = CRect(rect.TopLeft(), CSize((int)width*ScaleIndex, (int)height*ScaleIndex));
                //��ͼƬ����Picture�ؼ���ʾ�ľ�������  
                graphics.DrawImage(roteimage, roterect.top, roterect.left, roterect.Width(), roterect.Height());
                ReleaseDC(pDc);
            }
        }
        delete image;
        image = nullptr;
    }
    return 0;
}
LRESULT CMainDlg::OnBnClickedBtnhoriz(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
    if (!m_wstrImagePath.empty())
    {
        //ˮƽ
        Gdiplus::Image *image = new Gdiplus::Image(m_wstrImagePath.c_str());
        //Gdiplus::Image image1;
        if (NULL != image)
        {
            Gdiplus::Image *roteimage = FilpImageX(image);
            //���
            DestoryPicture();
            CRect rect;
            CRect filprect;
            //�õ�ͼƬ�߶� 
            int height = roteimage->GetHeight();
            //�õ�ͼƬ���� 
            int width = roteimage->GetWidth();

            CWindow wnd = this->GetDlgItem(IDC_PIC);
            //���pictrue�ؼ����ڵľ�������
            wnd.GetClientRect(&rect);
            CDC pDc(wnd.GetDC());
            Gdiplus::Graphics graphics(pDc.m_hDC);
            //��ֹʧ��
            SetStretchBltMode(pDc.m_hDC, STRETCH_HALFTONE);
            //���ͼƬ�ĳߴ�С��ͼƬ�ؼ��ߴ磬��������ʾ  
            if (width <= rect.Width() && height <= rect.Width())
            {
                filprect = CRect(rect.TopLeft(), CSize(width, height));
                //��ͼƬ����Picture�ؼ���ʾ�ľ�������  
                graphics.DrawImage(roteimage, filprect.top, filprect.left, filprect.Width(), filprect.Height());

            }
            //���ͼƬ�ĳߴ����ͼƬ�ؼ��ĳߴ� 
            else
            {
                //��Y�������������
                float xScale = (float)rect.Width() / (float)width;
                float yScale = (float)rect.Height() / (float)height;
                float ScaleIndex = (xScale <= yScale ? xScale : yScale);
                filprect = CRect(rect.TopLeft(), CSize((int)width*ScaleIndex, (int)height*ScaleIndex));
                //��ͼƬ����Picture�ؼ���ʾ�ľ�������  
                graphics.DrawImage(roteimage, filprect.top, filprect.left, filprect.Width(), filprect.Height());
                ReleaseDC(pDc);
            }
        }
        delete image;
        image = nullptr;
    }
    return 0;

}

LRESULT CMainDlg::OnBnClickedBtnvert(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
    if (!m_wstrImagePath.empty())
    {
        //��ֱ
        Gdiplus::Image *image = new Gdiplus::Image(m_wstrImagePath.c_str());
        //Gdiplus::Image image1;
        if (NULL != image)
        {
            Gdiplus::Image *roteimage = FilpImageY(image);
            //���
            DestoryPicture();
            CRect rect;
            CRect filprect;
            //�õ�ͼƬ�߶� 
            int height = roteimage->GetHeight();
            //�õ�ͼƬ���� 
            int width = roteimage->GetWidth();

            CWindow wnd = this->GetDlgItem(IDC_PIC);
            //���pictrue�ؼ����ڵľ�������
            wnd.GetClientRect(&rect);
            CDC pDc(wnd.GetDC());
            Gdiplus::Graphics graphics(pDc.m_hDC);
            //��ֹʧ��
            SetStretchBltMode(pDc.m_hDC, STRETCH_HALFTONE);
            //���ͼƬ�ĳߴ�С��ͼƬ�ؼ��ߴ磬��������ʾ  
            if (width <= rect.Width() && height <= rect.Width())
            {
                filprect = CRect(rect.TopLeft(), CSize(width, height));
                //��ͼƬ����Picture�ؼ���ʾ�ľ�������  
                graphics.DrawImage(roteimage, filprect.top, filprect.left, filprect.Width(), filprect.Height());

            }
            //���ͼƬ�ĳߴ����ͼƬ�ؼ��ĳߴ� 
            else
            {
                //��Y�������������
                float xScale = (float)rect.Width() / (float)width;
                float yScale = (float)rect.Height() / (float)height;
                float ScaleIndex = (xScale <= yScale ? xScale : yScale);
                filprect = CRect(rect.TopLeft(), CSize((int)width*ScaleIndex, (int)height*ScaleIndex));
                //��ͼƬ����Picture�ؼ���ʾ�ľ�������  
                graphics.DrawImage(roteimage, filprect.top, filprect.left, filprect.Width(), filprect.Height());
                ReleaseDC(pDc);
            }
        }
        delete image;
        image = nullptr;
    }
    return 0;
}

LRESULT CMainDlg::OnBnClickedBtnclone(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
    if (!m_wstrImagePath.empty())
    {
        //��ֱ
        Gdiplus::Image *image = new Gdiplus::Image(m_wstrImagePath.c_str());
        //Gdiplus::Image image1;
        if (NULL != image)
        {
            Gdiplus::Image *roteimage = CloneImage(image);
            //���
            DestoryPicture();
            CRect rect;
            CRect filprect;
            //�õ�ͼƬ�߶� 
            int height = roteimage->GetHeight();
            //�õ�ͼƬ���� 
            int width = roteimage->GetWidth();

            CWindow wnd = this->GetDlgItem(IDC_PIC);
            //���pictrue�ؼ����ڵľ�������
            wnd.GetClientRect(&rect);
            CDC pDc(wnd.GetDC());
            Gdiplus::Graphics graphics(pDc.m_hDC);
            //��ֹʧ��
            SetStretchBltMode(pDc.m_hDC, STRETCH_HALFTONE);
            //���ͼƬ�ĳߴ�С��ͼƬ�ؼ��ߴ磬��������ʾ  
            if (width <= rect.Width() && height <= rect.Width())
            {
                filprect = CRect(rect.TopLeft(), CSize(width, height));
                //��ͼƬ����Picture�ؼ���ʾ�ľ�������  
                graphics.DrawImage(roteimage, filprect.top, filprect.left, filprect.Width(), filprect.Height());

            }
            //���ͼƬ�ĳߴ����ͼƬ�ؼ��ĳߴ� 
            else
            {
                //��Y�������������
                float xScale = (float)rect.Width() / (float)width;
                float yScale = (float)rect.Height() / (float)height;
                float ScaleIndex = (xScale <= yScale ? xScale : yScale);
                filprect = CRect(rect.TopLeft(), CSize((int)width*ScaleIndex, (int)height*ScaleIndex));
                //��ͼƬ����Picture�ؼ���ʾ�ľ�������  
                graphics.DrawImage(roteimage, filprect.top, filprect.left, filprect.Width(), filprect.Height());
                ReleaseDC(pDc);
            }
        }
        delete image;
        image = nullptr;
    }
    return 0;
}
LRESULT CMainDlg::OnBnClickedBtncast(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
    if (!m_wstrImagePath.empty())
    {
        //����
        Gdiplus::Image *image = new Gdiplus::Image(m_wstrImagePath.c_str());
        //Gdiplus::Image image1;
        if (NULL != image)
        {
            Gdiplus::Image *roteimage = CastImage(image);
            //���
            DestoryPicture();
            CRect rect;
            CRect filprect;
            //�õ�ͼƬ�߶� 
            int height = roteimage->GetHeight();
            //�õ�ͼƬ���� 
            int width = roteimage->GetWidth();

            CWindow wnd = this->GetDlgItem(IDC_PIC);
            //���pictrue�ؼ����ڵľ�������
            wnd.GetClientRect(&rect);
            CDC pDc(wnd.GetDC());
            Gdiplus::Graphics graphics(pDc.m_hDC);
            //��ֹʧ��
            SetStretchBltMode(pDc.m_hDC, STRETCH_HALFTONE);
            //���ͼƬ�ĳߴ�С��ͼƬ�ؼ��ߴ磬��������ʾ  
            if (width <= rect.Width() && height <= rect.Width())
            {
                filprect = CRect(rect.TopLeft(), CSize(width, height));
                //��ͼƬ����Picture�ؼ���ʾ�ľ�������  
                graphics.DrawImage(roteimage, filprect.top, filprect.left, filprect.Width(), filprect.Height());

            }
            //���ͼƬ�ĳߴ����ͼƬ�ؼ��ĳߴ� 
            else
            {
                //��Y�������������
                float xScale = (float)rect.Width() / (float)width;
                float yScale = (float)rect.Height() / (float)height;
                float ScaleIndex = (xScale <= yScale ? xScale : yScale);
                filprect = CRect(rect.TopLeft(), CSize((int)width*ScaleIndex, (int)height*ScaleIndex));
                //��ͼƬ����Picture�ؼ���ʾ�ľ�������  
                graphics.DrawImage(roteimage, filprect.top, filprect.left, filprect.Width(), filprect.Height());
                ReleaseDC(pDc);
            }
        }
        delete image;
        image = nullptr;
    }
    return 0;
}

void CMainDlg::ShowPicture(const std::wstring& pFilepath)
{
    CWindow wnd = GetDlgItem(IDC_PIC);
    CDC pDc(wnd.GetDC());
    Gdiplus::Graphics graphics(pDc.m_hDC);
    Gdiplus::Image image(pFilepath.c_str()); 
    int height = image.GetHeight();
    int width = image.GetWidth();
    CRect rect;
    CRect fatorect;
    wnd.GetClientRect(&rect);
    //��X�������������  
    float xScale = (float)rect.Width() / (float)width;
    float yScale = (float)rect.Height() / (float)height;
    float ScaleIndex = (xScale <= yScale ? xScale : yScale);
    fatorect = CRect(rect.TopLeft(), CSize((int)width*ScaleIndex, (int)height*ScaleIndex));
    //��ͼƬ��ʾ���ؼ���
    graphics.DrawImage(&image, 0, 0, fatorect.Width(), fatorect.Height());
    ReleaseDC(pDc);
}
void CMainDlg::DestoryPicture()
{
    CWindow wnd = GetDlgItem(IDC_PIC);
    CRect rect;
    wnd.GetClientRect(&rect);
    CDC pDc(wnd.GetDC());
    pDc.FillSolidRect(rect.left, rect.top, rect.Width(), rect.Height(), RGB(240, 240, 240));
    ReleaseDC(pDc);
}
void CMainDlg::ZoomPicture(const std::wstring& pFilepath, const float fFator)
{
    CRect rect;
    CRect fatorrect;
    Gdiplus::Image * image = new Gdiplus::Image(pFilepath.c_str());
    int height = image->GetHeight();
    int width = image->GetWidth();
    CWindow wnd = this->GetDlgItem(IDC_PIC);
    wnd.GetClientRect(&rect);
    CDC pDc(wnd.GetDC());
    Gdiplus::Graphics graphics(pDc.m_hDC);
    //��ֹʧ��
    SetStretchBltMode(pDc.m_hDC, STRETCH_HALFTONE);

    //���ͼƬ�ĳߴ�С��ͼƬ�ؼ��ߴ磬��������ʾ  
    if (width <= rect.Width() && height <= rect.Width())
    {
        fatorrect = CRect(rect.TopLeft(), CSize(width * fFator, height * fFator));
        graphics.DrawImage(image, fatorrect.top, fatorrect.left, fatorrect.Width(), fatorrect.Height());
    }
    //���ͼƬ�ĳߴ����ͼƬ�ؼ��ĳߴ� 
    else
    {
        //��XY�������������  
        float xScale = (float)rect.Width() / (float)width;
        float yScale = (float)rect.Height() / (float)height;
        float ScaleIndex = (xScale <= yScale ? xScale : yScale);
        fatorrect = CRect(rect.TopLeft(), CSize((int)width * ScaleIndex * fFator, (int)height * ScaleIndex * fFator));
        graphics.DrawImage(image, fatorrect.top, fatorrect.left, fatorrect.Width(), fatorrect.Height());
    }
    delete image;
    ReleaseDC(pDc);

}
Gdiplus::Image* CMainDlg::RotateImage(Gdiplus::Image* source,  float fRoate)
{
    int nAngle = (int)fRoate % 360;
    int srcWidth = source->GetWidth();
    int srcHeight = source->GetHeight();
    //ͼ����ת֮����ռ������͸�
    Gdiplus::Rect *rotateRec = GetRotateRectangle(srcWidth, srcHeight, nAngle);
    int rotateWidth = rotateRec->Width;
    int rotateHeight = rotateRec->Height;
    //���廭��������Ϊͼ����ת��Ŀ���
    Gdiplus::Bitmap  *pImageScale = new Gdiplus::Bitmap(rotateWidth, rotateHeight);
    //graphics����destImage�����������ԭ���ʱ��destImage���Ͻ�
    Gdiplus::Graphics graphics(pImageScale);
    //Ҫ��graphicsΧ��ĳ�������ĵ���תN�ȣ�������
    //��һ������graphics����ԭ���Ƶ��������ĵ�,�������е����꣨x,y��
    //�ڶ�����graphics��ת��Ӧ�ĽǶ�(�ص�ǰԭ��)
    //���������ƻأ�-x,-y��
    //��ȡ�������ĵ�
    Gdiplus::Point *centerPoint = new Gdiplus::Point(rotateWidth / 2, rotateHeight / 2);
    //��graphics����ԭ���Ƶ����ĵ�
    graphics.TranslateTransform(centerPoint->X, centerPoint->Y);
    //graphics��ת��Ӧ�ĽǶ�(�Ƶ�ǰԭ��)
    graphics.RotateTransform(nAngle);
    //�ָ�graphics��ˮƽ�ʹ�ֱ�����ƽ��(�ص�ǰԭ��)
    graphics.TranslateTransform(-centerPoint->X, -centerPoint->Y);
    //��ʱ�Ѿ������graphics����ת 
    //����:���Ҫ��Դͼ�񻭵��������������뻭�������غϣ���Ҫ��ƫ����
    Gdiplus::Point *Offset = new Gdiplus::Point((rotateWidth - srcWidth) / 2, (rotateHeight - srcHeight) / 2);
    //��ԴͼƬ����rect�rotateRec�����ģ�
    //Gdiplus::TextureBrush brush(source);
    graphics.DrawImage(source,Offset->X,Offset->Y,srcWidth,srcHeight);
    //������ͼ�����б任
    graphics.ResetTransform();
    graphics.Save();          
    return pImageScale;
}

Gdiplus::Rect* CMainDlg::GetRotateRectangle(int width, int height, float angle)
{
    float radian = angle * 3.1415926 / 180;;
    float fcos = cos(radian);
    float fsin = sin(radian);
    //ֻ��Ҫ���ǵ��������޺͵������޵����ȡ��ֵ(�м��þ���ֵ�Ϳ��԰�����һ�͵ڶ�����)
    int newWidth = (int)(max(abs(width * fcos - height * fsin), abs(width * fcos + height * fsin)));
    int newHeight = (int)(max(abs(width * fsin - height * fcos), abs(width * fsin + height * fcos)));
    //������ת���ͼƬ��С
    return new Gdiplus::Rect(0, 0, newWidth, newHeight);
}
Gdiplus::Image* CMainDlg::FilpImageX(Gdiplus::Image* source)
{
    int width = source->GetWidth();
    int height = source->GetHeight();
    Gdiplus::Matrix matrix;
    Gdiplus::Graphics graphics(source);
    Gdiplus::Image *Filpimge = source->Clone();
    Filpimge->RotateFlip(Gdiplus::RotateNoneFlipX);
    return Filpimge;
}
Gdiplus::Image* CMainDlg::FilpImageY(Gdiplus::Image* source)
{
    int width = source->GetWidth();
    int height = source->GetHeight();
    Gdiplus::Matrix matrix;
    Gdiplus::Graphics graphics(source);
    Gdiplus::Image *Filpimge = source->Clone();
    Filpimge->RotateFlip(Gdiplus::RotateNoneFlipY);
    return Filpimge;
}
Gdiplus::Image* CMainDlg::CloneImage(Gdiplus::Image* source)
{
    //Bitmap bmp(L"wlh.bmp");
    int height = source->GetHeight();
    int width = source->GetWidth();
    //Bitmap�����ʵ�־ֲ���¡�������¡����Image��ֻ�������¡
    Gdiplus::Bitmap *scrBitTemp = new Gdiplus::Bitmap(width, height);
    Gdiplus::Image *scrBit = new Gdiplus::Bitmap(width, height);
    scrBitTemp = (Gdiplus::Bitmap*)source;
    Gdiplus::Rect rect[4];
    rect[0] = Gdiplus::Rect(0, 0, width / 2, height / 2);
    rect[1] = Gdiplus::Rect(width / 2, 0, width / 2, height / 2);
    rect[2] = Gdiplus::Rect(0, height / 2, width / 2, height / 2);
    rect[3] = Gdiplus::Rect(width / 2, height / 2, width / 2, height / 2);
    //��ԭͼ���Ϊ���Ŀ��¡  
    Gdiplus::Bitmap *scrBitmap[4];
    scrBitmap[0] = scrBitTemp->Clone(rect[0], PixelFormatDontCare);
    scrBitmap[1] = scrBitTemp->Clone(rect[1], PixelFormatDontCare);
    scrBitmap[2] = scrBitTemp->Clone(rect[2], PixelFormatDontCare);
    scrBitmap[3] = scrBitTemp->Clone(rect[3], PixelFormatDontCare);

    Gdiplus::Graphics graphics(scrBit);
    //��ͼ  
    graphics.DrawImage(scrBitmap[0], Gdiplus::Rect(0, 0, width / 2, height / 2));
    graphics.DrawImage(scrBitmap[1], Gdiplus::Rect(width / 2 + 10, 0, width / 2, height / 2));
    graphics.DrawImage(scrBitmap[2], Gdiplus::Rect(0, height / 2 + 10, width / 2, height / 2));
    graphics.DrawImage(scrBitmap[3], Gdiplus::Rect(width / 2 + 10, height / 2 + 10, width / 2, height / 2));
    graphics.ResetTransform();
    graphics.Save();

    //delete scrBitTemp;
    //scrBitTemp = nullptr;
    return scrBit;
}
Gdiplus::Image* CMainDlg::CastImage(Gdiplus::Image* source)
{
    int left = 0;
    int top = 700;
    int height = source->GetHeight();
    int width = source->GetWidth();
    Gdiplus::Image *scrImage = new Gdiplus::Bitmap(width*2, height);
    Gdiplus::Graphics graphics(scrImage);
    //
    //graphics.Clear(Gdiplus::Color::Green);
    //���ģʽ
    graphics.SetInterpolationMode(Gdiplus::InterpolationModeHighQualityBicubic); 
    //����  
    Gdiplus::Point desFace[] = {
        Gdiplus::Point(left, top),
        Gdiplus::Point(left + width, top),
        Gdiplus::Point(left, top + width),
    };
    graphics.DrawImage(source, desFace, 3);
    //����Ͷ��  
    Gdiplus::Point destTop[] = {
        Gdiplus::Point(left + width / 2, top - width / 2),
        Gdiplus::Point(left + width / 2 + width, top - width / 2),
        Gdiplus::Point(left, top),
    };
    graphics.DrawImage(source, destTop, 3);
    //����Ͷ��  
    Gdiplus::Point desRight[] = {
        Gdiplus::Point(left + width, top),
        Gdiplus::Point(left + width / 2 + width, top - width / 2),
        Gdiplus::Point(left + width, top + width)
    };
    graphics.DrawImage(source, desRight, 3);

    return scrImage;
}







