using System;
using System.Collections.Generic;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Windows.Forms;
using CommonUtil.CommonLib;
using EDCS.WF.DSL;

using Telerik.WinControls.UI;
using Configuration.Properties;
using EDCS.WF.DSL.Entities;

namespace Configuration
{
    public partial class UserControlSPPIDProjectInfoCopy : UserControlBase
    {
        public Information mInformation;
        public long mCopyProjectFrom;
        public long mCopyProjectMasterFrom;
        public long mCopyProjectTo;
        public long mCopyProjectMasterTo;
        public long mSystemID;
        private const string CONST_DEFAULT = "Default";
        // 본메뉴에서는 불필요하지만 Edcs로 넘기기 위해 설정
        public int mListType = -1;
        public long mErrorCheckID = -1;
        public long mColumnGroupID = -1;

        private const int CONST_DEFAULT_ORDER = 999999999;

        public UserControlSPPIDProjectInfoCopy()
        {
            InitializeComponent();

        }

        public override void LoadData()
        {
            InitDropdownListProject();
            mInformation = Information.GetInstance();
            GetSPPIDSystemID();
        }

        /// <summary>
        /// Load Project List
        /// </summary>
        private void InitDropdownListProject()
        {
            radDropDownListProjectList_Default.Items.Clear();
            radDropDownListProjectDBList_TO.Items.Clear();
            radDropDownListProjectDBList_FROM.Items.Clear();

            var proxy = WcfHelper.CreateProxyNetTcp<IEdcsDac>(EDCS.WF.BSL.Variable.SharedVariable.IsIE);
            using (proxy as IDisposable)
            {
                List<EntitySPTool_Project> vSPToolProjectList = proxy.ReadAllSPPIDProjectDetailInfo(EDCS.WF.BSL.Variable.SharedVariable.IsIE).Where(w => w.SPToolType == ToolType.SPPID).OrderByDescending(o => o.ProjectMasterID).ThenByDescending(t => t.DefaultProjectFlag).ToList();

                // Set Project DB
                foreach (var result in vSPToolProjectList)
                {
                    RadListDataItem item_Default = new RadListDataItem(result.ProjectNo);
                    RadListDataItem item_DBTO = new RadListDataItem(result.ProjectNo);
                    RadListDataItem item_DBFROM = new RadListDataItem(result.ProjectNo);
                    RadListDataItem item_TO = new RadListDataItem(result.ProjectNo);
                    RadListDataItem item_FROM = new RadListDataItem(result.ProjectNo);

                    item_Default.Tag = result;
                    item_DBTO.Tag = result;
                    item_DBFROM.Tag = result;
                    item_TO.Text = result.ProjectNo;
                    item_FROM.Text = result.ProjectNo;
                    item_TO.Tag = result;
                    item_FROM.Tag = result;

                    /// comboBox ProjectDBList~, ProjectList_Default 에는 project전체를 list-up
                    radDropDownListProjectList_Default.Items.Add(item_Default);
                    radDropDownListProjectDBList_TO.Items.Add(item_DBTO);
                    radDropDownListProjectDBList_FROM.Items.Add(item_DBFROM);

                    /// comboBox ProjectList~ 에는 MASTER project list-up
                    if (continueAddProjectList(radDropDownListProjectList_TO, item_TO) == true)
                        radDropDownListProjectList_TO.Items.Add(item_TO);
                    if (continueAddProjectList(radDropDownListProjectList_FROM, item_FROM) == true)
                        radDropDownListProjectList_FROM.Items.Add(item_FROM);
                }
            }
        }

        /// <summary>
        /// comboBox ProjectList~ 에 추가할지 결정하는 함수
        /// 추가로 MASTER project중  DefaultProject만 list-up 진행
        /// </summary>
        /// <param name="ProjectList"></param>
        /// <param name="NewItem"></param>
        /// <returns></returns>
        private bool continueAddProjectList(RadDropDownList ProjectList, RadListDataItem NewItem)
        {
            bool bExistProject = false;

            foreach (RadListDataItem Item in ProjectList.Items)
            {
                EntitySPTool_Project ExistProject = (EntitySPTool_Project)Item.Tag;
                EntitySPTool_Project NewProject = (EntitySPTool_Project)NewItem.Tag;

                if (ExistProject.ProjectMasterID == NewProject.ProjectMasterID)
                {
                    bExistProject = true;

                    if (NewProject.DefaultProjectFlag == true)
                    {
                        ProjectList.Items.Remove(Item);
                        return true;
                    }
                }

            }

            // ProjectList item이 0개 이거나 동일한 ProjectMasterID가 없을시 return true
            if (bExistProject == false)
                return true;
            else
                return false;
        }

