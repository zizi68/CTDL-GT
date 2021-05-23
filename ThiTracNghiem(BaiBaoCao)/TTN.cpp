#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <string>
#include "mylib.h"
#include <windows.h>
#include <time.h>
#include <fstream>
#include <iomanip>

#define MAX 100

//Ve dau mui ten:
#define ArrowRIGHT 27
#define ArrowLEFT 26
#define ArrowUP 25
#define ArrowDOWN 24

//Cac phim ban phim:
#define DOWN 80
#define UP 72
#define LEFT 75
#define RIGHT 77
#define ENTER 13
#define ESC 27
#define SPACE 32
#define BACKSPACE 8
//#define DELETE 83
#define INSERT 82
#define END 79

#define LH -1 //Cay con trai cao hon.
#define EH 0 //Hai cay con bang nhau.
#define RH 1 //Cay con phai cao hon.
using namespace std;

//Danh sach mon hoc.
struct MONHOC
{
	string MAMH;
	string TENMH;
};
struct LISTMONHOC
{ //Mang con tro.
	int n;
	MONHOC *nodes[MAX];
};

//Danh sach cac cau hoi sinh vien da thi
struct dethi
{
	int n;
	int nodes[MAX];
};
typedef struct dethi DETHI;

//Danh sach diem
struct Diemthi
{
	string MAMH;
	DETHI DeThi;
	float DIEM;
};
struct NodeDiem
{
	Diemthi DataDiem;
	NodeDiem *pNext;
};
typedef struct NodeDiem* Listdiemthi;

//Danh sach sinh vien
struct Sv
{
	string MSSV;
	string ho;
	string ten;
	string gioitinh;
	string password;
	Listdiemthi FirstDiem;
};
// Node sinh vien
struct DSSV
{
	Sv sv;
	DSSV *pNext;
};
typedef struct DSSV *LISTSV;

//Danh sach lop
struct LopHoc
{
	string MaLop;
	string TenLop;
	LISTSV FirstSV;
};
struct DsLop
{
	int n;
	LopHoc lh[MAX];
};

//Danh sach cau hoi
struct CAUHOI
{
	int id; //Key
	string NOIDUNG, CAUA, CAUB, CAUC, CAUD;
	string MAMH;
	char DAPAN;
};
struct NODECH
{
	CAUHOI info;
	int ID, bf;
	struct NODECH *left;
	struct NODECH *right;
};
typedef struct NODECH *TREECH;

void resizeConsole(int width, int height)
{
	HWND console = GetConsoleWindow();
	RECT r;
	GetWindowRect(console, &r);
	MoveWindow(console, r.left, r.top, width, height, TRUE);
}
void ShowCur(bool CursorVisibility)
{
	HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_CURSOR_INFO cursor = { 1, CursorVisibility };
	SetConsoleCursorInfo(handle, &cursor);
}

//In ra ky tu hoa va so
string ChuanHoaNhap1()
{
	string s = "";
	char c;
	do
	{
		c = _getch();
		if (c == 27) //Esc
		{
			return "\0";
		}
		if (c >= 'a' && c <= 'z')
		{
			c = c - 32;
		}
		if (c >= 'A' && c <= 'Z' || c >= '0' && c <= '9' || c == ' ')
		{
			cout << c;
			s += c;
		}
		else if (c == 8) //Backspace
		{
			if (s.length() > 0)
			{
				cout << "\b \b";
				s.replace(s.length() - 1, 1, "");
			}
		}
	}while (c != 13); //Chuyen dong, xuong dong
	return s;
}

int kiemTraChuoi(string &ma)
{
	int length = ma.length();
	for (int i = 0; i < length; i++)
	{ //Xoa khoang trang dau chuoi
		if (ma[0] == ' ')
		{
			ma = ma.substr(1); //Lay tu vi tri 1 den cuoi chuoi
		}
		else
		{
			break;
		}
	}
	length = ma.length();
	for (int i = length - 1; i > 0; i--)
	{ //Xoa khoang trang o cuoi chuoi
		if (ma[ma.length() - 1] == ' ')
		{
			ma.erase(ma.length() - 1);
		}
		else
		{
			break;
		}
	}
	if (ma == "")
	{
		return 1; //1 la chuoi rong
	}
	return 0; //0 la chuoi binh thuong
}

// Ham nhap password
string ChuanHoaPass(string &s, int x, int y)
{
	gotoxy(x, y);
	char c;
	do
	{
		c = _getch();
		if (c == 27)
		{
			return "\0"; //NULL
		}
		if (c >= 'a' && c <= 'z' || c >= 'A' && c <= 'Z' || c >= '0' && c <= '9')
		{
			cout << "*";
			s += c;
		}
		else if (c == 8)
		{
			if (s.length() > 0)
			{
				cout << "\b";
				s.replace(s.length() - 1, 1, "");
			}
		}
	}while (c != 13);
	return s;
}

LISTSV TimKiemSV_DSSV(LISTSV &First, string MaSV)
{
	for (LISTSV p = First; p != NULL; p = p->pNext)
	{
		if (p->sv.MSSV == MaSV)
		{
			return p;
	    }
	}
	return NULL;
}

LISTSV TimKiemSV_DSLOP(DsLop &dslop, string mmasv)
{
	LISTSV p;
	for (int i = 0 ;i < dslop.n; i++)
	{
		for (p = dslop.lh[i].FirstSV; p != NULL; p = p->pNext)
		{
			if (p->sv.MSSV == mmasv)
			{
				return p;
			}
		}
	}
	return NULL;
}

int TimKiemLopCuaSV(DsLop &dslop, string mmasv)
{
	LISTSV p;
	for (int i = 0; i < dslop.n; i++)
	{
		for (p = dslop.lh[i].FirstSV; p != NULL; p = p->pNext)
		{
			if (p->sv.MSSV == mmasv)
			{
				return i;
			}
		}
	}
	return 0;
}

int KiemTraMaSV_School(DsLop &dslop, string masv)
{
	LISTSV p;
	for (int i = 0; i < dslop.n; i++)
	{
		for (p = dslop.lh[i].FirstSV; p != NULL; p = p->pNext)
		{
			if (p->sv.MSSV == masv)
			{
				return 1; //1 la trung
			}
		}
	}
	return 0; //0 la khong trung
}

// Tim kiem lop
int TimKiemMaLop(DsLop dslop, string mmalop)
{
	for (int i = 0; i < dslop.n; i++)
	{
		if (dslop.lh[i].MaLop == mmalop) 
		{
			return i;
	    }
	}
	return -1;
}

void Insert_Last(LISTSV &First, Sv sv)
{
	LISTSV p = new DSSV;
	p->sv = sv; p->pNext = NULL;
	if (First == NULL)
	{
		First = p;
	}
	else
	{
		LISTSV Last ;
		for (Last = First; Last->pNext != NULL; Last = Last->pNext);
		Last->pNext = p;
	}
}

////////////////LOP HOC////////////////
void designNhapLop()
{
	gotoxy(1, 5);
	SetBGColor(11);
	for (int i = 0; i < 8; i++)
	{
		cout << " ";
	}
	gotoxy(1,6);
	SetBGColor(11);
	for (int i = 0; i < 8; i++)
	{
		cout << " ";
	}
	gotoxy(1,7);
	SetBGColor(11);
	for (int i = 0; i < 8; i++)
	{
		cout << " ";
	}
	gotoxy(1, 10);
	SetBGColor(11);
	for (int i = 0; i < 8; i++)
	{
		cout << " ";
	}
	gotoxy(1, 11);
	SetBGColor(11);
	for (int i = 0; i < 8; i++)
	{
		cout << " ";
	}
	gotoxy(1, 12);
	SetBGColor(11);
	for(int i = 0; i < 8; i++)
	{
		cout << " ";
	}
	gotoxy(15, 1);
	SetBGColor(0);
	SetColor(10);
	cout << "NHAP LOP";
	gotoxy(2, 11);
	SetBGColor(11);
	SetColor(5);
	cout << "TEN LOP";
	gotoxy(2, 6);
	cout << "MA LOP";
}

bool isEmpty(DsLop dslop)
{
	return (dslop.n == 0);
}

void createLOP(DsLop &dslop)
{
	dslop.n = 0;
}

void NhapLop(DsLop &dslop) //Tao 1 lop
{
	int a;
	LopHoc lh;
	designNhapLop();
	if (dslop.n == MAX)
	{
		gotoxy(0, 15);
		ShowCur(0);
		cout << "DANH SACH LOP DA DAY!";
		getch();
	}
	while (dslop.n < MAX)
	{
		NHAPMALOP:
		designNhapLop();
		fflush(stdin);
		gotoxy(12, 6);
		SetBGColor(0);
		SetColor(15);
		ShowCur(1);
		getline(cin, lh.MaLop);
		if (kiemTraChuoi(lh.MaLop) == 1) //Chuoi rong
		{
			gotoxy(0, 20);
			ShowCur(1);
			cout << "Ban co muon thoat (Y/N): ";
			a = getch();
			if (a == 121)
			{
				return; //Phim y
			}
			else
			{
				system("cls");
				goto NHAPMALOP;
			}
		}
		if (TimKiemMaLop(dslop,lh.MaLop) >= 0) //Ma lop da ton tai
		{
			gotoxy(0, 20);
			ShowCur(0);
			cout << "Ma lop bi trung";
			Sleep(1000);
			system("cls");
			goto NHAPMALOP;
		}
		gotoxy(12, 11);
		ShowCur(1);
		getline(cin, lh.TenLop);
		do
		{
			if (kiemTraChuoi(lh.TenLop) == 1) //Chuoi rong
			{
				gotoxy(30, 11);
				ShowCur(0);
				cout << "Xin nhap ten lop";
				Sleep(1000);
				gotoxy(30, 11);
				cout << "                ";
				gotoxy(wherex() - 33, wherey());
				ShowCur(1);
				getline(cin, lh.TenLop);
			}
		}while (kiemTraChuoi(lh.TenLop) == 1);
		lh.FirstSV = NULL;
		dslop.lh[dslop.n++] = lh;
		gotoxy(12, 25);
		ShowCur(0);
		cout << "Them lop thanh cong";
		Sleep(1000);
		system("cls");
	}
}

int LuuFileDSLOP(DsLop &dslop);

