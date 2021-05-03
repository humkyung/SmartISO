using System;
using System.Collections.Generic;
using System.Drawing;
using System.Linq;
using System.Windows.Forms;
using CommonUtil.CommonLib;
using EDCS.WF.BSL;
using EDCS.WF.DSL;
using FarPoint.Win.Spread;
using FarPoint.Win.Spread.Model;
using Telerik.WinControls.UI;
using Configuration.Properties;
using TechSun.Theme;

namespace Configuration
{
    public partial class FrmSPPIDDrawingListCreate : KRadForm
    {
        private const string CONST_COLUMN_NAME = "Drawing No";
        private const int CONST_COLUMN_WIDTH = 200;

        private Information m_Information = Information.GetInstance();

        private long m_lDrawingListID = -1;

        private string m_sSettingPath = string.Empty;
        private bool m_IsAddForm = true;

        public string m_CategoryName = string.Empty;
        public string m_CategoryName_Before = string.Empty;
        public string m_LabelString = string.Empty;
        public Color m_FontColor = Color.Black;

        public FrmSPPIDDrawingListCreate()
        {
            m_lDrawingListID = -1;

            m_IsAddForm = true;

            InitializeComponent();
        }

        public FrmSPPIDDrawingListCreate(bool isIE, long lDrawingListID)
        {

            m_lDrawingListID = lDrawingListID;

            m_IsAddForm = false;

            InitializeComponent();
        }

        private void radButtonClose_Click(object sender, EventArgs e)
        {
            this.DialogResult = DialogResult.Cancel;
        }

        private void SetSpread(string sDrawingNo)
        {
            if (!string.IsNullOrEmpty(sDrawingNo))
            {
                List<string> lstDrawingNo = sDrawingNo.Split(";".ToCharArray()).ToList();

                fpSpreadDrawingList.ActiveSheet.RowCount = lstDrawingNo.Count;

                for (int i = 0; i < lstDrawingNo.Count; i++)
                {
                    fpSpreadDrawingList.ActiveSheet.Cells[i, 0].Text = Util.NullToSpace(lstDrawingNo[i]);
                }

                SetRowCount();
            }
        }

        private void InitSpread()
        {
            fpSpreadDrawingList.Reset();
            fpSpreadDrawingList.ActiveSheet.Columns.Count = 1;
            fpSpreadDrawingList.ActiveSheet.Rows.Count = 0;
            fpSpreadDrawingList.ActiveSheet.Columns[0].Label = CONST_COLUMN_NAME;
            fpSpreadDrawingList.ActiveSheet.Columns[0].Width = CONST_COLUMN_WIDTH;
            SpreadUtil.SetAllColumnLock(fpSpreadDrawingList.ActiveSheet);
            SpreadUtil.SetAllColumnAutoFilterAndSort(fpSpreadDrawingList.ActiveSheet);
        }

        private void radButtonAddAll_Click(object sender, EventArgs e)
        {
            FrmProgressStatus.FormShow();
            FrmProgressStatus.SetMarquee();
            FrmProgressStatus.ToolTip = EDCS.WF.BSL.Variable.SharedVariable.ToolTip;
            FrmProgressStatus.ProgressPer = "Adding drawing list.  Please wait!";
            List<string> lstDrawingNo = userControlPBS.GetDrawingList();
            fpSpreadDrawingList.ActiveSheet.Rows.Clear();
            fpSpreadDrawingList.ActiveSheet.Rows.Count = lstDrawingNo.Count;

            for (int i = 0; i < lstDrawingNo.Count; i++)
            {
                fpSpreadDrawingList.ActiveSheet.Cells[i, 0].Value = lstDrawingNo[i];
            }

            fpSpreadDrawingList.ActiveSheet.SortRows(0, true, true);
            SetRowCount();

            FrmProgressStatus.FormClose();
        }

