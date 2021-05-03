using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Drawing;
using System.Data;
using System.Linq;
using System.Text;
using System.Windows.Forms;
using EDCS.WF.BSL;
using EDCS.WF.DSL;
using FarPoint.Win.Spread;
using FarPoint.Win.Spread.CellType;
using FarPoint.Win.Spread.Model;
using EDCS.WF.DSL.Entities;
using TechSun.Theme;
using Configuration.Properties;

namespace Configuration
{

    public partial class UserControlCommonValidation_FilterCondition :  UserControlBase
    {
        string mItemType = string.Empty;
        EntitySPTool_Project mSelectedProject = new EntitySPTool_Project();
        private enum menColumn { Check, FilterCondition, Comment, SkipErrorCheck, Edit, Last };
        private string[] mColumnName = new string[] { "Check", "Filter Condition", "Comment", "Skip Error Check", "Edit" };
        private int[] mColumnWidth = new int[] { 50, 300, 300, 150, 100 };
        private bool[] mColumnLock = new bool[] { false, false, false, false, false };
        SpreadUtil.eFarpointCellType[] mColumnType = new SpreadUtil.eFarpointCellType[] { SpreadUtil.eFarpointCellType.CheckBox, SpreadUtil.eFarpointCellType.Text, SpreadUtil.eFarpointCellType.Text, SpreadUtil.eFarpointCellType.Text, SpreadUtil.eFarpointCellType.NormalButton };

        public UserControlCommonValidation_FilterCondition()
        {
            InitializeComponent();
        }
        public UserControlCommonValidation_FilterCondition(EntitySPTool_Project eSelectedProject, string sItemType)
        {
            InitializeComponent();
            mSelectedProject = eSelectedProject;
            mItemType = sItemType;
        }

        public override void LoadData()
        {
            InitSpreadSheet();
        }

        private void InitSpreadSheet()
        {
            string sLoadingTexts = "Loading data.  Please wait.";
            var frmPopupWaitingBarCallMethod = new TechSun.Theme.frmSplashScreen(1, EDCS.WF.BSL.Variable.SharedVariable.ToolTip);
            frmPopupWaitingBarCallMethod.MethodSetStart();
            frmPopupWaitingBarCallMethod.MethodStatusUpdateText(sLoadingTexts);

            try
            {
                //컬럼 세팅(셀타입, Visible, 정렬 등)
                setSpread();

                // FilterCondition 조회
                var proxy = WcfHelper.CreateProxyNetTcp<IEdcsDac>(EDCS.WF.BSL.Variable.SharedVariable.IsIE);
                using (proxy as IDisposable)
                {
                    List<EntitySPPID_FilterCondition> lstFilterCondition = proxy.ReadSPPID_FilterCondition(EDCS.WF.BSL.Variable.SharedVariable.IsIE, mSelectedProject.ProjectID, TagNoSetting.Validation, mItemType);

                    int iRowCount = 0;
                    foreach (EntitySPPID_FilterCondition FilterCondition in lstFilterCondition)
                    {
                        iRowCount++;
                        fpSpreadData.ActiveSheet.Rows.Count = iRowCount;

                        fpSpreadData.ActiveSheet.Cells[iRowCount - 1, (int)menColumn.Check].Value = false;
                        fpSpreadData.ActiveSheet.Cells[iRowCount - 1, (int)menColumn.FilterCondition].Value = FilterCondition.FIlter;
                        fpSpreadData.ActiveSheet.Cells[iRowCount - 1, (int)menColumn.Comment].Value = FilterCondition.Comment;
                        fpSpreadData.ActiveSheet.Cells[iRowCount - 1, (int)menColumn.SkipErrorCheck].Value = FilterCondition.SkipErrorCheckFlag;
                    }
                }

                frmPopupWaitingBarCallMethod.MethodClose();
            }
            catch (Exception ex)
            {
                frmPopupWaitingBarCallMethod.MethodClose();
                new TechSun.Theme.frmWarning(ex.Message.ToString()).ShowDialog(this);
            }
        }