void designXuatLop()
{
	gotoxy(3, 1);
	SetBGColor(11);
	for (int i = 0;i < 90; i++)
	{
		cout << " ";
	}
	for (int i = 1; i < 20; i ++) 
	{
		gotoxy(47, i);
		cout << " ";
	}
	SetBGColor(0);		
	gotoxy(20, 2);
	SetColor(5);
	cout << "MA LOP";
	gotoxy(70, 2);
	cout << "TEN LOP";
}
int XuatLop(DsLop dslop)
{
	ShowCur(0);
	LISTSV dssv;
	system("cls");
	designXuatLop();
	if (isEmpty(dslop))
	{
		gotoxy(0, 20);
		cout << "Khong co lop hoc nao!";
		Sleep(1000);
		system("cls");
	    return 2;
	}
	int j = 4;
	for (int i = 0; i < dslop.n; i++)
	{
		gotoxy(20, j);
		SetColor(10);
		cout << dslop.lh[i].MaLop;
		gotoxy(68, j);
		cout << dslop.lh[i].TenLop;
		j++;
	}
	gotoxy(0, 22);
	int a = getch();
	if (a == ENTER)
	{
		return 1;
	}
	system("cls");
}
void designNhapSV()
{
	gotoxy(20, 1);
	SetColor(14);
	cout << "NHAP SINH VIEN";
	gotoxy(18, 3);
	SetBGColor(11);
	for(int i = 0; i < 8; i++)
	{
		cout << " ";
	}
	gotoxy(18, 4);
	SetBGColor(11);
	for (int i = 0; i < 8; i++)
	{
		cout << " ";
	}
	gotoxy(18, 5);
	SetBGColor(11);
	for (int i = 0; i < 8; i++)
	{
		cout << " ";
	}
	gotoxy(19, 4);
	SetColor(29);
	ShowCur(1);
	cout << "MA LOP:";
	gotoxy(7, 8);
	SetBGColor(0);
	SetColor(15);
	cout << "MSSV: ";
	gotoxy(7, 10);
	cout << "Ho: ";
	gotoxy(7, 12);
	cout << "Ten: ";
	gotoxy(7, 14);
	cout << "Gioi tinh (Nam/Nu): ";
	gotoxy(7, 16);
	cout << "Password: ";
}

int LuuFileDSLOP(DsLop &dsLop); //Khai bao

void DocFileDSLOP(DsLop& dslop);

int NhapSV(DsLop &dsLop)
{ //Nhap sinh vien vao lop co san
	LISTSV p;
	int mmasv, viTri;
	string ho = "", ten = "", gioitinh = "", mssv = "", password = "";
	DocFileDSLOP(dsLop);
	NHAPMALOP:
	designNhapSV();
	string maLop;
    do
	{ //Kiem tra xem ma lop co rong khong
    	fflush(stdin);
		gotoxy(27, 4);
		SetBGColor(0);
		SetColor(15);
		ShowCur(1);
		getline(cin, maLop);
		if (kiemTraChuoi(maLop) == 1) //Chuoi rong
		{
			gotoxy(0, 20);
			ShowCur(1);
			cout << "Ban co muon thoat (y/n): ";
			int a = getch();
			if (a == 121)
			{
				return 2;
			}
			else
			{
				system("cls");
				goto NHAPMALOP;
			}
		}
	}while (kiemTraChuoi(maLop) == 1);
	viTri = TimKiemMaLop(dsLop,maLop); //Kiem tra xem ma lop co ton tai khong
	if (viTri == -1)
	{
		gotoxy(0, 20);
		ShowCur(0);
		cout << "Ma lop ban nhap khong co trong danh sach.";
		Sleep(1500);
		system("cls");
		goto NHAPMALOP;
	}
	while(1)
	{
		NHAPMSSV:
		designNhapSV();
		fflush(stdin);
		gotoxy(13, 8);
		ShowCur(1);
		getline(cin, mssv);
		if (kiemTraChuoi(mssv) == 1) //Chuoi rong khong
		{
			gotoxy(0, 18);
			ShowCur(1);
			cout << "Ban co muon ket thuc (y/n): ";
			int a = getch();
			if (a == 121)
			{
				return 2;
			}
			else
			{
				gotoxy(0, 18);
				ShowCur(0);
				cout << "                           ";
				goto NHAPMSSV;
			}
		}
		mmasv = KiemTraMaSV_School(dsLop,mssv); //Kiem tra mssv cua tat ca cac lop
		if (mmasv == 1)
		{
			gotoxy(0, 20);
			ShowCur(0);
			cout << "Ma so sinh vien bi trung! Nhap lai.";
			Sleep(1000);
			gotoxy(0, 20);
			cout << "                                    ";
			gotoxy(13, 8);
			cout << "                                    ";
			goto NHAPMSSV;
		}
		do
		{
			gotoxy(11, 10);
			fflush(stdin);
			ShowCur(1);
			getline(cin, ho);
			if (kiemTraChuoi(ho) == 1)
			{
				gotoxy(0, 20);
				ShowCur(0);
				cout << "Vui long nhap ho";
				Sleep(1000);
				gotoxy(0, 20);
				cout << "                ";
			}
		}while (kiemTraChuoi(ho) == 1);
		do
		{
			gotoxy(12, 12);
			fflush(stdin);
			ShowCur(1);
			getline(cin, ten);
			if (kiemTraChuoi(ten) == 1)
			{
				gotoxy(0, 20);
				ShowCur(0);
				cout << "Vui long nhap ten";
				Sleep(1000);
				gotoxy(0, 20);
				cout << "                ";
			}
		}while (kiemTraChuoi(ten) == 1);
		do
		{
			fflush(stdin);
			gotoxy(27, 14);
			ShowCur(1);
			getline(cin, gioitinh);
			if (kiemTraChuoi(gioitinh) == 1)
			{
				gotoxy(0, 20);
				ShowCur(0);
				cout << "Vui long nhap gioi tinh!";
				Sleep(1000);
				gotoxy(0, 20);
				cout << "                       ";
			}
			if (gioitinh != "Nam" && gioitinh != "Nu")
			{
				gotoxy(0, 20);
				ShowCur(0);
				cout << "Gioi tinh phai la Nam/Nu";
				gotoxy(0, 21);
				cout << "Vui long nhap chinh xac!";
				Sleep(1500);
				gotoxy(0, 20);
				cout << "                         ";
				gotoxy(0, 21);
				cout << "                         ";
				gotoxy(26, 14);
				ShowCur(1);
				cout << "                         ";
			}
		}while (kiemTraChuoi(gioitinh) == 1 || gioitinh != "Nam" && gioitinh != "Nu");
		do
		{
			gotoxy(17, 16);
			fflush(stdin);
			ShowCur(1);
			getline(cin, password);
			if (kiemTraChuoi(password) == 1)
			{
				gotoxy(0, 20);
				ShowCur(0);
				cout << "Vui long nhap password!";
				Sleep(1000);
				gotoxy(0, 20);
				cout << "                      ";
			}
		}while (kiemTraChuoi(password) == 1);
		Sv *sv = new Sv;
		sv->MSSV = mssv;
		sv->ho = ho;
		sv->ten = ten;
		sv->gioitinh = gioitinh;
		sv->password = password;
		sv->FirstDiem = NULL;
		Insert_Last(dsLop.lh[viTri].FirstSV, *sv);
		LuuFileDSLOP(dsLop);
		gotoxy(0, 20);
		ShowCur(0);
		cout << "Da them sinh vien!";
		Sleep(1000);
		gotoxy(13, 8);
		cout << "                      ";
		gotoxy(11, 10);
		cout << "                      ";
		gotoxy(12, 12);
		cout << "                      ";
		gotoxy(26, 14);
		cout << "                      ";
		gotoxy(17, 16);
		cout << "                      ";
		gotoxy(0, 20);
		cout << "                      ";
	}
}

int LuuFileDSLOP(DsLop &dslop);

void designDSSV_OF_LOP()
{
	gotoxy(3, 5);
	SetBGColor(11);
	for (int i = 0; i < 90; i++)
	{
		cout << " ";
	}
	for (int i = 5; i < 25; i++)
	{
		gotoxy(24, i);
		cout << " ";
	}
	for (int i = 5; i < 25; i++)
	{
		gotoxy(45, i);
		cout << " ";
	}
	for (int i = 5; i < 25; i++)
	{
		gotoxy(65, i);
		cout << " ";
	}
	for (int i = 5; i < 25; i++)
	{
		gotoxy(80, i);
		cout << " ";
	}
	gotoxy(20, 1);
	SetColor(10);
	SetBGColor(0);
	cout << "CHUC NANG XUAT SINH VIEN CUA 1 LOP";
	gotoxy(30, 3);
	SetColor(5);
	cout << "MA LOP: ";
	gotoxy(9, 6);
	SetColor(14);
	cout << "MSSV";
	gotoxy(34, 6);
	cout << "HO";
	gotoxy(54, 6);
	cout << "TEN";
	gotoxy(69, 6);
	cout << "GIOI TINH";
	gotoxy(83, 6);
	cout << "PASSWORD";
}

int XuatDSSV_OF_LOP(DsLop dslop)
{
	LISTSV First;
	int j, a, b, viTri;
	system("cls");
	designDSSV_OF_LOP();
	string malop;
	LISTSV p;
	do
	{
		gotoxy(38, 3);
		ShowCur(1);
		getline(cin, malop);
		SetColor(15);
		viTri = TimKiemMaLop(dslop, malop);
		if (kiemTraChuoi(malop) == 1)
		{
			gotoxy(0, 30);
			ShowCur(1);
			cout << "Ban co muon thoat (y/n): ";
			a = getch();
			if (a == 121)
			{
				return 2;
			}
			else
			{
				gotoxy(38, 3);
				ShowCur(0);
				cout << "                         ";
			}
		}
		if (viTri == -1)
		{
			gotoxy(0, 30);
			ShowCur(0);
			cout << "MA LOP nay khong ton tai.";
			gotoxy(38, 3);
			cout << "              ";
			Sleep(1000);
		}
		else
		{
			ShowCur(0);
			j = 7;
			for (p = dslop.lh[viTri].FirstSV; p != NULL; p = p->pNext)
			{
				gotoxy(7, j);
				cout << p->sv.MSSV << endl;
				gotoxy(25, j);
				cout << p->sv.ho << endl;
				gotoxy(54, j);
				cout << p->sv.ten << endl;
				gotoxy(72, j);
				cout << p->sv.gioitinh << endl;
				gotoxy(83, j);
				cout << p->sv.password << endl;
				j++;
			}
		}
	}while (TimKiemMaLop(dslop,malop) == -1);
	b = getch();
	if (b == ENTER)
	{
		return 2;
	}
	system("cls");
}

void Insert_LastDiem(Listdiemthi &First, Diemthi dt)
{
	Listdiemthi p = new NodeDiem;
	p->DataDiem = dt;
	p->pNext = NULL;
	if (First == NULL)
	{
		First = p;
	}
	else
	{
		Listdiemthi Last;
		for (Last = First; Last->pNext != NULL; Last = Last->pNext);
		Last->pNext = p;
	}
}

