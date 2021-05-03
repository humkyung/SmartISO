; CLW file contains information for the MFC ClassWizard

[General Info]
Version=1
LastClass=CLineManDataImportDlg
LastTemplate=CFormView
NewFileInclude1=#include "stdafx.h"
NewFileInclude2=#include "admin.h"
LastPage=0

ClassCount=24
Class1=CAdminApp
Class2=CAboutDlg
Class3=CAdminDoc
Class4=CAdminTreeView
Class5=CAdminUserDlg
Class6=CAdminView
Class7=CBorderSettingDlg
Class8=CDirTreeCtrl
Class9=CLineDataSettingDlg
Class10=CEditListBox
Class11=CFileNameEditListBox
Class12=CFileNameFormatDlg
Class13=CInPlaceFilterEdit
Class14=CIsoEditSettingDlg
Class15=CMainFrame
Class16=CProjectAccessDlg
Class17=CProjectDefTableDlg
Class18=CProjectSettingForm
Class19=TTreeCtrl

ResourceCount=18
Resource1=IDD_DIALOG_USER
Resource2=IDD_BORDER_SETTING
Resource3=IDD_ABOUTBOX
Resource4=IDD_ORACLE_SETTING
Resource5=IDD_LINE_DATA_SETTING
Resource6=IDR_MAINFRAME
Resource7=IDD_REF_DATA_SETTING
Resource8=IDD_LINEMAN_DATA_IMPORT
Resource9=IDD_DIALOG_PROJECT_SETTING
Resource10=IDD_ISO_EDIT_SETTING
Resource11=IDD_REVISION_STAGE
Resource12=IDD_WORK_STATUS
Resource13=IDD_DIALOG_PROJECT_DEF
Class20=COracleSettingDlg
Resource14=IDD_FILE_NAME_FORMAT
Class21=CAdminDateTimeCtrl
Class22=CRefDataSettingDlg
Resource15=IDD_DRAWING_REPORT
Resource16=IDD_ADMIN_FORM
Class23=CRevisionStageDlg
Class24=CLineManDataImportDlg
Resource17=IDD_HANWHA
Resource18=IDD_DIALOG_PROJECT_DEF (English (U.S.))

[CLS:CAdminApp]
Type=0
BaseClass=CWinApp
HeaderFile=Admin.h
ImplementationFile=Admin.cpp
Filter=N
VirtualFilter=AC
LastObject=ID_APP_CLOSE

[CLS:CAboutDlg]
Type=0
BaseClass=CDialog
HeaderFile=Admin.cpp
ImplementationFile=Admin.cpp
LastObject=CAboutDlg

[CLS:CAdminDoc]
Type=0
BaseClass=CDocument
HeaderFile=AdminDoc.h
ImplementationFile=AdminDoc.cpp

[CLS:CAdminTreeView]
Type=0
BaseClass=CView
HeaderFile=AdminTreeView.h
ImplementationFile=AdminTreeView.cpp

[CLS:CAdminUserDlg]
Type=0
BaseClass=CFormView
HeaderFile=AdminUserDlg.h
ImplementationFile=AdminUserDlg.cpp
Filter=D
VirtualFilter=VWC
LastObject=ID_APP_CLOSE

[CLS:CAdminView]
Type=0
BaseClass=CFormView
HeaderFile=AdminView.h
ImplementationFile=AdminView.cpp

[CLS:CBorderSettingDlg]
Type=0
BaseClass=CDialog
HeaderFile=BorderSettingDlg.h
ImplementationFile=BorderSettingDlg.cpp
Filter=D
VirtualFilter=dWC
LastObject=CBorderSettingDlg

[CLS:CDirTreeCtrl]
Type=0
BaseClass=CTreeCtrl
HeaderFile=DirTreeCtrl.h
ImplementationFile=DirTreeCtrl.cpp

[CLS:CLineDataSettingDlg]
Type=0
BaseClass=CDialog
HeaderFile=LineDataSettingDlg.h
ImplementationFile=DrawingReportSettingDlg.cpp

[CLS:CEditListBox]
Type=0
BaseClass=CListBox
HeaderFile=EditListBox.h
ImplementationFile=EditListBox.cpp

[CLS:CFileNameEditListBox]
Type=0
BaseClass=CEditListBox
HeaderFile=FileNameEditListBox.h
ImplementationFile=FileNameEditListBox.cpp
LastObject=CFileNameEditListBox

[CLS:CFileNameFormatDlg]
Type=0
BaseClass=CDialog
HeaderFile=FileNameFormatDlg.h
ImplementationFile=FileNameFormatDlg.cpp
Filter=D
VirtualFilter=dWC
LastObject=CFileNameFormatDlg

