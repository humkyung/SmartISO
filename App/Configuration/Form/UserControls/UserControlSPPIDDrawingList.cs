using System;
using System.Collections.Generic;
using System.Drawing;
using System.Windows.Forms;
using EDCS.WF.DSL;
using FarPoint.Win.Spread;
using FarPoint.Win.Spread.CellType;
using Telerik.WinControls.UI;
using Configuration.Properties;

using EDCS.WF.DSL.Entities;

namespace Configuration
{
    public partial class UserControlSPPIDDrawingList : UserControlBase
    {
        private enum m_enColumn { DrawingListID, CategoryName, DrawingCount, LabelString, FontColor, Edit, Delete, Last };
        private string[] m_arColumnName = { "DrawingList ID", "Category Name", "Drawing Count", "Label", "Font Color", "Edit", "Delete" };
        private int[] m_arColumnWidth = { 0, 220, 100, 140, 90, 70, 70 };
        private Information m_Information = Information.GetInstance();
        //private UserControlPlantStructure m_ctrlPlantStruture = new UserControlPlantStructure();

        public UserControlSPPIDDrawingList()
        {
            InitializeComponent();
        }

        public override void LoadData()
        {
            InitDropdownListProject();
            SetDropdownListPlantLIst();

            this.radDropDownListProjectList.SelectedIndexChanged += new Telerik.WinControls.UI.Data.PositionChangedEventHandler(this.radDropDownListProjectList_SelectedIndexChanged);
            this.radDropDownListPlant.SelectedIndexChanged += new Telerik.WinControls.UI.Data.PositionChangedEventHandler(this.radDropDownListPlant_SelectedIndexChanged);

            InitControl();
        }

        private void InitDropdownListProject()
        {
            GeneralUtil.SetRadDropDownListProjectNo(EDCS.WF.BSL.Variable.SharedVariable.IsIE, ref radDropDownListProjectList, this._entityEDBInformation, ToolType.SPPID);
        }

        private void SetDropdownListPlantLIst()
        {
            m_Information.Project = (EntitySPTool_Project)radDropDownListProjectList.SelectedItem.DataBoundItem;
            m_Information.Plant = null;
            radDropDownListPlant.Items.Clear();

            var proxy = WcfHelper.CreateProxyNetTcp<IEdcsDac>(EDCS.WF.BSL.Variable.SharedVariable.IsIE);
            using (proxy as IDisposable)
            {
                List<InfoPlant> lstPlant = proxy.GetPlantNames(EDCS.WF.BSL.Variable.SharedVariable.IsIE, m_Information, m_Information.Project, GeneralUtil.InitConnection(m_Information));
                foreach (InfoPlant data in lstPlant)
                {
                    RadListDataItem item = new RadListDataItem(data.SiteName);
                    item.Tag = data;
                    radDropDownListPlant.Items.Add(item);
                }

                if (radDropDownListPlant.Items.Count == 1)
                {
                    radDropDownListPlant.Items[0].Selected = true;
                    m_Information.Plant = (InfoPlant)radDropDownListPlant.SelectedItem.Tag;
                }
            }

            GeneralUtil.SaveRadDropDownListProjectNo(radDropDownListProjectList, this._entityEDBInformation, ToolType.SPPID);
        }

        private void SetPlant()
        {
            if (radDropDownListPlant.SelectedItem != null)
            {
                m_Information.Plant = (InfoPlant)radDropDownListPlant.SelectedItem.Tag;
                InitControl();
            }
        }

        private void radDropDownListProjectList_SelectedIndexChanged(object sender, Telerik.WinControls.UI.Data.PositionChangedEventArgs e)
        {
            SetDropdownListPlantLIst();
        }

        private void radDropDownListPlant_SelectedIndexChanged(object sender, Telerik.WinControls.UI.Data.PositionChangedEventArgs e)
        {
            SetPlant();
        }

        private void InitControl()
        {
            FrmProgressStatus.FormShow();
            FrmProgressStatus.SetMarquee();
            FrmProgressStatus.ToolTip = EDCS.WF.BSL.Variable.SharedVariable.ToolTip;
            FrmProgressStatus.ProgressPer = "Loading data.  Please wait!";

            InitSpread();
            SetSpread();

            FrmProgressStatus.FormClose();
        }