////////////MON HOC////////////
bool isEmpty(LISTMONHOC plist)
{
	return(plist.n == 0);
}

bool isFull(LISTMONHOC plist)
{
	return(plist.n == MAX);
}

void create(LISTMONHOC & plist)
{
	plist.n = 0;
}

bool kiemTraMAMH(string mamh, LISTMONHOC plist, int j)
{ //Ham kiem tra xem mamh co bi trung trong danh sach, da co san hay chua.
	for (int i = 0; i < j; i++)
	{ //Bien j la so phan tu da dung
		if (mamh == plist.nodes[i]->MAMH)
		{
			return false;
		}
	}
	return true;
}

int kiemTraMAMH(string mamh, LISTMONHOC plist)
{ //Kiem tra, neu ma mon hoc da ton tai tra ve vi tri, khong thi tra ve -1
	for (int i = 0; i < plist.n; i++)
	{
		if (mamh == plist.nodes[i]->MAMH)
		{
			return i;
		}
	}
	return -1;
}

int nhapMONHOC(MONHOC &mh)
{
	int a;
	do
	{ //Bat loi truong hop nhap vao la chuoi rong.
		fflush(stdin);
		gotoxy(14, 6);
		SetBGColor(0);
		SetColor(15);
		ShowCur(1);
		getline(cin, mh.MAMH);
		if (kiemTraChuoi(mh.MAMH) == 1)
		{
			gotoxy(0, 20);
			ShowCur(0);
			cout << "Ban co muon thoat (y/n) " << endl;
			a = getch();
			if (a == 121)
			{
				return -1;
			}
			else
			{
				gotoxy(0, 20);
				ShowCur(0);
				cout << "                                           ";
			}
		}
	}while (kiemTraChuoi(mh.MAMH) == 1);
	do
	{
		fflush(stdin);
		gotoxy(14, 11);
		ShowCur(1);
		getline(cin, mh.TENMH);
		if (mh.TENMH == "" )
		{
			gotoxy(0, 20);
			ShowCur(0);
			cout << "TENMH khong duoc de trong. Moi ban nhap lai." << endl;
			Sleep(1000);
			gotoxy(0, 20);
			cout << "                                            " << endl;
			gotoxy(14, 11);
			getline(cin, mh.TENMH);
		}
	}while (mh.TENMH == "");
	return 0;
}

void xuatMONHOC(MONHOC mh)
{
	cout << "MAMH: " << mh.MAMH << endl;
	cout << "TENMH: " << mh.TENMH << endl;
}

string themMONHOC(MONHOC temp, LISTMONHOC &plist)
{
	if (isFull(plist))
	{
		gotoxy(0, 14);
		return "Danh sach day roi. Khong them duoc nua.\n";
	}
	if (kiemTraMAMH(temp.MAMH, plist,plist.n) == false)
	{
		gotoxy(0, 15);
		return "MAMH nay da bi trung. Khong them duoc.\n";
	}
	plist.nodes[plist.n] = new MONHOC; //Khoi tao vung nho cho con tro
	*plist.nodes[plist.n] = temp; //Them mon hoc vao vung nho cua con tro, sau khi them vao thi tang so luong phan tu cua danh sach len 1
	plist.n += 1;
	gotoxy(0, 18);
	return "Them mon hoc thanh cong.\n";
	Sleep(1500);
}

void nhapLISTMONHOC(LISTMONHOC &plist)
{ //Phuong thuc nhap danh sach mon hoc.
	gotoxy(1, 5);
	SetBGColor(11);
	for (int i = 0; i < 12; i++)
	{
		cout << " ";
	}
	gotoxy(1, 6);
	SetBGColor(11);
	for (int i = 0; i < 12; i++)
	{
		cout << " ";
	}
	gotoxy(1, 7);
	SetBGColor(11);
	for (int i = 0; i < 12; i++)
	{
		cout << " ";
	}
	gotoxy(1, 10);
	SetBGColor(11);
	for (int i = 0; i < 12; i++)
	{
		cout << " ";
	}
	gotoxy(1, 11);
	SetBGColor(11);
	for (int i = 0; i < 12; i++)
	{
		cout << " ";
	}
	gotoxy(1, 12);
	SetBGColor(11);
	for (int i = 0; i < 12; i++)
	{
		cout << " ";
	}
	gotoxy(15, 1);
	SetBGColor(0);
	SetColor(10);
	ShowCur(0);
	cout << "CHUC NANG NHAP MON HOC";
	gotoxy(2, 6);
	SetBGColor(11);
	SetColor(5);
	cout << "MA MON HOC";
	gotoxy(2, 11);
	cout << "TEN MON HOC";
	int i;
	NHAPMH:
	int n;
	MONHOC mh;
	n = nhapMONHOC(mh);
	if (n != -1)
	{
		ShowCur(0);
		cout << themMONHOC(mh, plist) << endl;
		Sleep(1500);
	}
}

void designXuatMH()
{
	gotoxy(5, 2);
	SetBGColor(11);
	for (int i = 0; i < 60; i++)
	{
		cout << " ";
	}
	for (int i = 1; i < 30; i++)
	{
		gotoxy(35, i);
		cout << " ";
	}
	SetBGColor(0);
	gotoxy(20, 1);
	cout << "     CHUC NANG XUAT DANH SACH MON HOC\n\n";
	gotoxy(15, 3);
	SetColor(5);
	cout << "MA MON HOC";
	gotoxy(50, 3);
	cout << "TEN MON HOC ";
}

void xuatLISTMONHOC(LISTMONHOC plist)
{
	ShowCur(0);
	int j;
	system("cls");
	designXuatMH();
	if (isEmpty(plist))
	{
		gotoxy(0, 15);
		cout << "Khong co mon hoc nao ca.\n";
		Sleep(1500);
		return;
	}	
	int k = 4;
	for (int i = 0; i < plist.n; i++)
	{
		gotoxy(17, k);
		SetColor(15);
		cout << plist.nodes[i]->MAMH << endl;
		gotoxy(45, k);
		cout << plist.nodes[i]->TENMH << endl;
		k++;
	}	
	j = getch();
	if (j == ENTER)
	{
		return;
	}
}	

int writefileLISTMONHOC(LISTMONHOC plist)
{
	ofstream fo("Dsmonhoc.txt", ios::out);
    if (!fo)
	{
    	return -1;
    }
	fo << plist.n << "\n";
	for (int i = 0; i < plist.n; i++)
	{			  
    	fo << plist.nodes[i]->MAMH << "\n";
    	fo << plist.nodes[i]->TENMH <<"\n";
	}
    fo.close();
	return 0;
}

int readfileLISTMONHOC(LISTMONHOC &plist)
{ 
	int n;
	string temp;
	ifstream fi;
	fi.open("Dsmonhoc.txt", ios::in);
    if (!fi)
	{          
    	return -1;
    }
	fi >> n;
	getline(fi, temp);
	for (int i = 0; i < n; i++)
	{	
		MONHOC mh;
		getline(fi, mh.MAMH);
		getline(fi, mh.TENMH);
		themMONHOC(mh, plist);
	}
	fi.close();
	return 1;
}

void designSuaMonHoc()
{
	gotoxy(20, 1);
	cout << "SUA THONG TIN MON HOC";
	gotoxy(65, 25);
	SetBGColor(0);
	SetColor(4);
	cout << "Enter o Ma Mon Hoc Moi de thoat";
	gotoxy(7, 3);
	SetColor(3);
	cout << "THONG TIN HIEN TAI";
	gotoxy(10, 5);
	SetColor(5);
	cout << "MA MON HOC: ";
	gotoxy(10, 7);
	cout << "TEN MON HOC: ";
	gotoxy(37, 3);
	SetColor(3);
	cout << "CHINH SUA";
	gotoxy(38, 5);
	SetColor(5);
	cout << "MA MON HOC MOI: ";
	gotoxy(38, 7);
	cout << "TEN MON HOC MOI: ";
}

int Sua_MonHoc(LISTMONHOC &dsmh)
{
	string mamonhoc = "";
	string mamonhocmoi = "";
	string tenmonhocmoi = "";
	int viTri;
	fflush(stdin);
	designSuaMonHoc();
	while (true)
	{
		gotoxy(22, 5);
		SetColor(15);
		ShowCur(1);
		getline(cin, mamonhoc);
		viTri = kiemTraMAMH(mamonhoc, dsmh);
		if (viTri < 0)
		{
			gotoxy(0, 20);
			ShowCur(0);
			cout << "Ma Mon Hoc nay khong co!";
			Sleep(1000);
			system("cls");
			designSuaMonHoc();
		}
		else
		{
			gotoxy(22, 5);
			ShowCur(0);
			cout << dsmh.nodes[viTri]->MAMH;
			gotoxy(23, 7);
			cout << dsmh.nodes[viTri]->TENMH;
			break;
		}
	}
	gotoxy(54, 5);
	ShowCur(1);
	getline(cin, mamonhocmoi);
	while (kiemTraChuoi(mamonhocmoi) == 1)
	{
		gotoxy(0, 20);
		ShowCur(0);
		cout << "Ban co muon thoat (y/n)";
		int a = getch();
		if (a == 121)
		{
			return 2;
		}
		else
		{
			continue;
		}
	}
	while (kiemTraMAMH(mamonhocmoi, dsmh) >= 0)
	{
		gotoxy(0, 20);
		ShowCur(0);
		cout << "Ma Mon Hoc bi trung!";
		Sleep(1500);
		gotoxy(0, 20);
		cout << "                    ";
		gotoxy(54, 5);
		cout << "                     ";
		gotoxy(54, 5);
		ShowCur(1);
		getline(cin, mamonhocmoi);
	}
	dsmh.nodes[viTri]->MAMH = mamonhocmoi;
	gotoxy(55, 7);
	ShowCur(1);
	getline(cin, tenmonhocmoi);
	while (kiemTraChuoi(tenmonhocmoi) == 1)
	{
		gotoxy(0, 20);
		ShowCur(0);
		cout << "Xin nhap Ten Mon Hoc";
		Sleep(1500);
		gotoxy(0, 20);
		cout << "                ";
		gotoxy(55, 7);
		ShowCur(1);
		getline(cin, tenmonhocmoi);
	}
	dsmh.nodes[viTri]->TENMH = tenmonhocmoi;
	gotoxy(0, 20);
	cout << "Doi thong tin thanh cong";
	Sleep(1500);
	system("cls");
}
int writefileLISTMONHOC(LISTMONHOC plist);
void designXoaMonHoc()
{
	gotoxy(20, 1);
	SetColor(4);
	cout << "XOA MON HOC";
	gotoxy(15, 5);
	SetColor(5);
	cout << "MA MON HOC: ";
	gotoxy(15, 7);
	SetColor(5);
	cout << "TEN MON HOC: ";
}
int XoaMonHoc(LISTMONHOC &dsmh)
{
	system("cls");
	NHAP:
	string mamonxoa;
	int i;
	designXoaMonHoc();
	gotoxy(27, 5);
	ShowCur(1);
	SetColor(15);
	getline(cin, mamonxoa);
	while (1)
	{
		while (kiemTraChuoi(mamonxoa) == 1)// chuoi rong
		{
			gotoxy(0, 20);
			ShowCur(0);
			SetColor(15);
			cout << "Ban co muon thoat (y/n): ";
			int a = getch();
			if (a == 121)
			{
				return 2; //Phim y
			}
			else
			{
				system("cls");
				goto NHAP;
			}
		}
		i = kiemTraMAMH(mamonxoa, dsmh); //i = -1 or i >= 0
		if (i != -1)
		{
			gotoxy(28, 7);
			SetColor(15);
		    cout << dsmh.nodes[i]->TENMH;
			gotoxy(0, 20);
			ShowCur(0);
			SetColor(15);
			cout << "Ban chac chan xoa khong (y/n)? ";
			int a = getch();
			if (a == 121)
			{
				delete dsmh.nodes[i];
				for (int j = i + 1; j < dsmh.n; j++)
				{
					dsmh.nodes[j - 1] = dsmh.nodes[j];
				}
				dsmh.n--;
				gotoxy(32, 20);
				ShowCur(0);
				cout << "Da xoa mon hoc!";
				writefileLISTMONHOC(dsmh);
				Sleep(2000);
				return 2;
			}
			else
			{
				gotoxy(0, 20);
				ShowCur(0);
				cout << "                                ";
				gotoxy(27, 5);
				cout << "                                ";
				gotoxy(28, 7);
				cout << "                                ";
				goto NHAP;
			}
		}
		else
		{
			gotoxy(0, 20);
			ShowCur(0);
			cout << "Ma Mon Hoc khong chinh xac. Nhap lai!";
			Sleep(1500);
			gotoxy(0, 20);
			cout << "                                      ";
			gotoxy(27, 5);
			cout << "                                      ";
			gotoxy(27, 5);
			ShowCur(1);
			getline(cin, mamonxoa);
		}
	}
}

