; CLW file contains information for the MFC ClassWizard

[General Info]
Version=1
LastClass=CSupportCellView
LastTemplate=CDialog
NewFileInclude1=#include "stdafx.h"
NewFileInclude2=#include "SupportCell.h"
LastPage=0

ClassCount=5
Class1=CSupportCellApp
Class2=CSupportCellDoc
Class3=CSupportCellView
Class4=CMainFrame

ResourceCount=7
Resource1=IDR_MAINFRAME
Resource2=IDD_ABOUTBOX
Class5=CAboutDlg
Resource7=IDD_SUPPORTCELL_FORM

[CLS:CSupportCellApp]
Type=0
HeaderFile=SupportCell.h
ImplementationFile=SupportCell.cpp
Filter=N

[CLS:CSupportCellDoc]
Type=0
HeaderFile=SupportCellDoc.h
ImplementationFile=SupportCellDoc.cpp
Filter=N
BaseClass=CDocument
VirtualFilter=DC
LastObject=ID_FILE_OPEN

[CLS:CSupportCellView]
Type=0
HeaderFile=SupportCellView.h
ImplementationFile=SupportCellView.cpp
Filter=D
BaseClass=CFormView
VirtualFilter=VWC
LastObject=CSupportCellView


[CLS:CMainFrame]
Type=0
HeaderFile=MainFrm.h
ImplementationFile=MainFrm.cpp
Filter=T
LastObject=CMainFrame




[CLS:CAboutDlg]
Type=0
HeaderFile=SupportCell.cpp
ImplementationFile=SupportCell.cpp
Filter=D

[DLG:IDD_ABOUTBOX]
Type=1
Class=CAboutDlg
ControlCount=4
Control1=IDC_STATIC,static,1342177283
Control2=IDC_STATIC,static,1342308480
Control3=IDC_STATIC,static,1342308352
Control4=IDOK,button,1342373889

[MNU:IDR_MAINFRAME]
Type=1
Class=CMainFrame
Command1=ID_FILE_OPEN
Command2=ID_APP_EXIT
Command3=ID_APP_ABOUT
CommandCount=3

[ACL:IDR_MAINFRAME]
Type=1
Class=CMainFrame
Command1=ID_FILE_NEW
Command2=ID_FILE_OPEN
Command3=ID_FILE_SAVE
Command4=ID_FILE_PRINT
Command5=ID_EDIT_UNDO
Command6=ID_EDIT_CUT
Command7=ID_EDIT_COPY
Command8=ID_EDIT_PASTE
Command9=ID_EDIT_UNDO
Command10=ID_EDIT_CUT
Command11=ID_EDIT_COPY
Command12=ID_EDIT_PASTE
Command13=ID_NEXT_PANE
Command14=ID_PREV_PANE
CommandCount=14

[DLG:IDD_SUPPORTCELL_FORM]
Type=1
Class=CSupportCellView
ControlCount=1
Control1=IDC_LIST_CELL_INFO,SysListView32,1350631425

[TB:IDR_MAINFRAME]
Type=1
Class=?
Command1=ID_FILE_OPEN
CommandCount=1