        private void InitSpread()
        {
            fpErrorDrawingListSetting.Reset();

            fpErrorDrawingListSetting.RowSplitBoxPolicy = SplitBoxPolicy.Never;
            fpErrorDrawingListSetting.ColumnSplitBoxPolicy = SplitBoxPolicy.Never;
            fpErrorDrawingListSetting.VerticalScrollBarPolicy = ScrollBarPolicy.AsNeeded;
            fpErrorDrawingListSetting.HorizontalScrollBarPolicy = ScrollBarPolicy.AsNeeded;

            fpErrorDrawingListSetting.Font = new Font(fpErrorDrawingListSetting.Font, FontStyle.Regular);
            fpErrorDrawingListSetting.ActiveSheet.ColumnCount = (int)m_enColumn.Last;
            fpErrorDrawingListSetting.ActiveSheet.RowCount = 0;

            for (int i = 0; i < (int)m_enColumn.Last; i++)
            {
                Column col = fpErrorDrawingListSetting.ActiveSheet.Columns[i];
                col.Label = m_arColumnName[i];
                col.Width = m_arColumnWidth[i];
                col.Locked = true;

                if (col.Label == m_arColumnName[(int)m_enColumn.Delete])
                {
                    ButtonCellType ctDelete = new ButtonCellType();
                    ctDelete.ButtonColor = Color.Red;
                    ctDelete.TextColor = Color.White;
                    ctDelete.UseVisualStyleBackColor = false;
                    ctDelete.Text = "Delete";
                    col.CellType = ctDelete;
                    col.Locked = false;

                }
                else if (col.Label == m_arColumnName[(int)m_enColumn.Edit])
                {
                    ButtonCellType ctEdit = new ButtonCellType();
                    ctEdit.TextColor = Color.FromArgb(12, 109, 199);
                    ctEdit.ButtonColor = Color.FromArgb(233, 233, 236);
                    ctEdit.Text = "Edit";
                    col.CellType = ctEdit;
                    col.Locked = false;
                }
                else
                {
                    TextCellType ctText = new TextCellType();
                    col.CellType = ctText;
                }
            }

            fpErrorDrawingListSetting.ActiveSheet.GrayAreaBackColor = Color.Transparent;
        }

        private void SetSpread()
        {

            fpErrorDrawingListSetting.RowSplitBoxPolicy = SplitBoxPolicy.Never;
            fpErrorDrawingListSetting.ColumnSplitBoxPolicy = SplitBoxPolicy.Never;
            fpErrorDrawingListSetting.VerticalScrollBarPolicy = ScrollBarPolicy.AsNeeded;
            fpErrorDrawingListSetting.HorizontalScrollBarPolicy = ScrollBarPolicy.AsNeeded;

            SheetView sheetView = fpErrorDrawingListSetting.ActiveSheet;
            sheetView.RowCount = 0;

            if (m_Information != null && m_Information.Project != null && m_Information.Plant != null)
            {
                var proxy = WcfHelper.CreateProxyNetTcp<IEdcsDac>(EDCS.WF.BSL.Variable.SharedVariable.IsIE);
                using (proxy as IDisposable)
                {
                    List<EntitySPPID_DrawingList> lstDrawingList = proxy.ReadDrawingListPlantName(EDCS.WF.BSL.Variable.SharedVariable.IsIE, m_Information.Project.ProjectID, m_Information.Plant.SiteName);

                    foreach (EntitySPPID_DrawingList sDrawingList in lstDrawingList)
                    {
                        Color fontColor = Color.Black;
                        string sCategoryName = string.Empty;
                        string sLabelString = string.Empty;
                        int iDrawingCount = 0;

                        sCategoryName = sDrawingList.CategoryName;

                        string sColor = sDrawingList.FontColor;

                        if (!string.IsNullOrEmpty(sColor))
                        {
                            string[] arColor = sColor.Split("%".ToCharArray());
                            fontColor = Color.FromArgb(Convert.ToInt32(arColor[0])
                                                    , Convert.ToInt32(arColor[1])
                                                    , Convert.ToInt32(arColor[2]));
                        }

                        sLabelString = sDrawingList.DisplayName;

                        if (!string.IsNullOrEmpty(sDrawingList.DrawingNo))
                            iDrawingCount = sDrawingList.DrawingNo.Split(";".ToCharArray()).Length;

                        sheetView.RowCount++;
                        sheetView.Cells[sheetView.RowCount - 1, (int)m_enColumn.DrawingListID].Value = sDrawingList.DrawingListID;
                        sheetView.Cells[sheetView.RowCount - 1, (int)m_enColumn.CategoryName].Value = sCategoryName;
                        sheetView.Cells[sheetView.RowCount - 1, (int)m_enColumn.DrawingCount].Value = iDrawingCount;
                        sheetView.Cells[sheetView.RowCount - 1, (int)m_enColumn.LabelString].Value = sLabelString;
                        sheetView.Cells[sheetView.RowCount - 1, (int)m_enColumn.FontColor].BackColor = fontColor;
                    }
                }
            }
        }