[CLS:CInPlaceFilterEdit]
Type=0
BaseClass=CFilterEdit
HeaderFile=InPlaceFilterEdit.h
ImplementationFile=InPlaceFilterEdit.cpp

[CLS:CIsoEditSettingDlg]
Type=0
BaseClass=CDialog
HeaderFile=IsoEditSettingDlg.h
ImplementationFile=IsoEditSettingDlg.cpp
Filter=W
VirtualFilter=dWC
LastObject=CIsoEditSettingDlg

[CLS:CMainFrame]
Type=0
BaseClass=CFrameWnd
HeaderFile=MainFrm.h
ImplementationFile=MainFrm.cpp

[CLS:CProjectAccessDlg]
Type=0
BaseClass=CFormView
HeaderFile=ProjectAccessDlg.h
ImplementationFile=ProjectAccessDlg.cpp

[CLS:CProjectDefTableDlg]
Type=0
BaseClass=CFormView
HeaderFile=ProjectDefTableDlg.h
ImplementationFile=ProjectDefTableDlg.cpp
Filter=D
VirtualFilter=VWC
LastObject=CProjectDefTableDlg

[CLS:CProjectSettingForm]
Type=0
BaseClass=CFormView
HeaderFile=ProjectSettingForm.h
ImplementationFile=ProjectSettingForm.cpp

[CLS:TTreeCtrl]
Type=0
BaseClass=CTreeCtrl
HeaderFile=TTreeCtrl.h
ImplementationFile=TTreeCtrl.cpp

[DLG:IDD_ABOUTBOX]
Type=1
Class=CAboutDlg
ControlCount=4
Control1=IDC_STATIC,static,1342177283
Control2=IDC_STATIC,static,1342308480
Control3=IDC_STATIC,static,1342308352
Control4=IDOK,button,1342373889

[DLG:IDD_DIALOG_USER]
Type=1
Class=CAdminUserDlg
ControlCount=11
Control1=IDC_LIST_ASSIGNED_USER,SysListView32,1350631429
Control2=IDC_BUTTON_SAVE,button,1342242816
Control3=IDC_LIST_PROJECT_USER,SysListView32,1350631429
Control4=IDC_BUTTON_REFRESH,button,1342242816
Control5=IDC_STATIC_USER,static,1342308865
Control6=IDC_STATIC_ASSIGN_USER,static,1342308865
Control7=IDC_BUTTON_ADD,button,1342242816
Control8=IDC_STATIC_ID,static,1342308864
Control9=IDC_EDIT_ID,edit,1350633600
Control10=IDC_STATIC_AREA,static,1342308864
Control11=IDC_EDIT_AREA,edit,1350631552

[DLG:IDD_ADMIN_FORM]
Type=1
Class=CAdminView
ControlCount=0

[DLG:IDD_BORDER_SETTING]
Type=1
Class=CBorderSettingDlg
ControlCount=4
Control1=IDC_STATIC,static,1342308865
Control2=IDC_EDIT_BORDER_FILE_PATH,edit,1350631552
Control3=IDC_BUTTON_BORDER_FILE,button,1342242816
Control4=IDC_STATIC_EX,static,1342308352

[DLG:IDD_LINE_DATA_SETTING]
Type=1
Class=CLineDataSettingDlg
ControlCount=12
Control1=IDC_LIST_LINE_DATA_FIELD,SysListView32,1350631425
Control2=IDC_STATIC_ISSUE_DATE,button,1342177287
Control3=IDC_BUTTON_NEW,button,1342242816
Control4=IDC_BUTTON_REMOVE,button,1342242816
Control5=IDC_STATIC_SHOW_SET,button,1342177287
Control6=IDC_GROUP_LINE_DATA_MODULE_KEY,button,1342177287
Control7=IDC_LIST_LINE_DATA_MODULE_KEY,SysListView32,1350631429
Control8=IDC_RADIO_ISSUE_DATE_FORMAT_NO_1,button,1342177289
Control9=IDC_RADIO_ISSUE_DATE_FORMAT_NO_2,button,1342177289
Control10=IDC_STATIC_ISSUE_DATE_DELIMITER,static,1342308352
Control11=IDC_EDIT_ISSUE_DATE_DELIMITER,edit,1350631552
Control12=IDC_RADIO_ISSUE_DATE_FORMAT_NO_3,button,1342177289