///////////////////////////////////////////CAU HOI///////////////////////////////////////////
void nhapCAUHOI(CAUHOI &ch, LISTMONHOC lmh)
{
	do
	{
		SetColor(11);
		cout << "Nhap NOI DUNG cau hoi: ";
		fflush(stdin);
		ShowCur(1);
		SetColor(15);
		getline(cin, ch.NOIDUNG);
		if (ch.NOIDUNG == "")
		{
			ShowCur(0);
			SetColor(12);
			cout << "Noi dung cau hoi khong duoc bo trong. Moi ban nhap lai.\n";
		}
	}while (ch.NOIDUNG == "");
	do
	{
		SetColor(11);
		cout << "Nhap CAU A: ";
		fflush(stdin);
		ShowCur(1);
		SetColor(15);
		getline(cin, ch.CAUA);
		if (ch.CAUA == "")
		{
			ShowCur(0);
			SetColor(12);
			cout << "Noi dung cau tra loi khong duoc bo trong. Moi ban nhap lai.\n";
		}
	}while (ch.CAUA == "");
	do
	{
		SetColor(11);
		cout << "Nhap CAU B: ";
		fflush(stdin);
		ShowCur(1);
		SetColor(15);
		getline(cin, ch.CAUB);
		SetColor(12);
		if (ch.CAUB == "")
		{
			ShowCur(0);
			cout << "Noi dung cau tra loi khong duoc bo trong. Moi ban nhap lai.\n";
		}
		if (ch.CAUB == ch.CAUA)
		{
			ShowCur(0);
			cout << "Noi dung cau tra loi bi trung. Moi nhap lai.\n";
		}
	}while (ch.CAUB == "" || ch.CAUB == ch.CAUA);
	do
	{
		SetColor(11);
		cout << "Nhap CAU C: ";
		fflush(stdin);
		ShowCur(1);
		SetColor(15);
		getline(cin, ch.CAUC);
		SetColor(12);
		if (ch.CAUC == "")
		{
			ShowCur(0);
			cout << "Noi dung cau tra loi khong duoc bo trong. Moi ban nhap lai.\n";
		}
		if (ch.CAUC == ch.CAUA || ch.CAUC == ch.CAUB)
		{
			ShowCur(0);
			cout << "Noi dung cau tra loi bi trung. Moi nhap lai.\n";
		}
	}while (ch.CAUC == "" || ch.CAUC == ch.CAUA || ch.CAUC == ch.CAUB);
	do
	{
		SetColor(11);
		cout << "Nhap CAU D: ";
		fflush(stdin);
		ShowCur(1);
		SetColor(15);
		getline(cin, ch.CAUD);
		SetColor(12);
		if (ch.CAUD == "")
		{
			ShowCur(0);
			cout << "Noi dung cau tra loi khong duoc bo trong. Moi ban nhap lai.\n";
		}
		if (ch.CAUD == ch.CAUA || ch.CAUD == ch.CAUB || ch.CAUD == ch.CAUC)
		{
			ShowCur(0);
			cout << "Noi dung cau tra loi bi trung. Moi nhap lai.\n";
		}
	}while (ch.CAUD == "" || ch.CAUD == ch.CAUA || ch.CAUD == ch.CAUB || ch.CAUD == ch.CAUC);
	do
	{
		SetColor(11);
		cout << "Nhap DAP AN: ";
		ShowCur(1);
		SetColor(15);
		cin >> ch.DAPAN;
		if (ch.DAPAN != 'A' && ch.DAPAN != 'B' && ch.DAPAN != 'C' && ch.DAPAN != 'D' && ch.DAPAN != 'a' && ch.DAPAN != 'b' && ch.DAPAN != 'c' && ch.DAPAN != 'd')
		{
			ShowCur(0);
			SetColor(12);
			cout << "Dap an khong phu hop (a, b, c, d, A, B, C, D). Moi nhap lai.\n";
		}
	}while (ch.DAPAN != 'A' && ch.DAPAN != 'B' && ch.DAPAN != 'C' && ch.DAPAN!= 'D' && ch.DAPAN != 'a' && ch.DAPAN != 'b' && ch.DAPAN != 'c' && ch.DAPAN != 'd');
}

void xuatCAUHOI(CAUHOI ch)
{ //Xuat 1 cau hoi
	ShowCur(0);
	cout <<	"NOIDUNG: " << ch.NOIDUNG << endl;
	cout <<	"CAU A: " << ch.CAUA << endl;
	cout << "CAU B: " << ch.CAUB << endl;
	cout << "CAU C: " << ch.CAUC << endl;
	cout << "CAU D: " << ch.CAUD << endl;
	cout <<	"Dap an: " << ch.DAPAN << endl;
}

void Init(TREECH & root)
{
	root = NULL;
}

bool isEmpty(TREECH root)
{
	return(root == NULL);
}

/*LH: cay con trai cao hon
 EH: 2 cay con bang nhau
 RH: cay con phai cao hon*/
 
//Quay phai
void rotateLL (TREECH &T)
{
    NODECH *T1 = T->left;
    T->left = T1->right;
    T1->right = T;
    switch (T1->bf) //T1 ban dau khi chua xoay. Cap nhat lai chi so can bang
    { 
        case LH:
			T->bf = EH;
            T1->bf = EH;
            break;
        case EH:
			T->bf = LH;
            T1->bf = RH;
            break;
    } 
    T = T1;
}

//Quay trai
void rotateRR(TREECH &T)
{
    NODECH *T1 = T->right;
    T->right = T1->left;
    T1->left = T;
    switch (T1->bf)
    {
        case RH:
			T->bf = EH;
            T1->bf = EH;
            break;
        case EH: 
			T->bf = RH;
            T1->bf = LH;
            break;
    } 
    T = T1;
}

//Quay kep Left - Right
void rotateLR(TREECH &T)
{
    NODECH *T1 = T->left;
    NODECH *T2 = T1->right;
    T->left = T2->right;
    T2->right = T;
    T1->right = T2->left;
    T2->left = T1;
    switch (T2->bf)
    { 
        case LH:
			T->bf = RH;
            T1->bf = EH;
            break;
        case EH:
			T->bf = EH;
            T1->bf = EH;
            break;
        case RH:
			T->bf = EH;
            T1->bf = LH;
            break;
    }
    T2->bf = EH;
    T = T2;
}

//Quay kep Right - Left
void rotateRL (TREECH &T)
{ 
    NODECH *T1 = T->right;
    NODECH *T2 = T1->left;
    T->right = T2->left;
    T2->left = T;
    T1->left = T2->right;
    T2->right = T1;
    switch (T2->bf)
    { 
        case RH:
			T->bf = LH;
            T1->bf = EH;
            break;
        case EH:
			T->bf = EH;
            T1->bf = EH;
            break;
        case LH:
			T->bf = EH;
            T1->bf = RH;
            break;
    } 
    T2->bf = EH;
    T = T2;
}

//Can bang khi cay bi lech ve ben trai
void balanceLeft(TREECH &T)
{
    switch (T->left->bf)
    {
        case LH:
			rotateLL(T);
			break;
        case EH:
			rotateLL(T);
			break;
        case RH:
			rotateLR(T);
    }
}

//Can bang khi cay bi lech ve ben phai
void balanceRight(TREECH &T)
{
    switch (T->right->bf)
    {
        case LH:
			rotateRL(T);
			break;
        case EH:
			rotateRR(T);
			break;
        case RH:
			rotateRR(T);
    }
}