        private void setSpread()
        {
            //컬럼 헤드 초기화
            InitSpread();

            // 컬럼 세팅
            for (int i = 0; i < fpSpreadData.ActiveSheet.Columns.Count; i++)
            {
                switch (i)
                {
                    case (int)menColumn.Check:
                        CheckBoxCellType ctCheckBox = new CheckBoxCellType();
                        fpSpreadData.ActiveSheet.Columns[i].CellType = ctCheckBox;
                        fpSpreadData.ActiveSheet.Columns[i].HorizontalAlignment = CellHorizontalAlignment.Center;
                        break;
                    case (int)menColumn.FilterCondition:
                        TextCellType ctText = new TextCellType();
                        fpSpreadData.ActiveSheet.Columns[i].CellType = ctText;
                        break;
                    case (int)menColumn.Comment:
                        TextCellType ctText1 = new TextCellType();
                        fpSpreadData.ActiveSheet.Columns[i].CellType = ctText1;
                        break;
                    case (int)menColumn.SkipErrorCheck:
                        CheckBoxCellType ctCheckBox1 = new CheckBoxCellType();
                        fpSpreadData.ActiveSheet.Columns[i].CellType = ctCheckBox1;
                        fpSpreadData.ActiveSheet.Columns[i].HorizontalAlignment = CellHorizontalAlignment.Center;
                        break;
                    case (int)menColumn.Edit:
                        ButtonCellType ctButton = new ButtonCellType();
                        ctButton.Text = mColumnName[(int)menColumn.Edit];
                        fpSpreadData.ActiveSheet.Columns[i].CellType = ctButton;
                        fpSpreadData.ActiveSheet.Columns[i].HorizontalAlignment = CellHorizontalAlignment.Center;
                        break;
                }
            }
        }

        public void InitSpread()
        {
            SpreadUtil.InitSpread(fpSpreadData, mColumnWidth, mColumnName, mColumnLock, null, null, mColumnType, null, null, Color.LightGray, true, true);
        }

        private void fpSpreadData_CellClick(object sender, CellClickEventArgs e)
        {
            // for'Check-All' cell Clicked
            if (e.Column == (int)menColumn.Check)
            {
                CheckBoxCellType checkBoxCellType1 = new CheckBoxCellType();
                HitTestInformation hti;
                HeaderHitTestInformation hi;
                hti = fpSpreadData.HitTest(e.X, e.Y);
                hi = hti.HeaderInfo;
                if (hti.Type == HitTestType.ColumnHeader)
                {
                    if (hi.Column == 0)
                    {
                        bool check = !Convert.ToBoolean(fpSpreadData.ActiveSheet.ColumnHeader.Cells[0, hi.Column].Value);

                        fpSpreadData.ActiveSheet.ColumnHeader.Cells[0, hi.Column].Value = check;

                        SheetView sheetView = fpSpreadData.ActiveSheet;

                        if (sheetView != null)
                        {
                            for (int i = 0; i < sheetView.Rows.Count; i++)
                            {
                                sheetView.Cells[i, 0].Value = check;
                            }
                        }
                    }
                }
            }
        }
        /// <summary>
        /// FilterCondition Setting 신규 생성과 함께 설정된 data를 받아온다
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        private void radButtonCreate_Click(object sender, EventArgs e)
        {
            fpSpreadData.ActiveSheet.Rows.Count++;

            fpSpreadData.ActiveSheet.Cells[fpSpreadData.ActiveSheet.Rows.Count - 1, (int)menColumn.Check].Value = false;
            fpSpreadData.ActiveSheet.Cells[fpSpreadData.ActiveSheet.Rows.Count - 1, (int)menColumn.FilterCondition].Value = string.Empty;
            fpSpreadData.ActiveSheet.Cells[fpSpreadData.ActiveSheet.Rows.Count - 1, (int)menColumn.Comment].Value = string.Empty;
            fpSpreadData.ActiveSheet.Cells[fpSpreadData.ActiveSheet.Rows.Count - 1, (int)menColumn.SkipErrorCheck].Value = string.Empty;
            fpSpreadData.ActiveSheet.Cells[fpSpreadData.ActiveSheet.Rows.Count - 1, (int)menColumn.Edit].Value = string.Empty;

            // Setting pop-up 열기
            FrmCommonValidation_FilterConditionSetting frmFilterConditionSetting = new FrmCommonValidation_FilterConditionSetting(mSelectedProject, mItemType);

        }

        ///// <summary>
        ///// SPPID_FilterCondition 저장
        ///// </summary>
        public override void SaveData()
        {
            bool bResult = false;
            string sMessage = string.Empty;

            // Activesheet 로부터 Datatable을 만든다
            DataTable dtFilterCondition = SetDatatableFilterCondition(false);

            var proxy = WcfHelper.CreateProxyNetTcp<IEdcsDac>(EDCS.WF.BSL.Variable.SharedVariable.IsIE);
            using (proxy as IDisposable)
            {
                bResult = proxy.SaveSPPID_FilterCondition(EDCS.WF.BSL.Variable.SharedVariable.IsIE, mSelectedProject.ProjectID, mItemType, TagNoSetting.Validation, dtFilterCondition, ref sMessage);
            }

            DeleteSelectedRow();
            
            // 결과 메세지 출력
            if (bResult != false)
                new frmInfomation(Resources.Message10000).ShowDialog(this);
            else
                new frmWarning(sMessage).ShowDialog(this);
        }