[DLG:IDD_FILE_NAME_FORMAT]
Type=1
Class=CFileNameFormatDlg
ControlCount=7
Control1=IDOK,button,1342242817
Control2=IDCANCEL,button,1342242816
Control3=IDC_LIST_FILE_NAME_ITEM,listbox,1352728833
Control4=IDC_LISTBUDDY,button,1342275595
Control5=IDC_EDIT_DELIMITER,edit,1350631553
Control6=IDC_BUTTON_ADD,button,1342242816
Control7=IDC_STATIC,button,1342177287

[DLG:IDD_ISO_EDIT_SETTING]
Type=1
Class=CIsoEditSettingDlg
ControlCount=19
Control1=IDC_BUTTON_INPUT_FILE_NAME,button,1342242816
Control2=IDC_BUTTON_OUTPUT_FILE_NAME,button,1342242816
Control3=IDC_STATIC_GROUP_BOX1,button,1342177287
Control4=IDC_STATIC,static,1342308352
Control5=IDC_STATIC,static,1342308352
Control6=IDC_EDIT_INPUT_FILE_NAME,edit,1350633601
Control7=IDC_EDIT_OUTPUT_FILE_NAME,edit,1350633601
Control8=IDC_STATIC_2,static,1342308352
Control9=IDC_EDIT_OUTPUT_ISO_DWG_FOLDER,edit,1350631552
Control10=IDC_STATIC_OUTPUT_GROUP_BOX,button,1342177287
Control11=IDC_STATIC_EX2,static,1342308352
Control12=IDC_GROUPBOX_PRODUCT_BOM_FOLDER,button,1342177287
Control13=IDC_STATIC_PRODUCT_BOM_FOLDER,static,1342308352
Control14=IDC_EDIT_PRODUCT_BOM_FOLDER,edit,1350631552
Control15=IDC_STATIC_PROUDCT_BOM_FOLDER_EX,static,1342308352
Control16=IDC_BUTTON_DATABASE_REFERENCE_KEY,button,1342242816
Control17=IDC_STATIC_DATABASE_REFERENCE_KEY,button,1342177287
Control18=IDC_STATIC,static,1342308352
Control19=IDC_EDIT_DATABASE_REFERENCE_KEY,edit,1350633601

[DLG:IDD_DIALOG_PROJECT_DEF]
Type=1
Class=CProjectDefTableDlg
ControlCount=9
Control1=IDC_BUTTON_DEL,button,1342242816
Control2=IDC_BUTTON_SAVE,button,1342242816
Control3=IDC_BUTTON_RsDel,button,1342242816
Control4=IDC_BUTTON_RvDel,button,1342242816
Control5=IDC_BUTTON_RES_CREATE,button,1342242816
Control6=IDC_BUTTON_REV_CREATE,button,1342242816
Control7=IDC_STATIC_RES,button,1342177287
Control8=IDC_STATIC_REV,button,1342177287
Control9=IDC_STATIC_LINE_DATA,button,1342177287

[DLG:IDD_DIALOG_PROJECT_SETTING]
Type=1
Class=CProjectSettingForm
ControlCount=1
Control1=IDC_BUTTON_SAVE,button,1342242816

[DLG:IDD_ORACLE_SETTING]
Type=1
Class=COracleSettingDlg
ControlCount=11
Control1=IDC_STATIC_IDMS_KEY,button,1342177287
Control2=IDC_STATIC,static,1342308352
Control3=IDC_EDIT_DESIGN_DDL_FILE_PATH,edit,1350631552
Control4=IDC_STATIC_ORAKEY,static,1342308352
Control5=IDC_COMBO_ORAKEY,combobox,1344340226
Control6=IDC_STATIC_IDMSKEY,static,1342308352
Control7=IDC_EDIT_IDMSKEY,edit,1350633601
Control8=IDC_BUTTON_IDMSKEY,button,1342242816
Control9=IDC_STATIC_EX,static,1342308352
Control10=IDC_STATIC_MAPPING,button,1073741831
Control11=IDC_GROUP_MAPPING_FOR_INSERT,button,1342177287

[TB:IDR_MAINFRAME]
Type=1
Class=?
Command1=ID_FILE_NEW
Command2=ID_FILE_OPEN
Command3=ID_FILE_SAVE
Command4=ID_EDIT_CUT
Command5=ID_EDIT_COPY
Command6=ID_EDIT_PASTE
Command7=ID_FILE_PRINT
Command8=ID_APP_ABOUT
CommandCount=8

[MNU:IDR_MAINFRAME]
Type=1
Class=?
Command1=ID_APP_CLOSE
Command2=ID_TO_USERAPP
Command3=ID_APP_EXIT
Command4=ID_APP_ABOUT
CommandCount=4