        private void GetSPPIDSystemID()
        {
            var proxy = WcfHelper.CreateProxyNetTcp<IEdcsDac>(EDCS.WF.BSL.Variable.SharedVariable.IsIE);
            using (proxy as IDisposable)
            {
                //Read System table
                List<EntityCommon_System> lstCommon_System = proxy.ReadCommon_System(EDCS.WF.BSL.Variable.SharedVariable.IsIE);

                //사용 시스템 : SPPID 고정
                foreach (EntityCommon_System cmSystem in lstCommon_System)
                {
                    if (cmSystem.ToolType == ToolType.SPPID)
                        mSystemID = cmSystem.SystemID;
                }
            }
        }

        private void radDropDownListProjectList_Default_SelectedIndexChanged(object sender, Telerik.WinControls.UI.Data.PositionChangedEventArgs e)
        {
            SetSchemaInfomation(radDropDownListProjectList_Default);
        }

        private void SetSchemaInfomation(RadDropDownList radDropDownList)
        {
            string sError = string.Empty;
            GeneralUtil.SetSchemaInfoCopy(EDCS.WF.BSL.Variable.SharedVariable.IsIE, radDropDownList, ref mInformation, ref sError);
        }

        public static void SetItemTypeList(List<string> lstItemType)
        {
            foreach (string sitemType in Information.ITEMTYPE)
            {
                lstItemType.Add(sitemType);
            }
        }
        /// <summary>
        /// TO ProjectDBList 변경시 From ProjectDBList에 폰트색상 변경,  Project/Project Master 정보 변경
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        private void radDropDownListProjectDBList_TO_SelectedIndexChanged(object sender, Telerik.WinControls.UI.Data.PositionChangedEventArgs e)
        {
            SetSchemaInfomation(radDropDownListProjectDBList_TO);

            // 차후 Project To Project에서 사용
            mCopyProjectTo = mInformation.Project.ProjectID;
            mCopyProjectMasterTo = mInformation.Project.ProjectMasterID;

            // change status radDropDownListProjectDBList_FROM 
            string sCopyToProject = radDropDownListProjectDBList_TO.SelectedItem.Text;

            for (int i = 0; i < radDropDownListProjectDBList_FROM.Items.Count; i++)
            {
                if (sCopyToProject == radDropDownListProjectDBList_FROM.Items[i].Text)
                    radDropDownListProjectDBList_FROM.Items[i].ForeColor = Color.LightGray;
                else
                    radDropDownListProjectDBList_FROM.Items[i].ForeColor = Color.Black;

            }
        }
        /// <summary>
        /// From ProjectDBList 변경시 TO ProjectDBList에 폰트색상 변경, Project/Project Master 정보 변경
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        private void radDropDownListProjectDBList_FROM_SelectedIndexChanged(object sender, Telerik.WinControls.UI.Data.PositionChangedEventArgs e)
        {
            SetSchemaInfomation(radDropDownListProjectDBList_FROM);

            // 차후 Project To Project에서 사용
            mCopyProjectFrom = mInformation.Project.ProjectID;
            mCopyProjectMasterFrom = mInformation.Project.ProjectMasterID;

            // change status radDropDownListProjectDBList_TO 
            string sCopyFromProject = radDropDownListProjectDBList_FROM.SelectedItem.Text;

            for (int i = 0; i < radDropDownListProjectDBList_TO.Items.Count; i++)
            {
                if (sCopyFromProject == radDropDownListProjectDBList_TO.Items[i].Text)
                    radDropDownListProjectDBList_TO.Items[i].ForeColor = Color.LightGray;
                else
                    radDropDownListProjectDBList_TO.Items[i].ForeColor = Color.Black;
            }
        }
        /// <summary>
        /// TO ProjectList 변경시 From ProjectList에 폰트색상 변경,  Project/Project Master 정보 변경
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        private void radDropDownListProjectList_TO_SelectedIndexChanged(object sender, Telerik.WinControls.UI.Data.PositionChangedEventArgs e)
        {
            SetSchemaInfomation(radDropDownListProjectList_TO);

            // 차후 Project To Project에서 사용
            mCopyProjectTo = mInformation.Project.ProjectID;
            mCopyProjectMasterTo = mInformation.Project.ProjectMasterID;

            // change status radDropDownListProjectList_FROM 
            string sCopyToProject = radDropDownListProjectList_TO.SelectedItem.Text;

            for (int i = 0; i < radDropDownListProjectList_FROM.Items.Count; i++)
            {
                if (sCopyToProject == radDropDownListProjectList_FROM.Items[i].Text)
                    radDropDownListProjectList_FROM.Items[i].ForeColor = Color.LightGray;
                else
                    radDropDownListProjectList_FROM.Items[i].ForeColor = Color.Black;

            }
        }
        /// <summary>
        /// From ProjectList 변경시 To ProjectList에 폰트색상 변경,  Project/Project Master 정보 변경
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        private void radDropDownListProjectList_FROM_SelectedIndexChanged(object sender, Telerik.WinControls.UI.Data.PositionChangedEventArgs e)
        {
            SetSchemaInfomation(radDropDownListProjectList_FROM);

            // 차후 Project To Project에서 사용
            mCopyProjectFrom = mInformation.Project.ProjectID;
            mCopyProjectMasterFrom = mInformation.Project.ProjectMasterID;

            // change status radDropDownListProjectDBList_TO 
            string sCopyFromProject = radDropDownListProjectList_FROM.SelectedItem.Text;

            for (int i = 0; i < radDropDownListProjectList_TO.Items.Count; i++)
            {
                if (sCopyFromProject == radDropDownListProjectList_TO.Items[i].Text)
                    radDropDownListProjectList_TO.Items[i].ForeColor = Color.LightGray;
                else
                    radDropDownListProjectList_TO.Items[i].ForeColor = Color.Black;
            }
        }
        /// <summary>
        /// start Copy default Attribute 
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        private void radButtonDefaultCopy_Click(object sender, EventArgs e)
        {
            try
            {
                if (radDropDownListProjectList_Default.SelectedItem == null)
                    return;

                if (radCheckBox_DefaultAttribute.Checked == false)
                    return;

                // Delete 'SPPID_DataColumnGroup', 'SPPID_DataColumnGroupValue'?
                bool bDelete_DataColumnGroup = false;
                if (MessageBox.Show(string.Format(Resources.Message10016, "Attribute Set"), this.Text, MessageBoxButtons.YesNo) == DialogResult.Yes)
                    bDelete_DataColumnGroup = true;

                CopyDefaultProjectInfo(bDelete_DataColumnGroup);
            }
            catch (System.ServiceModel.FaultException ex)
            {

                new TechSun.Theme.frmWarning(ex.Message.ToString()).ShowDialog(this);
            }
        }
        /// <summary>
        /// 신규 프로젝트에 default Attribute 정보를 복사
        /// </summary>
        /// <returns></returns>
        private void CopyDefaultProjectInfo(bool bDelete_DataColumnGroup)
        {

            DataTable dtSPPID_DataFilter = new DataTable("SPPID_DataFilter");
            dtSPPID_DataFilter.Columns.Add("DataFilterID", typeof(long));
            dtSPPID_DataFilter.Columns.Add("ProjectID", typeof(long));
            dtSPPID_DataFilter.Columns.Add("ItemType", typeof(String));
            dtSPPID_DataFilter.Columns.Add("AttName", typeof(String));
            dtSPPID_DataFilter.Columns.Add("ColumnName", typeof(String));
            dtSPPID_DataFilter.Columns.Add("Path", typeof(String));
            dtSPPID_DataFilter.Columns.Add("ColumnOrder", typeof(int));

            DataTable dtSPPID_DataFilter_ColumnOrder = new DataTable("SPPID_DataFilter_ColumnOrder");
            dtSPPID_DataFilter_ColumnOrder.Columns.Add("DataFilterID", typeof(long));
            dtSPPID_DataFilter_ColumnOrder.Columns.Add("ProjectID", typeof(long));
            dtSPPID_DataFilter_ColumnOrder.Columns.Add("ItemType", typeof(String));
            dtSPPID_DataFilter_ColumnOrder.Columns.Add("AttName", typeof(String));
            dtSPPID_DataFilter_ColumnOrder.Columns.Add("ColumnName", typeof(String));
            dtSPPID_DataFilter_ColumnOrder.Columns.Add("Path", typeof(String));
            dtSPPID_DataFilter_ColumnOrder.Columns.Add("ColumnOrder", typeof(int));

            DataTable dtSPPID_AttributeSetValue = new DataTable("SPPID_DataColumnGroupValue");
            dtSPPID_AttributeSetValue.Columns.Add("ColumnID", typeof(long));
            dtSPPID_AttributeSetValue.Columns.Add("ColumnGroupID", typeof(long));
            dtSPPID_AttributeSetValue.Columns.Add("Name", typeof(String));
            dtSPPID_AttributeSetValue.Columns.Add("ColumnOrder", typeof(int));

            FrmProgressStatus.FormShow();
            FrmProgressStatus.SetMarquee();
            FrmProgressStatus.ToolTip = EDCS.WF.BSL.Variable.SharedVariable.ToolTip;
            FrmProgressStatus.ProgressPer = "Copy attribute.  Please wait.";
            string[] arAttributes = null;

            try
            {
                var proxy = WcfHelper.CreateProxyNetTcp<IEdcsDac>(EDCS.WF.BSL.Variable.SharedVariable.IsIE);
                using (proxy as IDisposable)
                {
                    if (mInformation != null)
                    {
                        List<string> lstItemType = new List<string>();
                        SetItemTypeList(lstItemType);

                        // foreach (Item Type)
                        foreach (string sItemType in lstItemType)
                        {
                            //Read All Attribute Data
                            DataTable dtDataFilter = proxy.GetDataFilter(EDCS.WF.BSL.Variable.SharedVariable.IsIE, mInformation, sItemType);

                            //Read Default table (SPPID_Default_DataFilter)
                            List<EntitySPPID_Default_DataFilter> lstDefaultDataFilter = proxy.ReadSPPID_Default_DataFilter(EDCS.WF.BSL.Variable.SharedVariable.IsIE, sItemType);

                            // Initialize
                            int iDataFilterCnt = 0;
                            dtSPPID_DataFilter_ColumnOrder.Clear();
                            dtSPPID_DataFilter.Clear();

                            if (dtDataFilter.Rows.Count > 0)
                            {
                                if (lstDefaultDataFilter.Count > 0)
                                {
                                    // foreach (All Attribute Data)
                                    foreach (DataRow dr in dtDataFilter.Rows)
                                    {
                                        bool bNotUseFlag = true;
                                        string sAtt_Name = Util.NullToSpace(dr["ATT_NAME"]);
                                        string sNAME = Util.NullToSpace(dr["NAME"]);
                                        string sPATH = Util.NullToSpace(dr["PATH"]);

                                        // search target Attribute name
                                        EntitySPPID_Default_DataFilter DataFilter = lstDefaultDataFilter.Where(w => w.ColumnName == sNAME).FirstOrDefault();

                                        if (DataFilter != null)
                                        {
                                            bNotUseFlag = false;
                                        }

                                        if (bNotUseFlag)
                                        {
                                            // 불일치하는것 => Data Filter(미사용)
                                            DataRow drInsertrow = dtSPPID_DataFilter.NewRow();
                                            drInsertrow["ProjectID"] = mInformation.Project.ProjectID;
                                            drInsertrow["ItemType"] = sItemType;
                                            drInsertrow["AttName"] = sAtt_Name;
                                            drInsertrow["ColumnName"] = sNAME;
                                            drInsertrow["Path"] = sPATH;
                                            drInsertrow["ColumnOrder"] = iDataFilterCnt++;

                                            dtSPPID_DataFilter.Rows.Add(drInsertrow);
                                        }
                                        else
                                        {
                                            // 일치하는것 => Data Filter ColumnOrder(사용)
                                            DataRow drInsertrow = dtSPPID_DataFilter_ColumnOrder.NewRow();
                                            drInsertrow["ProjectID"] = mInformation.Project.ProjectID;
                                            drInsertrow["ItemType"] = sItemType;
                                            drInsertrow["AttName"] = sAtt_Name;
                                            drInsertrow["ColumnName"] = sNAME;
                                            drInsertrow["Path"] = sPATH;
                                            drInsertrow["ColumnOrder"] = DataFilter.ColumnOrder;
                                            dtSPPID_DataFilter_ColumnOrder.Rows.Add(drInsertrow);
                                        }
                                    }

                                    string sError = string.Empty;
                                    proxy.SaveAdminDataFilter(EDCS.WF.BSL.Variable.SharedVariable.IsIE, mInformation.Project.ProjectID, sItemType, dtSPPID_DataFilter, dtSPPID_DataFilter_ColumnOrder, ref sError);

                                    // Delete all/Create SPPID_DataColumnGroup(Set) : "Default", "Admin"
                                    if (bDelete_DataColumnGroup)
                                    {
                                        // find all column group info
                                        var vDataColumnGroup = proxy.ReadDataColumnGroup(EDCS.WF.BSL.Variable.SharedVariable.IsIE, mInformation.Project.ProjectID, sItemType, "", false, true);

                                        // delete all Data column group
                                        foreach (EntitySPPID_DataColumnGroup DataColumnGroup in vDataColumnGroup)
                                        {
                                            proxy.SetDeleteColumnGroupValue(EDCS.WF.BSL.Variable.SharedVariable.IsIE, DataColumnGroup.ColumnGroupID);
                                        }

                                        var vDrfaultAttribute = lstDefaultDataFilter.Where(w => w.DefaultAttributeFlag == true).OrderBy(o => o.ColumnOrder).ToList();

                                        arAttributes = new string[vDrfaultAttribute.Count];

                                        int i = 0;
                                        foreach (EntitySPPID_Default_DataFilter DataFilter in vDrfaultAttribute)
                                        {
                                            arAttributes[i] = DataFilter.ColumnName;
                                            i++;
                                        }

                                        proxy.SetColumnGroupValue(EDCS.WF.BSL.Variable.SharedVariable.IsIE, mColumnGroupID, mInformation.Project.ProjectMasterID, mInformation.Project.ProjectID, CONST_DEFAULT, sItemType, true, "Admin", arAttributes);
                                    }
                                }
                            }

                            lstDefaultDataFilter.Clear();

                        }// End foreach (Item Type)
                    }
                }

                dtSPPID_DataFilter.Clear();
                dtSPPID_DataFilter_ColumnOrder.Clear();
                dtSPPID_AttributeSetValue.Clear();

            }
            catch (Exception ex)
            {
                FrmProgressStatus.FormClose();
                MessageBox.Show(this, ex.ToString(), this.Text);
                return;
            }

            FrmProgressStatus.FormClose();
            MessageBox.Show(this, Resources.Message10000, this.Text);
        }