        private void radButtonAdd_Click(object sender, EventArgs e)
        {
            FrmProgressStatus.FormShow();
            FrmProgressStatus.SetMarquee();
            FrmProgressStatus.ToolTip = EDCS.WF.BSL.Variable.SharedVariable.ToolTip;
            FrmProgressStatus.ProgressPer = "Adding drawing list.  Please wait!";

            List<string> lstDrawingNo = userControlPBS.GetCheckedList();
            SheetView sheetView = fpSpreadDrawingList.ActiveSheet;

            for (int i = 0; i < lstDrawingNo.Count; i++)
            {
                bool IsExist = false;
                for (int j = 0; j < sheetView.RowCount; j++)
                {
                    if (sheetView.Cells[j, 0].Text.ToUpper() == lstDrawingNo[i].ToUpper())
                    {
                        IsExist = true;
                        break;
                    }
                }

                if (!IsExist)
                {
                    sheetView.RowCount++;
                    sheetView.Cells[sheetView.RowCount - 1, 0].Value = lstDrawingNo[i];
                }
            }
            sheetView.SortRows(0, true, true);
            sheetView.SortColumns(0, true);
            SetRowCount();
            userControlPBS.ResetChecked();
            FrmProgressStatus.FormClose();
        }


        private void radButtonRemoveAll_Click(object sender, EventArgs e)
        {
            FrmProgressStatus.FormShow();
            FrmProgressStatus.SetMarquee();
            FrmProgressStatus.ToolTip = EDCS.WF.BSL.Variable.SharedVariable.ToolTip;
            FrmProgressStatus.ProgressPer = "Removing drawing list.  Please wait!";
            fpSpreadDrawingList.ActiveSheet.Rows.Clear();
            SetRowCount();
            FrmProgressStatus.FormClose();
        }

        private void SetRowCount()
        {
            radLabelRowCount.Text = string.Format("Row Count : {0}", fpSpreadDrawingList.ActiveSheet.Rows.Count);
        }

        private void radButtonRemove_Click(object sender, EventArgs e)
        {
            FrmProgressStatus.FormShow();
            FrmProgressStatus.SetMarquee();
            FrmProgressStatus.ToolTip = EDCS.WF.BSL.Variable.SharedVariable.ToolTip;
            FrmProgressStatus.ProgressPer = "Removing drawing list.  Please wait!";

            SheetView sheetView = fpSpreadDrawingList.ActiveSheet;
            CellRange[] cr = sheetView.GetSelections();

            if (cr.Count() > 0)
            {
                for (int i = cr[0].Row; i < cr[0].Row + cr[0].RowCount; i++)
                {
                    sheetView.Rows[i].Tag = "Delete";
                }
            }

            for (int i = 0; i < sheetView.RowCount; i++)
            {
                if (i < 0)
                    break;

                if (sheetView.Rows[i].Tag != null && sheetView.Rows[i].Tag.ToString() == "Delete")
                {
                    sheetView.Rows[i].Remove();
                    i--;
                }
            }

            SetRowCount();
            FrmProgressStatus.FormClose();
        }