[ACL:IDR_MAINFRAME]
Type=1
Class=?
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

[DLG:IDD_DRAWING_REPORT]
Type=1
Class=CRefDataSettingDlg
ControlCount=9
Control1=IDC_EDIT_DRAWINGREPORT,edit,1350633600
Control2=IDC_EDIT_ExlColumn,edit,1350762633
Control3=IDC_EDIT_ExlRow,edit,1350762633
Control4=IDC_STATIC,static,1342308864
Control5=IDC_STATIC_ROW,static,1342308352
Control6=IDC_STATIC_EX,static,1342308352
Control7=IDC_STATIC_COL,static,1342308352
Control8=IDC_LIST_DATA_NAME,SysListView32,1350631429
Control9=IDC_CHECK_REV_DATA,button,1342242819

[CLS:COracleSettingDlg]
Type=0
HeaderFile=OracleSettingDlg.h
ImplementationFile=OracleSettingDlg.cpp
BaseClass=CDialog
Filter=D
VirtualFilter=dWC
LastObject=COracleSettingDlg

[DLG:IDD_HANWHA]
Type=1
Class=?
ControlCount=0

[DLG:IDD_DIALOG_PROJECT_DEF (English (U.S.))]
Type=1
Class=CProjectDefTableDlg
ControlCount=4
Control1=IDC_BUTTON_RvDel,button,1342242816
Control2=IDC_BUTTON_REV_CREATE,button,1342242816
Control3=IDC_STATIC_REV,button,1342177287
Control4=IDC_STATIC_LINE_DATA,button,1342177287

[CLS:CAdminDateTimeCtrl]
Type=0
HeaderFile=AdminDateTimeCtrl.h
ImplementationFile=AdminDateTimeCtrl.cpp
BaseClass=CDateTimeCtrl
Filter=W
VirtualFilter=4WC
LastObject=CAdminDateTimeCtrl

[CLS:CRefDataSettingDlg]
Type=0
HeaderFile=RefDataSettingDlg.h
ImplementationFile=RefDataSettingDlg.cpp
BaseClass=CDialog
Filter=D
VirtualFilter=dWC
LastObject=CRefDataSettingDlg

[DLG:IDD_REF_DATA_SETTING]
Type=1
Class=?
ControlCount=26
Control1=IDC_EDIT_SPEC_FILE_FOLDER,edit,1350631552
Control2=IDC_EDIT_STANDARD_NOTE_FILE,edit,1350631552
Control3=IDC_COMBO_UNIT,combobox,1344340227
Control4=IDC_COMBO_DRAWING_TYPE,combobox,1344340227
Control5=IDC_EDIT_WORKING_UNIT,edit,1350639745
Control6=IDC_EDIT_DVCS_E,edit,1350631553
Control7=IDC_EDIT_DVCS_N,edit,1350631553
Control8=IDC_EDIT_DVCS_EL,edit,1350631553
Control9=IDC_STATIC,button,1342177287
Control10=IDC_STATIC,static,1342308352
Control11=IDC_STATIC,static,1342308352
Control12=IDC_STATIC,button,1342177287
Control13=IDC_STATIC,static,1342308352
Control14=IDC_STATIC,static,1342308352
Control15=IDC_STATIC,button,1342177287
Control16=IDC_STATIC,static,1342308352
Control17=IDC_STATIC,button,1342177287
Control18=IDC_STATIC,static,1342308352
Control19=IDC_STATIC,button,1342177287
Control20=IDC_STATIC,button,1342177287
Control21=IDC_STATIC,static,1342308352
Control22=IDC_STATIC,static,1342308352
Control23=IDC_STATIC,static,1342308352
Control24=IDC_STATIC,button,1342177287
Control25=IDC_RADIO1,button,1342308361
Control26=IDC_RADIO2,button,1342177289