        /// <summary>
        /// SpreadSheet 내 'Edit'버튼 클릭 : spread cellclick 이벤트 방식보다 안정적
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        private void fpSpreadData_ButtonClicked(object sender, EditorNotifyEventArgs e)
        {
            if (e.Column == (int)menColumn.Edit)
            {
                // Activesheet 로부터 Datatable을 만든다
                DataTable dtFilterCondition = SetDatatableFilterCondition(true);

                // Setting form 열기
                FrmCommonValidation_FilterConditionSetting frmFilterConditionSetting = new FrmCommonValidation_FilterConditionSetting(mSelectedProject, mItemType, dtFilterCondition);
                
                if (frmFilterConditionSetting.ShowDialog(this) == DialogResult.OK)
                {
                    fpSpreadData.ActiveSheet.Cells[e.Row, (int)menColumn.FilterCondition].Value = frmFilterConditionSetting.m_sFilter;
                    fpSpreadData.ActiveSheet.Cells[e.Row, (int)menColumn.Comment].Text = frmFilterConditionSetting.m_sComment;
                    fpSpreadData.ActiveSheet.Cells[e.Row, (int)menColumn.SkipErrorCheck].Value = frmFilterConditionSetting.m_bSkip;
                    //fpSpreadData.ActiveSheet.Rows[e.Row].Height = 18;
                }
            }
        }
        
        private DataTable SetDatatableFilterCondition(bool bEditMode)
        {
            DataTable dtFilterCondition = new DataTable("Table1");

            dtFilterCondition.Columns.Add("FilterConditionID", typeof(long));
            dtFilterCondition.Columns.Add("ProjectID", typeof(long));
            dtFilterCondition.Columns.Add("Type");
            dtFilterCondition.Columns.Add("ItemType");
            dtFilterCondition.Columns.Add("FIlter");
            dtFilterCondition.Columns.Add("Color");
            dtFilterCondition.Columns.Add("Comment");
            dtFilterCondition.Columns.Add("SkipErrorCheckFlag", typeof(bool));
            dtFilterCondition.Columns.Add("SortOrder", typeof(int));


            int iSortOrder = 0;
            for (int iRow = 0; iRow < fpSpreadData.ActiveSheet.RowCount; iRow++)
            {
                // Edit 버튼을 통해서 datatable을 만드는지 확인
                if(bEditMode == true)
                {
                    if (fpSpreadData.ActiveSheet.ActiveRow.Index != iRow)
                        continue;
                }
                else
                {   
                    // save 수행중에 Datatable을 만드는 것이라면 내용이 없는 row는 삭제 준비.
                    if (string.IsNullOrEmpty(fpSpreadData.ActiveSheet.GetValue(iRow, (int)menColumn.FilterCondition).ToString()))
                    {
                        fpSpreadData.ActiveSheet.Cells[iRow, (int)menColumn.Check].Value = true;
                        continue;
                    }
                }

                DataRow drNew = dtFilterCondition.NewRow();

                drNew["ProjectID"] = mSelectedProject.ProjectID;
                drNew["Type"] = TagNoSetting.Validation;
                drNew["ItemType"] = mItemType;
                drNew["FIlter"] = fpSpreadData.ActiveSheet.GetValue(iRow, (int)menColumn.FilterCondition);

                // color 필드는 Configuration에서 사용하지 않고 SPPID의 MarkByFilterCondition 메뉴에서 사용 예정.
                drNew["Color"] = string.Empty;
                drNew["Comment"] = fpSpreadData.ActiveSheet.Cells[iRow, (int)menColumn.Comment].Text.ToString();

                if (fpSpreadData.ActiveSheet.Cells[iRow, (int)menColumn.SkipErrorCheck].Text.ToUpper().Equals("TRUE"))
                    drNew["SkipErrorCheckFlag"] = true;
                else
                    drNew["SkipErrorCheckFlag"] = false;

                drNew["SortOrder"] = iSortOrder;

                dtFilterCondition.Rows.Add(drNew);
                iSortOrder++;
            }

            return dtFilterCondition;
        }

        private void radButtonDelete_Click(object sender, EventArgs e)
        {
            DeleteSelectedRow();  
        }
        private void DeleteSelectedRow()
        {
            for (int iRow = fpSpreadData.ActiveSheet.RowCount; iRow > 0; iRow--)
            {
                if (fpSpreadData.ActiveSheet.Cells[iRow-1, (int)menColumn.Check].Text.ToUpper().Equals("TRUE"))
                    fpSpreadData.ActiveSheet.Rows[iRow-1].Remove();


            }
        }
    }
}