/*return 1: Trang thai sau khi them bf = 0
return 2: Trang thai sau khi them bf = 1 hoac bf = -1*/
int Insert(TREECH &T, int X, CAUHOI ch)
{
    int res;
    if (T != NULL)
    {
        if (T->ID == X)
        {
        	return 0;
		}
        else if (T->ID > X)
        {
            res = Insert(T->left, X, ch);
            if (res < 2)
            {
            	return res;
			}
            switch (T->bf)
            {
                case RH:
					T->bf = EH;
                    return 1;
            	case EH:
					T->bf = LH;
                	return 2;
            	case LH:
					balanceLeft(T);
                	return 1;
            }
        }
        else //if (T->key < X)
        {
            res = Insert(T->right, X, ch);
            if (res < 2)
            {
            	return res;
			}
            switch (T->bf)
            {
                case LH:
					T->bf = EH;
                    return 1;
                case EH:
					T->bf = RH;
                    return 2;
                case RH:
					balanceRight(T);
                    return 1;
            }
        }
    }
    //T == NULL
    T = new NODECH;
    if (T == NULL)
    {
    	return -1; //Thieu bo nho
	}
    T->ID = X;
    T->bf = EH;
    T->info.MAMH = ch.MAMH;
	T->info.NOIDUNG = ch.NOIDUNG;
	T->info.CAUA = ch.CAUA;
    T->info.CAUB = ch.CAUB;
    T->info.CAUC = ch.CAUC;
    T->info.CAUD = ch.CAUD;
    T->info.DAPAN = ch.DAPAN;
    T->left = T->right = NULL;
    return 2;
}

void lengthCAUHOIMAMH(TREECH p, int &i, string mamh) //Ham nay tra ve do dai cua cac cauhoi ma mamh giong voi mamh ma nguoi dung yeu cau.
{
    if (p != NULL)
    {
    	if (p->info.MAMH == mamh)
    	{
    		i++;
		}
    	lengthCAUHOIMAMH(p->left, i, mamh);
    	lengthCAUHOIMAMH(p->right, i, mamh);
    }
}

void xuatLISTCAUHOI(TREECH p, string mamh)
{ //Xuat cac cau hoi co mamh bang voi mamh
	if (p != NULL)
    {
    	if(p->info.MAMH == mamh)
		{
    		xuatCAUHOI(p->info);
		}
    	xuatLISTCAUHOI(p->left, mamh);
    	xuatLISTCAUHOI(p->right, mamh);
    }
}

void xuatLISTCAUHOI_OF_MONHOC(TREECH p , LISTMONHOC lmh)
{ //Xuat danh sach cau hoi theo mon hoc
	string mamh;
	int i = 0;
	int dem = 0;
	do
	{
		system("cls");
		ShowCur(0);
		cout << "\nCHUC NANG XEM CAU HOI THI CUA MON HOC\n\n";
		do
		{
			SetColor(11);
			cout << "\nNhap ma mon hoc muon xem cau hoi: ";
			fflush(stdin);
			ShowCur(1);
			SetColor(15);
			getline(cin, mamh);
			if (kiemTraMAMH(mamh, lmh, lmh.n) == true)
			{
				ShowCur(0);
				SetColor(12);
				cout << "Ma mon hoc khong ton tai. Moi nhap Lai.\n";
				dem++;
			}
		}while (kiemTraMAMH(mamh, lmh, lmh.n) == true && dem < 4); //Cho nhap lai nhieu nhat 3 lan
		lengthCAUHOIMAMH(p, i, mamh);
		if (i == 0)
		{
			SetColor(12);
			ShowCur(0);
			cout << "\nKhong co cau hoi nao.\n";
		}
		else
		{
			xuatLISTCAUHOI(p, mamh);
		}
		ShowCur(0);
		SetColor(11);
		cout << "\nBan muon xem cau hoi mon khac khong? 0: THOAT, #0: TIEP TUC: ";
		cin >> i;
	}while (i != 0);
}

void kiemTraCAUHOI(TREECH p, CAUHOI ch, int &i) //Kiem tra cau hoi xem bi trung chua.
{
    if(p != NULL)
    {  
		if(p->info.MAMH == ch.MAMH && p->info.NOIDUNG == ch.NOIDUNG && p->info.CAUA == ch.CAUA && p->info.CAUB == ch.CAUB && p->info.CAUC == ch.CAUC && p->info.CAUD == ch.CAUD)
		{
			i++;
		}
    	kiemTraCAUHOI(p->left, ch, i);
    	kiemTraCAUHOI(p->right, ch, i);
    }
}

TREECH searchCAUHOIbyID(TREECH T, int x)
{ //x la id cua cau hoi minh muon tim trong cay
	TREECH p;
	p = T;
	while (p != NULL && x != p->ID)
	{
		if (x < p->ID)
		{
			p = p->left;
		}
		else
		{
			p = p->right;
		}
	}
	return(p); //Neu p tra ve NULL: tuc la trong cay chua ton tai id x, them vao cay; neu khac NULL la trong cay da ton tai roi, phai ramdom lai
}

int RANDOM()
{
    int m;
	int n;
	int A[2000];
	ifstream filein;
	filein.open("so_ngau_nhien.txt", ios_base::in);
	filein >> n;
	for (int i = 0; i < n; i++)
	{
		filein >> A[i];
	}
	filein.close();
	m = A[n - 1];
	n--;
	ofstream fileout;
	fileout.open("so_ngau_nhien.txt", ios_base::out);
	fileout << n << endl;
	for (int i = 0; i < n; i++)
	{
		fileout << A[i] << " ";
	}
	fileout.close();
	return m;
}

void nhapLISTCAUHOI(TREECH &p , LISTMONHOC lmh)
{
	int temp1;
	int a;
	do
	{
		system("cls");
		ShowCur(0);
		cout << "\nCHUC NANG NHAP CAU HOI THI TRAC NGHIEM\n\n";
		int id, temp, x; //x dung de kiem tra trung cau hoi, temp vong lap, id la key trong cay nhi phan
		NODECH *q, *kt;
		CAUHOI ch;
		do
		{
			ShowCur(0);
			SetColor(11);
			cout << "Nhap MA MON HOC: ";
			fflush(stdin);
			ShowCur(1);
			SetColor(15);
			getline(cin, ch.MAMH);
			if (ch.MAMH == "" || kiemTraMAMH(ch.MAMH, lmh, lmh.n))
			{
				if (kiemTraChuoi(ch.MAMH) == 1) //Chuoi rong
				{
					gotoxy(0, 20);
					ShowCur(1);
					SetColor(12);
					cout << "Ban co muon thoat (Y/N): ";
					a = getch();
					if (a == 121)
					{
						return; //Phim y
					}
					else
					{
						system("cls");
					}
				}
				else
				{
					ShowCur(0);
					SetColor(12);
					cout << "Ma mon hoc nay khong ton tai. Moi kiem nhap lai.\n";
				}
			}
		}while (ch.MAMH == "" || kiemTraMAMH(ch.MAMH, lmh, lmh.n));
		do
		{
			id = RANDOM();
			do
			{
				ShowCur(0);
				SetColor(11);
				cout << "\nNhap thong tin cua cau hoi.\n";
				nhapCAUHOI(ch, lmh);
				x = 0;
				kiemTraCAUHOI(p, ch, x);
				SetColor(12);
				if (x == 1)
				{
					ShowCur(0);
					cout << "\nCau hoi nay da bi trung. Moi ban nhap lai.\n";
				}
				else
				{
					int j = Insert(p, id, ch);
					if (j == 2 || j == 1)
					{
						ShowCur(0);
						cout << "Them cau hoi thanh cong.\n";
					}
					else
					{
						ShowCur(0);
						cout << "Them cau hoi that bai.\n";
					}
				}
			}while (x == 1);
			SetColor(12);
			ShowCur(1);
			cout << "\nBan muon them cau hoi cho MON HOC nay nua khong? 0: THOAT, #0:TIEP TUC: ";
			cin >> temp;
			if (temp == 0)
			{
				ShowCur(0);
				cout << "\nThoat chuc nang thanh cong.\n";
			}
		}while (temp != 0);
		ShowCur(1);
		cout << "\nBan muon them cau hoi cho mon khac khong? 0: THOAT, #0:TIEP TUC: ";
		cin >> temp1;
		if (temp1 == 0)
		{
			ShowCur(0);
			cout << "\nThoat chuc nang thanh cong.\n";
		}
	}while (temp1 != 0);
}

void writefileLISTCAUHOI(TREECH p, ofstream &fo)
{
	if (p != NULL)
	{
	    fo << p->ID << endl;
		fo << p->info.MAMH << endl;
		fo << p->info.NOIDUNG << endl;
		fo << p->info.CAUA << endl;
		fo << p->info.CAUB << endl;
		fo << p->info.CAUC << endl;
		fo << p->info.CAUD << endl; 
		fo << p->info.DAPAN << endl;      
	    writefileLISTCAUHOI(p->left, fo);
	    writefileLISTCAUHOI(p->right, fo);
	}
}

int writefileLISTCAUHOI(TREECH p)
{
	ofstream fo("filelistcauhoi.txt", ios::out); //ios::app: ghi tiep tuc vao file, chu khong phai xoa het file roi ghi lai tu dau. 
	if (!fo)
	{
		return -1;
	}
	writefileLISTCAUHOI(p, fo);
	fo.close();
	return 1;
}

int readfileLISTCAUHOI(TREECH &p)
{
	int id;
	CAUHOI ch;
	string temp;
	ifstream fi("filelistcauhoi.txt");
	if (!fi)
	{
		return -1;
	}
	while (!fi.eof())
	{
		fi >> id;
		getline(fi, temp); //Vi id la so, nen se con du khoang trang, phai dung 1 bien string tam de luu, neu khong, ma mon hoc se bi bo qua
		getline(fi, ch.MAMH);
		getline(fi, ch.NOIDUNG);
		getline(fi, ch.CAUA);
		getline(fi, ch.CAUB);
		getline(fi, ch.CAUC);
		getline(fi, ch.CAUD);
		fi >> ch.DAPAN;
		getline(fi, temp);
		Insert(p, id, ch);
	}
	fi.close();
	return 1;
}

///////////////////////////////////////////DE THI///////////////////////////////////////////
bool isEmpty(DETHI plist)
{
	return(plist.n == 0);
}

bool isFull(DETHI plist)
{
	return(plist.n == MAX);
}

void create(DETHI & plist)
{
	plist.n = 0;
}

bool kiemTraDETHI(int id, DETHI plist)
{ //Kiem tra cau hoi nay co trong de thi chua;
	for (int i = 0; i < plist.n; i++)
	{
		if (plist.nodes[i] == id)
		{
			return false; //Tra ve false khi cau hoi bi trung, ngc lai tra ve true.
		}
	}
	return true;
}

int themCAUHOI_DETHI(int id, DETHI &plist)
{ //Ham them mot cau hoi vao danh sach DETHI da co san.
	if (isFull(plist))
	{
		return -1;
	}	
	if (kiemTraDETHI(id, plist) == false)
	{
		return -1;
	}
	plist.nodes[plist.n] = id; //Danh sach de thi la mang tuyen tinh; ok het thi cho them vao, sau khi them vao thi tang so luong phan tu cua danh sach len 1
	plist.n += 1;
	return 1;
}