        private void radButtonAdd_Click(object sender, EventArgs e)
        {
            if (m_Information.Project == null)
            {
                // 빈값
                MessageBox.Show(string.Format(Resources.Message10021, "Project"), this.Text, MessageBoxButtons.OK, MessageBoxIcon.Information);
                return;
            }

            if (m_Information.Plant == null)
            {
                // 빈값
                MessageBox.Show(string.Format(Resources.Message10021, "Plant"), this.Text, MessageBoxButtons.OK, MessageBoxIcon.Information);
                return;
            }

            FrmSPPIDDrawingListCreate frm = new FrmSPPIDDrawingListCreate();
            if (frm.ShowDialog(this) == DialogResult.OK)
            {
                SetSpread();
            }
        }

        private void fpErrorDrawingListSetting_ButtonClicked(object sender, EditorNotifyEventArgs e)
        {
            try
            {
                var proxy = WcfHelper.CreateProxyNetTcp<IEdcsDac>(EDCS.WF.BSL.Variable.SharedVariable.IsIE);
                using (proxy as IDisposable)
                {
                    string sDrawingListID = fpErrorDrawingListSetting.ActiveSheet.Cells[e.Row, (int)m_enColumn.DrawingListID].Text;

                    if (e.Column == (int)m_enColumn.Delete)
                    {
                        List<long> lstDrawingList = new List<long>();
                        lstDrawingList.Add(long.Parse(sDrawingListID));
                        string sError = string.Empty;
                        proxy.DeleteSPPID_DrawingList(EDCS.WF.BSL.Variable.SharedVariable.IsIE, lstDrawingList, ref sError);
                        SetSpread();
                    }
                    else if (e.Column == (int)m_enColumn.Edit)
                    {
                        FrmSPPIDDrawingListCreate frm = new FrmSPPIDDrawingListCreate(EDCS.WF.BSL.Variable.SharedVariable.IsIE, long.Parse(sDrawingListID));

                        if (frm.ShowDialog(this) == DialogResult.OK)
                        {
                            SetSpread();
                        }
                    }
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

        private void radButtonDeleteAll_Click(object sender, EventArgs e)
        {
            try
            {
                var proxy = WcfHelper.CreateProxyNetTcp<IEdcsDac>(EDCS.WF.BSL.Variable.SharedVariable.IsIE);
                using (proxy as IDisposable)
                {
                    if (m_Information.Project == null)
                    {
                        // 빈값
                        MessageBox.Show(string.Format(Resources.Message10021, "Project"), this.Text, MessageBoxButtons.OK, MessageBoxIcon.Information);
                        return;
                    }

                    if (m_Information.Plant == null)
                    {
                        // 빈값
                        MessageBox.Show(string.Format(Resources.Message10021, "Plant"), this.Text, MessageBoxButtons.OK, MessageBoxIcon.Information);
                        return;
                    }

                    List<long> lstDrawingList = new List<long>();

                    SheetView sheetView = fpErrorDrawingListSetting.ActiveSheet;

                    for (int i = 0; i < sheetView.RowCount; i++)
                    {
                        lstDrawingList.Add(long.Parse(sheetView.Cells[i, (int)m_enColumn.DrawingListID].Text));
                    }

                    if (lstDrawingList.Count > 0)
                    {
                        string sError = string.Empty;
                        proxy.DeleteSPPID_DrawingList(EDCS.WF.BSL.Variable.SharedVariable.IsIE, lstDrawingList, ref sError);
                        SetSpread();
                    }
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
    }
}