[DLG:IDD_REVISION_STAGE]
Type=1
Class=CRevisionStageDlg
ControlCount=50
Control1=IDC_EDIT_GRADE_PREFIX,edit,1350639744
Control2=IDC_EDIT_GRADE_DESC,edit,1350631552
Control3=IDC_BUTTON_GRADE_ADD,button,1342242816
Control4=IDC_BUTTON_GRADE_DEL,button,1342242816
Control5=IDC_LIST_GRADE,SysListView32,1350631425
Control6=IDC_COMBO_GRADE1,combobox,1344340227
Control7=IDC_EDIT_GRADE_SUFFIX1,edit,1350631552
Control8=IDC_BUTTON_ADD1,button,1342242816
Control9=IDC_BUTTON_DEL1,button,1342242816
Control10=IDC_LIST_GRADE1,listbox,1352728833
Control11=IDC_STATIC,button,1342177287
Control12=IDC_COMBO_GRADE2,combobox,1344340227
Control13=IDC_EDIT_GRADE_SUFFIX2,edit,1350631552
Control14=IDC_BUTTON_ADD2,button,1342242816
Control15=IDC_BUTTON_DEL2,button,1342242816
Control16=IDC_LIST_GRADE2,listbox,1352728835
Control17=IDC_COMBO_GRADE3,combobox,1344340227
Control18=IDC_EDIT_GRADE_SUFFIX3,edit,1350631552
Control19=IDC_BUTTON_ADD3,button,1342242816
Control20=IDC_BUTTON_DEL3,button,1342242816
Control21=IDC_LIST_GRADE3,listbox,1352728835
Control22=IDC_COMBO_GRADE4,combobox,1344340227
Control23=IDC_EDIT_GRADE_SUFFIX4,edit,1350631552
Control24=IDC_BUTTON_ADD4,button,1342242816
Control25=IDC_BUTTON_DEL4,button,1342242816
Control26=IDC_LIST_GRADE4,listbox,1352728835
Control27=IDC_COMBO_GRADE5,combobox,1344340227
Control28=IDC_EDIT_GRADE_SUFFIX5,edit,1350631552
Control29=IDC_BUTTON_ADD5,button,1342242816
Control30=IDC_BUTTON_DEL5,button,1342242816
Control31=IDC_LIST_GRADE5,listbox,1352728835
Control32=IDC_EDIT_DRWN_BY,edit,1350631552
Control33=IDC_EDIT_CHECKED_BY,edit,1350631552
Control34=IDC_EDIT_REVIEWED_BY,edit,1350631552
Control35=IDC_EDIT_APPROVED_BY,edit,1350631552
Control36=IDC_CHECK1,button,1342251043
Control37=IDC_CHECK2,button,1342251043
Control38=IDC_CHECK3,button,1342251043
Control39=IDC_CHECK4,button,1342251043
Control40=IDC_CHECK5,button,1342251043
Control41=IDC_STATIC,button,1342177287
Control42=IDC_STATIC,static,1342308352
Control43=IDC_STATIC,static,1342308352
Control44=IDC_STATIC,static,1342308352
Control45=IDC_STATIC,static,1342177287
Control46=IDC_STATIC,static,1342177287
Control47=IDC_STATIC,static,1342177287
Control48=IDC_STATIC,button,1342177287
Control49=IDC_STATIC,static,1342308352
Control50=IDC_STATIC,static,1342177287

[CLS:CRevisionStageDlg]
Type=0
HeaderFile=RevisionStageDlg.h
ImplementationFile=RevisionStageDlg.cpp
BaseClass=CDialog
Filter=D
LastObject=CRevisionStageDlg
VirtualFilter=dWC

[DLG:IDD_LINEMAN_DATA_IMPORT]
Type=1
Class=CLineManDataImportDlg
ControlCount=15
Control1=IDC_BUTTON_SelectExcel,button,1342373888
Control2=IDC_LIST_SHEET_NAME,SysListView32,1350762521
Control3=IDC_EDIT_RowNum,edit,1350762497
Control4=IDC_BUTTON_LINE_MAN_DATA_IMPORT,button,1342373888
Control5=IDC_STATIC_CONVERT,static,1342308864
Control6=IDC_STATIC_ROW_NUMBER,static,1342308864
Control7=IDC_STATIC_ShowExcelName,static,1342308876
Control8=IDC_STATIC_3,button,1342308359
Control9=1122,static,1342308864
Control10=IDC_STATIC_1,button,1342308359
Control11=IDC_BUTTON_UNSELECT_ALL,button,1342242816
Control12=IDC_BUTTON_SELECT_ALL,button,1342242816
Control13=IDC_LIST_CURRENT_LINE_DATA,SysListView32,1350631425
Control14=IDC_STATIC,static,1342308352
Control15=IDC_STATIC,static,1342308352

[CLS:CLineManDataImportDlg]
Type=0
HeaderFile=LineManDataImportDlg.h
ImplementationFile=LineManDataImportDlg.cpp
BaseClass=CFormView
Filter=D
LastObject=CLineManDataImportDlg
VirtualFilter=VWC

[DLG:IDD_WORK_STATUS]
Type=1
Class=?
ControlCount=2
Control1=IDC_STATIC,button,1342177287
Control2=IDC_PROGRESS,msctls_progress32,1350565888