        private void radButtonOk_Click(object sender, EventArgs e)
        {

            try
            {
                var proxy = WcfHelper.CreateProxyNetTcp<IEdcsDac>(EDCS.WF.BSL.Variable.SharedVariable.IsIE);
                using (proxy as IDisposable)
                {
                    m_CategoryName = radTextBoxCategoryName.Text.Trim();
                    m_LabelString = radTextBoxDisplayLabel.Text.Trim();
                    m_FontColor = radLabelFontColor.BackColor;

                    //DataTable dtSettingData = SettingDataSet.Tables[m_CategoryName];
                    SheetView sheetView = fpSpreadDrawingList.ActiveSheet;

                    if (radTextBoxCategoryName.Text == string.Empty)
                    {
                        // 빈값
                        MessageBox.Show(string.Format(Resources.Message10011, "Category"), this.Text, MessageBoxButtons.OK, MessageBoxIcon.Information);
                        return;
                    }

                    if (fpSpreadDrawingList.ActiveSheet.RowCount == 0)
                    {
                        MessageBox.Show(string.Format(Resources.Message10021, "Drawing"), this.Text, MessageBoxButtons.OK, MessageBoxIcon.Information);
                        return;
                    }

                    FrmProgressStatus.FormShow();
                    FrmProgressStatus.SetMarquee();
                    FrmProgressStatus.ToolTip = EDCS.WF.BSL.Variable.SharedVariable.ToolTip;
                    FrmProgressStatus.ProgressPer = "Saving Setting.  Please wait!";

                    string sFontColor = string.Format(@"{0}%{1}%{2}", radLabelFontColor.BackColor.R
                                                  , radLabelFontColor.BackColor.G
                                                  , radLabelFontColor.BackColor.B);


                    List<string> lstDrawing = new List<string>();

                    for (int i = 0; i < sheetView.RowCount; i++)
                    {
                        lstDrawing.Add(sheetView.Cells[i, 0].Text);
                    }

                    string sDrawingNo = GeneralUtil.MergeListToString(lstDrawing, ";");

                    string sError = string.Empty;

                    proxy.SaveSPPID_DrawingList(EDCS.WF.BSL.Variable.SharedVariable.IsIE, m_IsAddForm, m_Information.Project.ProjectID, m_Information.Plant.SiteName, m_lDrawingListID, m_CategoryName, m_LabelString, sFontColor, sDrawingNo, ref sError);

                    FrmProgressStatus.FormClose();
                    this.DialogResult = DialogResult.OK;
                }
            }
            catch (System.ServiceModel.FaultException ex)
            {
                
                new TechSun.Theme.frmWarning(ex.Message.ToString()).ShowDialog(this);
            }
            catch (Exception ex)
            {
                
                new TechSun.Theme.frmWarning(ex.Message.ToString()).ShowDialog(this);
            }
        }

        private void radLabelFontColor_Click(object sender, EventArgs e)
        {
            ColorDialog dlgColor = new ColorDialog();

            DialogResult result = dlgColor.ShowDialog(this);

            if (result.Equals(DialogResult.Cancel))
                return;

            if (dlgColor.Color == new Color() || dlgColor.Color == Color.White)
            {
                MessageBox.Show(string.Format(Resources.Message10021, "Color"), this.Text, MessageBoxButtons.OK, MessageBoxIcon.Information);
                return;
            }
            else
            {
                radLabelFontColor.BackColor = dlgColor.Color;

            }
        }

        private void InitControl()
        {
            radTextBoxCategoryName.Text = string.Empty;
            radTextBoxDisplayLabel.Text = string.Empty;
            radLabelFontColor.BackColor = Color.Black;
            userControlPBS.SetPBSData(EDCS.WF.BSL.Variable.SharedVariable.IsIE);
        }

        private void SetControl(ref string sDrawingNo)
        {
            if (!m_IsAddForm)
            {
                var proxy = WcfHelper.CreateProxyNetTcp<IEdcsDac>(EDCS.WF.BSL.Variable.SharedVariable.IsIE);
                using (proxy as IDisposable)
                {
                    List<EntitySPPID_DrawingList> lstDrawinglist = proxy.ReadDrawingList(EDCS.WF.BSL.Variable.SharedVariable.IsIE, m_lDrawingListID);

                    foreach (EntitySPPID_DrawingList spDrawinglist in lstDrawinglist)
                    {
                        radTextBoxCategoryName.Text = spDrawinglist.CategoryName;
                        radTextBoxDisplayLabel.Text = spDrawinglist.DisplayName;

                        string sColor = spDrawinglist.FontColor;
                        string[] arColor = sColor.Split("%".ToCharArray());
                        radLabelFontColor.BackColor = Color.FromArgb(Convert.ToInt32(arColor[0])
                                                                    , Convert.ToInt32(arColor[1])
                                                                    , Convert.ToInt32(arColor[2]));

                        sDrawingNo = spDrawinglist.DrawingNo;
                    }
                }
            }
        }

        private void FrmDrawingList_Create_Load(object sender, EventArgs e)
        {
            InitControl();
            //InitSpread
            InitSpread();
            string sDrawingNo = string.Empty;
            SetControl(ref sDrawingNo);
            //SetSpread
            SetSpread(sDrawingNo);
        }
    }
}