void LayCauHoiCuaMon(int &n, int A[], TREECH lch, string mamh)
{
	if (lch != NULL)
	{
		if (lch->info.MAMH == mamh)
		{
			A[n] = lch->ID;
			n++;
		}
		LayCauHoiCuaMon(n, A, lch->left, mamh);
		LayCauHoiCuaMon(n, A, lch->right, mamh);
	}
}

void HoanDoi(int &a, int &b)
{
	int temp = a;
	a = b;
	b = temp;
}

void XaoTronMang(int arr[], int n)
{
	srand(time(NULL));
	int vitrimin;
	int vitrimax = n - 1;
	int vitrihoandoi;
	int i = 0;
	while (i < n - 1)
	{
		vitrimin = i + 1;
		vitrihoandoi = rand() % (vitrimax - vitrimin + 1) + vitrimin;
		HoanDoi(arr[i], arr[vitrihoandoi]);
		i++;
	}
}

void taiDETHI(int soch, DETHI &ldt, TREECH lch, string mamh)
{
	int n = 0;
	int A[4*MAX];
	LayCauHoiCuaMon(n, A, lch, mamh);
	XaoTronMang(A, n);
	ldt.n = soch;
	for (int i = 0; i < soch; i++)
	{
		ldt.nodes[i] = A[i];
	}
}

int LuuFileDSLOP(DsLop &dsLop)
{
	ofstream of("dslop.txt", ios::out);
	if (!of)
	{
		cout << "Loi luu file!!!";
		return -1;
	}
	of << dsLop.n << endl;
	for (int i = 0; i < dsLop.n; i++)
	{
		of << dsLop.lh[i].MaLop << endl;
		of << dsLop.lh[i].TenLop << endl;
		LISTSV p = dsLop.lh[i].FirstSV;
	
		while (p != NULL)
		{
			of << p->sv.MSSV << endl;
			of << p->sv.ho << endl;
			of << p->sv.ten << endl;
			of << p->sv.gioitinh << endl;
			of << p->sv.password << endl;
			Listdiemthi ptrDiem = p->sv.FirstDiem;
			while (ptrDiem != NULL)
			{
				of << ptrDiem->DataDiem.MAMH << endl;
				of << ptrDiem->DataDiem.DIEM << endl;
				ptrDiem = ptrDiem->pNext;
			}
			of << endl;
			p = p->pNext;
		}
		of << endl;
	}
	of.close();
}

void DocFileDSLOP(DsLop &dslop)
{
	ifstream fi("dslop.txt", ios::in);
	if (!fi)
	{
		cout << "Khong the doc file!!";
		return;
	}
	fi >> dslop.n;
	fi.ignore();
	for (int i = 0; i < dslop.n; i++)
	{
		getline(fi, dslop.lh[i].MaLop);
		getline(fi, dslop.lh[i].TenLop);
		dslop.lh[i].FirstSV = NULL;
		Sv *sv = new Sv; //Cap phat vung nho cho 1 sinh vien
		getline(fi, sv->MSSV);
		while (sv->MSSV.length() != 0)
		{
			getline(fi, sv->ho);
			getline(fi, sv->ten);
			getline(fi, sv->gioitinh);
			getline(fi, sv->password);
			sv->FirstDiem = NULL;
			Diemthi *diem = new Diemthi;
			getline(fi, diem->MAMH);
			while (diem->MAMH.length() != 0)
			{
				fi >> diem->DIEM;
				fi.ignore();
				Insert_LastDiem(sv->FirstDiem, *diem);
				diem = new Diemthi;
				getline(fi, diem->MAMH);
			}
			Insert_Last(dslop.lh[i].FirstSV, *sv);
			sv = new Sv;
			getline(fi, sv->MSSV);
		}
	}
	fi.close();
}

void InThoiGian(int tg)
{ //Ham hien thi thoi gian, thoi gian la so giay
	gotoxy(10, 1);
	ShowCur(0);
	cout << "TG: " << setw(2) << setfill('0') << tg/60 //In so phut
		<< ":" << setw(2) << setfill('0') << tg%60; //In so giay
}

void normal()
{
	SetColor(10); //Mau chu
	SetBGColor(0);
}

void hightline()
{
	SetColor(29); //Mau chu cua vi tri con tro hien tai
	SetBGColor(11);
}

char menu_GV[13][100] =   {   "1. THEM LOP MOI                      ",
							  "2. IN DS LOP                         ",
						   	  "3. THEM SINH VIEN CUA 1 LOP          ",
						   	  "4. IN DS SINH VIEN CUA 1 LOP         ",
						      "5. THEM MON HOC                      ",
						      "6. SUA MON HOC                       ",
						      "7. XOA MON HOC                       ",
						      "8. IN DS MON HOC                     ",
						      "9. NHAP CAU HOI THI                  ",
						      "10. IN DANH SACH CAU HOI             ",
						      "11. THI TRAC NGHIEM                  ",
						      "12. IN CAC CAU HOI DA THI CUA SV     ",
						      "13. IN BANG DIEM CUA LOP THEO MON HOC",	
};

int Select()
{
	system("cls");
	DsLop dslop;
	int select = 0;
	int i = 0;
	char c;
	gotoxy(25, 1);
	for (int i = 0; i < 23; i++)
	{
		cout << "+ ";
	}
	for (int i = 0; i < 33; i++)
	{
		gotoxy(25, i + 1);
		cout << "+ ";
	}
	gotoxy(25, 33);
	for (int i = 0; i < 23; i++)
	{
		cout << "+ ";
	}
	for (int i = 0; i < 33; i++)
	{
		gotoxy(69, i + 1);
		cout << "+ ";
	}
	gotoxy(2, 3);
	SetColor(14);
	cout << "UP: len";
	gotoxy(2, 4);
	cout << "DOWN: xuong";
	gotoxy(2, 5);
	cout << "ENTER: chon";
	gotoxy(35, 1);
	cout << "CHUC NANG CUA GIAO VIEN";
	gotoxy(30, 2);
	normal();
	for (int i = 0; i < 13; i++)
	{
		gotoxy(30, 5 + i*2);
		cout << menu_GV[i];
	}
	hightline();
	gotoxy(30,5);
	cout << menu_GV[0];
	while (1)
	{
		ShowCur(0);
		c = _getch();
		normal();
		gotoxy(26, (5 + select*2));
		cout << "  ";
		gotoxy(30, (5 + select*2));
		cout << menu_GV[select];
		switch (c)
		{
			case UP:
			{
				select -= 1;
				if (select < 0)
				{
					select = 12;
				}
				break;
			}
			case DOWN:
			{
				select += 1;
				if (select > 12)
				{
					select = 0;
				}
				break;
			}
			case ENTER:
			{
				return select + 1;
				break;
			}
			default:
			{
				break;
			}
		}
		hightline();
	    gotoxy(30, (5 + select*2));
		cout << menu_GV[select];
	}
	gotoxy(70, 70);
}

float chamDIEM(TREECH lch, DETHI dt, char* dapan)
{
	int j = 0;
	for (int i = 0; i < dt.n; i++)
	{
		NODECH *q = searchCAUHOIbyID(lch, dt.nodes[i]);
		if (dapan[i] < 123 && dapan[i] > 96) //Chuan hoa cac dap an ma co chu in thuong thanh in hoa
		{
			dapan[i] -= 32;
		}
		if (q->info.DAPAN < 123 && q->info.DAPAN > 96) //Chuan hoa cac dap an ma co chu in thuong thanh in hoa
		{
			q->info.DAPAN -= 32;
		}
		if (dapan[i] == q->info.DAPAN) //So sanh tung cau tra loi cua nguoi dung voi dap an cua cau hoi. Dung thi tang bien dem. Khong thi thoi.
		{
			j++;
		}
	}
	return ((float)j / (float)dt.n)*10; //Tra ve diem so dat duoc.
}

void inxy(int x, int y, string s)
{ //Ham di chuyen con tro lenh den vi tri (x,y) roi in ra xau s.
	gotoxy(x, y);
	cout << s;
}

void InCauTraLoi(CAUHOI ch, int i, bool highlight)
{ //Ham in cac cau tra loi, i la cau thu (A, B, C, D), highlight la co to sang hay khong
	string ctl; //ctl de chua cau tra loi tuong ung
	switch (i)
	{
		case 0:
			ctl = ch.CAUA;
			break;
		case 1:
			ctl = ch.CAUB;
			break;
		case 2:
			ctl = ch.CAUC;
			break;
		case 3:
			ctl = ch.CAUD;
	}
	ctl.insert(0, string(1, char(i + 65)) + ". "); //Them "A." (hoac "B.",...) truoc cau tra loi
	int padding = 80 - ctl.length() % 80; //padding luu so khoang trong can them de cho du 1 hang
	if (highlight)
	{ //Neu bien highlight = true thi to sang, = false thi thoi
		SetColor(0); //Dat mau chu den
		SetBGColor(15); //Dat nen trang
	}
	inxy(0, 7 + i * 3, ctl + string(padding,' ')); //In cau tra loi
	SetColor(15); //Dat mau chu trang
	SetBGColor(0); //Dat nen den
}

void GiaoDienThi(CAUHOI ch, string mamh, int soch, int i, char* dapan, int &luachon)
{ //Ham xay dung giao dien thi
	system("cls");
	ShowCur(0);
	inxy(33, 0, "THI TRAC NGHIEM");
	string mamon = "Ma mon hoc: " + mamh;
	inxy(40 - mamon.length()/2, 2, mamon);
	gotoxy(0, 4);
	cout << "Cau " << i + 1 << "/" << soch << ": " << ch.NOIDUNG;
	for (int j = 0; j < 4; j++)
	{
		InCauTraLoi(ch, j, false); //In ra 4 cau tra loi
	}
	if (dapan[i] != ' ')
	{ //Xem nguoi thi da tra loi cau i chua. Neu roi thi to sang dap an tuong ung, chua thi to sang cau A
		InCauTraLoi(ch, int(dapan[i]) - 65, true);
		luachon = int(dapan[i]) - 65;
	}
	else
	{
		InCauTraLoi(ch, 0, true);
		luachon = 0;
	}
	gotoxy(0, 19);
	ShowCur(0);
	cout << "Dap an ban chon: " << dapan[i];
	inxy(0, 21, "Huong dan: Nhan phim mui ten len, xuong de di chuyen thanh sang, enter de chon.");
	inxy(1, 22, "Nhan phim mui ten trai, phai de den cau truoc, cau sau, nhan \"N\" de nop bai.");
}

