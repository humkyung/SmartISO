using System;
using System.Collections.Generic;
using System.Drawing;
using System.Linq;
using System.Windows.Forms;
using EDCS.WF.BSL;
using EDCS.WF.DSL;
using FarPoint.Win.Spread;

using Telerik.WinControls.UI;
using TechSun.Theme;
using EDCS.WF.DSL.Entities;

namespace Configuration
{
    public partial class FrmSearchCodeList : KRadForm
    {
        public string mType = string.Empty;

        private enum LTColumn { ID, Name, Description, Last };
        private string[] mColumnName = new string[] { "ID", "Name", "Description" };
        private int[] marColumnWidths = new int[] { 100, 200, 300 };

        public string CodeID = string.Empty;
        public string CodeName = string.Empty;

        public FrmSearchCodeList()
        {
            InitializeComponent();
        }

        public FrmSearchCodeList(bool isIE, string sType)
        {
            InitializeComponent();

            mType = sType;
            InitSpread();
        }

        private void InitSpread()
        {
            SpreadUtil.InitSpread(fpSpreadData, marColumnWidths, mColumnName, null, null, null, null, null, null, Color.LightGray, true, true);
            fpSpreadData.ActiveSheet.Columns[(int)LTColumn.ID].HorizontalAlignment = CellHorizontalAlignment.Center;
        }

        private void SetSpread(string sType, string sCode, string sCodeName)
        {
            FrmProgressStatus.FormShow();
            FrmProgressStatus.SetMarquee();
            FrmProgressStatus.ToolTip = EDCS.WF.BSL.Variable.SharedVariable.ToolTip;
            FrmProgressStatus.ProgressPer = "Load Data.  Please wait!";

            InitSpread();

            var proxy = WcfHelper.CreateProxyNetTcp<IEdcsDac>(EDCS.WF.BSL.Variable.SharedVariable.IsIE);
            using (proxy as IDisposable)
            {
                List<CodeItem> lstCodeItem = new List<CodeItem>();

                switch (sType)
                {
                    case CodeListType.DEPARTMENT:
                        List<EntityGroupInfo> entityGroupInfos = new List<EntityGroupInfo>();
                        var userList = new List<EntityEDB_UserInfo>();
                        proxy.ReadInsaGroupInfo(EDCS.WF.BSL.Variable.SharedVariable.IsIE, ref entityGroupInfos);

                        foreach (EntityGroupInfo item in entityGroupInfos)
                        {
                            lstCodeItem.Add(new CodeItem() { CodeID = item.GroupCode, CodeName = item.GroupName, CodeDescription = string.Empty });
                        }

                        break;
                    case CodeListType.ROLE:

                        List<EntityCommon_Role> lstCommon_Role = proxy.ReadCommon_Role(EDCS.WF.BSL.Variable.SharedVariable.IsIE);

                        foreach (EntityCommon_Role item in lstCommon_Role)
                        {
                            lstCodeItem.Add(new CodeItem() { CodeID = item.RoleID.ToString(), CodeName = item.Name, CodeDescription = item.NameView });
                        }

                        break;
                }

                List<CodeItem> lstCodeItem_Search = new List<CodeItem>();

                if (!string.IsNullOrEmpty(sCodeName.Trim()) || !string.IsNullOrEmpty(sCode.Trim()))
                {
                    lstCodeItem_Search = lstCodeItem.Where(w => (w.CodeID.Contains(sCode) && w.CodeName.Contains(sCodeName))).OrderBy(w => w.CodeName).ToList();
                }
                else
                {
                    lstCodeItem_Search = lstCodeItem.OrderBy(w => w.CodeName).ToList();
                }

                if (lstCodeItem_Search != null)
                {
                    foreach (CodeItem item in lstCodeItem_Search)
                    {
                        int iRows = fpSpreadData.ActiveSheet.Rows.Count++;

                        fpSpreadData.ActiveSheet.Cells[iRows, (int)LTColumn.ID].Value = item.CodeID;
                        fpSpreadData.ActiveSheet.Cells[iRows, (int)LTColumn.Name].Value = item.CodeName;
                        fpSpreadData.ActiveSheet.Cells[iRows, (int)LTColumn.Description].Value = item.CodeDescription;
                    }
                }

                lstCodeItem.Clear();
                lstCodeItem_Search.Clear();

                FrmProgressStatus.FormClose();
            }
        }

        private void radButtonOK_Click(object sender, EventArgs e)
        {
            SetSpread(mType, radTextBoxCode.Text, radTextBoxSearch.Text);
        }

        private void fpSpreadData_CellClick(object sender, CellClickEventArgs e)
        {
            if (fpSpreadData.ActiveSheet != null)
            {
                CodeID = fpSpreadData.ActiveSheet.Cells[e.Row, (int)LTColumn.ID].Text;
                CodeName = fpSpreadData.ActiveSheet.Cells[e.Row, (int)LTColumn.Name].Text;
                this.DialogResult = DialogResult.OK;
            }
        }

        private void RadFormSearchCodeList_Load(object sender, EventArgs e)
        {
            //SetSpread(mType, string.Empty, string.Empty);
        }

        private void radTextBoxSearch_KeyPress(object sender, KeyPressEventArgs e)
        {
            if (e.KeyChar == Convert.ToChar(Keys.Enter))
            {
                SetSpread(mType, radTextBoxCode.Text, radTextBoxSearch.Text);
            }
        }

        private void radTextBoxCode_KeyPress(object sender, KeyPressEventArgs e)
        {
            if (e.KeyChar == Convert.ToChar(Keys.Enter))
            {
                SetSpread(mType, radTextBoxCode.Text, radTextBoxSearch.Text);
            }
        }
    }

    public class CodeItem
    {
        public string CodeID = string.Empty;
        public string CodeName = string.Empty;
        public string CodeDescription = string.Empty;
    }

}