        /// <summary>
        /// From/TO Project info copy (Master Project 단위)
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        private void radButtonProjectCopy_Click(object sender, EventArgs e)
        {
            if ((radDropDownListProjectList_TO.SelectedItem == null) || (radDropDownListProjectList_TO.SelectedItem == null))
                return;

            if (radCheckBox_CopyErrorCheck.Checked == false)
                return;

            FrmProgressStatus.FormShow();
            FrmProgressStatus.SetMarquee();
            FrmProgressStatus.ToolTip = EDCS.WF.BSL.Variable.SharedVariable.ToolTip;
            FrmProgressStatus.ProgressPer = "Copy Project.  Please wait.";

            try
            {
                CopySelectProjectInfo();

                FrmProgressStatus.FormClose();
                MessageBox.Show(this, Resources.Message10000, this.Text);
            }
            catch (System.ServiceModel.FaultException ex)
            {
                FrmProgressStatus.FormClose();

                new TechSun.Theme.frmWarning(ex.Message.ToString()).ShowDialog(this);
            }
            catch (Exception ex)
            {
                FrmProgressStatus.FormClose();

                new TechSun.Theme.frmWarning(ex.Message.ToString()).ShowDialog(this);
            }
        }

        /// <summary>
        /// 지정한 From/TO Master Project에 Check Error 정보 복사
        /// </summary>
        /// <param name="bDelete_DataColumnGroup"></param>
        private void CopySelectProjectInfo()
        {
            DataTable dtCommon_ErrorCheckDetailValue = new DataTable("dbo.Common_ErrorCheck_Detail");
            dtCommon_ErrorCheckDetailValue.Columns.Add("ErrorCheckDetailID", typeof(long));
            dtCommon_ErrorCheckDetailValue.Columns.Add("ErrorCheckID", typeof(long));
            dtCommon_ErrorCheckDetailValue.Columns.Add("ErrorCheckType", typeof(string));
            dtCommon_ErrorCheckDetailValue.Columns.Add("Value", typeof(string));

            // 복사 소스(from Project)를 현재 Project로 지정
            SetSchemaInfomation(radDropDownListProjectDBList_FROM);

            if (mInformation != null)
            {
                List<string> lstItemType = new List<string>();
                SetItemTypeList(lstItemType);

                // Copy ErrorCheck
                if (radCheckBox_CopyErrorCheck.Checked == true)
                {
                    // foreach (Item Type)
                    foreach (string ItemType in lstItemType)
                    {
                        //Read Default table (SPPID_Default_DataFilter)
                        string sError = string.Empty;

                        var proxy = WcfHelper.CreateProxyNetTcp<IEdcsDac>(EDCS.WF.BSL.Variable.SharedVariable.IsIE);
                        using (proxy as IDisposable)
                        {
                            #region Copy All ErrorCheck data
                            // start Copy All ErrorCheck : 차후 copy item(ex:errorcheck)이 늘어나면 'Checked == true'를 이부분으로 옮겨 진행할것
                            // ErrorCheck id를 참조해 TO project 모두 삭제
                            List<EntityCommon_ErrorCheck> lstErrorCheck_Delete = proxy.ReadErrorCheck(EDCS.WF.BSL.Variable.SharedVariable.IsIE, mCopyProjectMasterTo, mSystemID, ItemType);
                            foreach (EntityCommon_ErrorCheck Error_Check in lstErrorCheck_Delete)
                                proxy.DeleteCommon_ErrorCheckSettings(EDCS.WF.BSL.Variable.SharedVariable.IsIE, Error_Check.ErrorCheckID);

                            // Common_ErrorCheck 복사
                            long lErrorCheckGroupID = -1;
                            List<EntityCommon_ErrorCheck> lstErrorCheck = proxy.ReadErrorCheck(EDCS.WF.BSL.Variable.SharedVariable.IsIE, mCopyProjectMasterFrom, mSystemID, ItemType);
                            foreach (EntityCommon_ErrorCheck ErrorCheckGroup in lstErrorCheck)
                            {
                                // Common_ErrorCheck 생성
                                lErrorCheckGroupID = proxy.AddErrorCheckGroup(EDCS.WF.BSL.Variable.SharedVariable.IsIE, mCopyProjectMasterTo, ItemType, mListType, mSystemID, mErrorCheckID, ErrorCheckGroup.AttributeName, ref sError);

                                // Common_ErrorCheck_Detail 복사 
                                List<EntityCommon_ErrorCheck_Detail> ReadErrorCheckDetail = proxy.ReadErrorCheckDetail(EDCS.WF.BSL.Variable.SharedVariable.IsIE, ErrorCheckGroup.ErrorCheckID);

                                foreach (EntityCommon_ErrorCheck_Detail ErrorCheck_Detail in ReadErrorCheckDetail)
                                {
                                    DataRow drErrorCheckDetailValue = dtCommon_ErrorCheckDetailValue.NewRow();
                                    drErrorCheckDetailValue["ErrorCheckID"] = lErrorCheckGroupID;
                                    drErrorCheckDetailValue["ErrorCheckType"] = ErrorCheck_Detail.ErrorCheckType;
                                    drErrorCheckDetailValue["Value"] = ErrorCheck_Detail.Value; ;
                                    dtCommon_ErrorCheckDetailValue.Rows.Add(drErrorCheckDetailValue);
                                }

                                proxy.SaveCommon_ErrorCheckDetail(EDCS.WF.BSL.Variable.SharedVariable.IsIE, mErrorCheckID, dtCommon_ErrorCheckDetailValue, ref sError);
                            }
                            // Common_ErrorCheck 복사 종료
                            // Common_ErrorCheckDetail 복사 종료
                            #endregion
                        }

                    }// End foreach (Item Type)
                }//End CheckBox : CopyErrorCheck
            }
        }
        /// <summary>
        /// From/TO Project DB info copy (Database 단위)
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        private void radButtonProjectDBCopy_Click(object sender, EventArgs e)
        {
            if ((radDropDownListProjectDBList_TO.SelectedItem == null) || (radDropDownListProjectDBList_TO.SelectedItem == null))
                return;

            if (radCheckBox_CopyAttribute.Checked == false)
                return;

            // Confirm  Delete 'SPPID_DataColumnGroup', 'SPPID_DataColumnGroupValue'?
            bool bDelete_DataColumnGroup = false;
            if (MessageBox.Show(string.Format(Resources.Message10016, "AttributeSet data"), this.Text, MessageBoxButtons.YesNo) == DialogResult.Yes)
                bDelete_DataColumnGroup = true;
            else
                return;

            FrmProgressStatus.FormShow();
            FrmProgressStatus.SetMarquee();
            FrmProgressStatus.ToolTip = EDCS.WF.BSL.Variable.SharedVariable.ToolTip;
            FrmProgressStatus.ProgressPer = "Copy Project DB infomation.  Please wait.";

            try
            {
                CopySelectProjectDBInfo();

                FrmProgressStatus.FormClose();
                MessageBox.Show(this, Resources.Message10000, this.Text);

            }
            catch (System.ServiceModel.FaultException ex)
            {

                FrmProgressStatus.FormClose();
                new TechSun.Theme.frmWarning(ex.Message.ToString()).ShowDialog(this);
            }
            catch (Exception ex)
            {

                FrmProgressStatus.FormClose();
                new TechSun.Theme.frmWarning(ex.Message.ToString()).ShowDialog(this);
            }

        }
        /// <summary>
        /// 지정한 From/TO 프로젝트'DB'에 Attribute 정보를 복사
        /// </summary>
        /// <param name="bDelete_DataColumnGroup"></param>
        private void CopySelectProjectDBInfo()
        {
            // 사용되는 테이블의 컬럼추가
            DataTable dtSPPID_DataFilter = new DataTable("SPPID_DataFilter");
            dtSPPID_DataFilter.Columns.Add("DataFilterID", typeof(long));
            dtSPPID_DataFilter.Columns.Add("ProjectID", typeof(long));
            dtSPPID_DataFilter.Columns.Add("ItemType", typeof(String));
            dtSPPID_DataFilter.Columns.Add("AttName", typeof(String));
            dtSPPID_DataFilter.Columns.Add("ColumnName", typeof(String));
            dtSPPID_DataFilter.Columns.Add("Path", typeof(String));
            dtSPPID_DataFilter.Columns.Add("ColumnOrder", typeof(int));

            DataTable dtSPPID_DataFilter_ColumnOrder = new DataTable("SPPID_DataFilter_ColumnOrder");
            dtSPPID_DataFilter_ColumnOrder.Columns.Add("DataFilterID", typeof(long));
            dtSPPID_DataFilter_ColumnOrder.Columns.Add("ProjectID", typeof(long));
            dtSPPID_DataFilter_ColumnOrder.Columns.Add("ItemType", typeof(String));
            dtSPPID_DataFilter_ColumnOrder.Columns.Add("AttName", typeof(String));
            dtSPPID_DataFilter_ColumnOrder.Columns.Add("ColumnName", typeof(String));
            dtSPPID_DataFilter_ColumnOrder.Columns.Add("Path", typeof(String));
            dtSPPID_DataFilter_ColumnOrder.Columns.Add("ColumnOrder", typeof(int));

            // 복사 소스(from Project)를 '현재Project'로 지정
            SetSchemaInfomation(radDropDownListProjectDBList_FROM);

            string sError = string.Empty;

            GeneralUtil.SetSchemaInfoCopy(EDCS.WF.BSL.Variable.SharedVariable.IsIE, radDropDownListProjectDBList_TO, ref mInformation, ref sError);

            var proxy = WcfHelper.CreateProxyNetTcp<IEdcsDac>(EDCS.WF.BSL.Variable.SharedVariable.IsIE);
            using (proxy as IDisposable)
            {
                if (mInformation != null)
                {
                    List<string> lstItemType = new List<string>();
                    SetItemTypeList(lstItemType);

                    // checkBox확인 : [Attributes], 추후 복사항목 확장 될 수 있음
                    if (radCheckBox_CopyAttribute.Checked == true)
                    {
                        // foreach (Item Type)
                        foreach (string ItemType in lstItemType)
                        {
                            dtSPPID_DataFilter.Clear();
                            dtSPPID_DataFilter_ColumnOrder.Clear();

                            //Read Default table (SPPID_Default_DataFilter)
                            // TODO : 복사시 ReadSPPID_DataFilterbyItemType_ColumnOrder를 기준으로 데이터 생성 by lee
                            // ReadSPPID_DataFilterbyItemType_ColumnOrder를 All로 사용
                            // ReadDataColumnGroup의 Default를 기준
                            // Default를 먼저 표시 하고 나머지 칼럼을 알파벳 순으로 정렬

                            //Read All Attribute Data
                            DataTable dtDataFilter = proxy.GetDataFilter(EDCS.WF.BSL.Variable.SharedVariable.IsIE, mInformation, ItemType);

                            // 사용하는 Attribute
                            List<EntitySPPID_DataFilter_ColumnOrder> lstDataFilter_ColumnOrder = proxy.ReadSPPID_DataFilterbyItemType_ColumnOrder(EDCS.WF.BSL.Variable.SharedVariable.IsIE, mCopyProjectFrom, ItemType);

                            // Default Atribute를 가지고 옴
                            List<EntitySPPID_DataColumnGroup> lstDataColumnGroup = proxy.ReadDataColumnGroup(EDCS.WF.BSL.Variable.SharedVariable.IsIE, mCopyProjectFrom, ItemType, "Admin", false, true);
                            EntitySPPID_DataColumnGroup vDataColumnGroup_Default = lstDataColumnGroup.Where(w => w.Name == CONST_DEFAULT).FirstOrDefault();
                            List<EntitySPPID_DataColumnGroupValue> vReadDataColumnGroupValue_Default = new List<EntitySPPID_DataColumnGroupValue>();

                            if (vDataColumnGroup_Default != null)
                            {
                                vReadDataColumnGroupValue_Default = proxy.ReadDataColumnGroupValue(EDCS.WF.BSL.Variable.SharedVariable.IsIE, vDataColumnGroup_Default.ColumnGroupID);
                            }

                            int? iColumnOrder = -1;
                            foreach (DataRow dr in dtDataFilter.Rows)
                            {
                                string sAtt_Name = Util.NullToSpace(dr["ATT_NAME"]);
                                string sNAME = Util.NullToSpace(dr["NAME"]);
                                string sPATH = Util.NullToSpace(dr["PATH"]);

                                var vUseDataColumnGroup = lstDataFilter_ColumnOrder.Where(w => w.ColumnName == sNAME).FirstOrDefault();

                                if (vUseDataColumnGroup == null)
                                {
                                    // 미사용 항목 Data Filter
                                    DataRow drInsertrow = dtSPPID_DataFilter.NewRow();
                                    drInsertrow["ProjectID"] = mCopyProjectTo;
                                    drInsertrow["ItemType"] = ItemType;
                                    drInsertrow["AttName"] = sAtt_Name;
                                    drInsertrow["ColumnName"] = sNAME;
                                    drInsertrow["Path"] = sPATH;
                                    drInsertrow["ColumnOrder"] = iColumnOrder++;

                                    dtSPPID_DataFilter.Rows.Add(drInsertrow);
                                }
                                else
                                {
                                    // 일치 항목 dtSPPID_DataFilter_ColumnOrder
                                    DataRow drInsertrow = dtSPPID_DataFilter_ColumnOrder.NewRow();
                                    drInsertrow["ProjectID"] = mCopyProjectTo;
                                    drInsertrow["ItemType"] = ItemType;
                                    drInsertrow["AttName"] = sAtt_Name;
                                    drInsertrow["ColumnName"] = sNAME;
                                    drInsertrow["Path"] = sPATH;
                                    drInsertrow["ColumnOrder"] = CONST_DEFAULT_ORDER;
                                    dtSPPID_DataFilter_ColumnOrder.Rows.Add(drInsertrow);
                                }
                            }

                            // 순서 정렬 Default 항목 + 알파벳 순 (미작업)

                            int? iColumn_LastOrder = -1;
                            foreach (DataRow dr in dtSPPID_DataFilter_ColumnOrder.Rows)
                            {
                                // Default 정렬
                                string sColumnName = Util.NullToSpace(dr["ColumnName"]);

                                var vitem = vReadDataColumnGroupValue_Default.Where(w => w.Name == sColumnName).FirstOrDefault();

                                if (vitem != null)
                                {
                                    dr["ColumnOrder"] = vitem.ColumnOrder;

                                    if (iColumn_LastOrder < vitem.ColumnOrder)
                                    {
                                        iColumn_LastOrder = vitem.ColumnOrder;
                                    }
                                }
                            }

                            dtSPPID_DataFilter_ColumnOrder.AcceptChanges();

                            if (dtSPPID_DataFilter_ColumnOrder.Rows.Count > 0)
                            {
                                // 나머지 항목 알파벳 순으로 정렬
                                DataRow[] arrdr = dtSPPID_DataFilter_ColumnOrder.Select(string.Format(" ColumnOrder={0} ", CONST_DEFAULT_ORDER, "ColumnName"));

                                if (arrdr.Length > 0)
                                {
                                    foreach (DataRow dr in arrdr)
                                    {
                                        dr["ColumnOrder"] = ++iColumn_LastOrder;
                                    }
                                }
                            }

                            dtSPPID_DataFilter_ColumnOrder.AcceptChanges();

                            // SPPID_DataFilter, SPPID_DataFilter_ColumnOrder 정보 저장
                            proxy.SaveAdminDataFilter(EDCS.WF.BSL.Variable.SharedVariable.IsIE, mCopyProjectTo, ItemType, dtSPPID_DataFilter, dtSPPID_DataFilter_ColumnOrder, ref sError);

                            // column group id를 참조해 TO project DB 전체 삭제
                            List<EntitySPPID_DataColumnGroup> lstDataColumnGroupDelete = proxy.ReadDataColumnGroup(EDCS.WF.BSL.Variable.SharedVariable.IsIE, mCopyProjectTo, ItemType, "Admin", false, true);
                            foreach (EntitySPPID_DataColumnGroup DataColumnGroupDelete in lstDataColumnGroupDelete)
                                proxy.SetDeleteColumnGroupValue(EDCS.WF.BSL.Variable.SharedVariable.IsIE, DataColumnGroupDelete.ColumnGroupID);

                            string[] arAttributes = null;
                            foreach (EntitySPPID_DataColumnGroup DataColumnGroup in lstDataColumnGroup)
                            {
                                string sDataColumnGroupName = DataColumnGroup.Name;

                                // DataColumnGroupValue 복사
                                List<EntitySPPID_DataColumnGroupValue> ReadDataColumnGroupValue = proxy.ReadDataColumnGroupValue(EDCS.WF.BSL.Variable.SharedVariable.IsIE, DataColumnGroup.ColumnGroupID);

                                int i = 0;
                                arAttributes = new string[ReadDataColumnGroupValue.Count];
                                foreach (EntitySPPID_DataColumnGroupValue DataColumnGroupValue in ReadDataColumnGroupValue)
                                {
                                    arAttributes[i] = DataColumnGroupValue.Name;
                                    i++;
                                }

                                proxy.SetColumnGroupValue(EDCS.WF.BSL.Variable.SharedVariable.IsIE, mColumnGroupID, mCopyProjectMasterTo, mCopyProjectTo, sDataColumnGroupName, ItemType, true, "Admin", arAttributes);
                               
                            }
                        }// End foreach (Item Type)
                    }//End CheckBox : CopyAttribute
                }
            }

          

        }




    }
}