void hienThiCAUTRALOI(int soch, string dapan)
{//Ham nay hien thi cau tra loi cua nguoi thi khi nhap vao, giong nhu minh tra loi cau hoi 1 la A. Thi no hien ra la 1 - A.
	ShowCur(0);
	cout << "\nCau hoi:" ;
	for (int i = 0; i < soch; i++)
	{
		cout << "  "<< i + 1;
	}
	cout << "\nTra loi: ";
	for (int i = 0; i < soch; i++)
	{
		cout << "  " << dapan[i];
	}
}

string dapAnDETHI(DETHI dt, TREECH lch)
{ //Day la ham lay dap an cua 1 de thi de ti nua dung dap an do di cham diem.
	string da = "";
	for (int i = 0; i < dt.n; i++)
	{
		NODECH *p = searchCAUHOIbyID(lch, dt.nodes[i]);
		da += p->info.DAPAN; //Tra dap an cua de thi no ve 1 cai chuoi.
	}
	return da;
}

bool KiemTra(Sv sinhvien, string mamh)
{
	while (sinhvien.FirstDiem != NULL)
	{
		if(sinhvien.FirstDiem->DataDiem.MAMH == mamh)
		{
			return false;
		}
		sinhvien.FirstDiem = sinhvien.FirstDiem->pNext;
	}
	return true;
}

Listdiemthi searchDIEMTHIbyMASV_byMAMH (Listdiemthi ldt, string mamh)
{ //Tim x trong danh sach.
    Listdiemthi p = ldt;
	while (p != NULL)
	
    {
    	if (p->DataDiem.MAMH == mamh)
		{
    		return p;
    	}
        p = p->pNext;
    }
    return NULL; //Khong tim thay
}

void designTHITN()
{
	gotoxy(30, 1);
	SetColor(19);
	cout << "CHUC NANG THI TRAC NGHIEM";
	gotoxy(10, 3);
	SetColor(5);
	cout << "MA MON HOC: ";
	gotoxy(10, 4);
	cout << "SO LUONG CAU HOI: ";
	gotoxy(10, 5);
	cout << "SO PHUT: ";
}

void thiTRACNGHIEM(TREECH lch, LISTMONHOC lmh, LISTSV sv , Sv *sinhvien)
{ //Day la phan thi trac nghiem
	system("cls");
	string mamh; //Hoi sinh vien thi mon gi
	int soch; //Thi bao nhieu cau.
	int temp; //Bien lap. Nhap tat ca cac thong tin can thiet, roi kiem tra loi, neu co loi bat nhap lai.
	int cauthu = 0; //Bien quan ly cau hoi
	int luachon = 0; //Bien quan ly cau tra loi dang chon
	do
	{
		designTHITN();
		DETHI dethi;
		create(dethi); //dethi.n = 0;
		do
		{
			fflush(stdin);
			gotoxy(22, 3);
			SetColor(15);
			ShowCur(1);
			getline(cin, mamh);
			if (mamh == "" || kiemTraMAMH(mamh, lmh, lmh.n) == true)
			{
				gotoxy(0, 20);
				ShowCur(0);
				cout << "Ma Mon Hoc nay khong hop le. Moi nhap lai";
				Sleep(1500);
				gotoxy(22,3);
				cout << "                                         ";
				gotoxy(0, 20);
				cout << "                                         ";
			}
		}while (mamh == "" || kiemTraMAMH(mamh, lmh, lmh.n) == true);
		int i = 0;
		do
		{
			gotoxy(28, 4);
			ShowCur(1);
			cin >> soch;
			lengthCAUHOIMAMH(lch, i, mamh); //Lay so cau hoi toi da cua mon do bo vao i va di so sanh kiem tra dieu kien
			if (soch < 0 || soch > i)
			{
				gotoxy(0, 20);
				ShowCur(0);
				cout << "So cau hoi khong hop le (0 < x <= "<< "i"<<"). Moi ban nhap lai.";
				Sleep(1500);
				gotoxy(28, 4);
				cout << "																 ";
				gotoxy(0, 20);
				cout << "																 ";
			}
		}while (soch < 0 || soch > i);
		int nhaptg; //Nhap thoi gian thi
		gotoxy(19, 5);
		ShowCur(1);
		cin >> nhaptg;
	    Listdiemthi p;
		if (sv == NULL) //Day la admin thi.
		{
			p = NULL;
		}
		else //Day la sinh vien thi
		{
			p = searchDIEMTHIbyMASV_byMAMH(sinhvien->FirstDiem, mamh); //Kiem tra sinh vien da thi mon nay chua.
		}
		if (p == NULL)
		{ //Chua thi hoac giao vien thi
			taiDETHI(soch, dethi, lch, mamh); //Tai danh sach de thi dua vao cac thong tin moi nhap vao.
			char *dapan = new char[dethi.n];
			for (int i = 0; i < soch; i++)
			{ //Day la chuoi string de luu cau tra loi cua ng thi vao. Dau tien ta cho tat ca bang space het.
				dapan[i] = ' ';
			}
			int thoigian = nhaptg * 60; //Tinh thoi gian, don vi tinh la giay.
			unsigned char da;
			time_t batdau, hiengio; //batdau: chua thoi gian luc bat dau, hiengio: chua thoi gian hien gio
			time(&batdau); //Lay thoi gian bat dau thi
			NODECH* ch = searchCAUHOIbyID(lch, dethi.nodes[cauthu]); //Lay bo cau hoi & cau tra loi theo so thu tu
			GiaoDienThi(ch->info, mamh, soch, cauthu, dapan, luachon); //Hien thi giao dien thi
			do
			{
				if (kbhit())
				{ //Neu co nhan phim
					da = getch();
					switch (da)
					{
						case 13:
						{ //Nhan enter chon dap an
							dapan[cauthu] = char(luachon + 65); //Ghi nhan vao dap an
							gotoxy(0, 19);
							ShowCur(0);
							cout << "Dap an ban chon: " << dapan[cauthu]; //In cau tra loi ra
							break;
						}
						case 'N': //Nhan chu N nop bai
						case 'n':
						{ //Nhan chu n nop bai
							system("cls");
							ShowCur(0);
							inxy(33, 0, "THI TRAC NGHIEM"); //Ham di chuyen con tro lenh den vi tri (x,y) roi in ra xau s.
							string mamon = "Ma mon hoc: " + mamh;
							inxy(40 - mamon.length() / 2, 2, mamon);
							ShowCur(1);
							inxy(0, 4, " Ban da chac chan muon nop bai chua? (Nhan \"y\": dong y, #y: khong dong y)");
							ShowCur(0);
							inxy(0, 6, "Dap an ban lam:");
							gotoxy(0, 6);
							hienThiCAUTRALOI(soch, dapan);
							da = 0;
							do
							{ //Vong lap de xem nguoi thi co chac chan nop hay chua
								if (kbhit())
								{
									da = getch();
									if (da == 0 || da == 224)
									{ //Neu la nhan cac phim mui ten,... thi phai getch them 1 lan de lay ky tu du
										da = getch();
										break;
									}
									else
									{
										if (da == 'y' || da == 'Y')
										{ //Nguoi thi dong y nop bai thi cho da = chu "n"
											da = 'n';
										}
										else
										{ //An phim bat ky thi quay lai thi tiep neu con thoi gian
											da = 0;
											break;
										}
									}
								}
								time(&hiengio); //Lay gio hien tai
								ShowCur(0);
								InThoiGian(thoigian - hiengio + batdau); //In ra thoi gian
							}while (da != 'n' && da != 'N' && thoigian >= (hiengio - batdau));
							GiaoDienThi(ch->info, mamh, soch, cauthu, dapan, luachon);
							break;
						}
						case 0: //Nhan phim mui ten
						case 224:
						{ //Nhan phim mui ten
							da = getch();
							switch (da)
							{
								case 80:
								{ //Phim xuong di chuyen thanh sang xuong
									InCauTraLoi(ch->info, luachon, false); //Viet thuong lai cau tra loi dang to sang
									if (luachon == 3)
									{
										luachon = 0;
									}
									else
									{
										luachon++;
									}
									InCauTraLoi(ch->info, luachon, true); //To sang cau tra loi ke tiep
									break;
								}
								case 72:
								{ //Phim len di chuyen thanh sang len
									InCauTraLoi(ch->info, luachon, false); //Viet thuong lai cau tra loi dang to sang
									if (luachon == 0)
									{
										luachon = 3;
									}
									else
									{
										luachon--;
									}
									InCauTraLoi(ch->info, luachon, true); //To sang cau tra loi phia truoc
									break;
								}
								case 77:
								{ //Phim phai toi cau ke tiep
									if (cauthu == dethi.n - 1)
									{
										cauthu = 0;
									}
									else
									{
										cauthu++;
									}
									ch = searchCAUHOIbyID(lch, dethi.nodes[cauthu]);
									GiaoDienThi(ch->info, mamh, soch, cauthu, dapan, luachon);
									break;
								}
								case 75:
								{ //Phim trai quay lai cau truoc
									if (cauthu == 0)
									{
										cauthu = dethi.n - 1;
									}
									else
									{
										cauthu--;
									}
									ch = searchCAUHOIbyID(lch, dethi.nodes[cauthu]);
									GiaoDienThi(ch->info, mamh, soch, cauthu, dapan, luachon);
								}
							}
						}
					}
				}
				time(&hiengio); //Lay gio hien tai
				ShowCur(0);
				InThoiGian(thoigian - hiengio + batdau); //In ra thoi gian
			}while (da != 'n' && da != 'N' && thoigian >= (hiengio - batdau)); //Kiem tra thoi gian va xem nguoi ta nop bai chua. Neu chua thi cho ho thi tiep, khi nao het hoi gian thi thoat hoac khi ho nhan vao 'n' la thoat ra cham diem.
			//Het gio thi hoac nguoi ta da nhan vao nop bai.
			system("cls");
			da = 0;
			ShowCur(0);
			cout << "Dap an ban lam:";
			gotoxy(0, 0);
			hienThiCAUTRALOI(soch, dapan);
			gotoxy(0, 5);
			cout << "Dap an dung:";
			string ketqua = dapAnDETHI(dethi, lch); //Lay dap an cua de thi do.
			gotoxy(0, 5);
			hienThiCAUTRALOI(soch, ketqua);
			float diem = chamDIEM(lch, dethi, dapan);
			gotoxy(0, 9);
			cout << "\nBan duoc " << diem << " diem" << endl;
			if (sv == NULL)
			{ //Kiem tra nguoi thi la sv hay admin, neu admin thi goto EXIT.
				goto exit;
			}
			Listdiemthi dt = new NodeDiem;
			dt->DataDiem.DeThi.n = 0;
			for (int i = 0; i < dethi.n; i++)
			{
				themCAUHOI_DETHI(dethi.nodes[i], dt->DataDiem.DeThi);
			}
			dt->DataDiem.MAMH = mamh;
			dt->DataDiem.DIEM = diem;
			dt->pNext = NULL;
			Insert_LastDiem(sinhvien->FirstDiem, dt->DataDiem);
		}
		else
		{
			ShowCur(0);
			cout << "\nMon nay ban da thi roi.\n";
		}
		exit:
		ShowCur(1);
		cout << "---------------------------------------------------";
		cout << "\nBan co muon thi nua khong? 0: Thoat, #0: Tiep tuc: ";
		cin >> temp;
		system("cls");
	}while (temp != 0);
}

