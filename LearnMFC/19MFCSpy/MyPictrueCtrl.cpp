#include "pch.h"
#include "framework.h"
#include "MyPictrueCtrl.h"
BEGIN_MESSAGE_MAP(CMyPictrueCtrl, CStatic)
    ON_WM_LBUTTONDOWN()
    ON_WM_LBUTTONUP()
    ON_WM_MOUSEMOVE()
END_MESSAGE_MAP()


void CMyPictrueCtrl::OnLButtonDown(UINT nFlags, CPoint point)
{
    //���ͼƬ
    HICON hIcon = ::LoadIcon(::GetModuleHandle(NULL), MAKEINTRESOURCE(IDI_ICON2));
    HDC hDc = ::GetDC(this->m_hWnd);
    ::DrawIcon(hDc, 0, 0, hIcon);
    ::ReleaseDC(this->m_hWnd, hDc);
    //������
    HCURSOR hCursor = ::LoadCursor(::GetModuleHandle(NULL), MAKEINTRESOURCE(IDC_CURSOR1));
    ::SetCursor(hCursor);
    m_bOnDraw = TRUE;
    //���ò������
    ::SetCapture(this->m_hWnd);

    CStatic::OnLButtonDown(nFlags, point);
}


void CMyPictrueCtrl::OnLButtonUp(UINT nFlags, CPoint point)
{
    if (m_bOnDraw)
    {
        //��ԭͼƬ
        HICON hIcon = ::LoadIcon(GetModuleHandle(NULL), MAKEINTRESOURCE(IDI_ICON1));
        HDC hDc = ::GetDC(this->m_hWnd);
        ::DrawIcon(hDc, 0, 0, hIcon);
        ::ReleaseDC(this->m_hWnd, hDc);
        //��ԭ�ɴ��ڱ߿�
        DrawWnd(m_hOldWnd);
        m_hOldWnd = NULL;
        //��ԭ���
        m_bOnDraw = FALSE;
        //�ͷ���겶��
        ReleaseCapture();
    }
    CStatic::OnLButtonUp(nFlags, point);
}


void CMyPictrueCtrl::OnMouseMove(UINT nFlags, CPoint point)
{
    if (m_bOnDraw)
    {
        //���ù��
        //HCURSOR hCursor = ::LoadCursor(::GetModuleHandle(NULL), MAKEINTRESOURCE(IDC_CURSOR1));
        //::SetCursor(hCursor);

        //��ȡ������ڵĴ���
        POINT ptOfScreen = POINT{ point.x, point.y };
        ::ClientToScreen(this->m_hWnd, &ptOfScreen);
        HWND hWndOfPt = ::WindowFromPoint(ptOfScreen);
        //����׽Spy++�������Ĵ���
        HWND hWndSpyFind = ::FindWindow(NULL, _T("19MFCSpy++"));
        CRect rcSpyFindDlg;
        ::GetWindowRect(hWndSpyFind, &rcSpyFindDlg);
        HWND hWndpchunterDlg = AfxGetMainWnd()->GetSafeHwnd();
        if (hWndOfPt == this->GetSafeHwnd() ||
             hWndOfPt == hWndpchunterDlg ||
                rcSpyFindDlg.PtInRect(ptOfScreen))
        {
            return;
        }
        //���Ʊ߿�
        if (m_hOldWnd != hWndOfPt)
        {
            DrawWnd(m_hOldWnd);   //��ԭ�ɴ��ڱ߿�
            DrawWnd(hWndOfPt);    //�����´��ڱ߿�
            //�����Ϣ
            TCHAR szWndClass[MAXBYTE] = { 0 };
            TCHAR szWndText[MAXBYTE] = { 0 };
            ::GetClassName(hWndOfPt, szWndClass, sizeof(szWndClass) / sizeof(TCHAR));
            ::GetWindowText(hWndOfPt, szWndText, sizeof(szWndText) / sizeof(TCHAR));
            CString csHwnd; 
            csHwnd.Format(_T("%08X"), (DWORD)hWndOfPt);
            ::SetDlgItemText(hWndSpyFind, IDC_EDIT_SPYFIND_HANDLE, csHwnd.GetBuffer());
            ::SetDlgItemText(hWndSpyFind, IDC_EDIT_SPYFIND_TITLE, szWndText);
            ::SetDlgItemText(hWndSpyFind, IDC_EDIT_SPYFIND_CLASS, szWndClass);
            InvalidateRect(NULL, FALSE);
            m_hOldWnd = hWndOfPt;
        }
    }

    CStatic::OnMouseMove(nFlags, point);
}


void CMyPictrueCtrl::DrawWnd(HWND hWnd)
{
    if (hWnd == NULL)
    {
        return;
    }
    //���ƾ��Σ������棬����һ�����ο�
    HWND hWndDesk = ::GetDesktopWindow();
    HDC hDcDesk = ::GetDC(NULL);//ע�⣺���ﴫNULL�����õ�������Ļ��DC

    RECT rc;
    ::GetWindowRect(hWnd, &rc);

    HPEN hPen = ::CreatePen(PS_SOLID, 5, RGB(255, 255, 255));
    ::SelectObject(hDcDesk, hPen);
    ::SelectObject(hDcDesk, GetStockObject(NULL_BRUSH));
    ::SetROP2(hDcDesk, R2_XORPEN);//�����ʵ���ɫ��һ��ֵ�����
    ::Rectangle(hDcDesk, rc.left, rc.top, rc.right, rc.bottom);

    ::ReleaseDC(hWndDesk, hDcDesk);
}