void designXuatDSDiem_Lop()
{
	gotoxy(20, 1);
	SetColor(14);
	SetBGColor(0);
	cout << "CHUC NANG XUAT DIEM THI SINH VIEN CUA 1 LOP";
	gotoxy(30, 3);
	SetColor(3);
	cout << "MA LOP: ";
	gotoxy(30, 4);
	SetColor(3);
	cout << "MA MON HOC: ";
	gotoxy(9, 6);
	SetColor(14);
	cout << "MSSV: ";
	gotoxy(34, 6);
	cout << "HO: ";
	gotoxy(54, 6);
	cout << "TEN: ";
	gotoxy(69, 6);
	cout << "MA MON HOC: ";
	gotoxy(83, 6);
	cout << "DIEM: ";
}

void XuatDSDiem_Lop(DsLop dslop, LISTMONHOC plist)
{
	designXuatDSDiem_Lop();
	int line, a, viTril = -1, viTrim = -1;
	string malop = "";
	do
	{
		fflush(stdin);
		gotoxy(38, 3);
		SetColor(15);
		ShowCur(1);
		getline(cin, malop);
		viTril = TimKiemMaLop(dslop, malop);
		if (viTril == -1)
		{
			gotoxy(0, 25);
			ShowCur(0);
			cout << "Ma Lop khong ton tai!";
			Sleep(1000);
			gotoxy(0, 25);
			cout << "                     ";
			gotoxy(38, 3);
			cout << "                     ";
		}
	}while (viTril == -1);
	string mamh = "";
	do
	{
		fflush(stdin);
		gotoxy(42, 4);
		SetColor(15);
		ShowCur(1);
		getline(cin, mamh);
		viTrim = kiemTraMAMH(mamh, plist);
		if (viTrim == -1)
		{
			gotoxy(0, 25);
			ShowCur(0);
			cout << "Ma Mon Hoc khong ton tai!";
			Sleep(1000);
			gotoxy(0, 25);
			cout << "                         ";
			gotoxy(38, 3);
			cout << "                         ";
		}
	}while (viTrim == -1);
	line = 7;
	LISTSV lsv = dslop.lh[viTril].FirstSV;
	while (lsv != NULL)
	{
		ShowCur(0);
		gotoxy(9, line);
		cout << lsv->sv.MSSV << endl;
		gotoxy(34, line);
		cout << lsv->sv.ho << endl;
		gotoxy(54, line);
		cout << lsv->sv.ten << endl;
		Listdiemthi ldt = lsv->sv.FirstDiem;
		if (ldt != NULL)
		{
			bool thiRoi = false;
			while (ldt != NULL)
			{
				if (ldt->DataDiem.MAMH == mamh)
				{
					gotoxy(70, line);
					cout << ldt->DataDiem.MAMH;
					gotoxy(83, line);
					cout << ldt->DataDiem.DIEM;
					thiRoi = true;
					break;
				}
				ldt = ldt->pNext;
			}
			if (!thiRoi)
			{
				gotoxy(83, line);
				ShowCur(0);
				cout << "Chua thi";
			}
		}
		else
		{
			gotoxy(83, line);
			ShowCur(0);
			cout << "Chua thi"; //Neu danh sach diem thi rong thi chua thi
		}
		line++;
		cout << endl;
		lsv = lsv->pNext;
	}
	getch();
}

void xuLiMenuGV(DsLop &dslop, LISTMONHOC &lmh, TREECH &lch)
{
	Sv *sinhvien;
	Listdiemthi ldt;
	LopHoc lh;
	int choice = 0;
	MAIN:
	system("cls");
	while (choice = Select())
	{
		switch (choice)
		{
			case 1:
			{
				system("cls");
				NhapLop(dslop);
				LuuFileDSLOP(dslop);
				goto MAIN;
				break;
			}
			case 2:
			{
				system("cls");
				if (XuatLop(dslop) == 1)
				{
					goto MAIN;
				}
				break;
			}
			case 3:
			{
				system("cls");
				NhapSV(dslop);
				LuuFileDSLOP(dslop);
				goto MAIN;
				break;
			}
			case 4:
			{
				system("cls");
				DocFileDSLOP(dslop);
				if (XuatDSSV_OF_LOP(dslop) == 2)
				{
					goto MAIN;
				} 
				break;
			}
			case 5:
			{
				system("cls");
				nhapLISTMONHOC(lmh);
				writefileLISTMONHOC(lmh);
				goto MAIN;
				break;
			}
			case 6:
			{
				system("cls");
				Sua_MonHoc(lmh);
				writefileLISTMONHOC(lmh);
				goto MAIN;
				break;
			}
			case 7:
			{
				system("cls");
				XoaMonHoc(lmh);
				goto MAIN;
				break;
			}
			case 8:
			{
				system("cls");
				xuatLISTMONHOC(lmh);
				goto MAIN;
				break;
			}
			case 9:
			{
				system("cls");
				nhapLISTCAUHOI(lch, lmh);
				writefileLISTCAUHOI(lch);
				goto MAIN;
				break;
			}
			case 10:
			{
				system("cls");
				readfileLISTCAUHOI(lch);
				xuatLISTCAUHOI_OF_MONHOC(lch, lmh);
				goto MAIN;
				break;
			}
			case 11:
			{
				system("cls");
				thiTRACNGHIEM(lch,lmh,NULL,sinhvien);
				break;
			}
			case 12:
			{
				break;
			}
			case 13:
			{
				system("cls");
				XuatDSDiem_Lop(dslop,lmh);
				goto MAIN;	
				break;
			}
		}
	}
}

void design_DangNhap()
{
	system ("cls");
	gotoxy(27, 9);
	for (int i = 0; i < 35; i++)
	{
		cout << (char)254;
	}
	for (int i = 10; i < 14; i++)
	{
		gotoxy(27, i);
		cout << (char)221;
	}
	for (int i = 10; i < 14; i++)
	{
		gotoxy(41, i);
		cout << (char)221;
	}
	for (int i = 10; i < 14; i++)
	{
		gotoxy(61, i);
		cout << (char)221;
	}
	gotoxy(27, 13);
	for (int i = 0; i < 35; i++)
	{
		cout << (char)254;
	}
	gotoxy(30, 10);
	SetColor(11);
	cout << "USERNAME: ";
	gotoxy(30, 12);
	SetColor(11);
	cout << "PASSWORD: ";
	gotoxy(0, 20);
}

LISTSV xuLiDangNhap(DsLop dsLop, int &i, Sv* &sv)
{
	system("cls");
	design_DangNhap();
	string username = "", password = "";
	do
	{
		fflush(stdin);
		gotoxy(44, 10);
		ShowCur(1);
		getline(cin, username);
		if (username.length() == 0)
		{
			gotoxy(40, 15);
			ShowCur(0);
			cout << "Hay nhap Username !";
			Sleep(1500);
			gotoxy(40, 15);
			cout << "                   ";
		}
	}while (username.length() == 0);
	ChuanHoaPass(password, 44, 12);
    if (username == "GV" && password == "GV")
	{
    	i = 1;
    	return NULL;
	}
	else
	{
		for (int j = 0; j < dsLop.n; j++)
		{
			LISTSV p = dsLop.lh[j].FirstSV;
			while (p != NULL)
			{
				if (p->sv.MSSV == username && p->sv.password == password)
				{
					i = 0;
					sv = &p->sv;
					return dsLop.lh[j].FirstSV;
				}
				p = p->pNext;
			}
		}
		i = -1;
		sv = NULL;
		return NULL;
	}
}

void GiaiPhongCay(TREECH &root)
{
	if (root->left != NULL)
	{
		GiaiPhongCay(root->left);
	}
	if (root->right != NULL)
	{
		GiaiPhongCay(root->right);
	}
	if (root->left == NULL && root->right == NULL)
	{
		delete root;
		root = NULL;
	}
}

void GiaiPhong(LISTMONHOC &lmh, DsLop &dslop, TREECH &cauhoi)
{
	for (int i = 0; i < lmh.n; i++)
	{
		delete lmh.nodes[i];
	}
	for (int i = 0; i < dslop.n; i++)
	{
		LISTSV h = dslop.lh[i].FirstSV;
		while (dslop.lh[i].FirstSV != NULL)
		{
			Listdiemthi k = dslop.lh[i].FirstSV->sv.FirstDiem;
			while (dslop.lh[i].FirstSV->sv.FirstDiem != NULL)
			{
				dslop.lh[i].FirstSV->sv.FirstDiem = dslop.lh[i].FirstSV->sv.FirstDiem->pNext;
				delete k;
				k = dslop.lh[i].FirstSV->sv.FirstDiem;
			}
			dslop.lh[i].FirstSV = dslop.lh[i].FirstSV->pNext;
			delete h;
			h = dslop.lh[i].FirstSV;
		}
	}
	GiaiPhongCay(cauhoi);
}
int main()
{
	Sv* sinhvien;
	DsLop dslop;
	dslop.n = 0;
	LISTMONHOC lmh;
	create(lmh);
	readfileLISTMONHOC(lmh);
	DocFileDSLOP(dslop);
	TREECH lch;
	Init(lch);
	readfileLISTCAUHOI(lch);
	SetConsoleTitle("Thi Trac Nghiem");
	resizeConsole(840, 720);
	do
	{
		int i = 0;
		LISTSV p = xuLiDangNhap(dslop, i, sinhvien);
		if (p == NULL && i == 1)
		{
			xuLiMenuGV(dslop, lmh, lch);
		}
		else
		{
			if (sinhvien != NULL && i == 0)
			{
				thiTRACNGHIEM(lch, lmh, p, sinhvien);
				LuuFileDSLOP(dslop);
			}
			else
			{
				system("cls");
				cout << "Ma dang nhap khong ton tai!";
				Sleep(2000);
			}
		}
	}while (1);
	GiaiPhong(lmh, dslop, lch);
	return 0;
}
